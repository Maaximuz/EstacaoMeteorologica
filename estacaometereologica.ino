#include "ThingSpeak.h"
#include "WiFi.h"

#include "DHT.h"


#define pin1 4      
//#define pin2 23      

const char* ssid = "test";                        //SSID do roteador.
const char* password = "pass";                //Senha do roteador.

unsigned long channelID = ****;                //ID do canal.
const char* WriteAPIKey = "****";     //Write API Key do canal.

WiFiClient cliente;

DHT dht1(pin1, DHT11);    //o DHT11.
int rele = 23;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Teste de sensores:");

  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conectado!");

  ThingSpeak.begin(cliente);

  dht1.begin();
  pinMode(rele, OUTPUT); 
  //rele1.begin();
}

void loop() {
  delay(2000);
  leerdht1();

  ThingSpeak.writeFields(channelID,WriteAPIKey);
  Serial.println("Dados enviados ao ThingSpeak!");
  delay(14000);
}

void leerdht1() {
  
  float t1 = dht1.readTemperature();
  float h1 = dht1.readHumidity();

  while (isnan(t1) || isnan(h1)){
    Serial.println("Leitura falhada, repetindo leitura...");
    delay(2000);
    t1 = dht1.readTemperature();
    h1 = dht1.readHumidity();
  }

  Serial.print("Temperatura DHT11: ");
  Serial.print(t1);
  Serial.println(" ºC.");

  Serial.print("Umidade DHT11: ");
  Serial.print(h1);
  Serial.println(" %."); 

  Serial.println("-----------------------");

  ThingSpeak.setField (1,t1);
  ThingSpeak.setField (2,h1);

  if(t1 >= 31){
    digitalWrite(rele, HIGH);
    Serial.println("Relé ligado");
  }else{
    digitalWrite(rele, LOW);
    Serial.println("Relé desligado");
  }
}
