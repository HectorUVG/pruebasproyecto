//Hector Manuel de Leon Salguero
//Universidad del Valle de Guatmala
//
//
//
//*****************************************************************************
//Librerias
//*****************************************************************************

#include <Arduino.h>
#include "Displays7seg.h"
#include "esp_adc_cal.h"
//*****************************************************************************
//Definicion de pines
//*****************************************************************************

//displays
#define Dis1 18
#define Dis2 19
#define Dis3 21

//pines del display
#define g 26
#define f 25
#define a 33
#define b 32
#define e 3
#define d 5
#define c 22
#define p 23

//prescaler para contador
#define prescaler 80

//leds
#define ledV 0
#define ledA 2
#define ledR 15
#define pwmChannelV 5 // 16 canales 0-15
#define pwmChannelA 4
#define pwmChannelR 3
#define freqPWM 5000 // Frecuencia en Hz
#define resolution 8 // 1-16 bits de resolución
//boton
#define Boton1 13

//servo
#define servo 12
// Paso 1: selección de parámetros de la señal PWM
#define pwmChnlServo 2  // 16 canales 0-15
#define freqPWMServo 50 // Frecuencia en Hz
//#define resolution 8  // 1-16 bits de resolución  creo que sobra

//sensor
#define sensor 14

//*****************************************************************************
//Prototipos de funciones
//*****************************************************************************
void configurarPWM(void);
void leds(void);
void boton(void);

//para prueba
void botonTemporal(void);
void temp2(void);

//Displays
void configurarTimer(void);
//void numDisplay(int);
void cambioDisplay(int);

//prototipo para sensor
void sens(void);

//*****************************************************************************
//Variables Globales
//*****************************************************************************
//Instanciar timer
hw_timer_t *timer = NULL;

//variable para el contador del timer
int contadorTimer = 0;

float temperatura = 0.0;

//variables al separar el valor del sensor
int decena = 0;
int unidad = 0;
int decimal = 0;

//var para el interruptor del boton
int banderaB1 = 0;

//boton
int contBoton = 0;

//funcion temporal prueba servo
int temporal = 0;

//Variables para sensor
int adcLM35 = 0;
int voltaje = 0;

//delays
unsigned long lastTime;
unsigned int sampleTime = 100;


//*****************************************************************************
//ISR
//*****************************************************************************

void IRAM_ATTR ISRTimer0()
{
  contadorTimer++; // es como sumarle 1
  if (contadorTimer > 2)
  {
    contadorTimer = 0;
  }
  //de 0 a 2 porque solo tengo 3 displays
  /*
  if(contadorTimer == 0){
    contadorTimer = 1;
  }*/
}

void IRAM_ATTR boton()
{
  contBoton = 1;
}
//*****************************************************************************
//Configuracion
//*****************************************************************************

void setup()
{
  Serial.begin(115200);
  lastTime = millis();

  configurarTimer();
  configurarPWM();
  configurarDisplay(a, b, c, d, e, f, g, p);

  //displays
  pinMode(Dis1, OUTPUT);
  pinMode(Dis2, OUTPUT);
  pinMode(Dis3, OUTPUT);
  digitalWrite(Dis1, 0);
  digitalWrite(Dis2, 0);
  digitalWrite(Dis3, 0);

  //boton
  pinMode(Boton1, INPUT_PULLUP);
  attachInterrupt(Boton1, boton, RISING);
}
//*****************************************************************************
//Loop principal
//*****************************************************************************

void loop()
{
  //llamar a la funcion que activa el sensor
  sens();


  leds();
  decena = 7;
  unidad = 4;
  decimal = 4;
 
/*
  numDisplay(decena);
  digitalWrite(Dis1, 1);
  digitalWrite(Dis2, 0);
  digitalWrite(Dis3, 0);
  delay(5);

  numDisplay(unidad);
  digitalWrite(Dis1, 0);
  digitalWrite(Dis2, 1);
  digitalWrite(Dis3, 0);
  digitalWrite(p, 1);

  delay(5);

  numDisplay(decimal);
  digitalWrite(Dis1, 0);
  digitalWrite(Dis2, 0);
  digitalWrite(Dis3, 1);
  delay(5);*/


  //para prueba
  botonTemporal();
  temp2();

  //contadorTimer permite cambiar entre los 3 displays cada 10 milisegundos
  cambioDisplay(contadorTimer);
}

//*****************************************************************************
//Funcion configurar timer
//*****************************************************************************
void configurarTimer(void)
{

  //Fosc(Frecuencia de oscilacion) = 80 MHz = 80,000,000 Hz
  // Fosc / prescaler = 80,000,000 / 80 = 1,000,000 Hz
  //Tosc(Tiempo de oscilacion) = 1 / Fosc = 1 uSeg

  //Paso2. Seleccionar timer
  // Timer 0, prescaler = 80, flanco de subida
  timer = timerBegin(0, prescaler, true);

  //paso3. Asignar el handler de la interrupcion

  timerAttachInterrupt(timer, &ISRTimer0, true);

  //paso4. Programar alarma
  // Tic = 1 uSeg
  // Frcuencia = se necesita 10 ms = 10,000 Tics, para que el ojo no lo note
  timerAlarmWrite(timer, 5000, true);

  //paso5. Iniciar la alarma
  timerAlarmEnable(timer);
}

//****************************************************************
// Función para configurar módulo PWM
//****************************************************************
void configurarPWM(void)
{

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
void leds()
{
  if (temperatura <= 37.0)
  {
    ledcWrite(pwmChannelV, 255);
    ledcWrite(pwmChannelA, 0);
    ledcWrite(pwmChannelR, 0);

    ledcWrite(pwmChnlServo, 5);
  }

  if (temperatura > 37.0 && temperatura < 37.5)
  {
    ledcWrite(pwmChannelV, 0);
    ledcWrite(pwmChannelA, 255);
    ledcWrite(pwmChannelR, 0);

    ledcWrite(pwmChnlServo, 19);
  }

  if (temperatura >= 37.5)
  {
    ledcWrite(pwmChannelV, 0);
    ledcWrite(pwmChannelA, 0);
    ledcWrite(pwmChannelR, 255);

    ledcWrite(pwmChnlServo, 32);
  }
}

//*****************************************************************************
//Funcion temporal de boton para probar servo
//*****************************************************************************
void botonTemporal()
{

  if (contBoton == 1 && temporal < 2)
  {
    temporal = temporal + 1;
    delay(150);
    contBoton = 0;
    delay(150);
  }

  if (contBoton == 1 && temporal == 2)
  {
    temporal = 0;
    delay(150);
    contBoton = 0;
    delay(150);
  }
}

//*****************************************************************************
//Funcion temporal de boton para probar servo2
//*****************************************************************************

void temp2()
{
  if (temporal == 0)
  {
    temperatura = 37.0;
  }

  if (temporal == 1)
  {
    temperatura = 37.3;
  }

  if (temporal == 2)
  {
    temperatura = 37.5;
  }
}

//*****************************************************************************
//Funcion cambio de display
//*****************************************************************************
void cambioDisplay(int variable)
{
  /**
   * si el case es 0, se enciende el diplay 1 mostrando el valor de las
   * decenas. Si case es 1, se enciende el display 2 mostrando el valor de 
   * las unidades, ademas del punto. Si case es 2 se muestra el valor de
   * los decimales en el display 3.
   */
  switch (variable)
  {
  case 0:
    numDisplay(decena);
    digitalWrite(Dis1, 1);
    digitalWrite(Dis2, 0);
    digitalWrite(Dis3, 0);
    break;

  case 1:
    numDisplay(unidad);
    digitalWrite(Dis1, 0);
    digitalWrite(Dis2, 1);
    digitalWrite(Dis3, 0);
    digitalWrite(p, 1);
    break;

  case 2:
    numDisplay(decimal);
    digitalWrite(Dis1, 0);
    digitalWrite(Dis2, 0);
    digitalWrite(Dis3, 1);
    break;

  default:
    break;
  }
}

//*****************************************************************************
//Funcion para comenzar lectura del sensor
//*****************************************************************************

void sens(){
  if(millis() - lastTime >= sampleTime){
    lastTime = millis();
    adcLM35 = analogRead(sensor);

    Serial.print("ADC: ");
    Serial.print(adcLM35);
    Serial.print(" grados: ");

    temperatura = (analogReadMilliVolts(sensor)/10.0);
    Serial.println(temperatura);
  }
}
