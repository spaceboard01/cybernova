#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Servo.h>

// Pantalla
Adafruit_SSD1306 pantalla(128, 64, &Wire, -1);

// Sensor DHT11
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pines
int llama = 5;
int buzzer = 10;
int humedadSuelo = A1;
int gas = A3;

// Servos
Servo servo1;
Servo servo2;

void setup() {

  Serial.begin(9600);

  pinMode(llama, INPUT);
  pinMode(buzzer, OUTPUT);

  dht.begin();

  servo1.attach(9);
  servo2.attach(3);

  //  ventanas estan cerradas
  servo1.write(0);
  servo2.write(0);

  // Iniciar pantalla
  pantalla.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pantalla.clearDisplay();
  pantalla.setTextColor(WHITE);

  pantalla.setTextSize(1);
  pantalla.setCursor(20, 20);
  pantalla.println("CYBERNOVA");

  pantalla.setCursor(10, 35);
  pantalla.println("Refugio inteligente");

  pantalla.display();

  delay(2000);
}

void loop() {

  // leer temperatura y humedad
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  // leer sensor de llama
  int fuego = digitalRead(llama);

  // leer humedad del suelo
  int lecturaSuelo = analogRead(humedadSuelo);

  int suelo = map(lecturaSuelo, 1023, 0, 0, 100);

  // leer gas
  int lecturaGas = analogRead(gas);

  int nivelGas = map(lecturaGas, 0, 1023, 0, 100);

  // si hay mucho calor, fuego o gas
  if (temperatura > 30 || fuego == HIGH || nivelGas > 60) {

    servo1.write(90);
    servo2.write(90);

    tone(buzzer, 2000);

  } else {

    servo1.write(0);
    servo2.write(0);

    noTone(buzzer);
  }

  // Mostrar datos en la pantalla
  pantalla.clearDisplay();

  pantalla.setTextSize(1);
  pantalla.setCursor(0, 0);
  pantalla.println("REFUGIO INTELIGENTE");

  pantalla.setCursor(0, 12);
  pantalla.print("Temperatura: ");
  pantalla.print(temperatura);
  pantalla.println(" C");

  pantalla.setCursor(0, 22);
  pantalla.print("Humedad: ");
  pantalla.print(humedad);
  pantalla.println(" %");

  pantalla.setCursor(0, 32);
  pantalla.print("Gas: ");
  pantalla.print(nivelGas);
  pantalla.println(" %");

  pantalla.setCursor(0, 42);
  pantalla.print("Suelo: ");
  pantalla.print(suelo);
  pantalla.println(" %");

  pantalla.setCursor(0, 52);

  if (fuego == HIGH) {
    pantalla.println("FUEGO: DETECTADO");
  } else {
    pantalla.println("FUEGO: NO");
  }

  pantalla.display();

  // Mostrar datos en el monitor serial
  Serial.println("--------------------");

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  Serial.print("Gas: ");
  Serial.print(nivelGas);
  Serial.println(" %");

  Serial.print("Humedad suelo: ");
  Serial.print(suelo);
  Serial.println(" %");

  if (fuego == HIGH) {
    Serial.println("Fuego detectado");
  } else {
    Serial.println("No hay fuego");
  }

  delay(1000);
}