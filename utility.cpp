#include "er.h"

void do_error(char *s)
{
	fprintf(stderr,"%s\n",s); exit( 1 );
}