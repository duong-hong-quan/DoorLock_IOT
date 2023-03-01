#include <Servo.h> // Thư viện servo motor

#define ledRedPin 4
#define ledGreenPin 5
Servo myServo; // Tạo đối tượng servo

int switchPin = 2; // Chân của công tắc
int servoPin = 9; // Chân của servo motor

void setup() {
  myServo.attach(servoPin); // Khởi tạo servo
  pinMode(switchPin, INPUT_PULLUP); // Thiết lập chân của công tắc là INPUT_PULLUP
}

void loop() {
  if (isSwitchPressed()) { // Kiểm tra xem công tắc đã được nhấn chưa
    unlockDoor(); // Mở khóa cửa
  }else{
    lockDoor();
  }
}

// Hàm kiểm tra xem công tắc đã được nhấn chưa
// bool isSwitchPressed() {
//   return digitalRead(switchPin) == LOW;
// }

bool isSwitchPressed() {
  static bool previousState = HIGH;
  bool currentState = digitalRead(switchPin);
  bool isPressed = currentState == LOW && previousState == HIGH;
  previousState = currentState;
  return isPressed;
}


// Hàm mở khóa cửa bằng cách quay servo motor
void unlockDoor() {
  digitalWrite(ledRedPin, LOW);
  digitalWrite(ledGreenPin, HIGH);
  myServo.write(90);  // Quay servo motor ở góc 90 độ
  delay(1000);        // Dừng 1 giây
  myServo.write(0);   // Quay servo motor về góc 0 độ
}

// Hàm  khóa cửa bằng cách quay servo motor
void lockDoor() {
  digitalWrite(ledRedPin, HIGH);
  digitalWrite(ledGreenPin, LOW);
  myServo.write(0);   // Quay servo motor ở góc 0 độ
  delay(1000);        // Dừng 1 giây
  myServo.write(90);  // Quay servo motor về góc 90 độ
}
