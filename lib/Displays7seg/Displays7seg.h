#ifndef __3DISPLAYS7SEG_H__
#define __3DISPLAYS7SEG_H__
#include <Arduino.h>

extern uint8_t a, b, c, d, e, f, g, p;

//Funcion para configurar display de 7seg
void configurarDisplay(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t p);

//Funcion para seleccionar numero
void numDisplay(uint8_t digito);

#endif // __3DISPLAYS7SEG_H__