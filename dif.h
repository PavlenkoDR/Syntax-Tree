#ifndef DIF_H_INCLUDED
#define DIF_H_INCLUDED

#include <iostream>
#include <cstring>

#include "tree.h"
#include "global.h"

MySTree *ReturnLeft(MySTree *tree);
MySTree *ReturnRight(MySTree *tree);
MySTree *ReturnSolve(MySTree *tree);
MySTree *Opti(MySTree *tree);
MySTree *dif(MySTree *tree,  char *str);

#endif // DIF_H_INCLUDED
