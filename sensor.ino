#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN D2           // DHT11 data pin is connected to GPIO2
#define DHTTYPE DHT11       // DHT11 sensor type
#define MQ2PIN A0           // MQ2 sensor analog pin is connected to A0
#define IRPIN D5            // IR sensor pin is connected to GPIO5

char auth[] = "YourAuthToken"; // Blynk authentication token
char ssid[] = "YourWiFiSSID";  // Your WiFi SSID
char pass[] = "YourWiFiPass";  // Your WiFi password

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(IRPIN, INPUT);
}

void loop() {
  float humidity = dht.readHumidity();   // Read humidity
  float temperature = dht.readTemperature(); // Read temperature in Celsius
  int gasValue = analogRead(MQ2PIN);   // Read MQ2 sensor value
  bool fireDetected = digitalRead(IRPIN); // Read IR sensor value (1 if fire detected, 0 otherwise)

  // Print sensor values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  Serial.print("Fire Detected: ");
  Serial.println(fireDetected ? "Yes" : "No");

  // Send sensor values to Blynk app
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, gasValue);

  // Convert gas value to parts per million (PPM)
  float ppm = (float)gasValue / 1024.0 * 5.0;
  // Send gas values to Blynk app
  Blynk.virtualWrite(V4, ppm); // Send ppm value

  delay(2000);  // Delay for 2 seconds
}
