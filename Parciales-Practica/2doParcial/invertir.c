#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define PGM_FILE "cat.pgm"
#define INVERT_FILE "catInvertidoF.pgm"
#define ENCABEZADO 15

void something_wrong(int fd, const char *m)
{
    if (fd >= 0)
        close(fd);
    printf("%s\n", m);
    exit(1);
}

int main()
{
    int in_fd = open(PGM_FILE, O_RDONLY);
    if (in_fd < 0){
        something_wrong(in_fd, "Error al abrir el archivo de entrada");
    }

    int out_fd = open(INVERT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0){
        something_wrong(out_fd, "Error al abrir/crear el archivo de salida");
    }

    // calculo el tamanio de la imagen
    off_t tamanioBytes = lseek(in_fd, 0, SEEK_END);
    // creo un arreglo estatico
    unsigned char imagen[tamanioBytes];
    lseek(in_fd, 0, SEEK_SET);       // Ir al inicio del archivo de entrada
    read(in_fd, imagen, ENCABEZADO); // Leer el encabezado del archivo

    unsigned char pix;
    for (off_t i = tamanioBytes - 1; i >= ENCABEZADO; i--){
        read(in_fd, &pix, 1);

        imagen[i] = pix;
    }

    write(out_fd, imagen, tamanioBytes);

    close(in_fd);
    close(out_fd);

    return 0;
}