#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale.bmp"

#pragma pack(push, 1)
typedef struct {
	unsigned char magic[2];
	unsigned int size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int offset;
} BMPHeader;

typedef struct {
	unsigned int headerSize;
	int width;
	int height;
	unsigned short planes;
	unsigned short bpp;
	unsigned int compression;
	unsigned int imageSize;
	int xPixelsPerM;
	int yPixelsPerM;
	unsigned int colorsUsed;
	unsigned int colorsImportant;
} BMPInfoHeader;

typedef struct {
    BMPHeader h;
    BMPInfoHeader infoh;
    int inicio;
    int fin;
} arg_threads;

#pragma pack(pop)

unsigned char nueva_imagen [2000*2000*3];

sem_t sem;


void convertir(BMPHeader h, BMPInfoHeader infoh, int inicio, int fin) {

	int width = infoh.width;
	unsigned char gray;
	int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
	

    for (int y = inicio; y < fin; y++){
        for (int x = 0; x < width; x++){
            int pos1 =  y * (width * 3 + padding) + x * 3;
            int pos2 = pos1 + 1;
            int pos3 = pos2 + 1;

            gray = (unsigned char)(0.3 * nueva_imagen[pos3] + 0.59 * nueva_imagen[pos2] + 0.11 * nueva_imagen[pos1]);
            nueva_imagen[pos1] = gray;
            nueva_imagen[pos2] = gray;
            nueva_imagen[pos3] = gray;
        }
    }
  
}

void something_wrong(int fd, const char *m){
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}


/* Ejemplo con threads en Linux. Compilar con: 
gcc -o thread ex03.c -lpthread */


void *thread(void *arg) {
    arg_threads *data = (arg_threads *)arg; // Convertir el puntero void* al tipo arg_threads
    printf("Este es el thread en rango [%d, %d]\n", data->inicio, data->fin);
    convertir(data->h, data->infoh, data->inicio, data->fin);
	
}

int main(){
    int in_fd = open(BMP_FILE, O_RDONLY);
    if (in_fd < 0){
        something_wrong(in_fd, "Error open");
    }

    BMPHeader h;
    read(in_fd, &h, sizeof(BMPHeader));

    if (h.magic[0] != 'B' || h.magic[1] != 'M'){
        something_wrong(in_fd, "No es BMP");
    }

    BMPInfoHeader infoh;
    read(in_fd, &infoh, sizeof(BMPInfoHeader));

    if (infoh.bpp != 24 || infoh.compression != 0){
        something_wrong(in_fd, "No 24-bit BMP");
    }

    int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0){
        something_wrong(in_fd, "Error en output");
    }

    write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));

    int width = infoh.width;
    int height = infoh.height;
    int padding = (4 - (width * 3) % 4) % 4;  
    //Copio los pixeles al array
    read(in_fd, &nueva_imagen, width*height*3);

    pthread_t tid[3];
    arg_threads args[3];
    int inicio, fin;
    inicio = 0;
    fin = 354;

    for (int i = 0; i < 3; i++){
        args[i].h = h;
        args[i].infoh = infoh;
        args[i].inicio = inicio;
        args[i].fin = fin;
        inicio = fin;
        fin = (i==1) ? height : fin + 354; //si es el ultimo le agrego el height x si faltan px | le agrego 354 px
    }

    

    for(int i = 0; i < 3; i++){
        pthread_create(&tid[i], NULL, thread, (void *)&args[i]);
    }

    for(int i = 0; i < 3; i++){
        pthread_join(tid[i], NULL);
    }

    write(out_fd, &nueva_imagen[0], infoh.width * infoh.height * 3); //escribo lo que hicieron los hilos en el archivo de salida

    close(in_fd);
    close(out_fd);

    printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
    return 0;
}