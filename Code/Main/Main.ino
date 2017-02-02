// Preprocessors
#include <Servo.h>

// Pin defination
#define D1 5
#define D2 4
#define D5 14
#define D6 12

// Interrrupter Trigger threshold
#define High 700
#define Low 300

// Create global variables
Servo myServo;
int reading = 0;
int step_count = 0;

// Run Once
void setup() {
  //Serial.begin(115200);
  
  // Define input and output
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  
  pinMode(A0, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);

  // Attach servo to pin
  myServo.attach(D2);

  // Attach interrupts to corresponding push buttons
  attachInterrupt(digitalPinToInterrupt(D5),ISR_1, HIGH);
  attachInterrupt(digitalPinToInterrupt(D6),ISR_2, HIGH);

  // Start up test
  digitalWrite(D1, HIGH);
  delay(15);
  blindsControl(110,59);
  delay(15);
  blindsControl(92,1);
  delay(15);
  digitalWrite(D1, LOW);
  delay(15);
  digitalWrite(D1, HIGH);
  delay(15);
  blindsControl(70,59);
  delay(15);
  digitalWrite(D1, LOW);
}

void loop() {
  yield();
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
  }
  //Serial.println("stopping servo");
  myServo.write(92);
  delay(15);
  digitalWrite(D1, LOW);
  //Serial.println("servo stopped");
}

// Interrupt Service Routine one
void ISR_1() {
  //Serial.println("D5 Interrupted");
  blindsControl(110,59);
}

// Interrupt Service Routine two
void ISR_2() {
  //Serial.println("D6 Interrupted");
  blindsControl(70,59);
}

