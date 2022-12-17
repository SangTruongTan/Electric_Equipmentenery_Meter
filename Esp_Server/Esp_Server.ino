#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// Typedef
struct data_t {
	String I;
	String P;
	String E;
};

// Function prototype
data_t parse(String inp);



// Replace with your network credentials
const char* ssid     = "P403_2.4G";
const char* password = "bontramleba";

// REPLACE with your Domain name and URL path or IP address with path
String serverName = "http://capnhatdiennuoc.000webhostapp.com/insert2.php";

HTTPClient http;
WiFiClient wifiClient;

// Variable declaration
data_t Parameter;

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
  //Initialize values
  Parameter.I = "0";
  Parameter.P = "0";
  Parameter.E = "0";
}

void loop() {
  //Receive serial data
  if(Serial.available()) {
    String Data = Serial.readStringUntil('\n');
    if(Data != "") {
      Parameter = parse(Data);
      Serial.println("Received data");
    }
  }
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    // Url
    String url = serverName + "?Dongdien=" + Parameter.I + "&Congsuat=" + Parameter.P + "&Power=" + Parameter.E + "";
    Serial.println(url);
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Your Domain name with URL path or IP address with path
    if(http.begin(wifiClient, url)) {
      Serial.println("HTTP GET...");
      int httpCode = http.GET();
      String payload = http.getString(); 
      if(httpCode > 0) {
        Serial.printf("HTTP code:%d\r\n", httpCode);
        Serial.println(payload);
      } else {
        Serial.printf("HTTP GET Failed with code:%d\r\n", httpCode);
      }
      // Free resources
      http.end();
    } else {
      Serial.println("Unable to connect via HTTP");
    }
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Iteration time
  delay(60000);  
}

data_t parse(String input) {
    data_t retval;
    int i = 0;
    while(input != "") {
        int index = input.indexOf(',');
        String token = input.substring(0, index);
        switch (i) {
            case 0:
                retval.I = token;
                break;
            case 1:
                retval.P = token;
                break;
            default:
                retval.E = token;
                break;
        }
        if(index != -1) {
            input.remove(0, index + 1);
        } else {
            input.remove(0, input.length());
        }
        i++;
    }
    return retval;
}