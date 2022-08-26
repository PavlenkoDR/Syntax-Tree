#ifndef SOLVETREE_H_INCLUDED
#define SOLVETREE_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cmath>


#define BUFF_SIZE 10000

#include "numpars.h"
#include "global.h"
#include <cstdlib>
#include <clocale>

class voc_func
{
private:
    typedef struct vocabulary
    {
        char str[BUFF_SIZE];
        int id;
    } vocabulary;
public:
    const char *vocstr[100];
    vocabulary *_myvoc;
    int sizevoc;
    voc_func();
    inline vocabulary &operator [](int index){return _myvoc[index];}
};

///*
//void inilib();
int funorvar(char *str);
void PrintAllVoc();
//*/
class opervoc
{
public:
    char str[BUFF_SIZE];
    double val;
    opervoc *prew;
};

static opervoc *vochead;
static opervoc *voctail;

void addoper(char *str);
void deopervoc();
opervoc *findopervoc(char *str);
void valadd();
/**
/********************************************************************************************************
*/
class MySTree
{
public:
    int flag;
    double num;
    char *oper;
    int func;
    MySTree *left;
    MySTree *right;
};

typedef MySTree *(*t_LeafCreate)(double num, char *oper, int bfunc, int ufunc);
///*
MySTree *CreateLeafNum(double num, char *oper, int bfunc, int ufunc);
MySTree *CreateLeafOper(double num, char *oper, int bfunc, int ufunc);
MySTree *CreateLeafBFunc(double num, char *oper, int bfunc, int ufunc);
MySTree *CreateLeafUFunc(double num, char *oper, int bfunc, int ufunc);

void iniLeafCreate();

MySTree *DestroyTree(MySTree *tree);
void ChildLeaf(MySTree *tree, MySTree *left, MySTree *right);
MySTree *CopyTree(MySTree *tree);
char *printTree(MySTree *ttmp);
char *printLeaf(int param, char *str, MySTree *ftmp);
char *AddBrack(char *str, char *tmpstr);

double SolveTree(MySTree *tree);
double bPlusSolve(MySTree *left, MySTree *right);
double bMinusSolve(MySTree *left, MySTree *right);
double bMultSolve(MySTree *left, MySTree *right);
double bDivSolve(MySTree *left, MySTree *right);
//*/
typedef double (*t_bFunc)(MySTree *left, MySTree *right);

void inibFunc();

///*
double uZero(MySTree *tree);
double uMinus(MySTree *tree);
double uFloor(MySTree *tree);
double uLog(MySTree *tree);
double uLog10(MySTree *tree);
double uSin(MySTree *tree);
double uSinh(MySTree *tree);
double uSqrt(MySTree *tree);
double uTan(MySTree *tree);
double uTanh(MySTree *tree);
double uAbs(MySTree *tree);
double uAcos(MySTree *tree);
double uAsin(MySTree *tree);
double uAtan(MySTree *tree);
double uCeil(MySTree *tree);
double uCos(MySTree *tree);
double uCosh(MySTree *tree);
//*/
typedef double (*t_uFunc)(MySTree *tree);
void iniuFunc();
///*
double SolveTree(MySTree *tree);

//*/
#endif // SOLVETREE_H_INCLUDED
