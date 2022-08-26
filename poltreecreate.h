#ifndef POLTREECREATE_H_INCLUDED
#define POLTREECREATE_H_INCLUDED

#include <cstring>

#include "numpars.h"
#include "tree.h"
#include "global.h"

/*
    ���������� ����� ��� �������� ������
*/

class MySStack
{
public:
    MySStack *next;
    MySStack *prew;
    MySTree *pTree;
};

enum class PolTreeParsStateID : int {
    ERROR = -1,
    START = 0,
    NUMBER = 1,
    BINARY = 2,
    FUNCTION = 3,
    END = 4
};

enum class PolTreeSymbolTypeID : int {
    ERROR = -1,
    SPACE = 0,
    NUMBER = 1,
    MINUS = 2,
    BINARY = 3,
    SYMBOL = 4,
    BRASCKET_OPEN = 5,
    BRASCKET_CLOSE = 6,
    END = 7,
    PARSE_ERROR = 254,
    PARSE_SUCCESS = 255
};

void Push(int flag, double num,  char *oper, int bfunc, int ufunc);
/*
    ���������� ��������� �� �������
*/

PolTreeSymbolTypeID ParsNum( char **str);
char *CharPars( char **str);
PolTreeSymbolTypeID ParsChar( char **str);
PolTreeSymbolTypeID ParsUnaryMinus( char **str);
PolTreeSymbolTypeID ParsBinaryFunc( char **str);
PolTreeSymbolTypeID ParsSpace( char **str);
PolTreeSymbolTypeID ParsOpenBkt( char **str);
PolTreeSymbolTypeID ParsCloseBkt( char **str);
PolTreeSymbolTypeID ParsEnd( char **str);
PolTreeSymbolTypeID ParsERR( char **str);
typedef PolTreeSymbolTypeID(*t_ParsFunc)( char **str);
int Parsing( char **str);
/*
    �������� �������� + 2 ����� ��� ���
*/

void AddKurwaOper();
void AddKurwaFunc();
void PopKurwa();
void PolskiKurwa();

/*
    ������ �������� ������ � ������ ������
*/

void AddTree(int par);
MySTree *TreeCreator();
#endif // POLTREECREATE_H_INCLUDED
