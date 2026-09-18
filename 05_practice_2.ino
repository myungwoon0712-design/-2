const int ledPin = 7;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Active Low: 초기 꺼짐 상태
}

void loop() {
  // 1. 처음 1초 동안 LED 켜기 (Active Low: LOW = ON)
  digitalWrite(ledPin, LOW);
  delay(1000);

  // 2. 다음 1초 동안 LED 5회 깜빡이기
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH); // Off
    delay(100);
    digitalWrite(ledPin, LOW);  // On
    delay(100);
  }

  // 3. LED 끄고 무한루프 종료
  digitalWrite(ledPin, HIGH); // Off

  while (1) {
    // infinite loop
  }
}
