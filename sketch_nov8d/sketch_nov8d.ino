// button
const int start = 2;  
const int reverse = 3;   
int startState = HIGH;
int prevStartState = HIGH; // Tracks the previous state of the start button
int reverseState = HIGH;
// lcd
#include <LCD-I2C.h>
LCD_I2C lcd(0x27, 16, 2);

int button = 0; // Tracks if we are in "start" or "reset" state
// vibration sensor
int vib_pin = 6;
int Sensor_State = 1;
double vib[8];

// frequency list and index tracking
int index = 0;
double freqlist[10] = {150, 200, 250, 170, 190, 166, 177, 188, 199, 160};

void setup() {
  Serial.begin(9600);
  
  // button
  pinMode(start, INPUT);    
  pinMode(reverse, INPUT);
  
  // lcd
  lcd.begin();
  lcd.display();
  lcd.backlight();
  
  // Initial message
  lcd.setCursor(0, 0);
  lcd.print("Press Button to");
  lcd.setCursor(0, 1);
  lcd.print("Start ");
}

void loop() {
  // Read button states
  startState = digitalRead(start);
  reverseState = digitalRead(reverse);
  
  // Check if the start button was just pressed (edge detection)
  if (startState == HIGH && prevStartState == LOW && button == 0) { 
    // Start button pressed and system not in "started" state
    displayFrequency();
    index =+ 1;
    button = 1; // Enter "started" state
  } 
  else if (reverseState == LOW && button == 1) { 
    // Reset button pressed
    resetDisplay();
    button = 0; // Return to "not started" state
  }

  // Update the previous state of the start button
  prevStartState = startState;
}

void displayFrequency() {
  lcd.clear();
  
  if (index >= 10) { 
    index = 0; // Reset index if it exceeds the array size
  }
  index = index + 1;
  double maxFrequency = freqlist[index];
  Serial.println(maxFrequency);  // Print frequency to Serial Monitor
  delay(900);  // Brief delay for readability

  // Display frequency and type
  if (maxFrequency < 132 || maxFrequency > 262) {
    lcd.setCursor(0, 0);
    lcd.print("Reposition");
    lcd.setCursor(0, 1);
    lcd.print("Coconut");
  } else if (maxFrequency >= 132 && maxFrequency < 172) {
    lcd.setCursor(0, 0);
    lcd.print("Frequency: "); 
    lcd.print(maxFrequency);
    lcd.setCursor(0, 1);
    lcd.print("Type: Malakatad");
  } else if (maxFrequency >= 172 && maxFrequency < 200) {
    lcd.setCursor(0, 0);
    lcd.print("Frequency: "); 
    lcd.print(maxFrequency);
    lcd.setCursor(0, 1);
    lcd.print("Type: Malakanin");
  } else if (maxFrequency >= 200 && maxFrequency < 262) {
    lcd.setCursor(0, 0);
    lcd.print("Frequency: "); 
    lcd.print(maxFrequency);
    lcd.setCursor(0, 1);
    lcd.print("Type: Malauhog");
  }
  
    // Move to the next frequency in freqlist
}

void resetDisplay() {
  lcd.clear();
  lcd.print("Resetting...");
  delay(800);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Button to");
  lcd.setCursor(0, 1);
  lcd.print("Start ");
  
  index = 0;  // Reset index when resetting
}
