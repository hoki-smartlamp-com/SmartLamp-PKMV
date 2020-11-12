#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <Adafruit_NeoPixel.h>
#include "FS.h"
#include <ArduinoJson.h>
#include "FirebaseESP8266.h"

#include "index_html.h"
#include "sw_js.h"
#include "style_css.h"
#include "manifest_json.h"

#define FIREBASE_HOST "hoki-smartlamp-c540f.firebaseio.com"
#define FIREBASE_AUTH "l08XZktIBxpTvcyZq6BeyteLzSPDF492cdJuPRdP"

FirebaseData firebase_power,firebase_variasi,firebaseData;

#define NUM_PIX 16
const char *ssid = "Hoki_SmartLamp";
//const char* password = "Hoki_SmartLamp";
unsigned char red, green, blue, bright;
String wifi_ssid, wifi_password, wifi_status;
bool power = true;
bool wifi_try = false;
bool turnOnWifi = false;
bool wifiOn = false;
int variasi = 6; // 0 = tanpa variasi
int retryCounter = 0;

char setWifi[128];

String lamp_id = "112opso"; // ID Lampu
FirebaseJson json_color;

DynamicJsonDocument json_wifi(200);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIX, D4, NEO_GRB + NEO_KHZ800);

AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssid);
  wifi_status = "disconnected";
  Serial.println(wifi_status);
  
//  WiFi.begin("Farizawm", "Fariza2496");
  
  Serial.print("set");
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());

  red = 255;
  green = 255;
  blue = 255;
  bright = 50;

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/sw.js", [](AsyncWebServerRequest *request){
    request->send_P(200, "application/javascript", sw_js);
  });
  server.on("/manifest.json", [](AsyncWebServerRequest *request){
    request->send_P(200, "application/json", manifest_json);
  });
  server.on("/app.js", [](AsyncWebServerRequest *request){
    request->send_P(200, "application/javascript", app_js);
  });
  server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    json_wifi["ssid"] = wifi_ssid;
    json_wifi["password"] = wifi_password;
    json_wifi["status"] = wifi_status;
    serializeJson(json_wifi, setWifi);

    request->send_P(200, "application/json", setWifi);
  });
  server.on("/wifi/set", HTTP_POST, [](AsyncWebServerRequest * request) {
    String ssidMsg = request->getParam("ssid", true)->value();
    String passwordMsg = request->getParam("password", true)->value();

    if (request->hasParam("ssid", true))
    {
      Serial.println(ssidMsg);
      Serial.println(passwordMsg);
      wifi_ssid = ssidMsg;
      wifi_password = passwordMsg;
      wifi_try = true;
      wifi_status = "connecting";
    }
    else return;
    String redirect = "<html><script>window.location.href = \"/\";</script></html>";
    request->send(200, "text/html", redirect);
  });
  
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.ico", "image/png");
  });
  server.on("/img/icons-192.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/icons-192.png", "image/png");
  });
  server.on("/img/icons-512.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/icons-512.png", "image/png");
  });
  server.begin();
  pixels.begin();
  pixels.setBrightness(bright);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  beginFirebaseStream();
}

void loop() {
  if(turnOnWifi && !wifiOn) {
    WiFi.softAP(ssid);
    Serial.println("AP Hidup");
    wifiOn = true;
    turnOnWifi = true;
  }
  if(wifi_try) {
    WiFi.begin(wifi_ssid, wifi_password);
    int second = 0;
    int stats;
    while (WiFi.status() != WL_CONNECTED)
    {
      stats = WiFi.status();
      wifi_status = "connecting";
      Serial.println(wifi_status);
      delay(1000);
      second++;
      if (stats == 1 || second >= 10) break;
    }
    if (stats == 1 || second >= 10) wifi_status = "disconnected";
    else {
      wifi_status = "connected";
      WiFi.softAPdisconnect(true);
      wifiOn = false;
      turnOnWifi = false;
      Serial.println("AP Mati");
    }
    Serial.println(wifi_status);
    wifi_try = false;
  }
  if(wifi_status == "connected") getFirebaseData();

  switch(variasi) {
    case 0:
      for (int i = 0; i < NUM_PIX; i++) {
        pixels.setPixelColor(i, pixels.Color(red, green, blue));
        pixels.show();
      }
      break;
    case 1:
      colorWipe(pixels.Color(255,   0,   0), 50); // Red
      colorWipe(pixels.Color(  0, 255,   0), 50); // Green
      colorWipe(pixels.Color(  0,   0, 255), 50); // Blue
      colorWipe(pixels.Color(255, 255, 255), 50); // White
      break;
    case 2:
      rainbowFade(3, 3, 1);
      break;
    case 3:
      rainbowFade(3, 3, 0);
      break;
    case 4:
      colorWave(50);
      break;
    case 5:
      rainbow(15);
      break;
    case 6:
      rainbowCycle(15);
      break;
    default:
      pulseWhite(5);
      break;
  }

  if (!power) {
    pixels.setBrightness(0);
    //    Serial.println("off");
  } else {
    pixels.setBrightness(bright);
    //    Serial.println("on");
  }
  delay(500);
}

void beginFirebaseStream() {
  if (!Firebase.beginStream(firebaseData, "/lamp/" + lamp_id + "/warna")) {
      Serial.println("------------------------------------");
      Serial.println("Can't begin stream connection in \"warna\"...");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
//      variasi = -1;
  }
  if (!Firebase.beginStream(firebase_power, "/lamp/" + lamp_id + "/power")) {
      Serial.println("------------------------------------");
      Serial.println("Can't begin stream connection in \"power\"...");
      Serial.println("REASON: " + firebase_power.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
  }
  if (!Firebase.beginStream(firebase_variasi, "/lamp/" + lamp_id + "/variasi")) {
      Serial.println("------------------------------------");
      Serial.println("Can't begin stream connection in \"variasi\"...");
      Serial.println("REASON: " + firebase_variasi.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
  }
}

void getFirebaseData() {
  if (Firebase.get(firebaseData, "/lamp/" + lamp_id + "/warna")) {
      Serial.println("PATH: " + firebaseData.dataPath());
      printResult(firebaseData);
      Serial.println();
  }
  else {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println();
      retryCounter++;
      if(retryCounter >= 6) {
        turnOnWifi = true;
        wifi_status = "disconnected";
      }
  //      variasi = -1;
  }
  
  if (Firebase.get(firebase_power, "/lamp/" + lamp_id + "/power")) {
      Serial.println("PATH: " + firebase_power.dataPath());
      printResult(firebase_power);
      Serial.println();
  } else {
      Serial.println("Get Power FAILED");
      Serial.println("REASON: " + firebase_power.errorReason());
      Serial.println();
  }
  
  if (Firebase.get(firebase_variasi, "/lamp/" + lamp_id + "/variasi")) {
      Serial.println("PATH: " + firebase_variasi.dataPath());
      printResult(firebase_variasi);
      Serial.println();
  }
  else {
      Serial.println("Get Variasi FAILED");
      Serial.println("REASON: " + firebase_variasi.errorReason());
      Serial.println();
  }
}

void setColor(String key, int value) {
  if (key == "red") {
      Serial.print("Red: ");
      Serial.println(value);
      red = value;
  } else if (key == "green") {
      Serial.print("Green: ");
      Serial.println(value);
      green = value;
  } else if (key == "blue") {
      Serial.print("Blue: ");
      Serial.println(value);
      blue = value;
  } else if (key == "bright") {
      Serial.print("Bright: ");
      Serial.println(value);
      bright = value;
  }
}

void printResult(FirebaseData &data) {
  if (data.dataType() == "int") {
      Serial.print("Variasi: ");
      Serial.println(data.intData());
      variasi = data.intData();
  }
  else if (data.dataType() == "boolean") {
      Serial.print("Power: ");
      Serial.println(data.boolData() == 1 ? "true" : "false");
      power = data.boolData() == 1 ? true : false;
  }
  else if (data.dataType() == "json") {
    FirebaseJson &json = data.jsonObject();
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;
    for (size_t i = 0; i < len; i++) {
        json.iteratorGet(i, type, key, value);
        setColor(key, value.toInt());
    }
    json.iteratorEnd();
    Serial.println();
  }
}

void setFirebase(int red, int green, int blue, int bright) {
  Serial.println("kirim");

  json_color.add("red", red);
  json_color.add("green", green);
  json_color.add("blue", blue);
  json_color.add("bright", bright);
  if (Firebase.set(firebaseData, "/lamp/" + lamp_id + "/warna", json_color))
  {
      Serial.println("PATH: " + firebaseData.dataPath());
      printResult(firebaseData);
      Serial.println();
  }
  else
  {
      Serial.println("FAILED Set Color");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println();
  }

  delay(1000);
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < NUM_PIX; i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
  }
}
void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire pixels with white at gamma-corrected brightness level 'j':
    pixels.fill(pixels.Color( pixels.gamma8(j),  pixels.gamma8(j), pixels.gamma8(j)));
    pixels.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    pixels.fill(pixels.Color( pixels.gamma8(j),  pixels.gamma8(j), pixels.gamma8(j)));
    pixels.show();
    delay(wait);
  }
}
void rainbowFade(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in pixels...

      uint32_t pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());

      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    pixels.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  if (whiteLoops > 0) {
    for(int k=0; k<whiteLoops; k++) {
      for(int j=0; j<256; j++) { // Ramp up 0 to 255
        pixels.fill(pixels.Color( pixels.gamma8(j),  pixels.gamma8(j), pixels.gamma8(j)));
        pixels.show();
      }
      delay(1000); // Pause 1 second
      for(int j=255; j>=0; j--) { // Ramp down 255 to 0
        pixels.fill(pixels.Color( pixels.gamma8(j),  pixels.gamma8(j),  pixels.gamma8(j)));
        pixels.show();
      }
    }
    delay(500); // Pause 1/2 second
  }
}
void colorWave(uint8_t wait) {
  int i, j, pixelssize, cycle;
  float ang, rsin, gsin, bsin, offset;
  static int tick = 0;
  
  pixelssize = pixels.numPixels();
  cycle = pixelssize * 25; // times around the circle...
  while (++tick % cycle) {
    offset = map2PI(tick);
    for (i = 0; i < pixelssize; i++) {
      ang = map2PI(i) - offset;
      rsin = sin(ang);
      gsin = sin(2.0 * ang / 3.0 + map2PI(int(pixelssize/6)));
      bsin = sin(4.0 * ang / 5.0 + map2PI(int(pixelssize/3)));
      pixels.setPixelColor(i, pixels.Color(trigScale(rsin), trigScale(gsin), trigScale(bsin)));
    }
    pixels.show();
    delay(wait);
  }
 
}
float map2PI(int i) {
  return PI*2.0*float(i) / float(pixels.numPixels());
}
byte trigScale(float val) {
  val += 1.0; // move range to [0.0, 2.0]
  val *= 127.0; // move range to [0.0, 254.0]
  return int(val) & 255;
}
void rainbow(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
