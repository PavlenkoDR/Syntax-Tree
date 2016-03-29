#include "numpars.h"

const static  char NumState[7][6] =
{
    {
        0, 1, 8, 8, 7, 7
    },
    {
        1, 1, 2, 4, 7, 7
    },
    {
        3, 3, 8, 8, 8, 7
    },
    {
        3, 3, 8, 4, 7, 7
    },
    {
        6, 6, 8, 8, 5, 7
    },
    {
        6, 6, 8, 8, 8, 7
    },
    {
        6, 6, 8, 8, 7, 7
    }
};

int RNum( char **str)
{
    nummantis = nummantis * 10 + (**str) - 48;
    (*str)++;
    return 0;
}

int RNumCom( char **str)
{
    nummantis = nummantis * 10 + (**str) - 48;
    numexptmp++;
    (*str)++;
    return 0;
}

int RNumExp( char **str)
{
    numexp = numexp * 10 + (**str) - 48;
    (*str)++;
    return 0;
}

int RNumPlusMinus( char **str)
{
    if ((**str) == '+')
        numexpsign = 0;
    else
        numexpsign = 1;
    (*str)++;
    return 0;
}

int RNumEnd( char **str)
{
    return 1;
}

int RSkip( char **str)
{
    (*str)++;
    return 0;
}

t_NumFunc NumFunc[7][6] =
{
    ///    0            1         2             3             4                 5
    {
        &RSkip,     &RNum,      NULL,       NULL,       &RNumEnd,           &RNumEnd ///+
    },
    {
        &RNum,      &RNum,      &RSkip,     &RSkip,     &RNumEnd,           &RNumEnd ///+
    },
    {
        &RNumCom,   &RNumCom,   NULL,       NULL,       NULL,               &RNumEnd ///+
    },
    {
        &RNumCom,   &RNumCom,   NULL,       &RSkip,     &RNumEnd,           &RNumEnd ///+
    },
    {
        &RNumExp,  &RNumExp,    NULL,       NULL,       &RNumPlusMinus,     &RNumEnd ///+
    },
    {
        &RNumExp,  &RNumExp,    NULL,       NULL,       NULL,               &RNumEnd ///+
    },
    {
        &RNumExp,  &RNumExp,    NULL,       NULL,       &RNumEnd,           &RNumEnd ///+
    }
};

double NumPars(char **str)
{
    nummantis = 0;
    numexp = 0;
    numexpsign = 0;
    numexptmp = 0;
    t_NumFunc func;
    int state2[256], state1 = 0;
    int i;
    for (i = 0; i < 256; i++)
        state2[i] = 5;
    state2['0'] = 0;
    for (i = '1'; i<= '9'; i++)
        state2[i] = 1;
    state2['.'] = 2;
    state2[','] = 2;
    state2['e'] = 3;
    state2['+'] = 4;
    state2['-'] = 4;
    state2[0] = 5;
    while (1)
    {
        func = *NumFunc[state1][state2[(int)**str]];
        state1 = NumState[state1][state2[(int)**str]];
        if (func(str) != 0)
            break;
    }
    for (i = 0; i < numexptmp; i++)
        nummantis /= 10;
    if (numexpsign == 0)
        for (i = 0; i < numexp; i++)
            nummantis *= 10;
    else
        for (i = 0; i < numexp; i++)
            nummantis /= 10;
    return nummantis;
}
