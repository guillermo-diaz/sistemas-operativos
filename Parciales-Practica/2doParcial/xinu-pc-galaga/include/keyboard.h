
#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)

#define KEYBOARD_DATAREG 0x60  // :Data Register(Read\Write)
#define KEYBOARD_CMDREG 0x64   // :Command Register(Read\Write)
#define KEYBOARD_BUFFER_SIZE 10

extern unsigned char kblayout [128];  // { ... } Fill your layout yourself 

struct BufferC{
    unsigned char buffer [KEYBOARD_BUFFER_SIZE];
    int32 inicio;
    int32 fin;
    sid32 semEntrada;
};

