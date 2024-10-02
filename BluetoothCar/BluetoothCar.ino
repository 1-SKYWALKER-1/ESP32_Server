// This example code is in the Public Domain (or CC0 licensed, at your option.)
// By Evandro Copercini - 2018
//
// This example creates a bridge between Serial and Classical Bluetooth (SPP)
// and also demonstrate that SerialBT have the same functionalities of a normal Serial
// Note: Pairing is authenticated automatically by this device

#include "BluetoothSerial.h"

String device_name = "CarServer";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
String receivedString;  // Переменная для хранения полученной строки
String myString = "A";  // Ваша строка для сравнения
const int ledPin = 13;

void setup() {
  pinMode(ledPin,OUTPUT);
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
}


void loop() {
  if (SerialBT.available()) {
    receivedString = SerialBT.readString();  // Считываем строку через Bluetooth

    // Сравниваем полученную строку с вашей строкой
    if (receivedString.equals(myString)) {
      digitalWrite(ledPin, HIGH);  // Включаем светодиод на D13
      Serial.println("Строки совпадают! Светодиод включен.");  // Сообщаем об этом в Serial монитор
    } else {
      digitalWrite(ledPin, LOW);  // Выключаем светодиод на D13
      Serial.println("Строки не совпадают. Светодиод выключен.");  // Сообщаем об этом в Serial монитор
    }
  }
  delay(20);
}
