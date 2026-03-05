// Definizione del pin analogico
const int analogPin = A0;

// Costanti del circuito
const float R1 = 1100.0;  // Resistenza fissa (1.1 kΩ)
const float Vcc = 5.0;    // Tensione di alimentazione del partitore

// Costanti del termistore KE 164 (dal datasheet)
const float T0 = 25.0;    // Temperatura nominale (°C)
const float B = 3730.0;   // Costante B del termistore
const float R0 = 1000.0; // Resistenza a 25°C (1 kΩ)

// Numero di letture per la media
const int numReadings = 100;

// Variabili globali per il minimo e massimo
float minTemp = 1000.0;  // Inizializzato a un valore alto
float maxTemp = -1000.0; // Inizializzato a un valore basso

void setup() {
  Serial.begin(9600);  // Avvio della comunicazione seriale
  Serial.println("Temperatura\tMedia\tMin\tMax"); // Etichette per il Plotter Serial
}

void loop() {
  float sumTemperature = 0.0;
  float sumVoltage = 0.0;

  // Effettua numReadings letture e calcola la media
  for (int i = 0; i < numReadings; i++) {
    float voltage = analogRead(analogPin) * (Vcc / 1023.0);
    sumVoltage += voltage;
    delay(10);  // Piccolo ritardo tra le letture
  }

  // Calcolo della media del voltaggio
  float Vout = sumVoltage / numReadings;

  // Calcolare la resistenza del termistore RT
  float RT = R1 * (Vcc / Vout - 1);

  // Calcolare la temperatura usando la formula di Steinhart-Hart (approssimata)
  float temperatureK = 1.0 / (1.0 / (T0 + 273.15) + (1.0 / B) * log(RT / R0));
  float temperatureC = temperatureK - 273.15;  // Convertire in °C

  // Aggiornamento del minimo e massimo registrato
  if (temperatureC < minTemp) {
    minTemp = temperatureC;
  }
  if (temperatureC > maxTemp) {
    maxTemp = temperatureC;
  }

  // Stampa dati nel Serial Monitor
  Serial.print("Vout medio: ");
  Serial.print(Vout);
  Serial.print(" V, Temperatura: ");
  Serial.print(temperatureC);
  Serial.print(" °C, Min: ");
  Serial.print(minTemp);
  Serial.print(" °C, Max: ");
  Serial.println(maxTemp);

  // Stampa per il Serial Plotter di Arduino
  Serial.print(temperatureC);  // Temperatura attuale
  Serial.print("\t");
  Serial.print((minTemp + maxTemp) / 2.0);  // Temperatura media
  Serial.print("\t");
  Serial.print(minTemp);  // Minimo registrato
  Serial.print("\t");
  Serial.println(maxTemp);  // Massimo registrato

  delay(1000);  // Attendere 1 secondo prima di una nuova lettura
}
