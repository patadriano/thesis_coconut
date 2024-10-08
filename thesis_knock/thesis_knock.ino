//knocking
const int pwm = 10; 
const int dir = 9;  
int i = 0;
const int start = 2; 
const int reverse = 3; 
int startState = 1;
int reverseState = 0;

void setup() {
    Serial.begin(9600);  
    pinMode(start, INPUT);
    pinMode(reverse, INPUT);
    pinMode(pwm,OUTPUT); 
    pinMode(dir,OUTPUT);
}

void loop() {
        // startState = digitalRead(start);

        // if (startState == HIGH) {
        //     Serial.println("1 low"); 
        // } else {
        //     digitalWrite(dir,HIGH);
        //     analogWrite(pwm,i);
        //     Serial.println("1 high"); 
        // }

        reverseState = digitalRead(reverse);

        if (reverseState == HIGH) {
          digitalWrite(dir,HIGH);
          i = 255;
          analogWrite(pwm,i); 
          Serial.println("2 low");    

        } else {
          digitalWrite(dir,LOW);
          i = 0;
          Serial.println("2 high"); 
          analogWrite(pwm,i);
        }

}
