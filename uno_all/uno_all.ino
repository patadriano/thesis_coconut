bool measuring = false;  
//button
const int start = 2;  
const int reverse = 3;   
int startState = HIGH;
int reverseState = HIGH;

//knock
const int pwm = 10;     
const int dir = 9;

int button=0;
//
void setup() {
  //
  Serial.begin(9600); 
  //button
  pinMode(start, INPUT_PULLUP);    // Use INPUT_PULLUP to avoid needing external pull-up resistors
    pinMode(reverse, INPUT_PULLUP);

  //knock
  pinMode(pwm, OUTPUT); 
  pinMode(dir, OUTPUT);
}

void loop() {
  startState = digitalRead(start);
  reverseState = digitalRead(reverse);

    if (startState == HIGH && button==0) { 
      
        measuring = true;
        Serial.println("start"); 
        digitalWrite(dir, HIGH);  
         analogWrite(pwm, 250); 
         Serial.println("start measure"); 
         delay(200);   
        button = 1;
        
    } 
  
    else if (reverseState == LOW && button==1) { 
        digitalWrite(dir, LOW);  
        analogWrite(pwm, 120);    
        delay(1000);
        Serial.println("reverse"); 
        button = 0;
    } 
    else { 
        analogWrite(pwm, 0); 
        Serial.println("stop");      // Stop motor
    }
}

  /*while (measuring) {
        
         digitalWrite(dir, HIGH);  
         analogWrite(pwm, 230); 
         Serial.println("start measure");     
    }
 delay(500);
  Serial.println(button);
  startState = digitalRead(start);
  if (startState == HIGH) {
    measuring = true;
    Serial.println("1 high");

  } else {
    measuring = false;
    Serial.println("1 low"); 
    
  }
}*/




  




