/* kbdread.c  -  kbdread */

#include <xinu.h>
#include <mouse.h>

extern pid32 pidKbd;

/*------------------------------------------------------------------------
 * kbdread  -  Read the status of the keyboard driver
 *------------------------------------------------------------------------
 */
devcall	kbdread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char          *buffer,        /* Address of buffer            */
          uint32        count           /* Length of buffer             */
	)
{
	if (pidKbd != getpid()){
		return SYSERR;
	}
	for(int i=0; i<count; i++)
		buffer[i] = kbdgetc(devptr);
	// buffer[0] = kbdgetc();
	return OK;
}
