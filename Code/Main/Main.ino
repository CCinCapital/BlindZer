/*  
 *   ----------------------------About---------------------------- 
 *   This Program controls a wireless enabled device that rolls window blinds
 *   -- Author: Can Chen 
 *   -- More Details at: https://github.com/CCinCapital/Blinder/
 *   
 *   ----------------------------Brief----------------------------
 *   User may send commands remotely(HTTP request) to the servo to control its behavior  
 *   
 *   --------------------------Hareware---------------------------- 
 *   The device contains : 
 *    - one NodeMCU v0.9 (ESP8266-12) 
 *    - one TowerPro MG996R Servo
 *    - one NMOS
 *    - two Push Button
 *   ***************************Note*******************************
 *     Make sure to modify SSID(Line 54) and PASSWORD(Line 55) 
 *     to connect the device to your local WLAN
 */

// Preprocessors
#include <ESP8266WiFi.h>
#include <Servo.h>

// Pin defination
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1

// optical Interrrupter Trigger threshold
#define IR_Through 700
#define IR_Blocked 300

// servo rotation direction and default steps
/* 
 *  Note: Servo was modified to think it's always at the middle (92) position
 *        so, number greater than 92 will cause it to rotate clockwise
 *        and number smaller than 92 will cause it to rotate counter clockwise
 *        by writing 92 to the servo will cause it to stop (idealy)
 *        -- the difference between numbers determines the rotation speed
*/       
#define Servo_Clockwise 70
#define Servo_Counter_Clockwise 110
#define Servo_Half_Cycle 30    // One full rotation of the servo (determined by the Optical Encoder Whell's resulotion)

// SSID and Password used to connect to Router
const char* ssid = "YourWiFiNameHere";
const char* password = "YourWiFiPasswordHere";

// Create WiFi Server and Servo
WiFiServer server(80);
Servo myServo;

// Run Once
void setup() {
  // Set the baud rate for Serial communication between Board and PC
  Serial.begin(115200);
  delay(10);
  
  // Port Directions
  pinMode(D1, OUTPUT);              // NMOS
  pinMode(D2, OUTPUT);              // Servo
  
  pinMode(A0, INPUT);               // Optical Interrupter
  pinMode(D4, INPUT_PULLUP);        // Push button 1 (Pull-Down)
  pinMode(D9, INPUT_PULLUP);        // Push button 2 (Pull-Down)

  // Attach servo to pin D2
  myServo.attach(D2);

  // Set interrupt listener
  // ([Pin to listen], [Function to run upon event], [Event to listen])
  attachInterrupt(digitalPinToInterrupt(D4),ISR_1, FALLING);
  attachInterrupt(digitalPinToInterrupt(D9),ISR_2, FALLING);

  // Connecting to Router & Print out the status though Serial terminal
  Serial.println();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi Connected.");
  
  server.begin();
  Serial.println("Server started.");
  
  // Start up test
  Serial.println("Start Servo Test");
  digitalWrite(D1, HIGH);
  delay(15);
  blindsControl(Servo_Clockwise,Servo_Half_Cycle);
  delay(15);
  digitalWrite(D1, LOW);
  delay(15);
  digitalWrite(D1, HIGH);
  delay(15);
  blindsControl(Servo_Counter_Clockwise,Servo_Half_Cycle);
  delay(15);
  digitalWrite(D1, LOW);
  Serial.println("Servo Test Finished");

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the client's request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
    
  // Match the request
  if (request.indexOf("/Servo=UP") != -1)  {
    blindsControl(Servo_Clockwise,Servo_Half_Cycle);
  }
  if (request.indexOf("/Servo=DOWN") != -1)  {
    blindsControl(Servo_Counter_Clockwise,Servo_Half_Cycle);
  }

  // Send the HTTP webpage to client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Please choose the operation of Servo:");
  client.println("<br><br>");
  client.println("<a href=\"/Servo=UP\"\"><button>Roll UP </button></a>");
  client.println("<a href=\"/Servo=DOWN\"\"><button>Roll DOWN </button></a>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}

/*  --Method that controls the motion of Servo--
 *  INPUT:  roller direction
 *          steps to rotate
 *  OUTPUT: Debugging information through serial
 *  RETURN: None
 */
void blindsControl(int Roller_Direction, int Steps) {
  // Declare local variables
  int IR_Reading = analogRead(A0);    
  int Previous_Servo_Staus = IR_Reading;            // 1 => IR signal blocked (Optical Interrupter)
  int Current_Servo_Staus = Previous_Servo_Staus;   // 2 => IR signal through (Optical Interrupter)
  
  digitalWrite(D1, HIGH);                 // Power Up Servo (Close the NMOS switch)
  delay(5);                              // Wait Servo to Boot Up
  myServo.write(Roller_Direction);        // Give Servo direction to rotate
  
  int Steps_Count = 0;
  while(Steps_Count < Steps){
    //Serial.println("Rotating ");
    //Serial.print(Steps_Count);
    
    // Read status of Optical Interrupter 
    IR_Reading = analogRead(A0);   
    // Compare the reading to Triggering Threshold
    // Store the current servo status if conditions are met       
    if (IR_Reading >= IR_Through) {
      Current_Servo_Staus = IR_Through;
    }
    else if (IR_Reading <= IR_Blocked) {
      Current_Servo_Staus = IR_Blocked;
    }

    // Compare the previous servo staus and its current status
    // if difference were found, record new status, increment step count
    if (Previous_Servo_Staus != Current_Servo_Staus) {
      Previous_Servo_Staus = Current_Servo_Staus;
      Steps_Count += 1;
    }
  }
  myServo.write(92);
  delay(5);
  digitalWrite(D1, LOW);                  // Power Down Servo (Open the NMOS switch)
  //Serial.println("servo stopped");
}

/* --Interrupt Service Routine ONE--
 * --Turn Servo Clockwise about # of Steps(Default)--
 * INPUT: NONE 
 * OUTPUT: NONE
 * RETURN: NONE
 */
void ISR_1() {
  blindsControl(Servo_Clockwise,Servo_Half_Cycle);
}

/* --Interrupt Service Routine TWO--
 * --Turn Servo Counter Clockwise about # of Steps(Default)--
 * INPUT: NONE 
 * OUTPUT: NONE
 * RETURN: NONE
 */
void ISR_2() {
  blindsControl(Servo_Counter_Clockwise,Servo_Half_Cycle);
}

