#include <Adafruit_Fingerprint.h>

// Khai báo chân kết nối với vân tay
#define fingerRX 2
#define fingerTX 3

// Khai báo đối tượng vân tay
// Nếu board mega 2560 thì thay bằng &serial1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial);

// Khai báo đối tượng LED
const int ledPin = 13;

void setup() {
  // Khởi tạo kết nối Serial
  Serial.begin(9600);
  while (!Serial); // chờ kết nối
  
  // Khởi tạo kết nối với vân tay
  finger.begin(57600);
  
  // Kiểm tra xem vân tay có đang hoạt động không
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor is active.");
  } else {
    Serial.println("Fingerprint sensor is not responding. Please check wiring.");
    while (1); // Dừng chương trình
  }
  
  // Khởi tạo chân LED
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Đợi đến khi có vân tay được đặt lên
  Serial.println("Please place your finger on the sensor.");
  while (finger.getImage() != FINGERPRINT_OK);
  
  // Lấy đặc trưng của vân tay
  if (finger.image2Tz() != FINGERPRINT_OK) {
    Serial.println("Failed to convert image to template.");
    return;
  }
  
  // So sánh đặc trưng với danh sách vân tay đã được đăng ký
  int fingerprintID = finger.fingerFastSearch();
  if (fingerprintID == FINGERPRINT_OK) {
    Serial.println("Fingerprint found. Access granted.");
    digitalWrite(ledPin, HIGH);
    delay(5000);
    digitalWrite(ledPin, LOW);
  } else {
    Serial.println("Fingerprint not found. Access denied.");
  }
  
  delay(1000); // Chờ 1 giây trước khi quét vân tay tiếp theo
}
