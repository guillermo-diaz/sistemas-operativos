/* kbdclose.c  -  kbdclose */

#include <xinu.h>

/*------------------------------------------------------------------------
 * kbdclose  -  Close the keyboard device
 *------------------------------------------------------------------------
 */
extern sid32 semKbd;
extern pid32 pidKbd;
devcall	kbdclose (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	if (pidKbd != getpid()){
		return SYSERR;
	}
	
	signal(semKbd);
	
}
