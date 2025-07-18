#define ENCODER_PIN 13
#define RESOLUTION 40  // 20 รู x 2 edge

// Motor A
int motor1Pin1 = 22;
int motor1Pin2 = 23;
int enable1Pin = 25;

// Motor B
int motor2Pin1 = 18;
int motor2Pin2 = 19;
int enable2Pin = 26;

int dutyCycle = 255;  // ใช้ได้กับ Arduino UNO, ถ้า ESP32 ต้องเปลี่ยนเป็น ledcWrite()

volatile unsigned long gulCount = 0;
unsigned long gulStart_Timer = 0;
unsigned long gulStart_Read_Timer = 0;
short gsRPM = 0;

bool testMotorA = true;  // เปลี่ยนเป็น false เพื่อทดสอบมอเตอร์ B

volatile unsigned long lastInterruptTime = 0;   // เก็บเวลา ISR ล่าสุด
const unsigned long debounceDelay = 4000;       // debounce delay 4 ms

void setup() {
  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  pinMode(ENCODER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), COUNT, CHANGE);

  gulStart_Timer = millis();
  gulStart_Read_Timer = millis();
}

void loop() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  analogWrite(enable1Pin, dutyCycle);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(enable2Pin, dutyCycle);

  if ((millis() - gulStart_Read_Timer) >= 1000) {
    gsRPM = usRead_RPM();
    Serial.print("RPM: ");
    Serial.println(gsRPM);
    gulStart_Read_Timer = millis();

    // ทดสอบสลับมอเตอร์ทุก 5 วินาที
    static unsigned long lastToggle = millis();
    if (millis() - lastToggle > 5000) {
      testMotorA = !testMotorA;
      lastToggle = millis();
    }
  }
}

short usRead_RPM(void) {
  unsigned long ulRPM = 0;
  unsigned long ulTimeDif = millis() - gulStart_Timer;

  detachInterrupt(digitalPinToInterrupt(ENCODER_PIN));

  ulRPM = (60000 * gulCount) / (ulTimeDif * RESOLUTION);
  gulCount = 0;
  gulStart_Timer = millis();

  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), COUNT, CHANGE);

  return (short)ulRPM;
}

void COUNT(void) {
  unsigned long now = micros();
  if (now - lastInterruptTime > debounceDelay) {
    gulCount++;
    lastInterruptTime = now;
  }
}
