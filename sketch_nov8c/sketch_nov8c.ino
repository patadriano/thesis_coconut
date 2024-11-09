const int start = 2;  
const int reverse = 3;   
int startState = HIGH;
int reverseState = HIGH;
double freqlist[10] = {150,200,250,170,190,166,177,188,199,160};
int button=0;


void setup() {
  Serial.begin(9600); 
  pinMode(start, INPUT);    
  pinMode(reverse, INPUT);
}

void loop() {
  
 for(int x=0;x<=9;x++){
      startState = digitalRead(start);
  if (startState == HIGH) {
    
  } else {
    measuring = false;
    clearData();
  }
    Serial.println(freqlist[x]);
    delay(500);
    

}
}
