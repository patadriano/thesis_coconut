
//FFT
#include "arduinoFFT.h"
#include <LCD-I2C.h>
#define SAMPLES 128             
#define SAMPLING_FREQUENCY 2048 
#define INPUT_PIN A0            
arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];         
double vImag[SAMPLES];         
bool measuring = false;        
double frequencies[10];       
double sound[10];              
const int sampleWindow = 50;   
unsigned int sample;

//lcd
LCD_I2C lcd(0x27, 16, 2); 

//knocking
const int pwm = 10; // pin 10 as pwm
const int dir = 9;  // pin 9 as dir
int i = 0;
const int start = 2; //Button
const int reverse = 3; //Button2
int startState = 1;
int reverseState = 0;

void setup() {
    //
    Serial.begin(9600);  
    //
    samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
    //
    lcd.begin();
    lcd.display();
    lcd.backlight();
    //
    pinMode(start, INPUT);
    pinMode(reverse, INPUT);
    pinMode(pwm,OUTPUT); 
    pinMode(dir,OUTPUT);
}

void loop() {

    startState = digitalRead(start);
        if (startState == HIGH) {
            measuring = true;
        } else {
            measuring = false;
            clearData();  
            digitalWrite(dir,HIGH);
            // Serial.println("1 high"); 
            analogWrite(pwm,i);
        }
        reverseState = digitalRead(reverse);
        if (reverseState == HIGH) {
          digitalWrite(dir,HIGH);
          i = 255;
          analogWrite(pwm,i); 
          // Serial.println("2 high");    
          delay(1000);
        } else {
          digitalWrite(dir,LOW);
          i = 0;
          // Serial.println("2 low"); 
          analogWrite(pwm,i);
        }
    
    while (measuring) {
        lcd.clear();
        digitalWrite(dir,!HIGH);
        i = 255;
        analogWrite(pwm,i);  
        Serial.println("1 high"); 
        for (int j = 0; j < 10; j++) {
            double volts = getSoundLevel();
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
            frequencies[j] = peak;
            sound[j] = volts;

            Serial.print("Frequency ");
            Serial.print(frequencies[j]);
            Serial.print(" Hz, Sound Value ");
            Serial.print(j + 1);
            Serial.print(": ");
            Serial.println(sound[j]);  
        }

        int maxSoundIndex = findMaxSoundIndex(sound, 10);
        double maxSoundValue = sound[maxSoundIndex];
        double maxFrequency = frequencies[maxSoundIndex];

        Serial.print("The frequency with the highest sound value is: ");
        Serial.print(maxFrequency);
        Serial.print(" Hz, with a sound value of: ");
        Serial.println(maxSoundValue);
        lcd.setCursor(0, 0);
        lcd.print("Frequency: ");
        lcd.println(maxFrequency);
        //insert range
        lcd.setCursor(0, 1);
        lcd.print("Type: ");
        lcd.println(maxFrequency);



        startState = digitalRead(start);
        if (startState == HIGH) {
            measuring = true;
        } else {
            measuring = false;
            clearData();  
        }

    }
}

double getSoundLevel() {
    unsigned long startMillis = millis();  
    unsigned int peakToPeak = 0;   
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;
    while (millis() - startMillis < sampleWindow) {
        sample = analogRead(A0);
        if (sample < 1024) {  
            if (sample > signalMax) {
                signalMax = sample;  
            } else if (sample < signalMin) {
                signalMin = sample;  
            }
        }
    }

    peakToPeak = signalMax - signalMin; 
    return (peakToPeak * 5.0) / 1024; 
}


int findMaxSoundIndex(double arr[], int size) {
    int maxIndex = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] > arr[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}


void clearData() {
    for (int i = 0; i < 10; i++) {
        frequencies[i] = 0;
        sound[i] = 0;
    }
}

