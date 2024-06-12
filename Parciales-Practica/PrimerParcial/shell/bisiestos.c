/* bisiestos.c en XINU */

#include <xinu.h>

#define ASCII_ESC 27
long int calculo[2];

/* completar el codigo del programa bisiesto */
bisiesto(int base, int limite, int n)
{
	int i;

	/* base, limite y n vienen como argumentos */
	for (i=base; i<=limite; i++) {
		if  ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0)) {
			calculo[n] = i;	
			sleepms(300);
        }
	}

}

muestra_bisiestos() 
{
	/* limpiar toda la pantalla */
	printf( "%c[2J", ASCII_ESC );

	while(1) {
		/* Nos posicionamos en la fila 5 columna 10 */
    		printf( "%c[5;10f", ASCII_ESC );
    		printf( "proceso a: %d \n", calculo[0]);

		/* Nos posicionamos en la fila 6 columna 10 */
    		printf( "%c[7;10f", ASCII_ESC );
    		printf( "proceso b: %d \n", calculo[1]);

		sleepms(50);
	}
}


/* completar el gestor de procesos bisiestos */
gestor_bisiestos()
{
    sid32 semA, semB;
    semA = semcreate(1);
    semB = semcreate(1);
    char c;
    int stateA, stateB;
    stateA = 1;
    stateB = 1;
    pid32 pidA = create(bisiesto, 1024, 20, "procesoA", 3, 0, 2000, 0);
    pid32 pidB = create(bisiesto, 1024, 20, "procesoB", 3, 2001, 4000, 1);
    pid32 pidM = create(muestra_bisiestos, 1024, 20, "procesoM", 0);
    resume(pidA);
    resume(pidB);
    resume(pidM);


    control(CONSOLE, TC_MODER, 0, 0);

    while (1)
    {
        /* gestionar las pulsaciones del teclado, procesar, informar al usuario */
        c = getchar();
        switch (c) {
        case '1':
         
            printf( "%c[9;10f", ASCII_ESC );
            if (stateA){
                suspend(pidA);
                printf( "primer proceso suspendido                   \n");
                stateA = 0;
            } else {
                resume(pidA);
                printf( "primer proceso reanudado                    \n");
                stateA = 1;
            }
            
            break;
        case '3':
            printf( "%c[9;10f", ASCII_ESC );
            printf( "primer proceso fulminado                        \n");
            kill(pidA);
            break;
        case '7':
            printf( "%c[9;10f", ASCII_ESC );
            if (stateB){
                printf( "segundo proceso suspendido                  \n");
                stateB = 0;
                suspend(pidB);
            } else {
                printf( "segundo proceso reanudado                   \n");
                stateB = 1;
                resume(pidB);
            }
            break;
        case '9':
            printf( "%c[9;10f", ASCII_ESC );
            printf( "segundo proceso fulminado                       \n");
            kill(pidB);
            break;
        case 'q':
            printf( "%c[9;10f", ASCII_ESC );
            printf( "Programa finalizado                             \n");
            kill(pidA);
            kill(pidB);
            kill(pidM);
            kill(currpid);
            break;
        }

        sleepms(5);
    }
    control(CONSOLE, TC_MODEC, 0, 0);

    /*
    d) Yo no creo que sea necesario hacer una exclusion mutua en al consola, ya que no utilizan (o están muy lejos) el mismo espacio, es decir no se solapan,
     por lo tanto no lo vi necesario
    */


}