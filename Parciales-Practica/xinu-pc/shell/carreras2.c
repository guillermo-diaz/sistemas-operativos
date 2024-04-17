// /*#include <xinu.h>

// #define ASCII_ESC 27

// int vel_a = 0;
// int vel_b = 0;
// int distancia = 0;
// int stateA, stateB;
// sid32 mutex;

// corredor_a()
// {
// 	while(1) {
// 		vel_a++;
// 		wait(mutex);
// 		distancia++;
// 		signal(mutex);
// 	}
// }

// corredor_b()
// {

// 	while(1) {
// 		vel_b++;
// 		wait(mutex);
// 		distancia++;
// 		signal(mutex);
// 	}
// }


// muestra_carrera() 
// {
// 	/* limpiar toda la pantalla */
// 	printf( "%c[2J", ASCII_ESC );

// 	while(1) {

// 		printf( "%c[3;10f", ASCII_ESC);
// 		printf( "Distancia: %d \n", distancia);

// 		/* Nos posicionamos en la fila 5 columna 10 */
//     		printf( "%c[5;10f", ASCII_ESC );
//     		printf( "Velocidad corredor A: %d \n", vel_a);

// 		/* Nos posicionamos en la fila 6 columna 10 */
//     		printf( "%c[6;10f", ASCII_ESC );
//     		printf( "Velocidad corredor B: %d \n", vel_b);

// 		sleepms(50);
// 	}
// }

// int proceso[3];
// uint32 prflags = 0;
// //No le den bola a esto, se puede hacer teniendo variables, me complique al pedo
// void process_switch(int my_pr)
// {
// 	if( (prflags >> my_pr) & 1U )
// 	{
// 		wait(mutex);
// 		suspend(proceso[my_pr]);
// 		signal(mutex);
// 		prflags  ^= 1U << my_pr;
// 	}
// 	else
// 	{
// 		resume(proceso[my_pr]);
// 		prflags ^= 1U << my_pr;
// 	}
// }

// shellcmd carreras(void) {

	
// 	mutex = semcreate(1);
// 	prflags = 0;
// 	distancia = 0;

// 	chprio(currpid, 20);

// 	proceso[0] = create(corredor_a, 1024, 20, "Corredor A", 0);
// 	proceso[1] = create(corredor_b, 1024, 20, "Corredor B", 0);
// 	stateA = 1;
// 	stateB = 1;
// 	proceso[2] = create(muestra_carrera, 1024, 20, "Mostrar Carrara", 0);

// 	for(int i=0; i<3; i++) 
// 		process_switch(i);

// 	control(CONSOLE, TC_MODER, NULL, NULL);
// 	int c=1;
// 	while(c!='0')
// 	{
// 		c = getchar();
// 		switch(c) {
// 		case '1': 
			
// 		; break;
// 		case '2': kill(proceso[0]); break;
// 		case '5': process_switch(1); break;
// 		case '6': kill(proceso[1]); break;
// 		default: break;
// 		}
// 	}
// 	control(CONSOLE, TC_MODEC, NULL, NULL);

// }