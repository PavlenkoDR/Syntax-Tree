#ifndef POLTREECREATE_H_INCLUDED
#define POLTREECREATE_H_INCLUDED

#include <cstring>

#include "numpars.h"
#include "tree.h"
#include "global.h"

/*
    Реализация стека для хранения лексем
*/

class MySStack
{
public:
    MySStack *next;
    MySStack *prew;
    MySTree *pTree;
};

void Push(int flag, double num,  char *oper, int bfunc, int ufunc);
/*
    Реализация разбиения на лексемы
*/

int ParsNum( char **str);
char *CharPars( char **str);
int ParsChar( char **str);
int ParsUnaryMinus( char **str);
int ParsBinaryFunc( char **str);
int ParsSpace( char **str);
int ParsOpenBkt( char **str);
int ParsCloseBkt( char **str);
int ParsEnd( char **str);
int ParsERR( char **str);
typedef int (*t_ParsFunc)( char **str);
int Parsing( char **str);
/*
    Обратная польская + 2 стека для нее
*/

void AddKurwaOper();
void AddKurwaFunc();
void PopKurwa();
void PolskiKurwa();

/*
    Вертим Польскую запись и строим дерево
*/

void AddTree(int par);
MySTree *TreeCreator();
#endif // POLTREECREATE_H_INCLUDED
