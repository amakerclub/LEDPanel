
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>


/*Bitmap of ANSI chars - letters+numbers+common signs:  starts at char Ox21, ends at 0x7E */
 String  ansiCharsByCols [97] = {
  "0000000000000000", //20
  "000000fa00000000", 
  "0000000000000000",
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
  if (value >= 20 && value <= 0x7e) {
    return ansiCharsByCols[value - 31];
  }
  else {
    return "ffffffffffffffff";
  }
}
void handleHexaStringRequest(String req, int waitms) {
  printDebug("req="+req);

  RgbColor white = RgbColor(128,128,128);
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
/* REST service to set entire image colors=RRGGBB,RRGGBB,RRGGBB,RRGGBB,....,RRGGBB (64 colors passed */
void handleMessageScrollRequest(String req ){
    blink(1);

    printDebug("Confirm handleMessageScrollRequest  received");
    printDebug(req);
    
    size_t equals = req.indexOf("?");
    if (equals != -1) {
      String message=req.substring(equals+1, req.length()-9);
      printDebug("message : " + message);
      printScroll(message, 100, 0);
      
      
    }
}


void handleDefault(WiFiClient client){
blink(1);

 
  
  client.println("<html><head><title>aMaker club - CE aMaDEUS</title></head>\r\n<script>\r\nfunction sendColor(t){ httpGetAsync( this.location + \"/set?\"+t.id+\"=\"+t.value);}\r\nfunction callback(){ } ");
    client.println("function httpGetAsync(theUrl, callback) {    var xmlHttp = new XMLHttpRequest();\r\n   xmlHttp.onreadystatechange = function() { if (xmlHttp.readyState == 4 && xmlHttp.status == 200) \r\ncallback(xmlHttp.responseText); }  \r\nxmlHttp.open(\"GET\", theUrl, true);\r\nxmlHttp.send(); }");
   client.println("function sendAll() { s= \"\"; for (int i=0; i<8; i++) { for (int j=0; j<8; j++) { s=s+document.getElementById(\"\"+i+\".\"+\"j\")+\",\";}} httpGetAsync( this.location + \"/do?colors=\"+s) }");
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
client.print ("</body></html>");
}

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  client.setTimeout(120000);
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();
  printDebug("Request : " + req);

  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n";
  s += "<html><body><i>ok</i></body></html>";

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
  
  client.print(s);
  
  client.flush();

  delay(1);
  printDebug("Client disconnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

