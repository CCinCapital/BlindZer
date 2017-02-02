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

// Interrrupter Trigger threshold
#define High 700
#define Low 300

// Create global variables
const char* ssid = "CCnTT";
const char* password = "!Gurute23394581";
int reading = 0;
int step_count = 0;

// Create WiFi Server and Servo
WiFiServer server(80);
Servo myServo;

// Run Once
void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Define input and output
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  
  pinMode(A0, INPUT);
  pinMode(D4, INPUT_PULLUP);
  pinMode(D9, INPUT_PULLUP);

  // Attach servo to pin
  myServo.attach(D2);

  // Attach interrupts to corresponding push buttons
  attachInterrupt(digitalPinToInterrupt(D4),ISR_1, FALLING);
  attachInterrupt(digitalPinToInterrupt(D9),ISR_2, FALLING);

  // Connecting to Router
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
  Serial.println("Starting Test Servo");
  digitalWrite(D1, HIGH);
  delay(15);
  blindsControl(110,59);
  delay(15);
  digitalWrite(D1, LOW);
  delay(15);
  digitalWrite(D1, HIGH);
  delay(15);
  blindsControl(70,59);
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
  
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
    
  // Match the request
  if (request.indexOf("/Servo=UP") != -1)  {
    blindsControl(110,59);
  }
  if (request.indexOf("/Servo=DOWN") != -1)  {
    blindsControl(70,59);
  }
  
  // Return the response
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

/*  Method that controls the motion of blinder
 * 
 */
void blindsControl(int roller_direction, int steps) {
  //Serial.println("defining parameters");
  boolean previous_position = false;
  boolean current_position = false;
  int interrupter_reading = analogRead(A0);
  int step_count = 0;
  //Serial.println("entering loop");
  while(step_count < steps){
    if (interrupter_reading >= High) {
      current_position = true;
    }
    else if (interrupter_reading <= Low) {
      current_position = false;
    }
    if (previous_position != current_position) {
      previous_position = current_position;
      step_count += 1;
    }
    //Serial.println("in loop");
    digitalWrite(D1, HIGH);
    delay(15);
    myServo.write(roller_direction);
    interrupter_reading = analogRead(A0);
    //Serial.println(interrupter_reading);
  }
  //Serial.println("stopping servo");
  myServo.write(92);
  delay(15);
  digitalWrite(D1, LOW);
  //Serial.println("servo stopped");
}

// Interrupt Service Routine one
void ISR_1() {
  //Serial.println(digitalRead(D4));
  blindsControl(110,59);
}

// Interrupt Service Routine two
void ISR_2() {
  //Serial.println(digitalRead(D9));
  blindsControl(70,59);
}

