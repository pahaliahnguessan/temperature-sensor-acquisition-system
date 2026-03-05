
// Definizione dei pin
const int analogPin = A0;     // Pin di lettura del sensore
const int buttonPin = 2;      // Pin del pulsante per cambiare sensore

// Costanti circuito
const float R1 = 1100.0;      // Resistenza fissa (1.1 kΩ)
const float Vcc = 5.0;        // Tensione di alimentazione del partitore

// Costanti del termistore NTC
const float B = 3730.0;       // Costante B del termistore
const float R0_NTC = 1000.0; // Resistenza NTC a 25°C (1 kΩ)
const float T0 = 25.0;        // Temperatura nominale (°C)

// Costanti della Pt1000
const float R0_Pt1000 = 1000.0;   // Resistenza a 0°C (1000 Ω)
const float alpha = 0.00385;       // Coefficiente termico della Pt1000

// Numero di letture per la media
const int numReadings = 100;

// Variabili per il termistore
float minTemp_NTC = 1000.0;
float maxTemp_NTC = -1000.0;

// Variabili per la Pt1000
float minTemp_Pt1000 = 1000.0;
float maxTemp_Pt1000 = -1000.0;

// Stato attuale del sensore (0 = NTC, 1 = Pt1000)
bool sensorType = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("NTC_Temp\tNTC_Media\tNTC_Min\tNTC_Max\tPt1000_Temp\tPt1000_Media\tPt1000_Min\tPt1000_Max");
  
  pinMode(buttonPin, INPUT_PULLUP); // Pulsante con pull-up interno
}

void loop() {
  // Controllo se il pulsante è stato premuto per cambiare sensore
  if (digitalRead(buttonPin) == LOW) {
    sensorType = !sensorType; // Cambia sensore
    delay(300); // Debounce per evitare rimbalzi
  }

  float sumVoltage = 0.0;

  // Effettua numReadings letture e calcola la media
  for (int i = 0; i < numReadings; i++) {
    float voltage = analogRead(analogPin) * (Vcc / 1023.0);
    sumVoltage += voltage;
    delay(10);
  }

  // Calcolo della media del voltaggio
  float Vout = sumVoltage / numReadings;

  // Calcolare la resistenza del sensore
  float RT = R1 * (Vcc / Vout - 1);
  float temperatureC = 0.0;

  if (sensorType == 0) {
    // Calcolo temperatura per il termistore NTC
    float temperatureK = 1.0 / (1.0 / (T0 + 273.15) + (1.0 / B) * log(RT / R0_NTC));
    temperatureC = temperatureK - 273.15;

    // Aggiornamento min e max per il termistore
    if (temperatureC < minTemp_NTC) minTemp_NTC = temperatureC;
    if (temperatureC > maxTemp_NTC) maxTemp_NTC = temperatureC;
  } else {
    // Calcolo temperatura per la Pt1000
    temperatureC = (RT - R0_Pt1000) / (R0_Pt1000 * alpha);

    // Aggiornamento min e max per la Pt1000
    if (temperatureC < minTemp_Pt1000) minTemp_Pt1000 = temperatureC;
    if (temperatureC > maxTemp_Pt1000) maxTemp_Pt1000 = temperatureC;
  }

  // Stampa dati nel Serial Monitor
  Serial.print("Sensore: ");
  Serial.print(sensorType == 0 ? "NTC" : "Pt1000");
  Serial.print(", Vout: ");
  Serial.print(Vout);
  Serial.print(" V, Temperatura: ");
  Serial.print(temperatureC);
  Serial.print(" °C");

  if (sensorType == 0) {
    Serial.print(", Min NTC: ");
    Serial.print(minTemp_NTC);
    Serial.print(" °C, Max NTC: ");
    Serial.println(maxTemp_NTC);
  } else {
    Serial.print(", Min Pt1000: ");
    Serial.print(minTemp_Pt1000);
    Serial.print(" °C, Max Pt1000: ");
    Serial.println(maxTemp_Pt1000);
  }

  // Stampa per il Plotter Seriale con grafici separati
  Serial.print(sensorType == 0 ? temperatureC : 0); // Temperatura NTC
  Serial.print("\t");
  Serial.print(sensorType == 0 ? (minTemp_NTC + maxTemp_NTC) / 2.0 : 0); // Media NTC
  Serial.print("\t");
  Serial.print(sensorType == 0 ? minTemp_NTC : 0); // Min NTC
  Serial.print("\t");
  Serial.print(sensorType == 0 ? maxTemp_NTC : 0); // Max NTC
  Serial.print("\t");
  
  Serial.print(sensorType == 1 ? temperatureC : 0); // Temperatura Pt1000
  Serial.print("\t");
  Serial.print(sensorType == 1 ? (minTemp_Pt1000 + maxTemp_Pt1000) / 2.0 : 0); // Media Pt1000
  Serial.print("\t");
  Serial.print(sensorType == 1 ? minTemp_Pt1000 : 0); // Min Pt1000
  Serial.print("\t");
  Serial.println(sensorType == 1 ? maxTemp_Pt1000 : 0); // Max Pt1000

  delay(1000);
}
