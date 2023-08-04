#include <stdio.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define PIN D8

// Defaultni konfigurace WiFi pro AP
const char* my_ssid = "MikroTik";
const char* my_pass = "nechodsemTY!";

//the Wemos WS2812B RGB shield has 1 LED connected to pin 2
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(23, PIN, NEO_GRB + NEO_KHZ800);

ESP8266WebServer server(80);

String p[24];

// Zakladni stranka pro ovladani LED svetel
void indexHTML() {
  String cas = String(millis() / 1000);
  String zprava = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>LEDSign by robezonetv</title><style>.picker {position: absolute;}.p1 {margin-top: 130px;margin-left: 45px;}.p2 {margin-top: 70px;margin-left: 60px;}.p3 {margin-top: 30px;margin-left: 135px;}.p4 {margin-top: 80px;margin-left: 190px;}.p5 {margin-top: 130px;margin-left: 230px;}.p6 {margin-top: 182px;margin-left: 270px;}.p8 {margin-top: 240px;margin-left: 310px;}.p9 {margin-top: 300px;margin-left: 345px;}.p10 {margin-top: 360px;margin-left: 360px;}.p11 {margin-top: 420px;margin-left: 380px;}.p12 {margin-top: 515px;margin-left: 210px;}.p13 {margin-top: 455px;margin-left: 205px;}.p14 {margin-top: 395px;margin-left: 200px;}.p15 {margin-top: 335px;margin-left: 190px;}.p16 {margin-top: 275px;margin-left: 180px;}.p17 {margin-top: 215px;margin-left: 180px;}.p18 {margin-top: 505px;margin-left: 330px;}.p19 {margin-top: 482px;margin-left: 390px;}.p20 {margin-top: 450px;margin-left: 450px;}.p21 {margin-top: 420px;margin-left: 500px;}.p22 {margin-top: 390px;margin-left: 560px;}.p23 {margin-top: 360px;margin-left: 610px;}</style></head><body>";
  zprava += "<center><h1>DINO control</h1><h3>uptime: ";
  zprava += cas;
  zprava += " s<br>IP Adresa: " + WiFi.localIP().toString() + "</h3></center>";
  zprava += "<form method=\"GET\" action=\"./led\">";
  zprava += "<input class=\"picker p1\" type=\"color\" name=\"p0\" value=\""+p[0]+"\">";
  zprava += "<input class=\"picker p2\" type=\"color\" name=\"p1\" value=\""+p[1]+"\">";
  zprava += "<input class=\"picker p3\" type=\"color\" name=\"p2\" value=\""+p[2]+"\">";
  zprava += "<input class=\"picker p4\" type=\"color\" name=\"p3\" value=\""+p[3]+"\">";
  zprava += "<input class=\"picker p5\" type=\"color\" name=\"p4\" value=\""+p[4]+"\">";
  zprava += "<input class=\"picker p6\" type=\"color\" name=\"p5\" value=\""+p[5]+"\">";
  zprava += "<input class=\"picker p8\" type=\"color\" name=\"p7\" value=\""+p[7]+"\">";
  zprava += "<input class=\"picker p9\" type=\"color\" name=\"p8\" value=\""+p[8]+"\">";
  zprava += "<input class=\"picker p10\" type=\"color\" name=\"p9\" value=\""+p[9]+"\">";
  zprava += "<input class=\"picker p11\" type=\"color\" name=\"p10\" value=\""+p[10]+"\">";
  zprava += "<input class=\"picker p12\" type=\"color\" name=\"p11\" value=\""+p[11]+"\">";
  zprava += "<input class=\"picker p13\" type=\"color\" name=\"p12\" value=\""+p[12]+"\">";
  zprava += "<input class=\"picker p14\" type=\"color\" name=\"p13\" value=\""+p[13]+"\">";
  zprava += "<input class=\"picker p15\" type=\"color\" name=\"p14\" value=\""+p[14]+"\">";
  zprava += "<input class=\"picker p16\" type=\"color\" name=\"p15\" value=\""+p[15]+"\">";
  zprava += "<input class=\"picker p17\" type=\"color\" name=\"p16\" value=\""+p[16]+"\">";
  zprava += "<input class=\"picker p18\" type=\"color\" name=\"p17\" value=\""+p[17]+"\">";
  zprava += "<input class=\"picker p19\" type=\"color\" name=\"p18\" value=\""+p[18]+"\">";
  zprava += "<input class=\"picker p20\" type=\"color\" name=\"p19\" value=\""+p[19]+"\">";
  zprava += "<input class=\"picker p21\" type=\"color\" name=\"p20\" value=\""+p[20]+"\">";
  zprava += "<input class=\"picker p22\" type=\"color\" name=\"p21\" value=\""+p[21]+"\">";
  zprava += "<input class=\"picker p23\" type=\"color\" name=\"p22\" value=\""+p[22]+"\">";
  zprava += "<img width=\"720px\" src=\"data:image/png;base64,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\" />";
  zprava += "<input type=\"submit\">";
  zprava += "</form>";
  zprava += "</body></html>";
  server.send(200, "text/html", zprava);
}

// Chybova stranka, ktera udela redirect na hlavni
void error404() {
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

// Pripojit se na WiFi, kterou jsme nakonfigurovali.
// Nefunguje? Proste resni ESP pres 5s tlacitko
void startWifiAsStation(String nazevWifi, String hesloWifi){
  WiFi.begin(nazevWifi, hesloWifi);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Pripojeno k WiFi siti ");
  Serial.println(nazevWifi);
  Serial.print("IP adresa: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("onair")) {
    Serial.println("MDNS responder je zapnuty (onair.local) ... ");
  }

  server.on("/", indexHTML);

  server.on("/led", []() {
    /*
    int red = server.arg("red").toInt();
    int green = server.arg("green").toInt();
    int blue = server.arg("blue").toInt();
    */
    String red;
    String green;
    String blue;

    int led;

    for(led=0; led <= 23; led++) {
      p[led] = server.arg("p"+String(led));

      red = "0x"+String(p[led][1])+String(p[led][2]);
      green = "0x"+String(p[led][3])+String(p[led][4]);
      blue = "0x"+String(p[led][5])+String(p[led][6]);

      //Serial.println("led["+String(led)+"] red["+red+"] green["+String(strtol(green.c_str(), NULL, 16))+"] blue["+blue+"]");

      setColor(led, strtol(red.c_str(), NULL, 16), strtol(green.c_str(), NULL, 16), strtol(blue.c_str(), NULL, 16), 100);
    }
    
    //setColor(20, 0x00, 0x00, 0xFF, 100);

    /*
    for(led=0; led <= 23; led++) {
     if (red >= 0 && green >= 0 && blue >= 0) {
        setColor(led, red, green, blue, 100);
      }
    }
    */
    
    indexHTML();
  });

  server.onNotFound(error404);

  server.begin();
  Serial.println("HTTP server je zapnuty.");
}

//simple function which takes values for the red, green and blue led and also
//a delay
void setColor(int led, int redValue, int greenValue, int blueValue, int delayValue)
{
  pixels.setPixelColor(led, pixels.Color(redValue, greenValue, blueValue)); 
  pixels.show();
  delay(delayValue);
}

void setup() 
{
  // Seriova linka
  Serial.begin(115200, SERIAL_8N1);

  pixels.begin(); // This initializes the NeoPixel library.

  int pickers;
  for(pickers=0; pickers <= 23; pickers++) {
    p[pickers] = "#000000";
  }

  /*
  int led;
  for(led=0; led <= 23; led++) {
    setColor(led, 0xFF, 0x00, 0x00, 100);
  }
  */

  Serial.println("Starting WiFi as Station ...");
  startWifiAsStation(my_ssid, my_pass);
}

void loop() 
{
  server.handleClient();
  MDNS.update();
  delay(10);
}