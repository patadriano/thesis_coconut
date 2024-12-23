//mic
#include "arduinoFFT.h"
#define SAMPLES 128             // SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 // Sampling frequency in Hz
#define INPUT_PIN A0            // Analog input pin
arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];         // Array to hold real part of FFT
double vImag[SAMPLES];         // Array to hold imaginary part of FFT
bool measuring = false;  
     // Flag to indicate if measurements should be taken
double frequencies[8];        // Array to store the first s0 frequencies
double sound[8];  
            // Array to store the first s0 sound values
const int sampleWindow = 50;   // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

//button
const int start = 2;  
const int reverse = 3;   
int startState = HIGH;
int reverseState = HIGH;
//lcd
#include <LCD-I2C.h>
LCD_I2C lcd(0x27, 16, 2);


int button=0;
//vib
int vib_pin = 6;
int Sensor_State = 1;
double vib[8];

int in2 = 9;
int ct = 0;

void setup() {
  //
  Serial.begin(9600); 
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
        
    } 
  
    else if (reverseState == LOW && button==1) { 
      lcd.clear();
      lcd.print("Resetting... ");
        delay(800);
        button = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Press Button to");
      lcd.setCursor(0, 1);
      lcd.print("Start ");
    } 
    else { // Neither button pressed
        clearData();
    }
  while (measuring) {
    
    for (int j = 0; j < 8; j++) {
        // Measure sound value
        // double volts = getSoundLevel();
        

        // Perform FFT
        for (int i = 0; i < SAMPLES; i++) {
            microSeconds = micros();  // Get current time in microseconds
            vReal[i] = analogRead(INPUT_PIN);  // Read analog value
            vImag[i] = 0;  // Imaginary part is always 0


            // Wait until the next sample period
            while (micros() - microSeconds < samplingPeriod) {
                // Do nothing, just wait
            }
        }


        FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); // Apply windowing
        FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD); // Compute FFT
        FFT.ComplexToMagnitude(vReal, vImag, SAMPLES); // Convert to magnitude


        // Store the peak frequency and sound value
        double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

        int vibration = getVib();
        frequencies[j] = peak;
        // sound[j] = volts;
        vib[j] = vibration;
        //
        if (vib[j] == 1){
          if(frequencies[j] > 110 && frequencies[j] < 260 && ct == 0){
            in2 = j; 
            ct++;
          } 
          }
        
        ct = 0;

        // Print the acquired frequency and sound value
        Serial.print("Frequency ");
        Serial.print(j + 1);
        Serial.print(": ");
        Serial.print(frequencies[j]);
        // Serial.print(" Hz, Sound Value ");
        // Serial.print(j + 1);
        // Serial.print(": ");
        // Serial.println(sound[j]);
        Serial.print(" Vibration ");
        Serial.print(j + 1);
        Serial.print(": ");
        Serial.println(vib[j]);

        // Small delay to ensure different samples

        
        
    }
    double maxFrequency;

    // Find the index of the maximum sound value
    // int maxSoundIndex = findMaxSoundIndex(sound, 8);
    int maxVibIndex = findMaxVib(vib, 8);

    // double maxSoundValue = sound[maxSoundIndex];
    int maxVib = vib[maxVibIndex];
    // double maxFrequency = frequencies[maxSoundIndex];
    
    if (in2 == 9){
      maxFrequency = frequencies[maxVibIndex];
    }else{
      maxFrequency = frequencies[in2];
    }
    

    in2 = 9;
    Serial.print("The frequency with the highest sound value is: ");
    Serial.print(maxFrequency);
    // Serial.print(" Hz, with a sound value of: ");
    Serial.print(" Hz, with a vibration value of: ");
    // Serial.println(maxSoundValue);
    Serial.println(maxVib);


    if(maxVib == 0){
        lcd.clear();
        lcd.setCursor(0,0);
    lcd.print("Try Again");

    }else if (maxFrequency < 110 && maxFrequency < 260){
      lcd.setCursor(0,0);
      lcd.print("Reposition"); 
      
      lcd.setCursor(0, 1); // Or setting the cursor in the desired position. 
      lcd.print("Coconut");
    }else{
      lcd.setCursor(0,0);
      lcd.print("Frequency: "); 
      lcd.print(maxFrequency);// You can make spaces using well... spaces
      lcd.setCursor(0, 1); // Or setting the cursor in the desired position. 
      lcd.print("Type: ");
    }

    
    // delay(500);
    // delay(500);
    // lcd.setCursor(0,0);
    // lcd.print("Type: "); 
    


  startState = digitalRead(start);
  if (startState == HIGH) {
    measuring = true;
    Serial.println("1 high"); 
  } else {
    measuring = false;
    clearData();
    Serial.println("1 low"); 
  }
}

  
}


// Function to measure sound level (peak-to-peak voltage)
double getSoundLevel() {
    unsigned long startMillis = millis();  // Start of sample window
    unsigned int peakToPeak = 0;   // peak-to-peak level
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;


    // Collect data for 50 mS
    while (millis() - startMillis < sampleWindow) {
        sample = analogRead(A0);
        if (sample < 1024) {  // Toss out spurious readings
            if (sample > signalMax) {
                signalMax = sample;  // Save the max levels
            } else if (sample < signalMin) {
                signalMin = sample;  // Save the min levels
            }
        }
    }


    peakToPeak = signalMax - signalMin;
      // max - min = peak-peak amplitude
    // return (peakToPeak * 5.0) / 1024;  // Convert to volts
    return peakToPeak;
}


// Function to find the index of the maximum value in an array
int findMaxSoundIndex(double arr[], int size) {
    int maxIndex = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] > arr[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
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




int findSecondMaxSoundIndex(double arr[], int size) {
    if (size < 2) {
        return -1; // Return an invalid index if there aren't enough elements
    }

    int maxIndex = 0, secondMaxIndex = -1;

    // Find the maximum element and the second maximum element
    for (int i = 1; i < size; i++) {
        if (arr[i] > arr[maxIndex]) {
            secondMaxIndex = maxIndex;
            maxIndex = i;
        } else if (arr[i] > arr[secondMaxIndex] && arr[i] != arr[maxIndex]) {
            secondMaxIndex = i;
        }
    }

    return secondMaxIndex;
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

