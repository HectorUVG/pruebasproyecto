//Hector Manuel de Leon Salguero
//Universidad del Valle de Guatmala
//
//
//
//*****************************************************************************
//Librerias
//*****************************************************************************

#include <Arduino.h>
//*****************************************************************************
//Definicion de pines
//*****************************************************************************

#define ledV 0
#define ledA 2
#define ledR 15
#define pwmChannelV 5  // 16 canales 0-15
#define pwmChannelA 4
#define pwmChannelR 3
#define freqPWM 5000  // Frecuencia en Hz
#define resolution 8  // 1-16 bits de resolución 

#define boton1 13

#define servo 12
// Paso 1: selección de parámetros de la señal PWM
#define pwmChnlServo 2 // 16 canales 0-15
#define freqPWMServo 50 // Frecuencia en Hz
//#define resolution 8  // 1-16 bits de resolución  creo que sobra


//*****************************************************************************
//Prototipos de funciones
//*****************************************************************************
void configurarPWM(void);
void leds(void);
//*****************************************************************************
//Variables Globales
//*****************************************************************************
float temperatura = 0.0;

//variables al separar el valor del sensor
int decenas = 0;
int unidades = 0;
int decimales = 0;

//var para el interruptor del boton
int banderaB1 = 0;




//*****************************************************************************
//ISR
//*****************************************************************************

//*****************************************************************************
//Configuracion
//*****************************************************************************

void setup() {
  configurarPWM();

}
//*****************************************************************************
//Loop principal
//*****************************************************************************

void loop() {
  temperatura = 37.5;
  leds();
 //yes
}

//****************************************************************
// Función para configurar módulo PWM
//****************************************************************
void configurarPWM(void){

  // Paso 1: Configurar el módulo PWM
  ledcSetup(pwmChannelV, freqPWM, resolution);
  ledcSetup(pwmChannelA, freqPWM, resolution);
  ledcSetup(pwmChannelR, freqPWM, resolution);

  ledcSetup(pwmChnlServo, freqPWMServo, resolution);


  // Paso 2: seleccionar en que GPIO tendremos nuestra señal PWM
  ledcAttachPin(ledV, pwmChannelV);
  ledcAttachPin(ledA, pwmChannelA);
  ledcAttachPin(ledR, pwmChannelR);

  ledcAttachPin(servo, pwmChnlServo);

}

//*****************************************************************************
//Funcion Leds
//*****************************************************************************
void leds(){
  if (temperatura <= 37.0){
    ledcWrite(pwmChannelV, 200);
    ledcWrite(pwmChannelA, 0);
    ledcWrite(pwmChannelR, 0);
  }

  if  (temperatura > 37.0 && temperatura < 37.5){
    ledcWrite(pwmChannelV, 0);
    ledcWrite(pwmChannelA, 200);
    ledcWrite(pwmChannelR, 0);
  }

  if (temperatura >= 37.5){
    ledcWrite(pwmChannelV, 0);
    ledcWrite(pwmChannelA, 0);
    ledcWrite(pwmChannelR, 200);
  }

}