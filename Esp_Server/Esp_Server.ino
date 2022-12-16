#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// Replace with your network credentials
const char* ssid     = "P403_2.4G";
const char* password = "bontramleba";

// REPLACE with your Domain name and URL path or IP address with path
String serverName = "https://capnhatdiennuoc.000webhostapp.com/insert2.php";

HTTPClient http;
WiFiClient wifiClient;

// Variable declaration
String I = "3.3";
String P = "2.3";
String E = "5.5";

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    // Url
    String url = serverName + "?DongDien=" + I + "&CongSuat=" + P + "&Power=" + E;
    Serial.println(url);
    // Your Domain name with URL path or IP address with path
    if(http.begin(wifiClient, url)) {
      Serial.println("HTTP GET...");
      int httpCode = http.GET();
      if(httpCode > 0) {
        Serial.println("Http code:" + httpCode);
      } else {
        Serial.println("HTTP GET Failed with code:" + httpCode);
      }
      // Free resources
      http.end();
    } else{
      Serial.println("Unable to connect via HTTP");
    }
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Iteration time
  delay(60000);  
}