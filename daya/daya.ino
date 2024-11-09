

//button
const int start = 2;  
const int reverse = 3;   
int startState = HIGH;
int reverseState = HIGH;
//lcd
#include <LCD-I2C.h>
LCD_I2C lcd(0x27, 16, 2);

//
int button=0;
//vib
int vib_pin = 6;
int Sensor_State = 1;
double vib[8];

int index = 9;
bool ct = false;

double freqlist[10] = {157.38, 145.67, 162.49, 138.76, 169.41, 152.04, 165.02, 149.87, 142.3, 95.7};
int in = 0;

void setup() {
  Serial.begin(9600); 
  //mic
  
  //button
  pinMode(start, INPUT);    
  pinMode(reverse, INPUT);
  //lcd
  lcd.begin();
  lcd.display();
  lcd.backlight();
  //
  pinMode(vib_pin, INPUT); 

  lcd.setCursor(0, 0);
  lcd.print("Press Button to");
  lcd.setCursor(0, 1);
  lcd.print("Start ");
}

void loop() {
    if (in < 10) {
       startState = digitalRead(start);
       reverseState = digitalRead(reverse);
      if (startState == HIGH && button==0) {
        if (freqlist[in] < 100) {
          lcd.clear();
          delay(1000);
          lcd.setCursor(0, 0);
          lcd.print("Reposition"); 
          lcd.setCursor(0, 1);
          lcd.print("Coconut: 95.7");
          
          delay(1000); 
          button = 1;
        }else{
          lcd.clear();
          delay(1000);
          lcd.setCursor(0, 0);
          lcd.print("Frequency:"); 
          lcd.print(freqlist[in]);
          lcd.setCursor(0, 1);
          lcd.print("Type: Malakatad");
          
          delay(1000); 
          button = 1;
        }
        in++;
          
      }else if (reverseState == LOW && button==1) { 
          lcd.clear();
          lcd.print("Resetting... ");
          delay(1500);
          button = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Press Button to");
          lcd.setCursor(0, 1);
          lcd.print("Start ");

    
    }
}
}
  





