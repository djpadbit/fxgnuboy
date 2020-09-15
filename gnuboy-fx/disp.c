#include <gint/display.h>
#include "disp.h"

int text_length(const char *str)
{
	int l;
	dsize(str,NULL,&l,NULL);
	return l;
}