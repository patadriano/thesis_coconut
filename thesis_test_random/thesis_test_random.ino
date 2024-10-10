const int pwm = 10;     // PWM pin for motor speed control
const int dir = 9;      // Direction control pin
const int start = 2;    // Start button pin
const int reverse = 3;  // Reverse button pin

int startState = HIGH;
int reverseState = HIGH;

void setup() {
    Serial.begin(9600);  
    pinMode(start, INPUT_PULLUP);    // Use INPUT_PULLUP to avoid needing external pull-up resistors
    pinMode(reverse, INPUT_PULLUP);
    pinMode(pwm, OUTPUT); 
    pinMode(dir, OUTPUT);
}

void loop() {
    startState = digitalRead(start);
    reverseState = digitalRead(reverse);

    if (startState == LOW) { // Start button pressed
        digitalWrite(dir, HIGH);  // Set direction to clockwise
        analogWrite(pwm, 100);    // Set speed to max (0-255 range)
        Serial.println("Clockwise rotation");
    } 
    else if (reverseState == LOW) { // Reverse button pressed
        digitalWrite(dir, LOW);   // Set direction to counterclockwise
        analogWrite(pwm, 100);    // Set speed to max
        Serial.println("Counterclockwise rotation");
        
    } 
    else { // Neither button pressed
        analogWrite(pwm, 0);      // Stop motor
        Serial.println("Motor stopped");
    }
}

