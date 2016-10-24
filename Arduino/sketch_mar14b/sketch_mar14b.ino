
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>


/*Bitmap of ANSI chars - letters+numbers+common signs:  starts at char Ox21, ends at 0x7E */
 String  ansiCharsByCols [96] = {
  "0000000000000000", //32
  "000000fa00000000", 
  "0000c000c0000000",
  "000028fe28fe2800",
  "00002454fe544800",
  "00c4c810204c8c00",
  "000c72926a040a00",
  "0000002040800000",
  "0000003844820000",
  "0000824438000000",
  "000028107c102800",
  "000010107c101000",
  "0000020c0c000000",
  "0000101010101000",
  "0000000c0c000000",
  "0000040810204000",
  "00007c8a92a27c00",
  "00000042fe020000",
  "0000468a8a926200",
  "00008482a2d28c00",
  "0000182848fe0800",
  "0000e4a2a2a29c00",
  "00003c5292920c00",
  "0000808e90a0c000",
  "00006c9292926c00",
  "0000609292947800",
  "0000006c6c000000",
  "0000026c6c000000",
  "0000102844820000",
  "0000282828282800",
  "0000824428100000",
  "0040809a90600000",
  "00007c82baaa7a00",
  "00007e9090907e00", // A
  "000082fe92926c00", // B
  "00007c8282824400", // C
  "000082fe82827c00", // D
  "0000fe9292828200", // E
  "0000fe9090808000", // F
  "00007c828a8a4e00", // G
  "0000fe101010fe00", // H
  "00000082fe820000", // I
  "0000040282fc8000", // J
  "0000fe1028448200", // K
  "0000fe0202020200", // L
  "0000fe403040fe00", // M
  "0000fe201008fe00", // N
  "00007c8282827c00", // O
  "0000fe9090906000", // P
  "00007c828a847a00", // Q
  "0000fe9098946200", // R
  "000044a2928a4400", // S
  "00008080fe808000", // T
  "0000fc020202fc00", // U
  "0000f8040204f800", // V
  "0000fc021c02fc00", // W
  "0000c6281028c600", // X
  "0000e0100e10e000", // Y
  "0000868a92a2c200", // Z
  "00003cc242c22400",
  "00003c4242c22400",
  "000092fe92827c00",
  "000024d24aca2400",
  "000042c64ad26200",
  "0000204080402000",
  "00000854543c0000",
  "0000fc2424180000",
  "0000384444440000",
  "0000182424fc0000",
  "0000385454200000",
  "0000107e90400000",
  "0000304a4a7c0000",
  "0000fc20201c0000",
  "00000024bc040000",
  "00000422bc000000",
  "0000fc1028040000",
  "00000084fc040000",
  "00007c403c403c00",
  "00007c40403c0000",
  "0000384444380000",
  "00007e4848300000",
  "0030484a7e020200",
  "00007c2040400000",
  "0000245454480000",
  "00000040f8440000",
  "00007804047c0000",
  "0000700804087000",
  "0000780418047800",
  "0000442810284400",
  "0000700a0a7c0000",
  "0000444c54640000",
  "000018a4a4a40000",
  "0000182424a40000",
  "0000081454fc4000",
  "000024d454c80000",
  "000024acb4a40000"};

  
#define pixelCount 64
//amaker 
#define PANELPIN 4

#define LEDPIN 2
const char WiFiAPPSK[] = "aMakerESP8266";

NeoPixelBus strip = NeoPixelBus(pixelCount, PANELPIN);
uint16_t effectState = 0;
byte pixelMapping [8][8]; 

WiFiServer server(80);

bool debug = false;

String getTD(String id, String value){
return ((String)("<td id=\"") + id + (String)("\">") + value + (String)("</td>"));
}

void printCSS(WiFiClient client){
client.println("<style>");
client.println("input[type=color],table,td{padding:0;margin:0}body{background-color:#101010;color:#f0f0f0}table{border:none;text-align:center;vertical-align:middle;background-color:silver}colgroup{border-right:2px outset #fff;border-left:2px outset #fff}tfoot,thead{border-top:2px outset #fff;border-bottom:2px outset #fff}input[type=color]{background-color:#000;border:none;height:20px;width:20px;cursor:crosshair}.drop_hover{border:2px double #00f}td{width:1.4em;height:1.4em}"
);
client.println("</style>");
}

void printJavascript(WiFiClient client){
client.println("<script>");
client.println("function httpGetAsync(a,b){var c=new XMLHttpRequest;c.onreadystatechange=function(){console.log(\"callback : \"),console.log(\"xmlHttp.readyState=\"+c.readyState),console.log(\"xmlHttp.status=\"+c.status),console.log(\"xmlHttp.response=\"+c.response)},console.log(\"http \"+a),c.open(\"GET\",a,!0),c.send()}function sndarr(){s=\"\";for(var a=0;a<8;a++)for(var b=0;b<8;b++)col=rgb2hex(document.getElementById(\"\"+a+\".\"+b).style.color),null==col?col=\"000000\":col=col.substr(col.length-6),s=s+col+\",\";httpGetAsync(this.location+\"/do?colors=\"+s)}function sendmsg(){httpGetAsync(this.location+\"/msg?\"+document.getElementById(\"msg\").value)}function hex(a){return isNaN(a)?\"00\":hexDigits[(a-a%16)/16]+hexDigits[a%16]}function rgb2hex(a){return a=a.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/),\"#\"+hex(a[1])+hex(a[2])+hex(a[3])}function rgb2array(a){return a.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/)}function arr2hex(a){return\"#\"+hex(a[0])+hex(a[1])+hex(a[2])}function setColor(a,b){null!=a&&(a.target?fill(a.target.id.substr(0,1),a.target.id.substr(2,1),b):fill(a.srcElement.id.substr(0,1),a.srcElement.id.substr(2,1),b))}function fill(a,c,d){if(\"*\"==a&&\"*\"==c)for(var e=0;e<8;e++)for(var f=0;f<8;f++)document.getElementById(\"\"+e+\".\"+f).style.color=d;else if(\"-\"==a&&\"-\"==c)for(var e=0;e<8;e++)for(var f=0;f<8;f++)document.getElementById(\"\"+e+\".\"+f).style.color=d;else if(\"*\"==a)for(var e=0;e<8;e++)document.getElementById(\"\"+e+\".\"+c).style.color=d;else if(\"*\"==c)for(var e=0;e<8;e++)document.getElementById(\"\"+a+\".\"+e).style.color=d;else if(\"-\"==c){fromC=rgb2array(document.getElementById(\"\"+a+\".0\").style.color),toC=rgb2array(document.getElementById(\"\"+a+\".7\").style.color);for(var e=0;e<8;e++)r=parseInt(fromC[1])+Math.floor((parseInt(toC[1])-parseInt(fromC[1]))*e/7),g=parseInt(fromC[2])+Math.floor((parseInt(toC[2])-parseInt(fromC[2]))*e/7),b=parseInt(fromC[3])+Math.floor((parseInt(toC[3])-parseInt(fromC[3]))*e/7),document.getElementById(\"\"+a+\".\"+e).style.color=arr2hex([r,g,b])}else if(\"-\"==a)for(var e=0;e<8;e++){fromC=rgb2array(document.getElementById(\"0.\"+c).style.color),toC=rgb2array(document.getElementById(\"7.\"+c).style.color);for(var e=0;e<8;e++)document.getElementById(\"\"+e+\".\"+c).style.color=arr2hex([parseInt(fromC[1])+Math.floor((parseInt(toC[1])-parseInt(fromC[1]))*e/7),parseInt(fromC[2])+Math.floor((parseInt(toC[2])-parseInt(fromC[2]))*e/7),parseInt(fromC[3])+Math.floor((parseInt(toC[3])-parseInt(fromC[3]))*e/7)])}else document.getElementById(\"\"+a+\".\"+c).style.color=d;arrtocnv()}function onload(){for(i=0;i<8;i++)for(j=0;j<8;j++)");
client.println("document.getElementById(\"\"+i+\".\"+j).style.color=\"#101010\";var a=document.querySelectorAll(\"td\");for(i=0;i<a.length;i++)a[i].addEventListener(\"mouseover\",function(a){a.shiftKey&&(a.ctrlKey?setColor(a,rndcol()):setColor(a,document.getElementById(\"selcol\").value))}),");
client.println("a[i].addEventListener(\"click\",function(a){a.ctrlKey?setColor(a,rndcol()):setColor(a,document.getElementById(\"selcol\").value)}),3==a[i].id.length&&(a[i].style.cursor=\"pointer\")}function rndcol(){return\"#\"+hex(Math.floor(256*Math.random()))+hex(Math.floor(256*Math.random()))+hex(Math.floor(256*Math.random()))}function arrtocnv(){for(e=document.getElementById(\"canvas1\"),cn=e.getContext(\"2d\"),width=e.width,height=e.height,img=cn.createImageData(width,height),i=0;i<8;i++)for(j=0;j<8;j++)cl=rgb2array(document.getElementById(\"\"+i+\".\"+j).style.color),ix=4*(j+i*img.width),img.data[ix+0]=cl[1],img.data[ix+1]=cl[2],img.data[ix+2]=cl[0],img.data[ix+3]=256;cn.putImageData(img,0,0)}var hexDigits=new Array(\"0\",\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\",\"8\",\"9\",\"a\",\"b\",\"c\",\"d\",\"e\",\"f\");");
client.println("</script>");
}

void printPanelTable(WiFiClient client) {
  client.println("<html>");
  printCSS(client);
  printJavascript(client);
  client.println("<body onload=\"javascript:onload()\">");
  client.println("<label for=\"selcol\" style=\"width: 6em;\">Color picker </label><input type=\"color\" id=\"selcol\" style=\"width: 2em; height: 1em;\"><br />");
  client.println("<table><thead><tr>"+getTD("*.*","&#x21D8"));
  for (int i=0; i<8; i++) 
    client.println(getTD("*."+String(i), "&#x21D3"));
  client.println(getTD("?.7","&#x25A7"));
  client.println("</tr></thead>");
  client.println("<tbody>");
  for (int i=0; i<8; i++) {
    client.println("<tr>"+getTD(String(i)+".*","&#x21D2"));
    for (int j=0; j<8; j++) {
      client.println(getTD(String(i)+"."+String(j),"&#x25C9"));
    }
    client.println(getTD(String(i)+".-","&#x25A5"));
    client.println("</tr>");
  }
  client.println("</tbody>");
  client.println("<tr>"+getTD("-.-","&#x25A9"));
  for (int i=0; i<8; i++) {
    client.println(getTD("-."+String(i), "&#x25A4"));
  }
  client.println(getTD("-?","&#x25A8"));
  client.println("</tr><tfoot><tr><td colspan=9><button onclick=\"javascript:sndarr();\" style=\"width: 15em;\">Send matrix. &#x21F6;</button><canvas id=\"canvas1\" width=\"8px\" height=\"8px\" style=\"border: thick; border-color: white\"></canvas></td></tr></tfoot></table>");
  client.println("<br/><table><tr><td colspan=\"2\"><input id=\"msg\" type=\"text\" style=\"width: 15em\"></td></tr><tr><td id=\"M.F\">&#x25cf;Fgd</td><td id=\"M.B\">&#x25d8;Bkg</td></tr><tr><td colspan=\"2\"><button style=\"width: 15em\" onclick=\"sendmsg()\">Send text.&#x2933;</button></td></tr></table>");
  client.println("</body>");
  client.println("</html>");
}

void blink(byte n){
  for (byte i=0; i<n; i++) {
  digitalWrite(LEDPIN, LOW);    // turn the LED off by making the voltage LOW
  delay(100);              // wait for a second
  digitalWrite(LEDPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);              // wait for a second
  } }

int hexaToInt (char c) {
switch (c) { 
  case '0':return 0;
  case '1':return 1;
  case '2':return 2;
  case '3':return 3;
  case '4':return 4;
  case '5':return 5;
  case '6':return 6;
  case '7':return 7;
  case '8':return 8;
  case '9':return 9;
  case 'A':return 10;
  case 'B':return 11;
  case 'C':return 12;
  case 'D':return 13;
  case 'E':return 14;
  case 'F':return 15;
  case 'a':return 10;
  case 'b':return 11;
  case 'c':return 12;
  case 'd':return 13;
  case 'e':return 14;
  case 'f':return 15;
  default : return 0;
  }
}
void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];  
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "aMaker thing " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
blink(4);
}

void printDebug(String toDisplay) {
  if (debug) {
      Serial.println(toDisplay);
  }
}
void started(){
  //NOP 
 }

String getCharHexa(char value) {
  if (value >=32 && value <= 125) {
    return ansiCharsByCols[value - 32];
  }
  else {
    return "ffffffffffffffff";
  }
}
void handleHexaStringRequest(String req, int waitms) {
  printDebug("req="+req);

  RgbColor white = RgbColor(255,255,255);
  RgbColor black = RgbColor(0,0,0);
  int off = 0;
  int pixelNumber= 0;
  for (int curCol = 0; curCol < 8; ++curCol) {
  
    char colValue = (hexaToInt(req.charAt(2*curCol)) << 4 )+ hexaToInt(req.charAt(2*curCol + 1));
    int checker;
    if (curCol % 2 == 1) {
      checker = 1;
    }
    else {
      checker = 128;
    }

    for (int curBit = 0; curBit < 8; curBit++) {
      if ((colValue & checker) == checker) {
        strip.SetPixelColor(pixelNumber++, white); 
      }
      else {
        strip.SetPixelColor(pixelNumber++, black);            
      }
      if (curCol % 2 == 1) {
        checker = checker << 1;
      }
      else {
        checker = checker >> 1;
      }      
    }
  }
  strip.Show();
  delay(waitms);       
}




void printScroll(String input, int waitms, int nbLoops) {
  // Need to add a space at the beginning/ end of the string
  int curOffset_message = 0;
  
  for (int maxLoop=0; maxLoop<nbLoops*input.length(); ++maxLoop) {
    
    String Chars_2 =   getCharHexa(input.charAt(curOffset_message)) +  getCharHexa(input.charAt((curOffset_message + 1) % input.length()));
    
    for (int offsetLetter = 0; offsetLetter < 8; ++offsetLetter) {
      handleHexaStringRequest(Chars_2 , waitms);
      Chars_2=Chars_2.substring(2);
    }
    curOffset_message = curOffset_message + 1;
    if (curOffset_message > input.length() - 1) {
      curOffset_message = 0;
    }
  }
}

void initPixelMapping(){
  byte pixelNumber=0;
  for (byte x=0;x<8;x++)
    for (byte y=0;y<8;y++) {
      if (x % 2 == 0) { pixelMapping[x][y]=pixelNumber++; }
      else { pixelMapping[x][7-y]=pixelNumber++; }
    }
}

void setup()
{
  pinMode(LEDPIN, OUTPUT);    
  Serial.begin(9600);

  setupWiFi();
  server.begin();
  strip.Begin();
  initPixelMapping();
  strip.Show();
  started();

}


int getColorValueFromHexa(String c){

    int result = 16*hexaToInt(c.c_str()[0])+hexaToInt(c.c_str()[1]);
    //printDebug("c="+c+" converted : "+result);
    return result /2;
}

/* Set pixel request, in form of set?num=RRGGBB&num=RRGGBB*/
void handleSetRequest(String req ) {
  printDebug("req="+req);
  
    size_t equals = req.indexOf("?");
    if (equals != -1) {
      String colors=req.substring(equals+8);
      printDebug("colors : " + colors);
      
      int currentComma = 0;
      int nextComma = colors.indexOf(',',currentComma);
      int pixelNumber=0;
      while (pixelNumber <64) {
        printDebug("Color : " + colors.substring(currentComma, nextComma));
        String pixelRGB =colors.substring(currentComma, nextComma);
        int pixelR =getColorValueFromHexa(pixelRGB .substring(0,2));
        int pixelG =getColorValueFromHexa(pixelRGB.substring(2,4));
        int pixelB =getColorValueFromHexa(pixelRGB.substring(4,6));

        strip.SetPixelColor(pixelNumber++, RgbColor(pixelR,pixelG,pixelB));
        currentComma = nextComma + 1;
        nextComma = colors.indexOf(',',currentComma);
        
      } 
      strip.Show();
    }
}

/* REST service to set pixel by x.y=RRGGBB;x.y=RRGGBB;x.y=RRGGBB;.. */
void handleSetPixelRequest ( char *  req ){
blink(1);

    printDebug("Confirm request received");
    /* find first ?*/
    char * s; 
    s = strtok(req,"?");
    if (s!=NULL){
    printDebug("path");
    printDebug(s);
    s= strtok(NULL,"?");
    printDebug("params");
    printDebug(s);
    char * param;
    param=strtok(s,";");
    while (param !=NULL){
    printDebug(param);  
    param=strtok(NULL,";");
    }
    
    strip.Show();
    }
    /* loop : split at ; to get all x.y =rrggbb */
    /* split at . and then =  to get x y and rrggbb */
    else {
      /* nothing to do in absence of ? */
    }
     
    
    
}

/* REST service to set entire image colors=RRGGBB,RRGGBB,RRGGBB,RRGGBB,....,RRGGBB (64 colors passed */
void handleDoRequest(String req ){
    blink(1);

    printDebug("Confirm request received");
    printDebug(req);
    
    size_t equals = req.indexOf("?");
    if (equals != -1) {
      String colors=req.substring(equals+8);
      printDebug("colors : " + colors);
      
      int currentComma = 0;
      int nextComma = colors.indexOf(',',currentComma);
      int pixelNumber=0;
      while (pixelNumber <64) {
        //printDebug("Color : " + colors.substring(currentComma, nextComma));
        String pixelRGB =colors.substring(currentComma, nextComma);
        int pixelR =getColorValueFromHexa(pixelRGB .substring(0,2));
        int pixelG =getColorValueFromHexa(pixelRGB.substring(2,4));
        int pixelB =getColorValueFromHexa(pixelRGB.substring(4,6));

        strip.SetPixelColor(pixelNumber++, RgbColor(pixelR,pixelG,pixelB));
        currentComma = nextComma + 1;
        nextComma = colors.indexOf(',',currentComma);
        
      } 
      strip.Show();
    }
}

String decodeUrl(String req) {
  return req;
}

/* REST service to set entire image colors=RRGGBB,RRGGBB,RRGGBB,RRGGBB,....,RRGGBB (64 colors passed */
void handleMessageScrollRequest(String req ){
    blink(1);

    printDebug("Confirm handleMessageScrollRequest  received");
    printDebug(req);
    req.replace("%20", " ");
    req.replace("%21", "!");
    req.replace("%22", "\"");
    req.replace("%23", "#");
    req.replace("%24", "$");
    req.replace("%25", "%");
    req.replace("%26", "&");
    req.replace("%27", "'");
    req.replace("%28", "(");
    req.replace("%29", ")");
    req.replace("%2A", "*");
    req.replace("%2B", "+");
    req.replace("%2C", ",");
    req.replace("%2D", "-");
    req.replace("%2E", ".");
    req.replace("%2F", "/");
    req.replace("%3A", ":");
    req.replace("%3B", ";");
    req.replace("%3C", "<");
    req.replace("%3D", "?");
    req.replace("%3F", ">");
    req.replace("%3F", "?");
    req.replace("%40", "@");
    req.replace("%5B", "[");
    req.replace("%5C", "\\");
    req.replace("%5D", "]");
    req.replace("%5E", "^");
    req.replace("%5F", "_");
    req.replace("%60", "`");
    req.replace("%7B", "{");
    req.replace("%7C", "|");
    req.replace("%7D", "}");
    req.replace("%7E", "~");
    size_t equals = req.indexOf("?");
    if (equals != -1) {
      String message=req.substring(equals+1, req.length()-9);
      printDebug("message : " + message);
      printScroll(message, 100, 1);
    }
}

void handleDefault(WiFiClient client){
blink(1);
printPanelTable(client);
 
  /*
  client.println("<html><head><title>aMaker club - CE aMaDEUS</title></head>\r\n<script>\r\nfunction sendColor(t){ httpGetAsync( this.location + \"/set?\"+t.id+\"=\"+t.value);}\r\nfunction callback(){ } ");
    client.println("function httpGetAsync(theUrl, callback) {    var xmlHttp = new XMLHttpRequest();\r\n   xmlHttp.onreadystatechange = function() { if (xmlHttp.readyState == 4 && xmlHttp.status == 200) \r\ncallback(xmlHttp.responseText); }  \r\nxmlHttp.open(\"GET\", theUrl, true);\r\nxmlHttp.send(); }");
   client.println("function sendMsg() { httpGetAsync( this.location + \"/msg?  \" + document.getElementById(\"text\").value);}");
   client.println("function sendAll() { s= \"\"; for (var i=0; i<8; i++) { for (var j=0; j<8; j++) { s=s+document.getElementById(\"\"+i+\".\"+\"j\")+\",\";}} httpGetAsync( this.location + \"/do?colors=\"+s) }");
    client.println("</script>\r\n<style>\r\nbody{ background-color: black; }\r\ntable,tr,td { margin: 0px; padding:0px}\r\ninput[type=color] { background-color: black;border: solid 0px black; height: 20px;width: 20px; padding:0px;margin: 0px; }\r\n</style>\r\n<body>");
  
  for (int x=0;x<8;x++){
    String s="<br >";
    for (int y=0;y<8;y++){
      s+="<input id=\"";
      s+=x;
      s+=".";
      s+=y;     
      //s+="\" type=\"color\" onchange=\"javascript:sendColor(this)\">";
      s+="\" type=\"color\" onchange=\"javascript:sendAll()\">";
    }
    client.print (s+"\r\n");
  }
client.print ("<br ><input id=\"text\" type=\"text\" /> <input value=\"Afficher\" type=\"submit\" onclick=\"sendMsg()\"/>\r\n</body></html>");
*/}

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  client.setTimeout(10000);
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();
  printDebug("Request : " + req);

  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n";

  client.print(s);

  // Send the response to the client - always ok :-/
  
  if (req.indexOf("/do?") != -1) {
    handleDoRequest(req);
  }
  if (req.indexOf("/msg?") != -1) {
    handleMessageScrollRequest(req);
  }
  if (req.indexOf("/set?") != -1) {
    handleSetPixelRequest((char*)req.c_str());
  }
  else { handleDefault(client);}
  
  
  client.flush();

  delay(1);
  printDebug("Client disconnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

