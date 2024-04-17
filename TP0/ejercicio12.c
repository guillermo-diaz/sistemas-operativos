#include <stdio.h>
#include <stdlib.h>	/* para las funciones system y exit */
#include <time.h>
#include <string.h>

/*
    =======================
    Guillermo Diaz FAI-3197
    =======================
*/

/* 
12 a)
#include <stdio.h>: incluye la librería estándar de entrada/salida en C, 
que proporciona funciones para realizar operaciones de entrada y salida, como printf y scanf.

#include <stdlib.h>: incluye la librería estándar de funciones generales en 
C, que proporciona funciones para la gestión de memoria, generación de números aleatorios, 
conversión de cadenas, entre otras operaciones.

system("/bin/stty raw");: ejecuta un comando del sistema operativo utilizando 
la función system. El comando /bin/stty raw configura el modo de entrada del terminal 
en "raw", lo que significa que las teclas presionadas se envían directamente al 
programa sin esperar por la tecla Enter. 

system("/bin/stty sane erase ^H");: Esta línea tambien ejecuta un comando del 
sistema operativo utilizando la función system. 
El comando /bin/stty sane erase ^H restaura la configuración normal del 
terminal después de que el programa ha terminado, donde ^H representa la tecla 
retroceso (Backspace) que se usa para borrar caracteres en la línea de entrada.*/

#define INTENTOS 7
#define SIZE_PALABRAS 5

const char *palabraElegida = NULL;
const char *palabras[SIZE_PALABRAS] = {"ejemplo", "clase", "sistemas", "operativos", "ahorcado"};


void seleccionar_palabra() {
    srand(time(NULL));  //para números aleatorios
    int indice = rand() % SIZE_PALABRAS; // Obtener un índice aleatorio
    palabraElegida = palabras[indice]; 
}

int verificar_letra(char* palabraArmada, char letter){
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

int palabra_completa(char* palabraArmada) {
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


int main() {
	/* Decirle al sistema que el modo input es RAW */
	system ("/bin/stty raw");
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
		printf("\r Palabra: [%s] Intentos: %i |  c = %c  Ingrese una letra (0 para salir): ",palabraArmada, intentos, c);
		c = getchar();

		if (c == '0') {
			break;
        } 

        if (!verificar_letra(palabraArmada, c)){
            intentos--;
            if (intentos < 1) {
                printf(" <<< PERDISTE >>>\n");
                break;
            }
        } else if (palabra_completa(palabraArmada)){
            printf(" <<< GANASTE >>>\n");
            break;
        }

        


	}

	system ("/bin/stty sane erase ^H");
}