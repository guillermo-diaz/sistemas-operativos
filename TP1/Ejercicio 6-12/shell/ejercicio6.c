#include <xinu.h>

void	sndch(char);

/*------------------------------------------------------------------------
 * main  --  example of 2 processes executing the same code concurrently
 *------------------------------------------------------------------------
 */
void ejercicio6(void)
{
	resume( create(sndch, 8000, 20, "send A", 1, 'A') );
	resume( create(sndch, 8000, 20, "send B", 1, 'B') );
}

/*------------------------------------------------------------------------
 * sndch  --  output a character on a serial device indefinitely
 *------------------------------------------------------------------------
 */
void	sndch(char ch)

{

	while ( 1 )
		putc(CONSOLE, ch);
}