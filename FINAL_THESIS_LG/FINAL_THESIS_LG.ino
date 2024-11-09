//mic
#include "arduinoFFT.h"
#define SAMPLES 128            
#define SAMPLING_FREQUENCY 2048 
#define INPUT_PIN A0           
arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];         
double vImag[SAMPLES];         
bool measuring = false;  
double frequencies[8];       
double sound[8]; 
const int sampleWindow = 50;  
unsigned int sample;

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
int index2 = 9;
bool ct = false;
bool ct2 = false;

void setup() {
  Serial.begin(9600); 
  //mic
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
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
  startState = digitalRead(start);
  reverseState = digitalRead(reverse);
  if (startState == HIGH && button==0) { 
      measuring = true;
      lcd.clear();
      button = 1;
  }else if (reverseState == LOW && button==1) { 
    lcd.clear();
    lcd.print("Resetting... ");
    delay(800);
    button = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press Button to");
    lcd.setCursor(0, 1);
    lcd.print("Start ");
  }else { 
    clearData();
  }
  ct = false;
  ct2 = false;
  while (measuring) {
    for (int j = 0; j < 8; j++) {
        frequencies[j] = getFFT();
        vib[j] = getVib();
        if (vib[j] == 1 && ct == false){
            index = j; 
            ct = true;
        }
        if (j >= index && ct == true) {
            if(frequencies[j] >= 132 && frequencies[j] <= 262 && ct2 == false){
              index2 = j;
              ct2 = true;
            }
        }

        Serial.print("Frequency ");
        Serial.print(j + 1);
        Serial.print(": ");
        Serial.print(frequencies[j]);
        Serial.print(" Vibration ");
        Serial.print(j + 1);
        Serial.print(": ");
        Serial.println(vib[j]);
    }

    double maxFrequency;
    int maxVibIndex = findMaxVib(vib, 8);
    int maxVib = vib[maxVibIndex];
    
    if (ct2 == false){
      maxFrequency = frequencies[maxVibIndex];
    }else{
      maxFrequency = frequencies[index2];
    }

    index = 9;

    Serial.print("The frequency with the highest sound value is: ");
    Serial.print(maxFrequency);
    Serial.print(" Hz, with a vibration value of: ");
    Serial.println(maxVib);

    Serial.println(maxFrequency);
    if(maxVib == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Try Again");
      Serial.println("try");
    }else if (maxFrequency < 132 || maxFrequency > 262){
      lcd.setCursor(0,0);
      lcd.print("Reposition"); 
      lcd.setCursor(0, 1); 
      lcd.print("Coconut: ");
      lcd.print(maxFrequency);
      Serial.println("repo");
    }else{
      Serial.println("freq");
      if (maxFrequency > 132 && maxFrequency < 172) {
        lcd.setCursor(0,0);
      lcd.print("Frequency: "); 
      lcd.print(maxFrequency);
      lcd.setCursor(0, 1); 
      lcd.print("Type: ");
      lcd.print("Malakatad");
      }else if (maxFrequency > 172 && maxFrequency < 200){
        lcd.setCursor(0,0);
      lcd.print("Frequency: "); 
      lcd.print(maxFrequency);
      lcd.setCursor(0, 1); 
      lcd.print("Malakanin");
      }else if (maxFrequency > 200  && maxFrequency < 262){
      
        lcd.setCursor(0,0);
      lcd.print("Frequency: "); 
      lcd.print(maxFrequency);
      lcd.setCursor(0, 1); 
      lcd.print("Type: ");
      lcd.print("Malauhog");
      }
   
    }

  startState = digitalRead(start);
  if (startState == HIGH) {
    measuring = true;
  } else {
    measuring = false;
    clearData();
  }
}  
}

int findMaxVib(double arr[], int size) {
    int maxVibIndex = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] > arr[maxVibIndex]) {
            maxVibIndex = i;
        }
    }
    return maxVibIndex;
}

int getVib() {
  int vibb;
  Sensor_State = digitalRead(vib_pin);
  if (Sensor_State == 1) {
    vibb = 1;
  }
  else {
    vibb = 0;
  }
  delay(50);
  return vibb;
}


void clearData() {
    for (int i = 0; i < 8; i++) {
        frequencies[i] = 0;
        sound[i] = 0; 
    }
}

double getFFT(){
        for (int i = 0; i < SAMPLES; i++) {
            microSeconds = micros();  
            vReal[i] = analogRead(INPUT_PIN); 
            vImag[i] = 0;  
            while (micros() - microSeconds < samplingPeriod) {
            }
        }
        FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); 
        FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD); 
        FFT.ComplexToMagnitude(vReal, vImag, SAMPLES); 
        double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
        return peak;
}

