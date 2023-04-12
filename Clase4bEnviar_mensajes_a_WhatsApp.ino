#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

WiFiClient wificlient;

const char* ssid = "Livebox6-215D";
const char* password = "a3RKfxYKJ4nJ";

unsigned long debounce = 0;

const int inputPin = 0; // pin de entrada D3 para la ESP8266 MOD12+ NODEMCU V3
boolean inputState = false; //almacenar el estado actual de la salida
String solicitud;

void setup() {
  
  Serial.begin(115200);
  
  pinMode(inputPin, INPUT_PULLUP);

  Serial.print("Conectando a "); 
  Serial.println(ssid);

  WiFi.begin(ssid,password); // Inicializar conexion 

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Conexion exitosa

  Serial.println("");
  Serial.println("Dispositivo conectado.");
  Serial.println("Direccion IP: ");
  Serial.print(WiFi.localIP()); // Mostrar la direccion ip
  

}

void loop() {
  
   if(!digitalRead(inputPin)) pulse();

   if(inputState)
   {

      Serial.println("Enviando mensaje a WhatsApp");
      HTTPClient http;

      solicitud = "http://api.callmebot.com/whatsapp.php?";
      solicitud = solicitud + "phone=+34646559440";
      //solicitud = solicitud + "&text=Gpio+1+:"+String(inputState);
      solicitud = solicitud + "&text=Alarma+Puerta2+HA+SALTADO!:"+String(inputState);
      solicitud = solicitud + "&apikey=7645033";
      http.begin(wificlient,solicitud);

      int httpCode = http.GET();

      if(httpCode >0)
        {
          String payload = http.getString();
          Serial.println(payload);
        }

        http.end();

        inputState = false;
      

   }



}

void pulse()
{
 if(!digitalRead(inputPin) && (millis()-debounce > 500))
  {
    debounce = millis();
    inputState =  true;
  }
}
