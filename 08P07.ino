// Arduino pin assignment
#define PIN_LED   9
#define PIN_TRIG  12   // sonar sensor TRIGGER
#define PIN_ECHO  13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec) - 25ms 적용
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficient to convert duration to distance

unsigned long last_sampling_time;   // unit: msec

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW); // turn-off Sonar 
  
  // Active Low 초기화 (시작 시 LED OFF)
  analogWrite(PIN_LED, 255);

  // initialize serial port
  Serial.begin(57600);
}

void loop() { 
  float distance;
  float duty_cycle = 255.0; // 기본값: 255 (Active Low에서 꺼짐)

  // wait until next sampling time (non-blocking)
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  // 거리 구간별 duty_cycle (0 ~ 255) 계산
  if (distance <= _DIST_MIN || distance >= _DIST_MAX) {
    // 100mm 이하 또는 300mm 이상: OFF
    duty_cycle = 255.0; 
  } else if (distance < 200.0) {
    // 100mm ~ 200mm 구간: 거리가 멀어질수록 밝아짐 (255 -> 0)
    duty_cycle = 255.0 - ((distance - 100.0) / 100.0 * 255.0);
  } else {
    // 200mm ~ 300mm 구간: 거리가 멀어질수록 어두워짐 (0 -> 255)
    duty_cycle = ((distance - 200.0) / 100.0 * 255.0);
  }

  // LED 밝기 제어
  analogWrite(PIN_LED, (int)duty_cycle);

  // 시리얼 출력 (디버깅 및 검증용)
  Serial.print("Min:");         Serial.print(_DIST_MIN);
  Serial.print(",distance:");   Serial.print(distance);
  Serial.print(",duty:");       Serial.print(duty_cycle);
  Serial.print(",Max:");        Serial.print(_DIST_MAX);
  Serial.println("");

  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
