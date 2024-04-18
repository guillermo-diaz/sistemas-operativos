#include <xinu.h>
#define ASCII_ESC 27

int vel_a = 0;
int vel_b = 0;
int distancia = -1;
int valorMostrar = 0;
int c;
sid32 semA, semB, semC;
corredor_a()
{
    while (1)
    {
        wait(semA);
        wait(semC);
        distancia++;
        signal(semC);
        vel_a++;
        signal(semA);
    }
}

corredor_b()
{

    while (1)
    {
        wait(semB);
        wait(semC);
        distancia++;
        signal(semC);
        vel_b++;
        signal(semB);
    }
}

/*
Luego, el proceso control_race debe permitirle al usuario gestionar los procesos que corren de la siguiente
manera:
○ Si el usuario presiona la tecla 1, el proceso corredor_a se suspende/reanuda.
○ Si el usuario presiona la tecla 5 el proceso corredor_a muere.
○ Si el usuario presiona la tecla 2 el proceso corredor_b se suspende/reanuda.
○ Si el usuario presiona la tecla 6 el proceso corredor_b muere.
Implemente las pulsaciones del teclado (teclas 1, 2, 5, y 6). A partir de las pulsaciones
“control_race” realiza las acciones mencionadas (suspender/reanudar, matar).
*/
muestra_carrera()
{
    /* limpiar toda la pantalla */
    printf("%c[2J", ASCII_ESC);

    while (1)
    {
        printf("%c[4;10f", ASCII_ESC);
        printf("DISTANCIA   : %d \n", valorMostrar);
        /* Nos posicionamos en la fila 5 columna 10 */
        printf("%c[5;10f", ASCII_ESC);
        printf("Velocidad corredor A: %d \n", vel_a);

        /* Nos posicionamos en la fila 6 columna 10 */
        printf("%c[6;10f", ASCII_ESC);
        printf("Velocidad corredor B: %d \n", vel_b);
        /*
        printf("%c[7;10f", ASCII_ESC);
                printf("1: el proceso corredor_a se suspende/reanuda. ");
                printf("%c[8;10f", ASCII_ESC);
                printf("5: el proceso corredor_a muere.  \n");
                printf("%c[9;10f", ASCII_ESC);
                printf("2: el proceso corredor_b se suspende/reanuda. \n");
                printf("%c[10;10f", ASCII_ESC);
                printf("6: el proceso corredor_b muere. \n");
        */

        sleepms(50);
    }
}

control_race()
{
    semA = semcreate(1);
    semB = semcreate(1);
    semC = semcreate(1);
    uint32 estadoA = 1; // esta verdadero
    uint32 estadoB = 1;
    pid32 pidA = create(corredor_a, 1024, 20, "correadorA", 0);
    pid32 pidB = create(corredor_b, 1024, 20, "correadorA", 0);
    pid32 pidM = create(muestra_carrera, 1024, 20, "muestra_carrera", 0);
    resume(pidA);
    resume(pidB);
    resume(pidM);

    control(CONSOLE, TC_MODER, 0, 0);

    while (1)
    {
        c = getchar();
        switch (c)
        {
        case '1':
            wait(semA);
            if (estadoA)
            {
                // veradero entonces esta en listo
                suspend(pidA);
                signal(semC);
                estadoA = 0;
            }
            else
            {
                resume(pidA);
                estadoA = 1;
            }
            signal(semA);

            break;
        case '5':
            wait(semA);
            kill(pidA);
             signal(semC);
            signal(semA);
            break;
        case '2':
            wait(semB);
            if (estadoB)
            {
                // veradero entonces esta en listo
                suspend(pidB);
                 signal(semC);
                estadoB = 0;
            }
            else
            {
                resume(pidB);
                estadoB = 1;
            }
            signal(semB);
            break;
        case '6':
            wait(semB);
            kill(pidB);
             signal(semC);
            signal(semB);
            break;
        case 'p':
            wait(semC);
            valorMostrar = distancia;
            signal(semC);
            break;
        }
    }
    control(CONSOLE, TC_MODEC, 0, 0);
}