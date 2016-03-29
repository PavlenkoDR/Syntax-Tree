#ifndef NUMPARS_H_INCLUDED
#define NUMPARS_H_INCLUDED

#include <stdlib.h>
/*
    Парсинг числа
*/

static double nummantis;
static double numexp;
static double numexptmp;
static double numexpsign;

int RNum( char **str);
int RNumCom( char **str);
int RNumExp( char **str);
int RNumPlusMinus( char **str);
int RNumEnd( char **str);
int RSkip( char **str);
typedef int (*t_NumFunc)( char **str);

//t_NumFunc NumFunc[7][6];

double NumPars(char **str);
#endif // NUMPARS_H_INCLUDED
