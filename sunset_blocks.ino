/*
  SUNSET BLOCKS - control con 1 boton
  Slider:  extremos a 5V y GND, cursor -> A0
  Perilla: extremos a 5V y GND, pata del medio -> A1
  Boton:   una pata -> pin 2, la otra -> GND
  (opcional) Luz del boton -> pin 9 con resistencia 220 ohm -> GND
*/

const int PIN_SLIDER = A0;
const int PIN_KNOB   = A1;
const int PIN_BTN    = 2;
const int PIN_LED    = 9;

bool btnState = false;
byte btnCnt = 0;

char ledMode = 'A';
unsigned long ledStart = 0;
unsigned long lastSample = 0, lastSend = 0;

int readSmooth(int pin) {
  long s = 0;
  for (int i = 0; i < 8; i++) s += analogRead(pin);
  return s / 8;
}

void debounce() {
  bool raw = !digitalRead(PIN_BTN);
  if (raw != btnState) {
    if (++btnCnt >= 3) { btnState = raw; btnCnt = 0; }
  } else {
    btnCnt = 0;
  }
}

void setLed(char mode) { ledMode = mode; ledStart = millis(); }

void updateLed() {
  unsigned long e = millis() - ledStart;
  bool on = true;
  switch (ledMode) {
    case 'L': on = ((e / 60) % 2 == 0);  if (e > 360)  setLed('S'); break;
    case 'T': on = ((e / 90) % 2 == 0);  if (e > 1080) setLed('S'); break;
    case 'K': on = (e > 80);             if (e > 80)   setLed('S'); break;
    case 'G': on = ((e / 250) % 2 == 0); if (e > 2000) setLed('A'); break;
    case 'P': on = ((e / 500) % 2 == 0); break;
    case 'A': on = ((e / 400) % 2 == 0); break;
    default:  on = true; break;
  }
  digitalWrite(PIN_LED, on ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  setLed('A');
}

void loop() {
  unsigned long now = millis();

  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'L' || c == 'T' || c == 'G' || c == 'P' || c == 'S' || c == 'A' || c == 'K') setLed(c);
  }

  if (now - lastSample >= 5) { lastSample = now; debounce(); }

  if (now - lastSend >= 20) {
    lastSend = now;
    Serial.print("S,");
    Serial.print(readSmooth(PIN_SLIDER)); Serial.print(',');
    Serial.print(readSmooth(PIN_KNOB));   Serial.print(',');
    Serial.print(btnState ? 1 : 0);
    Serial.println(",0");            // el juego espera 2 botones: el segundo va siempre en 0
  }

  updateLed();
}