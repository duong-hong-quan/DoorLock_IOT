#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <Wire.h>;
#include <LiquidCrystal_I2C.h>;

#define password "1234"          // Mật khẩu mặc định
#define MAX_PASSWORD_ATTEMPTS 3  // Số lần nhập sai mật khẩu tối đa


const int relay_pin = 10
const byte ROWS = 4;
const byte COLS = 4;

LiquidCrystal_I2C lcd(0x27,16,2);


char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  mySerial.begin(57600);
  finger.begin(57600);
  lcd.init();  //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình
  lcd.backlight();
  if (finger.verifyPassword()) {
    Serial.println("Sensor OK");
  } else {
    Serial.println("Sensor not found");
    while (1) {
      delay(1);
    }
  }
}

void loop() {
  lcd.setCursor(0, 1);
  getFingerprintID();
  unlockDoor();
}

void getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");

      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");

      return;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");

      return;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");

      return;
    default:
      Serial.println("Unknown error");

      return;
  }

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");

      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");

      return;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");

      return;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");

      return;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");

      return;
    default:
      Serial.println("Unknown error");

      return;
  }

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerprint identified");
    lcd.print("Fingerprint identified...");
    delay(5000);
    lcd.clear();
    digitalWrite(relay_pin, HIGH);  // Kích hoạt relay để mở cửa
    delay(3000);             // Chờ 3 giây để cửa mở hoàn toàn
    digitalWrite(relay_pin, LOW);   // Tắt relay
    return;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.print("Communication error...");
    delay(5000);
    lcd.clear();
    return;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Fingerprint not found");
    lcd.print("Fingerprint not found...");
    delay(5000);
    lcd.clear();
  } else {
    Serial.println("Unknown error");
    lcd.print("Unknown error...");
    delay(5000);
    lcd.clear();
    return;
  }
}

void unlockDoor() {
  Serial.println("Enter password:");
  lcd.print("Enter password:...");
  delay(5000);
  lcd.clear();
  String enteredPassword = "1234";
  byte passwordAttempts = 0;

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '#') {
        if (enteredPassword == password) {
          Serial.println("Password correct");
          Serial.println("Door unlocked");
          lcd.println("Password correct!");
          lcd.println("Door unlocked!");
          delay(5000);
          lcd.clear();
          digitalWrite(relay_pin, HIGH);  // Kích hoạt relay để mở cửa
          delay(3000);             // Chờ 3 giây để cửa mở hoàn toàn
          digitalWrite(relay_pin, LOW);   // Tắt relay
          break;
        } else {
          Serial.println("Password incorrect");
          lcd.println("Password incorrect!");
          delay(5000);
          lcd.println("Attempt Password 3 times !");
          delay(5000);

          lcd.clear();
          passwordAttempts++;
          enteredPassword = "";
          if (passwordAttempts >= MAX_PASSWORD_ATTEMPTS) {
            Serial.println("Too many incorrect attempts");
            lcd.println("Too many incorrect attempts !");
            delay(5000);
            lcd.clear();
            return;
          }
        }
      } else {
        enteredPassword += key;
      }
    }
  }
}