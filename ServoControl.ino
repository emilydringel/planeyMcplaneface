#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>


//set up to connect to an existing network (e.g. mobile hotspot from laptop that will run the python code)
const char* ssid = "AndroidAP";
const char* password = "87654321";
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  //  port to listen on
char incomingPacket[255];  // buffer for incoming packets

String address= "http://134.122.113.13/bm3027/running";

/** Task handle for the light value read task */
TaskHandle_t APIlistenHandle = NULL;
bool RUNTASK = false;






//PINSTUFF
//Servo
Servo myservo;  // create servo object to control a servo
Servo rudderServo;
Servo eleServo;

int posVal = 0;    // variable to store the servo position
int servoPinRudder = 15; // Servo motor pin1
int servoPinElevator = 12;

int elePos = 90;
int rudPos = 90;





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


  rudderServo.setPeriodHertz(50);           // standard 50 hz servo
  eleServo.setPeriodHertz(50);           // standard 50 hz servo
  rudderServo.attach(servoPinRudder, 500, 2500);  // attaches the servo on servoPin to the servo 
  eleServo.attach(servoPinElevator, 500, 2500);
  
}

void loop(){
  if(RUNTASK){

    rudPos = int(random(20, 160));
    elePos = int(random(60, 120));

    Serial.print("Rudder: ");
    Serial.print(rudPos);
    Serial.print(" Elevator: ");
    Serial.println(elePos);

    rudderServo.write(rudPos);
    eleServo.write(elePos);
    

    delay(1000);
    
  }
  else{
    //return to default angle
    rudPos = 90;
    elePos = 90;
    
    rudderServo.write(rudPos);
    eleServo.write(elePos);
    
  }
  delay(500);
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
