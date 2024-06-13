#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define TAM 50261
#define PGM_FILE "cat.pgm"
#define ENCABEZADO 15
#define NAME "pgm_invert"

void something_wrong(int fd, const char *m)
{
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}

int main()
{
    int in_fd = open(PGM_FILE, O_RDONLY);
    if (in_fd < 0){
        something_wrong(in_fd, "Error open");
    }
   
    // off_t tamanioBytes = lseek(in_fd, 0, SEEK_END);

    // Crear un objeto de memoria compartida
    int shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);

    // Ajustar el tamaño del segmento de memoria compartida
    ftruncate(shm_fd, TAM);

    // Mapear el segmento de memoria compartida
    unsigned char *imagen = mmap(0, TAM, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (imagen == MAP_FAILED){
        close(in_fd);
        close(shm_fd);
        something_wrong(in_fd, "Error al mapear el segmento de memoria compartida");
    }
    // Volver al inicio del archivo de entrada
    lseek(in_fd, 0, SEEK_SET);
    // Leer el encabezado del archivo
    read(in_fd, imagen, ENCABEZADO);

    unsigned char pixel;
    for (int i = TAM - 1; i >= ENCABEZADO; i--) {
        read(in_fd, &pixel, 1);
        imagen[i] = pixel;
    }
    close(in_fd);
    return 0;
}