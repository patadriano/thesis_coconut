
#include "arduinoFFT.h"


#define SAMPLES 128             // SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 // Sampling frequency in Hz
#define INPUT_PIN A0            // Analog input pin
arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];         // Array to hold real part of FFT
double vImag[SAMPLES];         // Array to hold imaginary part of FFT
bool measuring = false;        // Flag to indicate if measurements should be taken
double frequencies[10];        // Array to store the first 30 frequencies
double sound[10];              // Array to store the first 30 sound values
const int sampleWindow = 50;   // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;


const int buttonPin1 = 2;  
const int buttonPin2 = 3;   
int buttonState1 = 0;  
int buttonState2 = 0;


void setup() {
  Serial.begin(9600);  // Baud rate for Serial Monitor
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  Serial.begin(9600);
}

void loop() {
  buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 == LOW) {
    measuring = true;
    Serial.println("1 high"); 
  } else {
    measuring = false;
    clearData();
    Serial.println("1 low"); 
  }

  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 == LOW) {
    Serial.println("2 high"); 
  } else {
    Serial.println("2 low"); 
  }

  while (measuring) {
    Serial.println("Collecting and processing frequencies...");
    for (int j = 0; j < 10; j++) {
        // Measure sound value
        double volts = getSoundLevel();


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
        frequencies[j] = peak;
        sound[j] = volts;


        // Print the acquired frequency and sound value
        Serial.print("Frequency ");
        Serial.print(j + 1);
        Serial.print(": ");
        Serial.print(frequencies[j]);
        Serial.print(" Hz, Sound Value ");
        Serial.print(j + 1);
        Serial.print(": ");
        Serial.println(sound[j]);


        // Small delay to ensure different samples

        
        
    }


    // Find the index of the maximum sound value
    int maxSoundIndex = findMaxSoundIndex(sound, 10);
    double maxSoundValue = sound[maxSoundIndex];
    double maxFrequency = frequencies[maxSoundIndex];


    Serial.print("The frequency with the highest sound value is: ");
    Serial.print(maxFrequency);
    Serial.print(" Hz, with a sound value of: ");
    Serial.println(maxSoundValue);

    buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 == LOW) {
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


    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    return (peakToPeak * 5.0) / 1024;  // Convert to volts
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



void clearData() {
    for (int i = 0; i < 10; i++) {
        frequencies[i] = 0;
        sound[i] = 0;
    }
}









