const int potPin = A0;    
const int ledPin = 9;   

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int potValue = analogRead(potPin);
  int pwmValue = map(potValue, 0, 1023, 0, 255);
  analogWrite(ledPin, pwmValue);
  delay(10);
}
