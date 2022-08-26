#include "tree.h"

#pragma warning(disable : 4996)

voc_func myvoc;
extern priora priority;
t_LeafCreate LeafCreate[4];
t_bFunc bFunc[4];
t_uFunc uFunc[21];
/*
    О хранении функций

    vocabulary - структура Функций и их id
    vocstr - по id получить название функции (Словарь)
    sizevoc - Размер словаря
    myvoc - сам словарь vocabulary
    inilib - инициализация словаря из файла
    funorvar - по сути попытка поиска функции для дальнейшего разяснения: Функция или переменная?
*/
voc_func::voc_func()
    {
        setlocale(LC_ALL, "RUS");
        FILE *f = fopen("library.lib", "rt");
        if (!f)
        {
            std::cout << "Библиотека не открыта" << std::endl;
            //exit(0);
        }
        fscanf(f, "%d", &sizevoc);
        _myvoc = (vocabulary*) malloc(sizeof(vocabulary)*sizevoc);
        int i, tmp;
        vocstr[0] = "+";
        vocstr[1] = "-";
        vocstr[2] = "*";
        vocstr[3] = "/";
        vocstr[4] = "(";
        vocstr[5] = ")";
        i = fgetc(f);
        i = 0;
        while ((!feof(f))&&(i < sizevoc))
        {
            fscanf(f, "%d", &_myvoc[i].id);
            fscanf(f, "%d", &tmp);
            fscanf(f, "%s", _myvoc[i].str);
            tmp = strlen((char*)_myvoc[i].str) - 1;
            if (_myvoc[i].str[tmp] == 10)
                _myvoc[i].str[tmp] = 0;
            vocstr[_myvoc[i].id] = _myvoc[i].str;
            i++;
        }
        fclose(f);
    }

int funorvar(char *str)
{
    int i;
    for (i = 0; i < myvoc.sizevoc; i++)
    {
        if (strcmp((char*)str, (char*)myvoc[i].str) == 0)
            return myvoc[i].id;
    }
    return -1;
}

void PrintAllVoc()
{
    printf("-------------------------------Доступные функции-------------------------------\n");
    printf("-------------------------------------------------------------------------------\n");
    int i;
    for (i = 0; i < myvoc.sizevoc; i++)
    {
        printf("%s", myvoc[i].str);
        if ((i + 1) % 2 == 0)
            printf(",\n");
        else
            printf(", ");
    }
    printf("+, -, *, /.\n");
    printf("\n-------------------------------------------------------------------------------\n> ");
    return;
}

/*
    О хранении переменных

    addoper - добавить переменную в словарь
    deopervoc - Разрушить словарь
    findopervoc - Найти функцию в словаре по ее названию
    valadd - инициализировать переменную числом для подстановки
*/
/*
typedef struct opervoc
{
     char str[BUFF_SIZE];
    double val;
    struct opervoc *prew;
} opervoc;

static opervoc *vochead;
static opervoc *voctail;
*/
void addoper(char *str)
{
    if (voctail)
    {
        voctail->prew = (opervoc*) malloc(sizeof(opervoc));
        voctail = voctail->prew;
    }
    else
    {
        voctail = (opervoc*) malloc(sizeof(opervoc));
        vochead = voctail;
    }
    voctail->prew = NULL;
    memcpy(( char*)voctail->str, ( char*)str, strlen((char*)str)+1);
    return;
}

void deopervoc()
{
    if (!vochead)
        return;
    opervoc *tmp = vochead->prew;
    while(vochead)
    {
        free(vochead);
        vochead = tmp;
        if (vochead)
            tmp = vochead->prew;
    }
    return;
}

opervoc *findopervoc(char *str)
{
    opervoc *tmp = vochead;
    while (tmp)
    {
        if (strcmp((char*)str, (char*)tmp->str) == 0)
        {
            return tmp;
        }
        tmp = tmp->prew;
    }
    return NULL;
}

//=================================================================================
//double NumPars(char **str);
/*
void valadd()
{
    opervoc *tmp = vochead;
    char str[BUFF_SIZE], *p;
    while(tmp)
    {
        printf("\n> %s = ", tmp->str);
        scanf("%s", str);
        p = str;
        tmp->val = NumPars(&p);
        tmp = tmp->prew;
    }
    return;
}
*/
//typedef MySTree *(*t_LeafCreate)(double num, char *oper, int bfunc, int ufunc);

MySTree *CreateLeafNum(double num, char *oper, int bfunc, int ufunc)
{
    MySTree *tmpt = (MySTree*) malloc(sizeof(MySTree));
    tmpt->oper = NULL;
    tmpt->left = NULL;
    tmpt->right = NULL;
    tmpt->flag = 0;
    tmpt->num = num;
    return tmpt;
}

MySTree *CreateLeafOper(double num, char *oper, int bfunc, int ufunc)
{
    MySTree *tmpt = (MySTree*) malloc(sizeof(MySTree));
    tmpt->oper = NULL;
    tmpt->left = NULL;
    tmpt->right = NULL;
    tmpt->flag = 1;
    tmpt->oper = oper;
    if (findopervoc(oper) == NULL)
        addoper(oper);
    return tmpt;
}

MySTree *CreateLeafBFunc(double num, char *oper, int bfunc, int ufunc)
{
    MySTree *tmpt = (MySTree*) malloc(sizeof(MySTree));
    tmpt->oper = NULL;
    tmpt->left = NULL;
    tmpt->right = NULL;
    tmpt->flag = 3;
    tmpt->func = bfunc;
    return tmpt;
}
MySTree *CreateLeafUFunc(double num, char *oper, int bfunc, int ufunc)
{
    MySTree *tmpt = (MySTree*) malloc(sizeof(MySTree));
    tmpt->oper = NULL;
    tmpt->left = NULL;
    tmpt->right = NULL;
    tmpt->flag = 2;
    tmpt->func = ufunc;
    return tmpt;
}

//====================================
/*

typedef struct MySTree
{
    int flag;
    double num;
    char *oper;
    int func;
    struct MySTree *left;
    struct MySTree *right;
} MySTree;
*/
void iniLeafCreate(){
    LeafCreate[0] = &CreateLeafNum;
    LeafCreate[1] = &CreateLeafOper;
    LeafCreate[2] = &CreateLeafBFunc;
    LeafCreate[3] = &CreateLeafUFunc;
    return;
}

//char *printTree(MySTree *ttmp);

char *AddBrack(char *str, char *tmpstr)
{
    int len = strlen(str);
    str[len+1] = 0;
    str[len] = '(';
    strcat(str, tmpstr);
    len = strlen(str);
    str[len+1] = 0;
    str[len] = ')';
    return str;
}

char *printLeaf(int param, char *str, MySTree *ftmp)
{
    MySTree *tmptree;
    char *tmpstr;
    if (param)
    {
        tmpstr = printTree(ftmp->right);
        tmptree = ftmp->right;
    }
    else
    {
        tmpstr = printTree(ftmp->left);
        tmptree = ftmp->left;
    }
    if (tmptree->flag >= 2)
    {
        if ((priority[ftmp->func] > priority[tmptree->func])||(ftmp->func!=tmptree->func))
        {
            AddBrack(str, tmpstr);
        }
        else
            strcat(str, tmpstr);
    }
    else
        strcat(str, tmpstr);
    free(tmpstr);
    return str;
}


char *printTree(MySTree *ttmp)
{
    char *str = (char*) malloc(sizeof(char)*BUFF_SIZE);
    char *tmpstr;
    MySTree *tmptree;
    if (ttmp)
    {
        switch(ttmp->flag)
        {
        case 0:
            sprintf(str, "%lf", ttmp->num);
            return str;
            break;
        case 1:
            memcpy(str, ttmp->oper, strlen(ttmp->oper)+1);
            return str;
            break;
        case 3:
            str[0] = 0;
            str = printLeaf(0, str, ttmp);
            strcat(str, myvoc.vocstr[ttmp->func]);
            str = printLeaf(1, str, ttmp);
            return str;
            break;
        case 2:
            str[0] = 0;
            strcat(str, myvoc.vocstr[ttmp->func]);
            tmpstr = printTree(ttmp->left);
            tmptree = ttmp->left;
            if ((tmptree->flag >= 2)||(ttmp->flag == 2))
            {
                AddBrack(str, tmpstr);
            }
            else
                strcat(str, tmpstr);
            free(tmpstr);
            return str;
            break;
        }
    }
    return str;
}

MySTree *CopyTree(MySTree *tree)
{
    MySTree *buff = (MySTree*) malloc(sizeof(MySTree));
    buff->flag = tree->flag;
    switch (buff->flag)
    {
    case 0:
        buff->num = tree->num;
        break;
    case 1:
        buff->oper = (char*) malloc(sizeof(char)*BUFF_SIZE);
        memcpy(buff->oper, tree->oper, strlen(tree->oper)+1);
        break;
    case 2:
        buff->func = tree->func;
        buff->left = CopyTree(tree->left);
        break;
    case 3:
        buff->func = tree->func;
        buff->left = CopyTree(tree->left);
        buff->right = CopyTree(tree->right);
        break;
    }
    return buff;
}

void ChildLeaf(MySTree *tree, MySTree *left, MySTree *right)
{
    tree->left = left;
    tree->right = right;
    return;
}

MySTree *DestroyTree(MySTree *tree)
{
    //return tree;
    if (!tree)
        return NULL;
    switch (tree->flag)
    {
    case 0:
        free(tree);
        break;
    case 1:
        free(tree->oper);
        free(tree);
        break;
    case 2:
        tree->left = DestroyTree(tree->left);
        free(tree);
        break;
    case 3:
        tree->left = DestroyTree(tree->left);
        tree->right = DestroyTree(tree->right);
        free(tree);
        break;
    }
    return NULL;
}

//====================================
//double SolveTree(MySTree *tree);

double bPlusSolve(MySTree *left, MySTree *right)
{
    return SolveTree(left) + SolveTree(right);
}

double bMinusSolve(MySTree *left, MySTree *right)
{
    return SolveTree(left) - SolveTree(right);
}

double bMultSolve(MySTree *left, MySTree *right)
{
    return SolveTree(left) * SolveTree(right);
}

double bDivSolve(MySTree *left, MySTree *right)
{
    double tmp = SolveTree(right);
    if (tmp != 0)
        return SolveTree(left) / tmp;
    else
    {
        std::cout << "Тут ноль выпал, я так не играю" << std::endl;
    }
    return 0;
}

//typedef double (*t_bFunc)(MySTree *left, MySTree *right);
void inibFunc(){
    bFunc[0] = &bPlusSolve;
    bFunc[1] = &bMinusSolve;
    bFunc[2] = &bMultSolve;
    bFunc[3] = &bDivSolve;
    return;
}

double uZero(MySTree *tree)
{
    return 0;
}

double uMinus(MySTree *tree)
{
    return -SolveTree(tree);
}

double uFloor(MySTree *tree)
{
    return floor(SolveTree(tree));
}

double uLog(MySTree *tree)
{
    return log(SolveTree(tree));
}

double uLog10(MySTree *tree)
{
    return log10(SolveTree(tree));
}

double uSin(MySTree *tree)
{
    return sin(SolveTree(tree));
}

double uSinh(MySTree *tree)
{
    return sinh(SolveTree(tree));
}

double uSqrt(MySTree *tree)
{
    return sqrt(SolveTree(tree));
}

double uTan(MySTree *tree)
{
    return tan(SolveTree(tree));
}

double uTanh(MySTree *tree)
{
    return tanh(SolveTree(tree));
}

double uAbs(MySTree *tree)
{
    return abs((int)SolveTree(tree));
}

double uAcos(MySTree *tree)
{
    return acos(SolveTree(tree));
}

double uAsin(MySTree *tree)
{
    return asin(SolveTree(tree));
}

double uAtan(MySTree *tree)
{
    return atan(SolveTree(tree));
}

double uCeil(MySTree *tree)
{
    return ceil(SolveTree(tree));
}

double uCos(MySTree *tree)
{
    return cos(SolveTree(tree));
}

double uCosh(MySTree *tree)
{
    return cosh(SolveTree(tree));
}
void iniuFunc(){
    uFunc[0] = &uZero;
    uFunc[1] = &uMinus;
    uFunc[2] = &uZero;
    uFunc[3] = &uZero;
    uFunc[4] = &uZero;
    uFunc[5] = &uZero;
    uFunc[6] = &uFloor;
    uFunc[7] = &uLog;
    uFunc[8] = &uLog10;
    uFunc[9] = &uSin;
    uFunc[10] = &uSinh;
    uFunc[11] = &uSqrt;
    uFunc[12] = &uTan;
    uFunc[13] = &uTanh;
    uFunc[14] = &uAbs;
    uFunc[15] = &uAcos;
    uFunc[16] = &uAsin;
    uFunc[17] = &uAtan;
    uFunc[18] = &uCeil;
    uFunc[19] = &uCos;
    uFunc[20] = &uCosh;
    return;
}

double SolveTree(MySTree *tree)
{
    switch (tree->flag)
    {
    case 0:
        return tree->num;
        break;
    case 1:
        return findopervoc(tree->oper)->val;
        break;
    case 2:
        return uFunc[tree->func](tree->left);
        break;
    case 3:
        return bFunc[tree->func](tree->left, tree->right);
        break;
    }
    return 0;
}

//typedef double (*t_uFunc)(MySTree *tree);
