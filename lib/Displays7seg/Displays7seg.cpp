#include "Displays7seg.h"

uint8_t a, b, c, d, e, f, g, p;

void configurarDisplay(uint8_t pina, uint8_t pinb, uint8_t pinc, uint8_t pind, uint8_t pine, uint8_t pinf, uint8_t ping, uint8_t pinp)
{
    //pines del display
    a = pina;
    b = pinb;
    c = pinc;
    d = pind;
    e = pine;
    f = pinf;
    g = ping;
    p = pinp;

    pinMode(g, OUTPUT);
    pinMode(f, OUTPUT);
    pinMode(a, OUTPUT);
    pinMode(b, OUTPUT);
    pinMode(e, OUTPUT);
    pinMode(d, OUTPUT);
    pinMode(c, OUTPUT);
    pinMode(p, OUTPUT);
    digitalWrite(g, 0);
    digitalWrite(f, 0);
    digitalWrite(a, 0);
    digitalWrite(b, 0);
    digitalWrite(e, 0);
    digitalWrite(d, 0);
    digitalWrite(c, 0);
    digitalWrite(p, 0);
}

void numDisplay(uint8_t digito)
{
    //se muestra el numero del case en el display
    switch (digito)
    {
    case 0:
        digitalWrite(a, 1);
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 1);
        digitalWrite(f, 1);
        digitalWrite(g, 0);
        digitalWrite(p, 0);

        break;

    case 1:
        digitalWrite(a, 0);
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 0);
        digitalWrite(e, 0);
        digitalWrite(f, 0);
        digitalWrite(g, 0);
        digitalWrite(p, 0);

        break;

    case 2:
        digitalWrite(a, 1);
        digitalWrite(b, 1);
        digitalWrite(c, 0);
        digitalWrite(d, 1);
        digitalWrite(e, 1);
        digitalWrite(f, 0);
        digitalWrite(g, 1);
        digitalWrite(p, 0);

        break;

    case 3:
        digitalWrite(a, 1);
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 0);
        digitalWrite(f, 0);
        digitalWrite(g, 1);
        digitalWrite(p, 0);

        break;

    case 4:
        digitalWrite(a, 0);
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 0);
        digitalWrite(e, 0);
        digitalWrite(f, 1);
        digitalWrite(g, 1);
        digitalWrite(p, 0);

        break;

    case 5:
        digitalWrite(a, 1);
        digitalWrite(b, 0);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 0);
        digitalWrite(f, 1);
        digitalWrite(g, 1);
        digitalWrite(p, 0);

        break;

    case 6:
        digitalWrite(a, 1);
        digitalWrite(b, 0);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 1);
        digitalWrite(f, 1);
        digitalWrite(g, 1);
        digitalWrite(p, 0);

        break;

    case 7:
        digitalWrite(a, 1);
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 0);
        digitalWrite(e, 0);
        digitalWrite(f, 0);
        digitalWrite(g, 0);
        digitalWrite(p, 0);

        break;

    case 8:
        digitalWrite(a, 1);
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 1);
        digitalWrite(f, 1);
        digitalWrite(g, 1);
        digitalWrite(p, 1);

        break;

    case 9:
        digitalWrite(a, 1);
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 0);
        digitalWrite(f, 0);
        digitalWrite(g, 0);
        digitalWrite(p, 0);

        break;

    default:
        break;
    }
}

