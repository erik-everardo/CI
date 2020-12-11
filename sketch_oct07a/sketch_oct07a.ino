/*Escrito por Erik Cavazos
** Computo Integrado
** Galled Gate
**/
#include <NewPing.h>
#include <LiquidCrystal.h>

/*Pines display*/
#define rs 10
#define rw 9
#define enable 8
#define d4 7
#define d5 6
#define d6 5
#define d7 4
#define display_led 13

/*Pines sensor ultrasonico*/
#define triger 3
#define echo 2

/*Actuadores*/
/*Ambos van conectados con transistores*/
#define dispensador 11
#define cerradura 1  /*12V*/

LiquidCrystal lcd(rs,rw, enable, d4,d5,d6,d7);
NewPing Sonar(triger,echo,200);
int distancia=0;



void setup() {
  lcd.begin(16,2);
  pinMode(display_led,OUTPUT);
  pinMode(dispensador,OUTPUT);
  pinMode(cerradura,OUTPUT);
  lcd.write("Bienvenido");
  lcd.setCursor(0,1);
  lcd.write("Use dispensador");
  digitalWrite(cerradura,LOW);
  digitalWrite(display_led,HIGH);
}

void loop() {
  // se utiliza la libreria NewPing (#include NewPing.h)
  distancia=Sonar.ping_median()/US_ROUNDTRIP_CM;
  
  // Se detecta la distancia al sensor. Si es menor a 5 cm,
  // entonces se comienza el proceso de dispensar el gel.
  if(distancia<=5) {
    dispensarGel();
  }
}

/*
* Esta funcion realiza la secuencia principal:
* => Imprimir en pantalla "Dispensando gel..."
* => Encender la salida digital a la que está
*    conectado el dispensador,
*    esperar 350 milisegundos y luego apagarla
*    de nuevo.
* => Mover el cursor de la pantalla al primer
*    renglon e imprimir "Puerta abierta".
*    Despues, mover el cursor al siguiente 
*    renglón e imprimir "Pase!".
* => Llamar a estadoCerradura(bool abrir), 
*    pasando true. Esto para abrir la cerradura.
* => Esperar 5 segundos (5000 milisegundos).
* => Llamar a estadoCerradura(bool abrir), pero
*    esta vez con false. Esto para cerrar la cerradura.
* => Volver a empezar. Imprime mensaje de 
*    bienvenida, que es el estado en reposo del sistema.
*/
void dispensarGel() {
  lcd.setCursor(0,1);
  lcd.write("Dispensando gel...");
  digitalWrite(dispensador,HIGH);
  delay(350);
  digitalWrite(dispensador,LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("Puerta abierta");
  lcd.setCursor(0,1);
  lcd.write("Pase!");
  estadoCerradura(true);
  delay(5000);
  estadoCerradura(false);
  lcd.clear();
  lcd.write("Bienvenido");
  lcd.setCursor(0,1);
  lcd.write("Use dispensador");
}

/*
* Esta funcion es autoexplicativa. La salida digital es
* conectada a un transistor y este a un modulo relevador.
* Al estar en true la salida digital, el relevador deja 
* pasar la corriente de 12V hacia la cerradura.
*/
void estadoCerradura(bool abrir){
  if(abrir) digitalWrite(cerradura,HIGH);
  else digitalWrite(cerradura,LOW);
}