#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>

#define pixelCount 64

const char WiFiAPPSK[] = "aMakerESP8266";

WiFiServer server(80);

bool debug = true;
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(pixelCount, 4);

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266 Thing " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void printDebug(String toDisplay) {
  if (debug) {
      Serial.println(toDisplay);
  }
}

void setup() 
{
  setupWiFi();
  server.begin();
  // strip.begin();
  Serial.begin(9600);
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

  
  if (req.indexOf("do") != -1) {
    printDebug("Confirm request received");
    size_t equals = req.indexOf("?");
    if (equals != -1) {
      String colors=req.substring(equals+1);
      printDebug("colors : " + colors);
      int currentComma = 0;
      int nextComma = colors.indexOf(',',currentComma);
      while (nextComma != -1) {
        printDebug("Color : " + colors.substring(currentComma, nextComma));
        currentComma = nextComma + 1;
        nextComma = colors.indexOf(',',currentComma);
      }
    }
  }
  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
