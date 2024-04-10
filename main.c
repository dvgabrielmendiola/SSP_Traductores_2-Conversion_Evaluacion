/*
prograna 08_01.c 
generador de expr*gioneg de 3 niveleg de precedencia 
*/

#include<stdio.h> 
#include<math.h>
#include<ctype.h> 

struct pila_char /*pila, conversion posfija*/
{
    unsigned char pila[50];
    int tope; 
};

struct pila_float /*pila, evalucion*/
{
    float pila [50]; 
    int tope;
};

/*prototipos de la conversi6n */ 
int asignar_prioridad (unsigned char a); 
float evaluacion (char *a); 
void posfija (char *a, char *b);

/* prototipos de pila */ 
void iniciar_pila_float (struct pila_float *a); 
void meter_pila_float (struct pila_float *a, float b); 
float sacar_pila_float(struct pila_float *a); 

void iniciar_pila_char(struct pila_char *a);
void meter_pila_char(struct pila_char *a, unsigned char b);
int pila_vacia_char(struct pila_char *a);
unsigned char sacar_pila_char(struct pila_char *a);

int main()
{
    unsigned char entrada [50], salida [50]; 
    printf ("Expresion : "); gets (entrada); 
    posfija (entrada, salida); 
    printf("Postija = %s", salida); 
    printf("\nResu1tado = %.2f", evaluacion (salida)); 
    return 0;
}

int asignar_prioridad(unsigned char a)
{
    if (toupper(a) == 'S' || toupper(a) == 'C')
        return 1;
    if (a == '*' || a == '/')
        return 2;
    if (a == '+' || a == '-')
        return 3;
}

float evaluacion(char *a)
{
    float operando_izquierdo, operando_derecho, operando_unico, resultado;
    struct pila_float pila;

    iniciar_pila_float(&pila);
    while (*a != NULL)
    {
        if (isdigit(*a)) /* operando o digito*/
            meter_pila_float(&pila, *a - '0'); /* convierte el caracter a número */
        if (*a == '+' || *a == '-' || *a == '*' || *a == '/') /* operador binario */
        {
            operando_derecho = sacar_pila_float(&pila);
            operando_izquierdo = sacar_pila_float(&pila);
            switch(*a)
            {
                case '+' : resultado = operando_izquierdo + operando_derecho; break;
                case '-' : resultado = operando_izquierdo - operando_derecho; break;
                case '*' : resultado = operando_izquierdo * operando_derecho; break;
                case '/' : resultado = operando_izquierdo / operando_derecho; 
            }
            meter_pila_float(&pila, resultado);
        }
        if (toupper(*a) == 'S' || toupper(*a) == 'C') /* operador unario */
        {
            operando_unico = sacar_pila_float(&pila);
            switch (*a)
            {
                case 's' :
                case 'S' : resultado = sin(3.14159 * operando_unico / 180); break;
                case 'c' :
                case 'C' : resultado = cos(3.14159 * operando_unico / 180); /* convierte radianes a grados */
            }
            meter_pila_float(&pila, resultado);
        }
        a++;
    }
    return sacar_pila_float(&pila);
}

void posfija (char *a, char * b)
{
    struct pila_char pila;
    unsigned char simbolo;
    int prioridad_entrada, prioridad_pila;
    
    iniciar_pila_char(&pila);

    while (*a != NULL)
    {
        switch (*a)
        {
            case '0' : /* Digito u operador */
            case '1' : 
            case '2' : 
            case '3' : 
            case '4' : 
            case '5' : 
            case '6' : 
            case '7' : 
            case '8' : 
            case '9' : *b = *a; /* insert a posfijo */
                        a++;
                        b++;
                        break;
            case '(' : meter_pila_char(&pila, '('); /* parantesis izquierdo */
                a++;
                break;
            case ')' :  while ((simbolo = sacar_pila_char(&pila)) != '(') /* parentesis derecho */
                        {
                            *b = simbolo;
                            b++;
                        }
                        a++;
                        break;
            case '+' :
            case '-' :
            case '*' :
            case '/' :
            case 's' :  /*  operador */
            case 'S' :
            case 'c' :
            case 'C' :  if (pila_vacia_char(&pila))
                        {
                            meter_pila_char(&pila, *a);
                            a++;
                            break;
                        }
                        simbolo = sacar_pila_char(&pila);
                        if (simbolo == '(')
                        {
                            meter_pila_char(&pila, simbolo); /* vuelve a meter parémtesis */
                            meter_pila_char(&pila, *a); /* inserta el operador */
                            a++;
                            break;
                        }

                        prioridad_entrada = asignar_prioridad(*a);
                        prioridad_pila = asignar_prioridad(simbolo);
                        if (prioridad_entrada >= prioridad_pila)
                        {
                            *b = simbolo;
                            b++;
                        }
                        else
                        {
                            meter_pila_char(&pila, simbolo); /* vuelve a meter el operador */
                            meter_pila_char(&pila, *a); /* inseter nuevo operador */
                            a++;
                        }
                        break;
                    default : printf("Token no valido");
        }
    }
    while (!pila_vacia_char(&pila))
    {
        *b = sacar_pila_char(&pila);
        b++;
    }
    *b = NULL;  /* cierra de la cadena de salida */
}

void iniciar_pila_char (struct pila_char *a)
{
    a->pila[0] = 0; /* inicia cadena de la pila */
    a->tope    = -1; /* inicia tope de la pila */
}

void meter_pila_char(struct pila_char *a, unsigned char b)
{
    a->tope++;
    a->pila[a->tope] = b;
}

int pila_vacia_char(struct pila_char *a)
{
    if (a->tope == -1)
        return 1;
    return 0;
}

unsigned char sacar_pila_char(struct pila_char *a)
{
    unsigned char simbolo;

    simbolo = a->pila[a->tope];
    a->tope--;
    return simbolo;
}

void iniciar_pila_float(struct pila_float *a) 
{
    a->tope = -1; /* inicia tope de la pila */
}

void meter_pila_float(struct pila_float *a, float b)
{
    a->tope++;
    a->pila[a->tope] = b;
}

float sacar_pila_float(struct pila_float *a)
{
    float simbolo;

    simbolo = a->pila[a->tope];
    a->tope--;
    return simbolo;
}
