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

// comment out the following lines if you are using fona or ethernet
#include "AdafruitIO_WiFi.h"

//*****************************************************************************
//Definicion de pines
//*****************************************************************************

/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "hector19511"
#define IO_KEY       "aio_Slxz93Yk7lB0mIUTNX7Ahk9lFlMa"

/******************************* WIFI **************************************/
#define WIFI_SSID "TIGO-45B1_Ext"
#define WIFI_PASS "2NJ555305153"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);//definicion de io

//displays
#define Dis1 18 //display1
#define Dis2 19 //display2
#define Dis3 21 //display3

//pines del display
#define g 26
#define f 25
#define a 33
#define b 32
#define e 3
#define d 1
#define c 22
#define p 23

//prescaler para contador
#define prescaler 80

//leds
#define ledR 0
#define ledG 2
#define ledB 15
#define pwmChannelR 5 // 16 canales 0-15
#define pwmChannelG 4
#define pwmChannelB 3
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
#define sensor 36

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

//para el promedio
void mediaMovilADC(void);

//temperatura a decenas, unidades y decimales
void tempAUnidades(void);

//adafruit
void adafruit(void);
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

//delay1
unsigned long lastTime;
unsigned int sampleTime = 500;

//delay1
unsigned long lastTime2;
unsigned int sampleTime2 = 3000;

//media movil
int numLecturas = 10;
float bufferLecturas[10];
int indexLecturas = 0;
long mAvgSuma = 0;
long adcfiltrado = 0;

//temperatura a decenas, unidades y decimales
int temp = 0;

//variables para desplegar
float temperaturaLoop = 0;
int decenaLoop = 0;
int unidadLoop = 0;
int decimalLoop = 0;

//adafruit
// this int will hold the current count for our sketch
//int count = 0;

// set up the 'counter' feed
AdafruitIO_Feed *ada = io.feed("temperaturaesp32");


//*****************************************************************************
//ISR
//*****************************************************************************

void IRAM_ATTR ISRTimer0()
{
  //interrupcion para cambiar de display
  contadorTimer++; // es como sumarle 1
  if (contadorTimer > 2)
  {
    contadorTimer = 0;
  }
  //de 0 a 2 porque solo tengo 3 displays
 
}

//interrupcion para el boton que realiza las lecturas
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
  lastTime = millis();//delay de 500milisegundos
  lastTime2 = millis();//delay de 3000milisegundos

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

  //adafruit
   // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    if(millis() - lastTime >= sampleTime){
      lastTime = millis();
      Serial.print(".");
    }

  }
  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}


//*****************************************************************************
//Loop principal
//*****************************************************************************

void loop()
{


  //llamar a la funcion del promedio
  mediaMovilADC();

  //definicion de decena unidad y decimal
  tempAUnidades();

  //para escoger que color de led mostrar
  leds();

  //para prueba
  botonTemporal();
  //temp2();

  //contadorTimer permite cambiar entre los 3 displays cada 10 milisegundos
  cambioDisplay(contadorTimer);

    
  adafruit();
  
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
  ledcSetup(pwmChannelR, freqPWM, resolution);//ledR
  ledcSetup(pwmChannelG, freqPWM, resolution);//legG
  ledcSetup(pwmChannelB, freqPWM, resolution);//ledB

  ledcSetup(pwmChnlServo, freqPWMServo, resolution);//servo motor

  // Paso 2: seleccionar en que GPIO tendremos nuestra señal PWM
  ledcAttachPin(ledR, pwmChannelR);
  ledcAttachPin(ledG, pwmChannelG);
  ledcAttachPin(ledB, pwmChannelB);

  ledcAttachPin(servo, pwmChnlServo);
}

//*****************************************************************************
//Funcion Leds
//*****************************************************************************
void leds()
{
  if (temperaturaLoop <= 37.0)
  {
    /**
     * ya que el led RGB es de anodo comun, se debe colocar 0 en el ledcWrite que 
     * se desea encender 
     */
    ledcWrite(pwmChannelR, 255);//se muestra color verde
    ledcWrite(pwmChannelG, 0);
    ledcWrite(pwmChannelB, 255);

    ledcWrite(pwmChnlServo, 5);
  }

  if (temperaturaLoop > 37.0 && temperatura < 37.5)
  {
    ledcWrite(pwmChannelR, 0);//se muestra Rojo + Verde = amarillo
    ledcWrite(pwmChannelG, 0);
    ledcWrite(pwmChannelB, 255);

    ledcWrite(pwmChnlServo, 19);
  }

  if (temperaturaLoop >= 37.5)
  {
    ledcWrite(pwmChannelR, 0);//se muestra rojo
    ledcWrite(pwmChannelG, 255);
    ledcWrite(pwmChannelB, 255);

    ledcWrite(pwmChnlServo, 32);
  }
}

//*****************************************************************************
//Funcion para tomar datos mediante el boton
//*****************************************************************************
void botonTemporal()
{
  /*////Codigo de prueba en caso de emergencia/////
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
  }*/

/**
 * Esta funcion sirve para tomar los datos de los valorqe que queremos desplegar, 
 * si que estos cambien siempre, ya que solo se quiere tomar un dato cada vez que
 * se presiona el boton. Las variables-Loop solo toman los datos de las otras 
 * variables en las ecuaciones.
 * */ 
  if (contBoton == 1 )//si se presiona el boton 
  {
    temperaturaLoop = temperatura;
    decenaLoop = decena;
    unidadLoop = unidad;
    decimalLoop = decimal;
    //delay(150);
    contBoton = 0;
    //delay(150);
  }

}

//*****************************************************************************
//Funcion temporal de boton para probar servo2
//*****************************************************************************
/*
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
}*/

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
    numDisplay(decenaLoop);
    digitalWrite(Dis1, 1);
    digitalWrite(Dis2, 0);
    digitalWrite(Dis3, 0);
    break;

  case 1:
    numDisplay(unidadLoop);
    digitalWrite(Dis1, 0);
    digitalWrite(Dis2, 1);
    digitalWrite(Dis3, 0);
    digitalWrite(p, 1);//enciende el punto en el display 2 para indicar 
    //que el display siguiente muestra decimales
    break;

  case 2:
    numDisplay(decimalLoop);
    digitalWrite(Dis1, 0);
    digitalWrite(Dis2, 0);
    digitalWrite(Dis3, 1);
    break;

  default:

    digitalWrite(Dis1, 0);
    digitalWrite(Dis2, 0);
    digitalWrite(Dis3, 0);
    break;
  }
}



//*****************************************************************************
//Funcion para media movil
//*****************************************************************************

void mediaMovilADC(){

  if(millis() - lastTime >= sampleTime){
    lastTime = millis(); //el if es un delay de 500milisegundos
  

    adcLM35 = analogReadMilliVolts(sensor);//realiza la lectura convirtiendo a voltios

    mAvgSuma = mAvgSuma - bufferLecturas[indexLecturas] + adcLM35;
    bufferLecturas[indexLecturas] = adcLM35;
    indexLecturas++;
    if(indexLecturas >= numLecturas){
      indexLecturas = 0;
    }
    adcfiltrado = mAvgSuma/numLecturas;

    temperatura = adcfiltrado/10.0;
    
    Serial.print("ADCmillivolts: ");
    Serial.print(adcLM35);
    Serial.print(" mediaMovil: ");
    Serial.print(adcfiltrado);
    Serial.print(" temperatura: ");
    Serial.println(temperatura);
  }



}

//*****************************************************************************
//Funcion para separar decenas unidades y decimales
//*****************************************************************************

void tempAUnidades(){
  
  /**
   * Esta funcion descompone la temperatura para poder identificar las
   * decenas, unidades y decimales
   */
  
  temp = adcfiltrado;
  decena = temp/100;
  temp = temp - (decena*100);
  unidad = temp/10;
  temp = temp - (unidad*10);
  decimal = temp;
 
}

//*****************************************************************************
//Funcion para adafruit
//*****************************************************************************
void adafruit(){

   if(millis() - lastTime2 >= sampleTime2){
    lastTime2 = millis(); 
    // Adafruit IO is rate limited for publishing, so a delay is required in
    // between feed->save events. In this example, we will wait three seconds
    // (1000 milliseconds == 1 second) during each loop.
  
    //adafruit
    // io.run(); is required for all sketches.
    // it should always be present at the top of your loop
    // function. it keeps the client connected to
    // io.adafruit.com, and processes any incoming data.
    io.run();

    // save count to the 'counter' feed on Adafruit IO
    Serial.print("sending -> ");
    Serial.println(temperatura);
    ada->save(temperatura);


    
  }

}