#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

// shared variable
TaskHandle_t t0;
TaskHandle_t t1;

const char* ssid = "YAJATI";
const char* password = "m7318014";
WiFiClient espClient;


void setup() {
  pinMode(0, INPUT);
  Serial.begin(115200);
  xTaskCreatePinnedToCore(
    tLedFunc,  /* Task function. */
    "LED",     /* name of task. */
    10000,        /* Stack size of task */
    NULL,         /* parameter of the task */
    1,            /* priority of the task */
    &t0,          /* Task handle to keep track of created task */
    0);           /* pin task to core 0 */
  delay(10);
  
  // create a task to handle led LED_BUILTIN (core 0)
  xTaskCreatePinnedToCore(
    tButtonFunc,     /* Task function. */
    "Button",        /* name of task. */
    10000,        /* Stack size of task */
    NULL,         /* parameter of the task */
    1,            /* priority of the task */
    &t0,          /* Task handle to keep track of created task */
    0);           /* pin task to core 1 */
  delay(10);

  Serial.begin(115200);
  xTaskCreatePinnedToCore(
    wifiFunc,  /* Task function. */
    "wifi",     /* name of task. */
    10000,        /* Stack size of task */
    NULL,         /* parameter of the task */
    1,            /* priority of the task */
    &t1,          /* Task handle to keep track of created task */
    1);           /* pin task to core 0 */
  delay(10);
}

void loop() {
  for (int i = 0; i == i; i++) {
    Serial.print("Counter is changed to ");
    Serial.println(i);
    delay(1000);
  }
}

// tButtonFunc: check button for push and releae events
void tButtonFunc(void *params) {
  // local variables
  bool lastState = false;

  // setup
  //Serial.print("tButtonFunc running on core ");
  //Serial.println(xPortGetCoreID());

  // loop
  while (true) {
    bool curState = digitalRead(0) == LOW;
    if (!lastState && curState) {     // push
      Serial.print("tButtonFunc running on core ");
      Serial.println(xPortGetCoreID());
      Serial.println("tButtonFunc: push");
    } else if (lastState && !curState) { // release
      Serial.print("tButtonFunc running on core ");
      Serial.println(xPortGetCoreID());
      Serial.println("tButtonFunc: release");
    }
    lastState = curState;
    delay(10);
  }
}

// tLedFunc: blinks every 1000ms
void tLedFunc(void *params) {
  // local variable

  // setup
  
  Serial.print("tLedFunc running on core ");
  Serial.println(xPortGetCoreID());

  // loop
  while (true) {
    //Serial.print("tLedFunc running on core ");
    //Serial.println(xPortGetCoreID());
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
  }
}

void wifiFunc(void *params) {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print("wifi running on core ");
    //Serial.println(xPortGetCoreID());
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
