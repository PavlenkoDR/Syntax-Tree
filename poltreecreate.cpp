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

PolTreeSymbolTypeID operator++(PolTreeSymbolTypeID& d)
{
    return static_cast<PolTreeSymbolTypeID>(static_cast<int>(d) + 1);
}

PolTreeSymbolTypeID operator++(PolTreeSymbolTypeID& c, int) {
    PolTreeSymbolTypeID result = c;
    ++c;
    return result;
}

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

const static PolTreeParsStateID ParsState[4][8] =
{
    {
        PolTreeParsStateID::START,
        PolTreeParsStateID::NUMBER, 
        PolTreeParsStateID::BINARY,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::FUNCTION,
        PolTreeParsStateID::START,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::ERROR
    },
    {
        PolTreeParsStateID::NUMBER,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::BINARY,
        PolTreeParsStateID::BINARY,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::NUMBER,
        PolTreeParsStateID::END
    },
    {
        PolTreeParsStateID::BINARY,
        PolTreeParsStateID::NUMBER,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::FUNCTION,
        PolTreeParsStateID::START,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::ERROR
    },
    {
        PolTreeParsStateID::FUNCTION,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::START,
        PolTreeParsStateID::ERROR,
        PolTreeParsStateID::ERROR
    }
};

PolTreeParsStateID getNextParseState(PolTreeParsStateID parsState, PolTreeSymbolTypeID stateFromSymbol) {
    return ParsState[static_cast<int>(parsState)][static_cast<int>(stateFromSymbol)];
}

t_ParsFunc getParsFunc(PolTreeSymbolTypeID stateFromSymbol) {
    switch (stateFromSymbol) {
    case PolTreeSymbolTypeID::SPACE:
        return &ParsSpace;
        break;
    case PolTreeSymbolTypeID::NUMBER:
        return &ParsNum;
        break;
    case PolTreeSymbolTypeID::MINUS:
        return &ParsUnaryMinus;
        break;
    case PolTreeSymbolTypeID::BINARY:
        return &ParsBinaryFunc;
        break;
    case PolTreeSymbolTypeID::SYMBOL:
        return &ParsChar;
        break;
    case PolTreeSymbolTypeID::BRASCKET_OPEN:
        return &ParsOpenBkt;
        break;
    case PolTreeSymbolTypeID::BRASCKET_CLOSE:
        return &ParsCloseBkt;
        break;
    case PolTreeSymbolTypeID::END:
        return &ParsEnd;
        break;
    case PolTreeSymbolTypeID::PARSE_ERROR:
        return &ParsERR;
        break;
    default:
        break;
    }
}

PolTreeSymbolTypeID ParsNum( char **str)
{
    double num = NumPars(str);
    Push(0, num, NULL, 0, 0);
    return PolTreeSymbolTypeID::SPACE;
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

PolTreeSymbolTypeID ParsChar( char **str)
{
    char *tmpstr = CharPars(str);
    int a = funorvar(tmpstr);
    if (a == -1)
    {
        Push(1, 0, tmpstr, 0, 0);
        return PolTreeSymbolTypeID::NUMBER;
    }
    else
    {
        Push(3, 0, NULL, 0, a);
        return PolTreeSymbolTypeID::SYMBOL;
    }
    return PolTreeSymbolTypeID::SPACE;
}

PolTreeSymbolTypeID ParsUnaryMinus( char **str)
{
    Push(3, 0, NULL, 0, 1);
    (*str)++;
    return PolTreeSymbolTypeID::SPACE;
}

PolTreeSymbolTypeID ParsBinaryFunc( char **str)
{
    int a[256];
    a['+'] = 0;
    a['-'] = 1;
    a['*'] = 2;
    a['/'] = 3;
    Push(2, 0, NULL, a[(int)**str], 0);
    (*str)++;
    return PolTreeSymbolTypeID::SPACE;
}

PolTreeSymbolTypeID ParsSpace( char **str)
{
    while ((**str == ' ')||(**str == '\t'))
        (*str)++;
    return PolTreeSymbolTypeID::SPACE;
}


PolTreeSymbolTypeID ParsOpenBkt( char **str)
{
    bktcou++;
    Push(3, 0, NULL, 0, 4);
    (*str)++;
    return PolTreeSymbolTypeID::SPACE;
}

PolTreeSymbolTypeID ParsCloseBkt( char **str)
{
    bktcou--;
    Push(2, 0, NULL, 5, 0);
    (*str)++;
    return PolTreeSymbolTypeID::SPACE;
}

PolTreeSymbolTypeID ParsEnd( char **str)
{
    return PolTreeSymbolTypeID::PARSE_SUCCESS;
}

PolTreeSymbolTypeID ParsERR( char **str)
{
    return PolTreeSymbolTypeID::PARSE_ERROR;
}

int Parsing( char **str)
{
    if (strlen(*str) > 90)
        {
            std::cout << "Превышен лимит строки" << std::endl;
        }
    t_ParsFunc func;
    PolTreeSymbolTypeID state2[256], stateFromSymbol = PolTreeSymbolTypeID::SPACE;
    PolTreeParsStateID parsState = PolTreeParsStateID::START;
    int i;
    for (i = 0; i < 256; i++)
        state2[i] = PolTreeSymbolTypeID::ERROR;
    state2[' '] = PolTreeSymbolTypeID::SPACE;
    state2['\t'] = PolTreeSymbolTypeID::SPACE;
    for (i = '0'; i <= '9'; i++)
        state2[i] = PolTreeSymbolTypeID::NUMBER;
    state2['-'] = PolTreeSymbolTypeID::MINUS;
    state2['+'] = PolTreeSymbolTypeID::BINARY;
    state2['*'] = PolTreeSymbolTypeID::BINARY;
    state2['/'] = PolTreeSymbolTypeID::BINARY;
    state2['^'] = PolTreeSymbolTypeID::BINARY;
    for (i = 'a'; i<= 'z'; i++)
        state2[i] = PolTreeSymbolTypeID::SYMBOL;
    for (i = 'A'; i<= 'Z'; i++)
        state2[i] = PolTreeSymbolTypeID::SYMBOL;
    state2['('] = PolTreeSymbolTypeID::BRASCKET_OPEN;
    state2[')'] = PolTreeSymbolTypeID::BRASCKET_CLOSE;
    state2[0] = PolTreeSymbolTypeID::END;
    state2[255] = PolTreeSymbolTypeID::END;
    state2[10] = PolTreeSymbolTypeID::END;
    PolTreeSymbolTypeID res;
    while(1)
    {
        stateFromSymbol = state2[(int)**str];
        if (stateFromSymbol == PolTreeSymbolTypeID::ERROR)
        {
            puts("Ошибка парсинга\n");
            return 1;
        }
        if ((parsState != PolTreeParsStateID::START)&&(**str == '-'))
            stateFromSymbol++;
        func = *getParsFunc(stateFromSymbol);
        res = func(str);
        if (res == PolTreeSymbolTypeID::PARSE_ERROR)
            return 1;
        if (stateFromSymbol == PolTreeSymbolTypeID::SYMBOL)
        {
            stateFromSymbol = res;
        }
        parsState = getNextParseState(parsState, stateFromSymbol);
        if (parsState == PolTreeParsStateID::ERROR)
        {
            puts("Ошибка парсинга\n");
            return 1;
        }
        if (res == PolTreeSymbolTypeID::PARSE_SUCCESS)
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

