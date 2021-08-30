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

#define B1 13

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
void boton(void);

//para prueba
void botonTemporal(void);
void temp2(void);
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

//boton
int contBoton = 0;

//funcion temporal prueba servo
int temporal = 0;

//*****************************************************************************
//ISR
//*****************************************************************************
void IRAM_ATTR boton(){
  contBoton = 1;
  
}
//*****************************************************************************
//Configuracion
//*****************************************************************************

void setup() {
  configurarPWM();

  //boton
  pinMode(B1, INPUT_PULLUP);
  attachInterrupt(B1, boton, RISING);
}
//*****************************************************************************
//Loop principal
//*****************************************************************************

void loop() {
 // temperatura = 37.5;

 //para prueba
  botonTemporal();
  temp2();

 
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
    ledcWrite(pwmChannelV, 255);
    ledcWrite(pwmChannelA, 0);
    ledcWrite(pwmChannelR, 0);

    ledcWrite(pwmChnlServo, 5);
  }

  if  (temperatura > 37.0 && temperatura < 37.5){
    ledcWrite(pwmChannelV, 0);
    ledcWrite(pwmChannelA, 255);
    ledcWrite(pwmChannelR, 0);

    ledcWrite(pwmChnlServo,19);
  }

  if (temperatura >= 37.5){
    ledcWrite(pwmChannelV, 0);
    ledcWrite(pwmChannelA, 0);
    ledcWrite(pwmChannelR, 255);

    ledcWrite(pwmChnlServo, 32);
  }

}

//*****************************************************************************
//Funcion temporal de boton para probar servo
//*****************************************************************************
void botonTemporal(){
  
  if (contBoton == 1 && temporal < 2){
    temporal = temporal + 1;
    delay(150);
    contBoton = 0;
    delay(150);
    
  }

  if (contBoton == 1 && temporal ==2 ){
    temporal = 0 ;
    delay(150);
    contBoton = 0;
    delay(150);
  }
  
 
}

//*****************************************************************************
//Funcion temporal de boton para probar servo2
//*****************************************************************************

void temp2(){
  if(temporal == 0){
    temperatura = 37.0;
  }

  if(temporal == 1){
    temperatura = 37.3;
  }

  if(temporal == 2){
    temperatura = 37.5;
  }
}