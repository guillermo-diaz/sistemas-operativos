/* kbdopen.c  -  kbdopen */

#include <xinu.h>

/*------------------------------------------------------------------------
 * kbdopen  -  Open the ps/2 keyboard device
 *------------------------------------------------------------------------
 */
extern sid32 semKbd;
extern pid32 pidKbd;
devcall	kbdopen (
	 struct	dentry	*devptr,	/* Entry in device switch table	*/
	 char	*name,			/* Unused for a kbd */
	 char	*mode			/* Unused for a kbd */
	)
{
	wait(semKbd); //semaforo de teclado
	pidKbd = getpid(); //actualizo
}
