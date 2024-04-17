#include <xinu.h>
#include <shprototypes.h>
#include <stdio.h>
// #include <stdlib.h>	/* para las funciones system y exit */
// #include <time.h>
#include <string.h>

/*
    =======================
    Guillermo Diaz FAI-3197
    =======================


    Comenté todas las modificaciones
*/

#define INTENTOS 7
#define SIZE_PALABRAS 5

const char *palabraElegida;
const char *palabras[SIZE_PALABRAS] = {"ejemplo", "clase", "sistemas", "operativos", "ahorcado"};


void seleccionar_palabra() {
    // srand(time(NULL));  //para números aleatorios
    // int indice = rand() % SIZE_PALABRAS; // Obtener un índice aleatorio

    //no encontre un equivalente a time para hacer nro randoms
    palabraElegida = palabras[4]; 
}

int verificar_letra(char *palabraArmada, char letter){
    int flag = 0;
    int i = 0;
    char caract = palabraElegida[i];

    while(caract != '\0') {
        if(caract == letter) {
            palabraArmada[i] = letter;
            flag = 1;
        }
        i++;
        caract = palabraElegida[i];
        
    }
    return flag;
}

int palabra_completa(char *palabraArmada) {
    int i = 0;
    char caract = palabraArmada[i];

    while (caract != '\0'){
        if (caract == '_'){
            return 0;
        }
        i++;
        caract = palabraArmada[i];
    }
    return 1;
}


void ejercicio12(void) {
	/* Decirle al sistema que el modo input es RAW */
	// system ("/bin/stty raw");

    //consola modo raw
    control(CONSOLE, TC_MODER, 0, 0);
	int c, intentos;
    intentos = INTENTOS;
    seleccionar_palabra();
    
    /* Descomentar si quiere ver la palabra que se seleccionó
    printf("Palabra verdadera: %s \n ",palabraElegida);
    */

    //esconder palabra
    int tam = strlen(palabraElegida);
    char palabraArmada[tam+1];
    for (int i = 0; i < tam; i++) {
        palabraArmada[i] = '_';
    }
    palabraArmada[tam] = '\0'; // Agregar el nulo al final

	while(1) {
		printf("\r                                                          ");
		printf("\r Palabra: [%s] Intentos: %d |  c = %c  Ingrese una letra (0 para salir): ",palabraArmada, intentos, c);
		c = getchar();

		if (c == '0') {
			break;
        } 

        if (!verificar_letra(palabraArmada, c)){
            intentos--;
            if (intentos < 1) {
                printf(" <<< PERDISTE --> Palabra Elegida: %s >>>\n", palabraElegida);
                break;
            }
        } else if (palabra_completa(palabraArmada)){
            printf(" <<< GANASTE --> Palabra Elegida: %s >>>\n", palabraElegida);
            break;
        }

        


	}

    //consola modo cooked
    control(CONSOLE, TC_MODEC, 0, 0);
	// system ("/bin/stty sane erase ^H");
}

