// Definizione del pin analogico
const int analogPin = A0;

// Costanti del circuito
const float R1 = 1100.0;  // Resistenza fissa (1.1 kΩ)
const float Vcc = 5.0;    // Tensione di alimentazione del partitore

// Costanti della Pt1000
const float R0 = 1000.0;   // Resistenza a 0°C (1000 Ω)
const float alpha = 0.00385; // Coefficiente termico della Pt1000 (Ω/°C)

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
  float sumVoltage = 0.0;

  // Effettua numReadings letture e calcola la media
  for (int i = 0; i < numReadings; i++) {
    float voltage = analogRead(analogPin) * (Vcc / 1023.0);
    sumVoltage += voltage;
    delay(10);  // Piccolo ritardo tra le letture
  }

  // Calcolo della media del voltaggio
  float Vout = sumVoltage / numReadings;

  // Calcolare la resistenza della Pt1000
  float RT = R1 * (Vcc / Vout - 1);

  // Calcolare la temperatura con la formula della Pt1000
  float temperatureC = (RT - R0) / (R0 * alpha);

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
