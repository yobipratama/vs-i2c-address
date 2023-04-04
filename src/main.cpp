#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>
#define DEGREE_SYMBOL 0xDF

const int DHT_PIN = D3;
SimpleDHT11 dht(DHT_PIN);

const int BLUE_LED_PIN = D5;
const int GREEN_LED_PIN = D6;
const int RED_LED_PIN = D7;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C untuk LCD

void setup()
{
    Serial.begin(115200);
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    lcd.init();      // Inisialisasi LCD
    lcd.backlight(); // Hidupkan backlight
    lcd.setCursor(0, 0);
    lcd.print("");
}

void loop()
{
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
    {
        Serial.print("Gagal membaca dari sensor DHT11, kode error: ");
        Serial.println(err);
        return;
    }

    float temperatureF = temperature * 1.8 + 32;

    Serial.print(String(temperature, DEC) + "°C ");
    Serial.print(String(temperatureF, 1) + "°F ");

    lcd.setCursor(0, 0);
    lcd.print(String(temperature, DEC));
    lcd.write(DEGREE_SYMBOL);
    lcd.print("C ");
    lcd.print(String(temperatureF, 1));
    lcd.write(DEGREE_SYMBOL);
    lcd.print("F");

    lcd.setCursor(0, 1);
    lcd.print("04-04-2023 16.00");
    if (temperature >= 29)
    { // suhu tinggi, LED merah akan berkedip
        digitalWrite(BLUE_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        for (int i = 0; i < 5; i++)
        {
            digitalWrite(RED_LED_PIN, HIGH);
            delay(100);
            digitalWrite(RED_LED_PIN, LOW);
            delay(100);
        }
    }
    else if (temperature <= 28)
    { // suhu rendah, LED hijau akan berkedip
        digitalWrite(BLUE_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        for (int i = 0; i < 5; i++)
        {
            digitalWrite(GREEN_LED_PIN, HIGH);
            delay(100);
            digitalWrite(GREEN_LED_PIN, LOW);
            delay(100);
        }
    }
    else
    { // suhu normal, LED biru akan berkedip
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        for (int i = 0; i < 5; i++)
        {
            digitalWrite(BLUE_LED_PIN, HIGH);
            delay(100);
            digitalWrite(BLUE_LED_PIN, LOW);
            delay(100);
        }
    }

    Serial.println("---");
    delay(1000);
}