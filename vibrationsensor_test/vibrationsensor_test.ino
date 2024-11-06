int Vibration_signal = 6; //Define the Digital Input on the Arduino for the sensor signal
int Sensor_State = 1; 
void setup() {
  pinMode(Vibration_signal, INPUT); //Set pin as input
  Serial.begin(9600); // Start the serial communication
}
 
void loop() {
  Serial.print("Vibration status: ");
  Sensor_State = digitalRead(Vibration_signal);
  if (Sensor_State == 1) {
    Serial.println("Sensing vibration");
  }
  else {
    Serial.println("No vibration");
  }
  delay(50);
}