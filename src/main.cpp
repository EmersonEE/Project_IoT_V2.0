#include <Arduino.h>

#include <Wire.h>
// #include <LCD.h>
#include <LiquidCrystal_I2C.h>

#if defined(ESP32)
#include <FirebaseESP32.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FirebaseESP8266.h>
#include <DNSServer.h>
#endif
#include <WiFiManager.h>
// #define WIFI_SSID "CLARO_h9hU3j"   // Nombre del WiFi al que se debe conectar
// #define WIFI_PASSWORD "7474FB19FD" // Contraseña WiFi

// Conectarse a la base de datos de Firebase

#define FIREBASE_HOST "https://iot-project-322be-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "RaPXJVlFUPV1fSaz2mthF8VP999tSDsqF8eilud3"

LiquidCrystal_I2C lcd(0x27, 16, 2); // DIR, E, RW, RS, D4, D5, D6, D7

const uint8_t rele1 = D5; // 7 const es como declararlo como varible no se le puede dar otro valor en el codigo
const uint8_t rele2 = D6; // const uint8_t es para ahorrar memoria del ESP32 / ESP8266
const uint8_t rele3 = D7;
const uint8_t rele4 = D8;
const uint8_t relayNumber = 4;
const uint8_t relays[relayNumber] = {rele1, rele2, rele3, rele4};
float datos = 0;
uint8_t inswitch1 = D0;

uint8_t estadopush2 = D3;
uint8_t estadopush3 = D4;
int estadopush1ant = 0;
int estadopush1 = 0;
int estadopush2ant = 0;
int estadopush3ant = 0;
bool prevLed1Status = false;
bool prevLed2Status = false;
bool prevLed3Status = false;
bool prevLed4Status = false;
bool enviarFirebase = false;
int conteo = 0;
int conteo2 = 0;
int conteo3 = 0;
int conteo4 = 0;
uint8_t inicio = 0;

byte CandadoCerrado[] = {
    0x0E,
    0x0A,
    0x0A,
    0x1F,
    0x1F,
    0x1B,
    0x1B,
    0x1F};
byte CandadoAbierto[] = {
    0x0E,
    0x08,
    0x08,
    0x1F,
    0x1F,
    0x1B,
    0x1B,
    0x1F};
byte HLH[] = {
    B00000,
    B00001,
    B00010,
    B00100,
    B01001,
    B10001,
    B10000,
    B11111};
byte HRH[] = {
    B00000,
    B10000,
    B01000,
    B00100,
    B10010,
    B10001,
    B00001,
    B11111};
byte HLL[] = {
    B10000,
    B10001,
    B10001,
    B10000,
    B10000,
    B10011,
    B10010,
    B11110};
byte HRL[] = {
    B00001,
    B10001,
    B10001,
    B00001,
    B00001,
    B11001,
    B01001,
    B01111};

FirebaseData firebaseData;
String path = "/Test";
void setup()
{
  Serial.begin(115200);

  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  // reset saved settings, descomentar solo en la primera subida de codigo
  // wifiManager.resetSettings();

  Wire.begin();
  lcd.backlight(); // habilita iluminacion posterior de LCD
  lcd.begin(16, 2);
  pinMode(LED_BUILTIN_AUX, OUTPUT);
  for (uint8_t i = 0; i < relayNumber; i++) // Declaro los Pines de salida
  {
    pinMode(relays[i], OUTPUT);
  }
  pinMode(inswitch1, INPUT);
  pinMode(estadopush2, INPUT);
  pinMode(estadopush3, INPUT);

  lcd.clear();
  lcd.createChar(1, CandadoAbierto);
  lcd.createChar(2, CandadoCerrado);
  lcd.createChar(3, HLH);
  lcd.createChar(4, HRH);
  lcd.createChar(5, HLL);
  lcd.createChar(6, HRL);

  // connect to wifi.

  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi ");
  lcd.setCursor(1, 0);
  lcd.print("Connecting To ");
  lcd.setCursor(3, 1);
  lcd.print("WiFi");
  wifiManager.autoConnect("IoT Project"); // Wifi que creara
  Serial.print("Connected");
  // Mensaje del WiFi en consola
  Serial.println();
  Serial.print("Connected To: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address");
  Serial.println(WiFi.localIP());
  Serial.print("mac address");
  Serial.println(WiFi.macAddress());
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Connected To:");
  lcd.setCursor(2, 1);
  lcd.print(WiFi.SSID());
  delay(3000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("IP Address:");
  lcd.setCursor(2, 1);
  lcd.print(WiFi.localIP());
  delay(3000);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IoT Project");
  lcd.setCursor(0, 1);
  lcd.print("By Emerson ");
  lcd.setCursor(14, 0);
  lcd.write(byte(3));
  lcd.setCursor(15, 0);
  lcd.write(byte(4));
  lcd.setCursor(14, 1);
  lcd.write(byte(5));
  lcd.setCursor(15, 1);
  lcd.write(byte(6));
  delay(2500);
  lcd.clear();
  lcd.home();
  lcd.print("Load1:  ");
  lcd.setCursor(8, 0);
  lcd.print("Load2: ");
  lcd.setCursor(0, 1);
  lcd.print("Load3: ");
  lcd.setCursor(8, 1);
  lcd.print("Load4: ");
  lcd.setCursor(6, 0);
  lcd.write(byte(2));
  lcd.setCursor(14, 0);
  lcd.write(byte(2));
  lcd.setCursor(6, 1);
  lcd.write(byte(2));
  lcd.setCursor(14, 1);
  lcd.write(byte(2));
}
// Para encenderlo con un Switch
void pruebaswitch1()
{
  // Leer el estado del botón
  estadopush1 = digitalRead(inswitch1);

  if (estadopush1 == HIGH && estadopush1 != estadopush1ant)
  {
    // El botón está presionado
    Firebase.setBool(firebaseData, "Led1Status", true);
    estadopush1ant = estadopush1;
    Serial.println("Botón presionado");
  }
  else if (estadopush1 == LOW && estadopush1 != estadopush1ant)
  {
    // El botón no está presionado
    Firebase.setBool(firebaseData, "Led1Status", false);
    estadopush1ant = estadopush1;
    Serial.println("Botón no presionado");
  }

  // Otras acciones y lógica del programa
}
// Para encenderlo con un boton
// void switchLed1()
// {
//   if (digitalRead(estadopush1))
//   {
//     while (digitalRead(estadopush1))
//     {
//     }
//     conteo += 1;
//     if (conteo > 1)
//     {
//       conteo = 0;
//     }
//   }
//   if (conteo != estadopush1ant)
//   {
//     estadopush1ant = conteo;
//     if (conteo)
//     { // si es int (1) si es booleano (true), compara estos valores
//       Firebase.setBool(firebaseData, "Led1Status", true);
//     }
//     else
//     {
//       Firebase.setBool(firebaseData, "Led1Status", false);
//     }
//   }
// }
void switchLed2()
{
  if (digitalRead(estadopush2))
  {
    while (digitalRead(estadopush2))
    {
    }
    conteo2 += 1;
    if (conteo2 > 1)
    {
      conteo2 = 0;
    }
  }
  if (conteo2 != estadopush2ant)
  {
    estadopush2ant = conteo2;
    if (conteo2)
    { // si es int (1) si es booleano (true), compara estos valores
      Firebase.setBool(firebaseData, "Led2Status", true);
    }
    else
    {
      Firebase.setBool(firebaseData, "Led2Status", false);
    }
  }
}

void switchLed3()
{
  if (digitalRead(estadopush3))
  {
    while (digitalRead(estadopush3))
    {
    }
    conteo3 += 1;
    if (conteo3 > 1)
    {
      conteo3 = 0;
    }
  }
  if (conteo3 != estadopush3ant)
  {
    estadopush3ant = conteo3;
    if (conteo3)
    { // si es int (1) si es booleano (true), compara estos valores
      Firebase.setBool(firebaseData, "Led3Status", true);
    }
    else
    {
      Firebase.setBool(firebaseData, "Led3Status", false);
    }
  }
}
void loop()
{

  // First Load
  // switchLed1();
  switchLed2();
  switchLed3();
  pruebaswitch1();
  if (Firebase.getString(firebaseData, "Led1Status"))
  {
    // switchLed1();
    pruebaswitch1();
    bool led1Status = (firebaseData.stringData() == "true");

    if (led1Status != prevLed1Status) // Compare with previous status
    {
      prevLed1Status = led1Status; // Update previous status
      if (led1Status)
      {
        Serial.println("on1");
        digitalWrite(rele1, HIGH);
        lcd.setCursor(6, 0);
        lcd.write(byte(1));
      }
      else
      {
        Serial.println("off1");
        digitalWrite(rele1, LOW);
        lcd.setCursor(6, 0);
        lcd.write(byte(2));
      }
    }
  }

  // Second Load
  if (Firebase.getString(firebaseData, "Led2Status"))
  {
    switchLed2();
    bool led2Status = (firebaseData.stringData() == "true");

    if (led2Status != prevLed2Status) // Compare with previous status
    {
      prevLed2Status = led2Status; // Update previous status
      if (led2Status)
      {
        Serial.println("on2");
        digitalWrite(rele2, 1);
        lcd.setCursor(14, 0);
        lcd.write(byte(1));
        conteo2 = 1;
      }
      else
      {
        Serial.println("off2");
        digitalWrite(rele2, 0);
        lcd.setCursor(14, 0);
        lcd.write(byte(2));
        conteo2 = 0;
      }
    }
  }

  // Third Load
  if (Firebase.getString(firebaseData, "Led3Status"))
  {
    switchLed3();
    bool led3Status = (firebaseData.stringData() == "true");

    if (led3Status != prevLed3Status) // Compare with previous status
    {
      prevLed3Status = led3Status; // Update previous status
      if (led3Status)
      {
        Serial.println("on3");
        digitalWrite(rele3, 1);
        lcd.setCursor(6, 1);
        lcd.write(byte(1));
        conteo3 = 1;
      }
      else
      {
        Serial.println("off3");
        digitalWrite(rele3, 0);
        lcd.setCursor(6, 1);
        lcd.write(byte(2));
        conteo3 = 0;
      }
    }
  }

  // Fourth Load
  if (Firebase.getString(firebaseData, "Led4Status"))
  {
    bool led4Status = (firebaseData.stringData() == "true");

    if (led4Status != prevLed4Status) // Compare with previous status
    {
      prevLed4Status = led4Status; // Update previous status
      if (led4Status)
      {
        Serial.println("on4");
        digitalWrite(rele4, 1);
        lcd.setCursor(14, 1);
        lcd.write(byte(1));
      }
      else
      {
        Serial.println("off4");
        digitalWrite(rele4, 0);
        lcd.setCursor(14, 1);
        lcd.write(byte(2));
      }
    }
  }
}
