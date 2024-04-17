#include <stdio.h>

/*
    =======================
    Guillermo Diaz FAI-3197
    =======================
*/

int main() {
    char caract1 = 127; // rango [-128, 127] 
    unsigned char caract2 = 255;  //rango [0, 255]

    printf("Variables antes del incremento: \n");
    printf("char: %i\n", caract1);
    printf("unsigned char: %d\n", caract2);
    caract1++;
    caract2++;
    printf("Variables después del incremento: \n");
    printf("char: %i\n", caract1);
    printf("unsignedchar: %i\n", caract2);

    return 0;
}
