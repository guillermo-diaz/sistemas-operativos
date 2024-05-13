#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h> /* Para ftruncate */
#include <fcntl.h>  /* Para constantes O_* */

const int SIZE = 4096;
const char *name = "compartido";
const char *pathName = "/usr/share/doc/aufs-dkms/filesystems/aufs/design/06mmap.txt";

int shm_fd, fileDescriptor;
void *ptr;

int main(){
    int buffSize = 1024;
    char buff[buffSize];

    /* Solicitar crear segmento de memoria compartida, escritura/lectura */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    /* Configurar el tamaño del segmento compartido */
    ftruncate(shm_fd, SIZE);

    /* Mapear segmento de memoria compartida en el espacion de dirección del proceso */
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED){
        printf("Mapeo fallido\n");
        return -1;
    }

    /* Abrimos el archivo solo para lectura */
    fileDescriptor = open(pathName, O_RDONLY);

    /* Copiamos el contenido del txt en buff */
    read(fileDescriptor, buff, buffSize);

    /* Copiamos el contenido de buff en el segmento de memoria compartida */
    sprintf(ptr, "%s", buff);

    /* Cerramos el archivo */
    if (close(fileDescriptor) == -1){
        printf("Error al cerrar el archivo");
    }

    return 0;
}