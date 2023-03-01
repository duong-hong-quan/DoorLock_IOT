#include <Keypad.h>

const byte ROWS = 4; // số hàng
const byte COLS = 4; // số cột

// Khai báo các ký tự của keypad 4x4
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Khai báo các chân kết nối giữa keypad và Arduino
byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

// Khai báo đối tượng Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Khai báo mật khẩu
const char passcode[4] = {'1', '2', '3', '4'};

void setup() {
  Serial.begin(9600); // khởi tạo Serial
  pinMode(13, OUTPUT); // khởi tạo chân kết nối với LED
}

void loop() {
  char key = keypad.getKey(); // đọc giá trị từ keypad
  
  if (key) {
    Serial.print(key); // hiển thị giá trị đọc được trên Serial Monitor
    
    // Kiểm tra mật khẩu
    if (key == passcode[0] && keypad.waitForKey() == passcode[1] &&
        keypad.waitForKey() == passcode[2] && keypad.waitForKey() == passcode[3]) {
      digitalWrite(13, HIGH); // bật đèn LED
      Serial.println(" - Unlocked"); // hiển thị thông báo trên Serial Monitor
      delay(5000); // chờ 5 giây
      digitalWrite(13, LOW); // tắt đèn LED
    }
    else {
      Serial.println(" - Incorrect passcode"); // hiển thị thông báo trên Serial Monitor
    }
  }
}
