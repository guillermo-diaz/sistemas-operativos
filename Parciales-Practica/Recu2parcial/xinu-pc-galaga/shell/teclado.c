#include <xinu.h>
#include <string.h>
#include <stdio.h>


unsigned char pulsaciones[20];


shellcmd tecladoP(){


    paint_screen();

    open(KEYBOARD, NULL, NULL);

    read(KEYBOARD, &pulsaciones, 20);
    
    close(KEYBOARD);

    print_text_on_vga(4, 164,pulsaciones);
    

}