#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#define RST_PIN 9
#define SS_PIN 10

const byte ROWS = 4;
const byte COLS = 3;
unsigned int result = 0;
unsigned int userInput = 0;
unsigned int amount = 5000;
int count = 0, value = 50, i = 0;
int flag, b, c;

String rfidTagId = "";
String password = "1234#";
String input = "";
int inputArray[4];

// Define the keypad matrix
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

// Define the keypad pins
byte rowPins[ROWS] = { 9, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8 };

// Create the keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  // Start serial communication
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
}
void (*resetFunc)(void) = 0;
void loop() {
  char key = keypad.getKey();
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    rfidTagId = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      rfidTagId += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      rfidTagId += String(mfrc522.uid.uidByte[i], HEX);
    }

    lcd.setCursor(0, 0);
    lcd.print("Welcome");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter password");
    lcd.setCursor(0, 1);
  }
  if (count < 5 && key && rfidTagId != "c0e179a3") {
    lcd.print("wcard");
    resetFunc();
  }

  if (count < 5 && key && rfidTagId == "c0e179a3") {
    input += key;
    lcd.setCursor(0, 0);
    lcd.print("Enter password");
    lcd.setCursor(0, 1);
    lcd.print("*");
    count++;
  }

  if (count >= 5 && count < 9 && rfidTagId == "c0e179a3" && key && i < 4 && isdigit(key)) {
    inputArray[i] = key - '0';
    lcd.print(key);
    count++;
    i++;
  }
  if (input == password && key == '#' && rfidTagId == "c0e179a3") {
    lcd.clear();
    lcd.setCursor(0, 0);  // Set the cursor to the top-left corner

    

    lcd.print("Welcome");
    lcd.setCursor(0, 1);
    lcd.print("Het");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("YOU HAVE ");
    lcd.setCursor(0, 1);
    lcd.print(amount);
    lcd.print("RS.");
    delay(3000);
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("ENTER AMOUNT");
    lcd.setCursor(0, 1);
    flag = 1;
  }
  if (flag = 1 && key == '*') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FILLING....");
    Serial.println("FILLING....\n");
    digitalWrite(2, HIGH);
    if (i == 4) { userInput = inputArray[0] * 1000 + inputArray[1] * 100 + inputArray[2] * 10 + inputArray[3]; }
    if (i == 3) { userInput = inputArray[0] * 100 + inputArray[1] * 10 + inputArray[2]; }
    if (i == 2) { userInput = inputArray[0] * 10 + inputArray[1]; }
    result = userInput * value;
    delay(result);
    digitalWrite(2, LOW);
    lcd.clear();

    int left_amount = amount - userInput;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("YOU HAVE ");
    lcd.setCursor(0, 1);
    lcd.print(left_amount);
    lcd.print("RS.");
    delay(3000);
    lcd.clear();
    lcd.print("THANK YOU....");
    delay(3000);
    resetFunc();
  }
  if (input != password && key == '#' && rfidTagId == "c0e179a3") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Declined");
    lcd.setCursor(0, 1);
    lcd.print("THANK YOU .. ");
    delay(3000);
    resetFunc();
  }
}