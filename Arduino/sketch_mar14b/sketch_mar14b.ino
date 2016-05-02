
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>


/*Bitmap of ANSI chars - letters+numbers+common signs:  starts at char Ox21, ends at 0x7E */
const char[] ansiCharsByRows = ("1818181818001800",
 "6c6c6c0000000000",
 "36367f367f363600",
 "0c3f683e0b7e1800",
 "60660c1830660600",
 "386c6c386d663b00",
 "0c18300000000000",
 "0c18303030180c00",
 "30180c0c0c183000",
 "00187e3c7e180000",
 "0018187e18180000",
 "0000000000181830",
 "0000007e00000000",
 "0000000000181800",
 "00060c1830600000",
 "3c666e7e76663c00",
 "1838181818187e00",
 "3c66060c18307e00",
 "3c66061c06663c00",
 "0c1c3c6c7e0c0c00",
 "7e607c0606663c00",
 "1c30607c66663c00",
 "7e060c1830303000",
 "3c66663c66663c00",
 "3c66663e060c3800",
 "0000181800181800",
 "0000181800181830",
 "0c18306030180c00",
 "00007e007e000000",
 "30180c060c183000",
 "3c660c1818001800",
 "3c666e6a6e603c00",
 "3c66667e66666600",
 "7c66667c66667c00",
 "3c66606060663c00",
 "786c6666666c7800",
 "7e60607c60607e00",
 "7e60607c60606000",
 "3c66606e66663c00",
 "6666667e66666600",
 "7e18181818187e00",
 "3e0c0c0c0c6c3800",
 "666c7870786c6600",
 "6060606060607e00",
 "63777f6b6b636300",
 "6666767e6e666600",
 "3c66666666663c00",
 "7c66667c60606000",
 "3c6666666a6c3600",
 "7c66667c6c666600",
 "3c66603c06663c00",
 "7e18181818181800",
 "6666666666663c00",
 "66666666663c1800",
 "63636b6b7f776300",
 "66663c183c666600",
 "6666663c18181800",
 "7e060c1830607e00",
 "7c60606060607c00",
 "006030180c060000",
 "3e06060606063e00",
 "183c664200000000",
 "00000000000000ff",
 "1c36307c30307e00",
 "00003c063e663e00",
 "60607c6666667c00",
 "00003c6660663c00",
 "06063e6666663e00",
 "00003c667e603c00",
 "1c30307c30303000",
 "00003e66663e063c",
 "60607c6666666600",
 "1800381818183c00",
 "1800381818181870",
 "6060666c786c6600",
 "3818181818183c00",
 "0000367f6b6b6300",
 "00007c6666666600",
 "00003c6666663c00",
 "00007c66667c6060",
 "00003e66663e0607",
 "00006c7660606000",
 "00003e603c067c00",
 "30307c3030301c00",
 "0000666666663e00",
 "00006666663c1800",
 "0000636b6b7f3600",
 "0000663c183c6600",
 "00006666663e063c",
 "00007e0c18307e00",
 "0c18187018180c00",
 "1818180018181800",
 "3018180e18183000",
 "316b460000000000") ; 

#define pixelCount 64
//amaker 
//#define PANELPIN 4
//perso
#define PANELPIN 4
#define LEDPIN 2
const char WiFiAPPSK[] = "aMakerESP8266";

NeoPixelBus strip = NeoPixelBus(pixelCount, PANELPIN);
uint16_t effectState = 0;
byte pixelMapping [8][8]; 

WiFiServer server(80);

bool debug = true;

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
  if (value >= 20) && (value <= 0x7e) {
    return ansiCharsByRows[value - 20];
  }
  else {
    return "ffffffffffffffff";
  }
}
void printScroll(String input, int delay) {
  // Need to add a space at the beginning/ end of the string
  int curOffset_message = 0;
  while (true) {
    String Chars_2 = getCharHexa[input.at(currOffset_message)] + getCharHexa(input.at((currOffset_message + 1) % length(input)));
    for (int offsetLetter = 0; offsetLetter < 8; ++offsetLetter) {
      
      sleep(delay);       
    }
    curOffset_message = curOffset_message + 1;
    if (curOffset_message > length(input)) {
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

void handleHexaStringRequest(String req) {
  printDebug("req="+req);

  Color white = RgbColor(128,128,128);
  Color black = RgbColor(0,0,0);
  int off = 0;
  for (int curCol = 0; curCol < 8; ++curCol) {
    int colValue = hexaToInt(req.at(2*curCol)) << 8 + hexaToInt(req.at(2*curcol + 1));
    int checker;
    if (curCol % 2 == 0) {
      checker = 1;
    }
    else {
      checker = 128;
    }
    for (int curBit = 0; curBit < 8; curBit++) {
      if ((curCol & checker) == checker) {
        strip.SetPixelColor(pixelNumber++, white);      
      }
      else {
        strip.SetPixelColor(pixelNumber++, black);            
      }
      if (curCol % 2 == 0) {
        checker << 1;
      }
      else {
        checker >> 1;
      }      
    }
  }
  
  strip.Show();
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

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();
  printDebug("Request : " + req);

  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n";
  s += "<html><body><i>ok</i></body></html>";

  // Send the response to the client - always ok :-/
  
  client.print(s);
  
  if (req.indexOf("/do?") != -1) {
    handleDoRequest(req);
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

