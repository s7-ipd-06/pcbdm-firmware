#define _DOOR_SWITCH 11

void setup() {
  Serial.begin(115200);
  
  pinMode(_DOOR_SWITCH, INPUT_PULLUP);
  
  Serial.println("Ready");
}

void loop() {
  Serial.println(digitalRead(_DOOR_SWITCH));
  delay(500);
}
