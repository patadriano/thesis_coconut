// Include necessary libraries
#include "arduinoFFT.h"
#include <LCD-I2C.h>
#include <Wire.h>

#define SAMPLES 128             // SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 // Sampling frequency in Hz
#define INPUT_PIN A0            // Analog input pin

arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];         // Array to hold real part of FFT
double vImag[SAMPLES];         // Array to hold imaginary part of FFT
bool measuring = false;  
double frequencies[7];          // Array to store the first 3 frequencies
double sound[7];                // Array to store the first 3 sound values                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
const int sampleWindow = 50;    // Sample window width in ms
unsigned int sample;

// Button definitions
const int start = 2;  
const int reverse = 3;  
int startState = LOW;
int reverseState = LOW;

// LCD initialization
LCD_I2C lcd(0x27, 16, 2);

// Other variables
int button = 0;
String type;

// Function prototypes
void measureFFT();
double getSoundLevel();
int findMaxSoundIndex(double arr[], int size);
void clearData();

void setup() {
    Serial.begin(9600);
    samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

    // Button setup
    pinMode(start, INPUT_PULLUP);  
    pinMode(reverse, INPUT_PULLUP);

    // LCD setup
    Wire.begin();
    lcd.begin(&Wire);
    lcd.backlight();
}

void loop() {
    startState = digitalRead(start);
    reverseState = digitalRead(reverse);

    if (startState == HIGH && button == 0) {
        measuring = true;
        lcd.clear();
        Serial.println("Measurement started");
        button = 1;    
    } if (reverseState == LOW && button == 1) {
        lcd.clear();  
        Serial.println("Microphone Reset");
        delay(800);
        button = 0;
    } else {
        clearData();
    }

    while (measuring) {
        delay(200);
        Serial.println("Collecting and processing frequencies...");

        for (int j = 0; j < 7; j++) {
            double volts = getSoundLevel();
            measureFFT();

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
        }

        // Find the index of the maximum sound value
        int maxSoundIndex = findMaxSoundIndex(sound, 3);
        double maxSoundValue = sound[maxSoundIndex];
        double maxFrequency = frequencies[maxSoundIndex];

        Serial.print("The frequency with the highest sound value is: ");
        Serial.print(maxFrequency);
        Serial.print(" Hz, with a sound value of: ");
        Serial.println(maxSoundValue);
       
        if (maxFrequency < 76 && maxFrequency > 55) {
            type = "Malakatad";
        }
       
        lcd.setCursor(0, 0);
        lcd.print("Frequency: ");
        lcd.print(maxFrequency);
        lcd.setCursor(0, 1);
        lcd.print("Type: ");
        lcd.print(type);

        delay(500);
        type = "";
        Serial.println("button state: ");
        Serial.println(button);
       
        // Update measuring state
        startState = digitalRead(start);
        if (startState == HIGH) {
            measuring = true;
        } else {
            measuring = false;
            clearData();
            Serial.println("Measurement stopped");
        }
    }
}

void measureFFT() {
    for (int i = 0; i < SAMPLES; i++) {
        microSeconds = micros();
        vReal[i] = analogRead(INPUT_PIN); // Read analog value
        vImag[i] = 0;                     // Imaginary part is always 0
        while (micros() - microSeconds < samplingPeriod) {
            // Do nothing, just wait
        }
    }
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); // Apply windowing
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD); // Compute FFT
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES); // Convert to magnitude
}

double getSoundLevel() {
    unsigned long startMillis = millis();
    double sum = 0;
    int count = 0;

    // Collect data for sampleWindow milliseconds
    while (millis() - startMillis < sampleWindow) {
        sample = analogRead(INPUT_PIN);
        if (sample < 1024) { // Ignore out-of-range readings
            double voltage = (sample * 5.0) / 1024; // Convert to volts
            sum += voltage; // Accumulate the voltage readings
            count++;
        }
    }
    if (count > 0) {
        return sum / count; // Return average voltage
    } else {
        return 0; // Avoid division by zero
    }
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
    for (int i = 0; i < 3; i++) {
        frequencies[i] = 0;
        sound[i] = 0;
    }
}
