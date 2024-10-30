const int buttonPin1 = 2;  
const int buttonPin2 = 3;   
int buttonState1 = 0;  
int buttonState2 = 0;
void setup() {
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  Serial.begin(9600);
}

void loop() {
   buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 == LOW) {
    Serial.println("1 low"); 
  } else {
    Serial.println("1 high"); 
  }

  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 == HIGH) {
    Serial.println("2 low"); 
  } else {
    Serial.println("2 high"); 
  }
}
