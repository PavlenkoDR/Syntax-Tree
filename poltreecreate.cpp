#include "poltreecreate.h"

//extern voc_func myvoc;
extern priora priority;
extern t_LeafCreate LeafCreate[4];
extern t_bFunc bFunc[4];
extern t_uFunc uFunc[21];

MySStack *tmpSpoint;
MySTree *tmpTpoint, *tmpT1point;
MySStack *tmp, *tmp_;
MySStack *tmpS1point;

/*
    Реализация стека для хранения лексем
*/

static struct MySStack *head = NULL;
static struct MySStack *tail = NULL;

static struct MySStack *phead = NULL;
static struct MySStack *ptail = NULL;

static struct MySStack *fhead = NULL;
static struct MySStack *ftail = NULL;

static int bktcou = 0;

void Push(int flag, double num,  char *oper, int bfunc, int ufunc)
{
    MySStack *tmps = (MySStack*) malloc(sizeof(MySStack));
    tmps->pTree = LeafCreate[flag](num, oper, bfunc, ufunc);
    if (head)
        head->next = tmps;
    tmps->prew = head;
    tmps->next = NULL;
    head = tmps;
    if (!tail)
        tail = head;
}

/*
    Реализация разбиения на лексемы
*/

const static int ParsState[4][8] =
{
    {
        0,  1,  2, -1, 3, 0, -1, -1
    },
    {
        1,  -1, 2, 2, -1, -1, 1, 4
    },
    {
        2,  1,  -1, -1, 3, 0, -1, -1
    },
    {
        3,  -1, -1, -1, -1, 0, -1, -1
    }
};

int ParsNum( char **str)
{
    double num = NumPars(str);
    Push(0, num, NULL, 0, 0);
    return 0;
}

char *CharPars( char **str)
{
     char *mystr = ( char*) malloc(sizeof( char)*1000);
     char *point = mystr;
    while (((**str >= 'a')&&(**str <= 'z'))||((**str >= 'A')&&(**str <= 'Z'))||((**str >= '0')&&(**str <= '9')))
    {
        *point = **str;
        point++;
        (*str)++;
    }
    *point = 0;
    return mystr;
}

int ParsChar( char **str)
{
    char *tmpstr = CharPars(str);
    int a = funorvar(tmpstr);
    if (a == -1)
    {
        Push(1, 0, tmpstr, 0, 0);
        return 1;
    }
    else
    {
        Push(3, 0, NULL, 0, a);
        return 4;
    }
    return 0;
}

int ParsUnaryMinus( char **str)
{
    Push(3, 0, NULL, 0, 1);
    (*str)++;
    return 0;
}
int ParsBinaryFunc( char **str)
{
    int a[256];
    a['+'] = 0;
    a['-'] = 1;
    a['*'] = 2;
    a['/'] = 3;
    Push(2, 0, NULL, a[(int)**str], 0);
    (*str)++;
    return 0;
}

int ParsSpace( char **str)
{
    while ((**str == ' ')||(**str == '\t'))
        (*str)++;
    return 0;
}


int ParsOpenBkt( char **str)
{
    bktcou++;
    Push(3, 0, NULL, 0, 4);
    (*str)++;
    return 0;
}

int ParsCloseBkt( char **str)
{
    bktcou--;
    Push(2, 0, NULL, 5, 0);
    (*str)++;
    return 0;
}

int ParsEnd( char **str)
{
    return 255;
}

int ParsERR( char **str)
{
    return 254;
}

t_ParsFunc ParsFunc[9] =
{
    &ParsSpace, &ParsNum, &ParsUnaryMinus, &ParsBinaryFunc, &ParsChar, &ParsOpenBkt, &ParsCloseBkt, &ParsEnd, &ParsERR
};

int Parsing( char **str)
{
    if (strlen(*str) > 90)
        {
            std::cout << "Превышен лимит строки" << std::endl;
        }
    t_ParsFunc func;
    int state1 = 0;
    int state2[256], tmpst = 0;
    int i;
    for (i = 0; i < 256; i++)
        state2[i] = -1;
    state2[' '] = 0;
    state2['\t'] = 0;
    for (i = '0'; i <= '9'; i++)
        state2[i] = 1;
    state2['-'] = 2;
    state2['+'] = 3;
    state2['*'] = 3;
    state2['/'] = 3;
    state2['^'] = 3;
    for (i = 'a'; i<= 'z'; i++)
        state2[i] = 4;
    for (i = 'A'; i<= 'Z'; i++)
        state2[i] = 4;
    state2['('] = 5;
    state2[')'] = 6;
    state2[0] = 7;
    state2[255] = 7;
    state2[10] = 7;
    int res;
    while(1)
    {
        tmpst = state2[(int)**str];
        if (tmpst == -1)
        {
            puts("Ошибка парсинга\n");
            return 1;
        }
        if ((state1 != 0)&&(**str == '-'))
            tmpst++;
        func = *ParsFunc[tmpst];
        res = func(str);
        if (res == 254)
            return 1;
        if (tmpst==4)
        {
            tmpst = res;
        }
        state1 = ParsState[state1][tmpst];
        if (state1 == -1)
        {
            puts("Ошибка парсинга\n");
            return 1;
        }
        if (res == 255)
            break;
    }
    if (bktcou == 0)
        return 0;
    else
    {
        return 1;
        printf("Не все скобки закрыты\n");
    }
}

/*
    Обратная польская + 2 стека для нее
*/

void AddKurwaOper()
{
    tmp = (MySStack*) malloc(sizeof(MySStack));
    tmp->pTree = tmpTpoint;
    if (phead)
        phead->next = tmp;
    tmp->prew = phead;
    tmp->next = NULL;
    phead = tmp;
    if (!ptail)
        ptail = phead;
    tmpSpoint = tmpSpoint->next;
    if (tmpSpoint)
        tmpTpoint = tmpSpoint->pTree;
    return;
}

void AddKurwaFunc()
{
    tmp = (MySStack*) malloc(sizeof(MySStack));
    tmp->pTree = tmpTpoint;
    if (fhead)
        fhead->next = tmp;
    tmp->prew = fhead;
    tmp->next = NULL;
    fhead = tmp;
    if (!ftail)
        ftail = fhead;
    tmpSpoint = tmpSpoint->next;
    if (tmpSpoint)
        tmpTpoint = tmpSpoint->pTree;
    return;
}

void PopKurwa()
{
    tmp = (MySStack*) malloc(sizeof(MySStack));
    tmp->pTree = fhead->pTree;
    if (phead)
        phead->next = tmp;
    tmp_ = fhead;
    fhead = fhead->prew;
    tmp->prew = phead;
    tmp->next = NULL;
    phead = tmp;
    if (!ptail)
        ptail = phead;
    free(tmp_);
    if (fhead)
        fhead->next = NULL;
    return;
}

void PolskiKurwa()
{
    priora priority;
    if (!tail) return;
    tmpSpoint = tail;
    tmpTpoint = tmpSpoint->pTree;
    while (tmpSpoint)
    {
        if (tmpTpoint->flag < 2)
            AddKurwaOper();
        else
        {
            if (tmpTpoint->flag == 2)
                AddKurwaFunc();
            else
            {
                if (fhead)
                {
                    tmpT1point = fhead->pTree;
                    if (priority[tmpTpoint->func] <= priority[tmpT1point->func])
                    {
                        if ((tmpTpoint->func == 5)&&(tmpT1point->func == 4))
                        {
                            tmp_ = fhead;
                            fhead = fhead->prew;
                            free(tmp_);
                            if (fhead)
                                fhead->next = NULL;
                            tmpSpoint = tmpSpoint->next;
                            free(tmpTpoint);
                            if (tmpSpoint)
                                tmpTpoint = tmpSpoint->pTree;
                        }
                        else
                            PopKurwa();
                    }
                    else
                        AddKurwaFunc();
                }
                else
                    AddKurwaFunc();
            }
        }
    }
    while (fhead)
        PopKurwa();
    return;
}

/*
    Вертим Польскую запись и строим дерево
*/

void AddTree(int par)
{
    tmpS1point = ptail->prew;
    if (par)
        tmpTpoint->right = tmpS1point->pTree;
    else
        tmpTpoint->left = tmpS1point->pTree;
    tmpSpoint = tmpS1point->prew;
    ptail->prew = tmpSpoint;
    if (tmpSpoint)
        tmpSpoint->next = ptail;
    free(tmpS1point);
    return;
}

MySTree *TreeCreator()
{
    while(ptail)
    {
        tmpTpoint = ptail->pTree;
        if (tmpTpoint->flag == 3)
        {
            AddTree(1);
            AddTree(0);
        }
        else if (tmpTpoint->flag == 2)
        {
            AddTree(0);
        }
        ptail = ptail->next;
    }
    return tmpTpoint;
}

