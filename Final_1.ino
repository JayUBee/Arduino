#define CW 6
#define CCW 5
//#define enable0 13   //pins for first MegaMoto
#define MTR 3

#define switch0 7   //Up button
#define switch1 8   //Down button

#define hall0 2 //interrupt pins for hall effect sensors

/*#define CW 12
#define CCW 13*/

int enable = 0; //enable pin for megaMoto

int count[] = {0};//Actuator

int sw[] = {1, 1}; //switch up, switch down
int prev[] = {0, 0};//previous switch state

int currentPos = 0;//current position
int threshold = 1;
int destination = 0;

bool forwards = false;
bool backwards = false;// motor states

void setup() {
  //pinMode(PWMA0, OUTPUT);
  //pinMode(PWMB0, OUTPUT);//set PWM outputs

  pinMode(CW, OUTPUT);
  pinMode(CCW, OUTPUT);
  pinMode(MTR, OUTPUT);

 // pinMode(enable0, OUTPUT);
  //digitalWrite(enable0, LOW);//set enable and turn board OFF

  pinMode(switch0, INPUT);
  pinMode(switch1, INPUT);
  digitalWrite(switch0, HIGH);
  digitalWrite(switch1, HIGH);//set up/down switch, enable enternal relays

  pinMode(hall0, INPUT);
  digitalWrite(hall0, LOW);//set hall, set low to start for rising edge

  attachInterrupt(0, speed0, RISING); //enable the hall effect interupts

  Serial.begin(9600);
}//end setup

void loop() {
  ReadInputs();//check input button, calculate speeds

  if (sw[0] == 0 && sw[1] == 1 && backwards == false) destination = currentPos - 1000;
  else if (sw[0] == 1 && sw[1] == 0 && forwards == false) destination = currentPos + 1000;

  if ((destination >= (currentPos - threshold)) && (destination <= (currentPos + threshold))) stopMoving();
  else if (destination > currentPos) goForwards();
  else if (destination < currentPos) goBackwards();

  Serial.print("Counts:      "); Serial.println(count[0]);
  Serial.print("currentPos:  "); Serial.println(currentPos);
  Serial.print("Destination: "); Serial.println(destination);
  Serial.print("\n");
  delay(50);

  for (int i = 0; i <= 1; i++) prev[i] = sw[i]; //store switch values as previous values
}//end loop


void speed0() {
  //Serial.println("Update 1");
  if (forwards == true) count[0]++; //if moving forwards, add counts
  else if (backwards == true) count[0]--; //if moving back, subtract counts
}//end speed0

void ReadInputs() {

  sw[0] = digitalRead(switch0), sw[1] = digitalRead(switch1);//check switches
  currentPos = count[0];
}//end read inputs

void goForwards()
{
  forwards = true;
  backwards = false;
  Serial.println("Moving forwards");
  //digitalWrite(enable0, HIGH);//enable board
  analogWrite(CW, 255);
  analogWrite(CCW, 0);
  digitalWrite(MTR, HIGH);
  //Serial.print(" Speeds "), Serial.print(spd[0]), Serial.print(", "), Serial.print(spd[1]);
  //Serial.print(" Counts "), Serial.println(count[0]);
 // analogWrite(PWMA0, 255);
 // analogWrite(PWMB0, 0);//apply speeds
}//end goForwards

void goBackwards()
{
  forwards = false;
  backwards = true;
  Serial.println("Moving backwards");
 // digitalWrite(enable0, HIGH);//enable board
  analogWrite(CW,0);
  analogWrite(CCW, 255);
  digitalWrite(MTR,HIGH);
  //Serial.print(" Speeds "), Serial.print(spd[0]), Serial.print(", "), Serial.print(spd[1]);
  //Serial.print(" Counts "), Serial.println(count[0]);
 // analogWrite(PWMA0, 0);
  //analogWrite(PWMB0, 255);//apply speeds
}//end goBackwards

void stopMoving()
{
  forwards = false;
  backwards = false;
  Serial.println("Stopped");
  analogWrite(CW, 0);
  analogWrite(CCW, 0);
 // analogWrite(PWMA0, 0);
 // analogWrite(PWMB0, 0);//set speeds to 0
  delay(10);

  //digitalWrite(enable0, LOW);//disable board
}//end stopMoving
