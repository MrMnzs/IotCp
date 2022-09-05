#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <Servo.h>
#define SERVO 6 
const int TAMANHO = 40;
Servo s; // Variável Servo
int pos; // Posição Servo
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 

 
char st[20];
 
void setup() 
{

  s.attach(SERVO);
  s.write(0); // Inicia motor posição zero
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
    
}
 
void loop() 
{ 

  if (Serial.available() > 0) {
    //Lê o JSON disponível na porta serial:
    StaticJsonDocument<TAMANHO> json;
    deserializeJson(json, Serial);
    if(json.containsKey("abre")) {
      int valor = json["abre"];
      
      s.write(valor);
      digitalWrite(LED_BUILTIN, HIGH);
    }
     digitalWrite(LED_BUILTIN, LOW);
  } 
    
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  //Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
 // Serial.println();
 // Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "B9 78 F0 98") //UID 1 - Chaveiro
  {
    StaticJsonDocument<TAMANHO> json;
    json["usuario"] = conteudo.substring(1);
    serializeJson(json, Serial);
    Serial.println();
    delay(3000);
    
  } 
} 
