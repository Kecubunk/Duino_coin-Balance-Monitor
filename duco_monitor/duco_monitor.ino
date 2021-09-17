#include <WiFi.h>
#include <HTTPClient.h>
#include <Ticker.h>
#include <Update.h>
#include <Arduino_JSON.h>
#include <LiquidCrystal_I2C.h>
#include <Ticker.h>
#include "LCDChar.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

//change with youre SSID and Pass
const char* ssid     = "SSID";
const char* password = "Password";

//change DUCO_Username with youre Duinocoin user name
const char* server = "https://server.duinocoin.com/users/DUCO_Username" ;
unsigned long last_time = 0;
unsigned long timer_delay = 10000;
String json_array;
int sgnl=2;

Ticker ticker;
HTTPClient http;

const char* ca=\
"-----BEGIN CERTIFICATE-----\n" \
"MIIFLTCCBBWgAwIBAgISAyPTd1rSvT4DQGIXJoAZ8ayRMA0GCSqGSIb3DQEBCwUA\n"
"MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n"
"EwJSMzAeFw0yMTA2MTkyMDU4MTVaFw0yMTA5MTcyMDU4MTRaMB8xHTAbBgNVBAMT\n"
"FHNlcnZlci5kdWlub2NvaW4uY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB\n"
"CgKCAQEAy7FAGxAZs7CMpdqzsuEMbI5EG2H8w/JQUvSeUeI7UNAotga0q8sBHPOH\n"
"8oXVBGZ4/ABnacrrA3RLCaGp//Dys7mMCS4MH11ZTqst179boV0fivSI9nnZNkoy\n"
"uynFGspmLk51Mh+1ikymkvwee6dS8UKc6eKOaxORo6h3ttcQTi2q9d6q1KIrkvT/\n"
"qBS5b0WXwiqcY8DSpfTvVfmQ13PL0uhG1M3UOFX+nZ5vvjohIj4FRDuIzifFQvE8\n"
"swrmLN+o8VDCgNnDtTyOOgyXtYI5jEfH+sA4LyRXpMWoDOjKlxMD58A29Fg76eYA\n"
"Ygt4kzWxGHqpLrB+8Yd3K0+Yx7obawIDAQABo4ICTjCCAkowDgYDVR0PAQH/BAQD\n"
"AgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAMBgNVHRMBAf8EAjAA\n"
"MB0GA1UdDgQWBBSnWT8u7P3TGyOfwSnf72Zh2/PgyDAfBgNVHSMEGDAWgBQULrMX\n"
"t1hWy65QCUDmH6+dixTCxjBVBggrBgEFBQcBAQRJMEcwIQYIKwYBBQUHMAGGFWh0\n"
"dHA6Ly9yMy5vLmxlbmNyLm9yZzAiBggrBgEFBQcwAoYWaHR0cDovL3IzLmkubGVu\n"
"Y3Iub3JnLzAfBgNVHREEGDAWghRzZXJ2ZXIuZHVpbm9jb2luLmNvbTBMBgNVHSAE\n"
"RTBDMAgGBmeBDAECATA3BgsrBgEEAYLfEwEBATAoMCYGCCsGAQUFBwIBFhpodHRw\n"
"Oi8vY3BzLmxldHNlbmNyeXB0Lm9yZzCCAQMGCisGAQQB1nkCBAIEgfQEgfEA7wB1\n"
"AFzcQ5L+5qtFRLFemtRW5hA3+9X6R9yhc5SyXub2xw7KAAABeiZIE/0AAAQDAEYw\n"
"RAIgGbVW5wsmqvZEDirCgwPzmUL64tn1B2HQcLa/Elg4IogCIBqa/JpGPmS9eSUf\n"
"Vf6cGMtzE1K8iFPAbxJnBHukPHcuAHYA9lyUL9F3MCIUVBgIMJRWjuNNExkzv98M\n"
"LyALzE7xZOMAAAF6JkgTlQAABAMARzBFAiBj4xJxmaCkCKr78pC7+1cgzZeVgYAG\n"
"Uaba6b5DONXVQAIhAIUc89dWQm/+audNxYBn1+WxMk6dJ6XhZy5FnrSXWdS3MA0G\n"
"CSqGSIb3DQEBCwUAA4IBAQCB303kviW1wH9jsMw7wSjOZPmg3KIhciQ7bn++whST\n"
"oqfaFcSxws8OnVuQFnDe4Dy4LdB+OqDPgHwONtPhiI2z5QPGv021hRH2LxLp4oEa\n"
"Vv027EaburXWCVZ/jd/Lb9uhs18sAqcKRDHR1QuKiuFvDc3OO6TzPCbtjAowJn5R\n"
"vQorWHmNLN/bsU2VzfMI0rLBkcQU3wVjJfGeBrQvEcB7DKarv8OI47QiaCUzscwR\n"
"JL9rkESZ3iElw2BuRdfyNr71pyWnJa4IEulaoP5akYSz+3qqBdyKYIDE50Zix9Fs\n"
"R053nfyD8r+XHVsJR/1h725+gZRyMLkahAzf0iW1Lf8O\n" \
"-----END CERTIFICATE-----\n";

void tick(){
  int state = digitalRead(sgnl);
  digitalWrite(sgnl, !state);
}

void setup() {
  pinMode(sgnl,OUTPUT);
  digitalWrite(sgnl,HIGH);
  Serial.begin(115200);
  while (!Serial) continue;
  ticker.attach(0.1, tick);
  Init();
  delay(500);
  WiFi.begin(ssid, password);
  //connecting();
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }
  ticker.detach();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
      GET_Request(server);
      ticker.attach(0.1, tick);
      json_array = GET_Request(server);
      //Serial.println(json_array);
      JSONVar obj = JSON.parse(json_array);
      if (JSON.typeof(obj) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      Serial.print("User : ");
      Serial.println(obj["result"]["balance"]["username"]);     
      Serial.print("Balance : ");
      Serial.print(obj["result"]["balance"]["balance"]);
      Serial.println(" Duco");
      Serial.println("");
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("User : ");
      lcd.setCursor(6, 0);
      lcd.print(obj["result"]["balance"]["username"]);
      lcd.setCursor(0, 1);
      lcd.print("Balance : ");
      lcd.setCursor(0, 2);
      lcd.print(obj["result"]["balance"]["balance"]);
      ticker.detach();      
      delay(10000);
}

String GET_Request(const char* server) {
  HTTPClient http;    
  http.begin(server);
  int httpResponseCode = http.GET();
  
  String payload = ""; 
 
  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
  return payload;
}
