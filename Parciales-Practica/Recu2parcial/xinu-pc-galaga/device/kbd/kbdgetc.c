#include <xinu.h>
#include <keyboard.h>
extern struct BufferC buffC;
extern pid32 pidKbd;

unsigned char kbdgetc(){

    if(pidKbd = getpid()){
        wait(buffC.semEntrada);
        unsigned char c = buffC.buffer[buffC.inicio];
        buffC.buffer[buffC.inicio] = 0;
        buffC.inicio = (buffC.inicio + 1) % KEYBOARD_BUFFER_SIZE;
        return c;
    } else {
        return SYSERR;
    }
}

