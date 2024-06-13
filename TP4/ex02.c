
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

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
#pragma pack(pop)

unsigned char nueva_imagen [2000*2000*3];

void convertir(int in_fd, int out_fd, BMPHeader h, BMPInfoHeader infoh, int inicio, int fin) 
{

	int width = infoh.width;
	int height = infoh.height;
	unsigned char gray;

	int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
	unsigned char pixel[3];

    lseek(in_fd, sizeof(BMPHeader) + sizeof(BMPInfoHeader) + (inicio * (width * 3 + padding)), SEEK_SET);
    
	for (int y = inicio; y < fin; y++) {
		for (int x = 0; x < width; x++) {
			read(in_fd, pixel, 3);
			gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
			nueva_imagen[y*(width*3+padding) + x*3]= gray;
			nueva_imagen[y*(width*3+padding) + x*3 + 1] = gray;
			nueva_imagen[y*(width*3+padding) + x*3 + 2] = gray;
		}
		lseek(in_fd, padding, SEEK_CUR); // Skipping padding
	}

}

void something_wrong(int fd, const char *m){
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}

int main() 
{
    int pidA, pidB, pidC;


	int in_fd = open(BMP_FILE, O_RDONLY);
	if (in_fd < 0) {
		something_wrong(in_fd, "Error open");
	}

	BMPHeader h;
	read(in_fd, &h, sizeof(BMPHeader));

	if (h.magic[0] != 'B' || h.magic[1] != 'M') {
		something_wrong(in_fd, "No es BMP");
	}

	BMPInfoHeader infoh;
	read(in_fd, &infoh, sizeof(BMPInfoHeader));

	if (infoh.bpp != 24 || infoh.compression != 0) {
		something_wrong(in_fd, "No 24-bit BMP");
	}

	int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd < 0) {
		something_wrong(in_fd, "error en output");
	}

	write(out_fd, &h, sizeof(BMPHeader));
	write(out_fd, &infoh, sizeof(BMPInfoHeader));

    pidA = fork();
    
    if(pidA == 0){
        // //codigo del hijoA
        printf("Codigo del hijo A \n");
        convertir(in_fd, out_fd, h, infoh, 0, 354);
    } else if(pidA != 0){
        pidB = fork();
        if(pidB == 0){
            //codigo hijo b
            printf("Codigo del hijo B \n");
            int in_fdB,out_fdB;
            in_fdB = open(BMP_FILE, O_RDONLY);
            out_fdB = open(GRAYSCALE_FILE, O_WRONLY, 0644);
            convertir(in_fdB, out_fdB, h, infoh, 354, 708);
        } else if(pidB != 0){
            pidC = fork();
            if(pidC == 0){
                // // //codigo hijo c
                printf("Codigo del hijo C \n");
                int in_fdC,out_fdC;
                in_fdC = open(BMP_FILE, O_RDONLY);
                out_fdC = open(GRAYSCALE_FILE, O_WRONLY, 0644);
                convertir(in_fdC, out_fdC, h, infoh, 708, 1063);
            } else if(pidC != 0){                    
                    wait(NULL);
                    wait(NULL);
                    wait(NULL);
					write(out_fd, &nueva_imagen[0], infoh.width * infoh.height * 3);
                    printf("Codigo del Padre \n");
                    close(in_fd);
                    close(out_fd);
                    printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
                    exit(0);
            }
        } 
    }
}