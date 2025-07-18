/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-dc-motor-l298n-motor-driver-control-speed-direction/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.  
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/
// Motor A
int motor1Pin1 = 22; 
int motor1Pin2 = 23; 
int enable1Pin = 25; 

int motor2Pin1 = 18; 
int motor2Pin2 = 19; 
int enable2Pin = 26; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 185;

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // configure LEDC PWM
  ledcAttachChannel(enable1Pin, freq, resolution, pwmChannel);
  ledcAttachChannel(enable2Pin, freq, resolution, pwmChannel);

  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");
}

void loop() {
  // Move the DC motor forward at maximum speed
  Serial.println("Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin2, HIGH); 
  delay(2000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);

  // Move DC motor backwards at maximum speed
  Serial.println("Moving Backwards");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin2, LOW); 
  delay(2000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);

  // Move DC motor forward with increasing speed
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin2, LOW);
  while (dutyCycle <= 255){
    ledcWrite(enable1Pin, dutyCycle);   
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 185;
}