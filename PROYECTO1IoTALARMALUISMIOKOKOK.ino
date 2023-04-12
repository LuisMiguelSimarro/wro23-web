/*Partiendo de los Sketchs de código secreto y de cerradura
  magnética en la que se usa la memoria EEPROM para guardar el
  código de seguridad de Simon Monk he añadido el
  buzzer y el sensor IPIR (DIGITAL)REGULABLE EN TIEMPO DE ESPERA, DISTANCIA DE DETECCIÓN Y REPETITIBILIDAD
*/
#include <Key.h>
#include <EEPROM.h>º
#include <Keypad.h>

char* secretCode = "1234";//es lo mismo que int secretCode[]={1,2,3,4}
int position = 0;
boolean cerrado = true;

const byte rows = 4;
const byte cols = 4;

char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
//Hay que realizar el mapa de cada teclado o keypad
byte rowPins[rows] = {9,8,7,6};
byte colPins[cols] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

const int redPin = 10;
const int greenPin = 11;
const int buzzerPin = 12;
const int PIRPin =13;
int PIRValor = 0;
const int SendALARMMessagePin=22;

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(PIRPin, INPUT);
  loadCode();
  flash();
  activaAlarma();//activa ALARMA
  pinMode(SendALARMMessagePin, OUTPUT);
  digitalWrite(SendALARMMessagePin, HIGH);
  delay(60000); //para darnos tiempo a irnos
}

void loop(){
  PIRValor = digitalRead(PIRPin);
  char key = keypad.getKey();
    if (PIRValor == 1 && cerrado == true) {
    position = 0;
    //cerrado = true;
    ALARMALEDS();
    ALARMABUZZER();
    digitalWrite(SendALARMMessagePin, LOW);//LA ESP32 ENVIA MENSAJE POR WASSAP A MI MOVIL
  } else if (key == secretCode[position]) {
    position ++;
    digitalWrite(SendALARMMessagePin, HIGH);//PARA QUE NO ENVIE MENSAJES UNA VEZ METIDO EL CODIGO
  } if (position == 4) {
    noTone(buzzerPin);
    cerrado = false;
    activaAlarma();
  } delay(100);
  
  if(key == '*' && ! cerrado)
  {
    position = 0;
    getNewCode();
    activaAlarma(); 
  }
  if(key == '#')
  {
    cerrado = true;
    position = 0;
    activaAlarma();
  }
  
}

void activaAlarma()
{
  if(cerrado)
  {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    
  }
  else
  {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    
  }
}

void getNewCode()
{
  flash();
  for (int i = 0; i < 4; i++ )
  {
    char key;
    key = keypad.getKey();
    while(key == 0)
    {
      key = keypad.getKey();
    }
    flash();
    secretCode[i] = key;
  }
  saveCode();
  flash();flash();
}

void loadCode()
{
  if (EEPROM.read(0) == 1)
  {
    secretCode[0] = EEPROM.read(1);
    secretCode[1] = EEPROM.read(2);
    secretCode[2] = EEPROM.read(3);
    secretCode[3] = EEPROM.read(4);
  }
}

void saveCode()
{
  EEPROM.write(1, secretCode[0]); 
  EEPROM.write(2, secretCode[1]);
  EEPROM.write(3, secretCode[2]); 
  EEPROM.write(4, secretCode[3]);
  EEPROM.write(0, 1);
}

void flash()
{
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  delay(500);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
    
}

void ALARMALEDS() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  delay(200);
  digitalWrite(redPin, LOW);
}
void ALARMABUZZER() {
  digitalWrite(buzzerPin, HIGH);
  // delay(500);
}
  
