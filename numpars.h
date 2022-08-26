#ifndef NUMPARS_H_INCLUDED
#define NUMPARS_H_INCLUDED

#include <stdlib.h>
/*
    Парсинг числа
*/

enum class NumTypeID : int {
    ZERO = 0,
    NUMBER = 1,
    DOT = 2,
    EXPONENT = 3,
    SIGN = 4,
    ERROR = 5
};

enum class NumParsStateID {
    SKIP = 0,
    NUMBER_WHOLE = 1,
    DOT = 2,
    NUMBER_FRACTION = 3,
    EXPONENT = 4,
    EXPONENT_SIGN = 5,
    NUMBER_EXPONENT = 6,
    END = 7,
    ERROR = 8,
};

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
