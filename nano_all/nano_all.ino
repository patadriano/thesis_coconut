// Mic
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
double frequencies[5];         // Array to store the first 3 frequencies
double sound[5];               // Array to store the first 3 sound values
const int sampleWindow = 50;   // Sample window width in ms (50 ms = 20Hz)
unsigned int sample;

// Button
const int start = 2;
const int reverse = 3;
int startState = HIGH;
int reverseState = HIGH;

// LCD
#include <LCD-I2C.h>
LCD_I2C lcd(0x27, 16, 2);

int button = 0;
String type;

void setup() {
  Serial.begin(9600);
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  
  // Button setup
  pinMode(start, INPUT);    // Use INPUT_PULLUP to avoid needing external pull-up resistors
  pinMode(reverse, INPUT);
  
  // LCD setup
  lcd.begin();
  lcd.display();
  lcd.backlight();
}

void loop() {
  
  startState = digitalRead(start);
  reverseState = digitalRead(reverse);

  if (startState == HIGH && button == 0) { 
    measuring = true;
    lcd.clear();
    button = 1;
  } 
  else if (reverseState == LOW && button == 1) { 
    lcd.clear();
    delay(800);
    button = 0;
  } 
  else if (!measuring) {
    clearData();
  }

  while (measuring) {
    delay(100);
    for (int j = 0; j < 5; j++) {
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

      double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      frequencies[j] = peak;
      sound[j] = volts;

      Serial.print("Frequency ");
      Serial.print(j + 1);
      Serial.print(": ");
      Serial.print(frequencies[j]);
      Serial.print(" Hz, Sound Value ");
      Serial.print(j + 1);
      Serial.println(sound[j]);
    }

    // Find the index of the maximum sound value
    int maxSoundIndex = findMaxSoundIndex(sound, 5);
    double maxSoundValue = sound[maxSoundIndex];
    double maxFrequency = frequencies[maxSoundIndex];

    Serial.print("The frequency with the highest sound value is: ");
    Serial.print(maxFrequency);
    Serial.print(" Hz, with a sound value of: ");
    Serial.println(maxSoundValue);

    if (maxFrequency < 76 && maxFrequency > 55) {
      type = "Malakatad";
    }

    // Display the data on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Frequency: ");
    lcd.print(maxFrequency);
    lcd.setCursor(0, 1);
    lcd.print("Type: ");
    lcd.print(type);
    delay(2000); // Allow time for data display stability

    // Reset type after display
    type = "";

    // Update measuring state
    startState = digitalRead(start);
    measuring = (startState == HIGH);
    if (!measuring) {
      clearData();
    }
  }
}

// Function to measure sound level
double getSoundLevel() {
  unsigned long startMillis = millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // Collect data for 50 ms
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
  // return (peakToPeak * 5.0) / 1024;  
  return peakToPeak; // Convert to volts
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

// Clear data arrays
void clearData() {
  for (int i = 0; i < 5; i++) {
    frequencies[i] = 0;
    sound[i] = 0;
  }
}
