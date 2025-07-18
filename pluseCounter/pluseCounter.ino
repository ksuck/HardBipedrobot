#define ENCODER_PIN 13
#define RESOLUTION 40  // 20 ช่อง x 2 edge = 40 counts ต่อรอบ

// Motor A (PWM ผ่าน ledc)
const int motor1Pin1 = 18;
const int motor1Pin2 = 19;
const int enable1Pin = 26;

int dutyCycle = 255;

volatile unsigned long gulCount = 0;
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 4000;  // หน่วงขอบสัญญาณ (us)

const unsigned int targetRevolutions = 5;
const unsigned int targetCount = targetRevolutions * RESOLUTION;

bool motorRunning = false;

// ประกาศฟังก์ชัน interrupt handler ไว้ก่อนใช้งาน
void IRAM_ATTR COUNT();

void setup() {
  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(ENCODER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), COUNT, CHANGE);

  stopMotor();
  Serial.println("พิมพ์ 'start' แล้วกด Enter เพื่อเริ่มหมุนมอเตอร์ 5 รอบ");
}

void loop() {
  if (!motorRunning && Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input == "start") {
      gulCount = 0;
      attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), COUNT, CHANGE);
      motorRunning = true;
      runMotorForward();
      Serial.println("มอเตอร์เริ่มหมุน...");
    }
  }

  if (motorRunning && gulCount >= targetCount - 1) {
    detachInterrupt(digitalPinToInterrupt(ENCODER_PIN));  // ปิดการนับก่อน
    stopMotor();
    delay(10);  // ให้ระบบพักก่อนเผื่อ WDT
    motorRunning = false;

    Serial.print("ครบ ");
    Serial.print(targetRevolutions);
    Serial.print(" รอบแล้ว (นับได้ ");
    Serial.print(gulCount);
    Serial.println(" ครั้ง)");
    Serial.println("พิมพ์ 'start' เพื่อเริ่มใหม่");
  }
}

void runMotorForward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  analogWrite(enable1Pin, dutyCycle);
}

void stopMotor() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(enable1Pin, 0);
}

void IRAM_ATTR COUNT() {
  unsigned long now = micros();
  if (now - lastInterruptTime > debounceDelay) {
    gulCount++;
    Serial.println(gulCount);
    lastInterruptTime = now;
  }
}
