/*
    Use this sketch to upload a .bin file to your ESP8266/NodeMCU via web browser

    Original file (WebUpdater.ino) modified by Marco Santoru (January 2020)
    to display IP Address and MAC Address in Serial Monitor, to make it easier
    to upload without having to find the board's IP Address from your Modem/Router.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>


/*  REPLACE SSID AND PASSWORD WITH YOUR OWN  */
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

String IP_ADDRESS;
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;


void setup(void) {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  httpServer.on("/", [&]() {
    httpServer.send(200, "text/html", RedirectToUpdate());  // IndexPage
  });

  // Print IP Address, MAC Address and URL
  Serial.println();
  Serial.println("HTTPUpdateServer ready!");
  Serial.println();
  IP_ADDRESS = WiFi.localIP().toString();
  Serial.print("IP ADDRESS: ");
  Serial.println(IP_ADDRESS);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println();

  Serial.print("Open  http://");
  Serial.print(IP_ADDRESS);
  Serial.print("/update  in your browser to upload your .bin file from your computer");
  Serial.println();

}

void loop(void) {
  httpServer.handleClient();
}


String RedirectToUpdate(){
  String ptr =  "<!DOCTYPE html>";
  ptr +="<html>";
  ptr +="<head>";
  ptr +="<meta http-equiv='refresh' content='3;url=/update' />";  /*  Redirect to /update after 3 seconds  */
  ptr +="</head>";
  ptr +="<body>";
  ptr +="If you are not redirected automatically, go to <a href='/update'>http://" + IP_ADDRESS + "/update</a> to upload your .bin file from your computer";
  ptr +="</body>";
  ptr +="</html>";
  return ptr;
}
