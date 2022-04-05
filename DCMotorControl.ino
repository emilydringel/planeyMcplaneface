#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>


//set up to connect to an existing network (e.g. mobile hotspot from laptop that will run the python code)
const char* ssid = "AndroidAP";
const char* password = "87654321";
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  //  port to listen on
char incomingPacket[255];  // buffer for incoming packets

String address= "http://134.122.113.13/bm3027/running";


/** Task handle for the light value read task */
TaskHandle_t APIlistenHandle = NULL;

int in1Pin = 12;      // Define L293D channel 1 pin
int in2Pin = 2;      // Define L293D channel 2 pin
int enable1Pin = 13;  // Define L293D enable 1 pin
int channel = 0;

bool RUNTASK = false;


void setup() {
  int status = WL_IDLE_STATUS;
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to wifi");
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  ledcSetup(channel,1000,11);         //Set PWM to 11 bits, range is 0-2047
  ledcAttachPin(enable1Pin,channel);


  //setup server listening thread

  
  xTaskCreatePinnedToCore(
      APIlisten,                       /* Function to implement the task */
      "APIlisten",                    /* Name of the task */
      4000,                           /* Stack size in words */
      NULL,                           /* Task input parameter */
      5,                              /* Priority of the task */
      &APIlistenHandle,                /* Task handle. */
      1);                             /* Core where the task should run */

 


  //if (tempTaskHandle == NULL) {
    //Serial.println("Failed to start task for temperature update");
  //} 
}

void loop(){
  if(RUNTASK){
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    ledcWrite(channel, 750);
    Serial.println("Val is TRUE");
  }
  else{
    Serial.println("Val is FALSE");
    ledcWrite(channel, 0);
  }
  delay(2000);
}


void APIlisten(void *pvParameters) {
  Serial.println("tempTask loop started");
  while (1) // tempTask loop
  {
    if((WiFi.status() == WL_CONNECTED)) {
      HTTPClient http;
      http.begin(address);
   
      int httpCode = http.GET(); // start connection and send HTTP header
      if (httpCode == HTTP_CODE_OK) { 
          String response = http.getString();
          if (response.equals("false")) {
              // Do not run sculpture, perhaps sleep for a couple seconds
              //Serial.println("FALSE");
              RUNTASK = false;
          }
          else if(response.equals("true")) {
              //Serial.println("TRUE");
              
              // Run sculpture
              RUNTASK = true;
          }
          Serial.println("Response was: " + response);
      } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
       
    }
    delay(1000); // sleep for half of a second
  }
}
