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

volatile unsigned long targetCount = 40;  // เริ่มต้น 1 รอบ = 40 count

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
  Serial.println("พิมพ์ 'start [จำนวนเพลา]' เช่น 'start 65' เพื่อเริ่มหมุนมอเตอร์");
}

void loop() {
  if (!motorRunning && Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("start")) {
      // ตัดคำว่า start ออก
      input = input.substring(5);
      input.trim();

      unsigned long val = 40;  // ค่า default

      if (input.length() > 0) {
        val = input.toInt();
        if (val == 0) {
          Serial.println("จำนวนเพลาที่ใส่ไม่ถูกต้อง ใช้ค่าเริ่มต้น 40 count");
          val = 40;
        }
      }

      targetCount = val;
      gulCount = 0;
      attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), COUNT, CHANGE);
      motorRunning = true;
      runMotorForward();

      Serial.print("มอเตอร์เริ่มหมุน ");
      Serial.print(targetCount);
      Serial.println(" count...");
    } else {
      Serial.println("คำสั่งไม่ถูกต้อง พิมพ์ 'start [จำนวนเพลา]' เช่น 'start 65'");
    }
  }

  if (motorRunning && gulCount >= targetCount) {
    detachInterrupt(digitalPinToInterrupt(ENCODER_PIN));
    stopMotor();
    delay(10);
    motorRunning = false;

    Serial.print("ครบ ");
    Serial.print(targetCount);
    Serial.print(" count แล้ว (นับได้ ");
    Serial.print(gulCount);
    Serial.println(" ครั้ง)");
    Serial.println("พิมพ์ 'start [จำนวนเพลา]' เพื่อเริ่มใหม่");
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
