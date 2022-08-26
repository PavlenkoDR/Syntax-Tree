#include "project_syn.h"
#include "global.h"
#include <iostream>

extern priora priority;

using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");
    iniLeafCreate();
    inibFunc();
    iniuFunc();
    PrintAllVoc();
    char *a = new char[BUFF_SIZE];
    MySTree *tmpt = NULL, *tmpt1 = NULL;
    std::cin.getline(a, BUFF_SIZE);
    std::string aBase(a);
    //ap = "log10(x*x)";
    char what[BUFF_SIZE];
    if (Parsing(&a) == 0)
    {
        system("cls");
        PolskiKurwa();
        tmpt = TreeCreator();
        //valadd();
        printf("%lf\n", SolveTree(tmpt));
        printf("По какой переменной?\n> ");
        std::cin.getline(what, BUFF_SIZE);
        printf("\n%s = %s\n", aBase.c_str(), printTree(Opti(tmpt)));
        printf("(%s)' = ", printTree(tmpt));
        tmpt1 = dif(tmpt, what);
        printf("%s\n", printTree(tmpt1));
        //printf("%lf\n", Series(phead->pTree));
    }
    else
    {
        printf("Не зашли в Польшу\n");
    }
    if (tmpt)
    {
        if (tmpt1)
            tmpt1 = DestroyTree(tmpt1);
        tmpt = DestroyTree(tmpt);
        free(tmpt);
    }
    deopervoc();
    system("pause");
    return 0;
}
