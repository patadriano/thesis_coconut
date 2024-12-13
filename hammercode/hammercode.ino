//HAMMER
//button

const int start = 2;

const int reverse = 3;

int startState = LOW;

int reverseState = LOW;


//knock

const int pwm = 10; 

const int dir = 9;

int button=0;


void setup() {

Serial.begin(9600); 
//knock

pinMode(pwm, OUTPUT); 
pinMode(dir, OUTPUT);

}


void loop() {

startState = digitalRead(start);

reverseState = digitalRead(reverse);


if (startState == HIGH && button==0) { 

Serial.println("1 high"); 
digitalWrite(dir, HIGH);
analogWrite(pwm, 230); 
delay(150);
analogWrite(pwm, 0);
button = 1;
} 

if (reverseState == LOW && button==1) { 

digitalWrite(dir, LOW); 

analogWrite(pwm, 80);

Serial.println("2 high"); 

delay(1500);

analogWrite(pwm, 0);
button = 0;

} 

else { // Neither button pressed
analogWrite(pwm, 0);
}

}
