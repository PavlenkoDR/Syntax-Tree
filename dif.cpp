
#include "dif.h"

extern priora priority;
//extern t_LeafCreate LeafCreate[4];
extern t_bFunc bFunc[4];
extern t_uFunc uFunc[21];

MySTree *ReturnLeft(MySTree *tree)
{
    MySTree *left;
    left = tree->left;
    DestroyTree(tree->right);
    free(tree);
    return left;
}

MySTree *ReturnRight(MySTree *tree)
{
    MySTree *right;
    right = tree->right;
    DestroyTree(tree->left);
    free(tree);
    return right;
}

MySTree *ReturnSolve(MySTree *tree)
{
    if (tree->left->flag == 1)
        return tree;
    if (tree->flag == 3)
    {
        if (tree->right->flag == 1)
            return tree;
    }
    if (tree->flag == 3)
        tree->num = bFunc[tree->func](tree->left, tree->right);
    else
        tree->num = uFunc[tree->func](tree->left);
    DestroyTree(tree->left);
    if (tree->flag == 3)
        DestroyTree(tree->right);
    tree->flag = 0;
    return tree;
}

MySTree *Opti(MySTree *tree)
{
    MySTree *left;
    MySTree *right;
    MySTree tmp;
    tmp.flag = 1;
    left = &tmp;
    right = &tmp;
    if (tree->flag >= 2)
    {
        //func = tree->func;
        left = tree->left;
        if (tree->flag == 3)
        {
            right = tree->right;
            if ((left->flag == 0)&&(right->flag == 0))
                return ReturnSolve(tree);
            else if (left->flag == 0)
            {
                if (((left->num == 0)&&(tree->func == 0))||((left->num == 1)&&(tree->func == 2)))
                    return ReturnRight(tree);
                else if ((left->num == 0)&&(tree->func == 1))
                {
                    if (right->num == 0)
                    {
                        return ReturnRight(tree);
                    }
                    tree->flag = 2;
                    tree->left = tree->right;
                    tree->right = NULL;
                    return tree;
                }
                else if ((left->num == 0)&&((tree->func == 2)||(tree->func == 3)))
                {
                    return ReturnLeft(tree);
                }
            }
            else if (right->flag == 0)
            {
                if (((right->num == 0)&&((tree->func == 0)||(tree->func == 1)))||((right->num == 1)&&(tree->func == 2)))
                    return ReturnLeft(tree);
                else if ((right->num == 0)&&(tree->func == 2))
                {
                    return ReturnRight(tree);
                }
                else if ((right->num == 0)&&(tree->func == 3))
                {
                    std::cout << "Деление на ноль не допустинмо" << std::endl;
                    exit(0);
                }
            }
        }
        else
        {
            if (tree->left->flag == 0)
            {
                return ReturnSolve(tree);
            }
        }
    }
    return tree;
}

MySTree *dif(MySTree *tree,  char *str)
{
    MySTree *buff,
            *left,
            *right,
            *dleft,
            *dright;
    if (!tree) return NULL;
    if (tree->flag < 2)
    {
        if (tree->flag == 0)
        {
            return CreateLeafNum(0, NULL, 0, 0);
        }
        else
        {
            if (strcmp(str, tree->oper) == 0)
            {
                return CreateLeafNum(1, NULL, 0, 0);
            }
            else
            {
                return CreateLeafNum(0, NULL, 0, 0);
            }
        }
    }
    else
    {
        switch (tree->func)
        {
        case 0:
            buff = CreateLeafBFunc(0, NULL, 0, 0);
            ChildLeaf(buff, Opti(dif(tree->left, str)), Opti(dif(tree->right, str)));
            return Opti(buff);
            break;
        case 1:
            if (tree->flag == 3)
                buff = CreateLeafBFunc(0, NULL, 1, 0);
            else
                buff = CreateLeafUFunc(0, NULL, 0, 1);
            if (tree->flag == 3)
                ChildLeaf(buff, Opti(dif(tree->left, str)), Opti(dif(tree->right, str)));
            else
                ChildLeaf(buff, Opti(dif(tree->left, str)), NULL);
            return Opti(buff);
            break;
        case 2:
            left = CreateLeafBFunc(0, NULL, 2, 0);
            right = CreateLeafBFunc(0, NULL, 2, 0);
            buff  = CreateLeafBFunc(0, NULL, 0, 0);
            ChildLeaf(left, dif(tree->left, str), CopyTree(tree->right));
            ChildLeaf(right, CopyTree(tree->left), dif(tree->right, str));
            ChildLeaf(buff, Opti(left), Opti(right));
            return Opti(buff);
            break;
        case 3:
            buff  = CreateLeafBFunc(0, NULL, 3, 0);
            left = CreateLeafBFunc(0, NULL, 2, 0);
            right = CreateLeafBFunc(0, NULL, 2, 0);
            dleft = CreateLeafBFunc(0, NULL, 1, 0);
            dright = CreateLeafBFunc(0, NULL, 2, 0);
            ChildLeaf(left, Opti(dif(tree->left, str)), Opti(CopyTree(tree->left)));
            ChildLeaf(right, Opti(CopyTree(tree->left)), Opti(dif(tree->right, str)));
            ChildLeaf(dleft, Opti(left), Opti(right));
            ChildLeaf(dright, Opti(CopyTree(tree->left)), Opti(CopyTree(tree->left)));
            ChildLeaf(buff, Opti(dleft), Opti(dright));
            return Opti(buff);
            break;
        case 6:
            return Opti(dif(tree->left, str));
            break;
        case 7://log
            buff  = CreateLeafBFunc(0, NULL, 3, 0);
            left = Opti(dif(tree->left, str));
            right = CopyTree(tree->left);
            ChildLeaf(buff, Opti(left), Opti(right));
            return Opti(buff);
            break;
        case 8://log10
            buff = CreateLeafUFunc(0, NULL, 0, 7);
            ChildLeaf(buff, CreateLeafNum(10, NULL, 0, 0), NULL);
            right = CreateLeafBFunc(0, NULL, 2, 0);
            ChildLeaf(right, CopyTree(tree->left), buff);
            buff = CreateLeafBFunc(0, NULL, 3, 0);
            ChildLeaf(buff, Opti(dif(tree->left, str)), right);
            return Opti(buff);
            break;
        case 9://sin
            buff  = CreateLeafBFunc(0, NULL, 2, 0);
            right = CreateLeafUFunc(0, NULL, 0, 19);
            ChildLeaf(buff, Opti(dif(tree->left, str)), right);
            ChildLeaf(right, CopyTree(tree->left), NULL);
            return Opti(buff);
            break;
        case 10://sinh
            buff  = CreateLeafBFunc(0, NULL, 2, 0);
            right = CreateLeafUFunc(0, NULL, 0, 20);
            ChildLeaf(buff, Opti(dif(tree->left, str)), right);
            ChildLeaf(right, CopyTree(tree->left), NULL);
            return Opti(buff);
            break;
        case 11://sqrt
            buff  = CreateLeafBFunc(0, NULL, 3, 0);
            right = CreateLeafBFunc(0, NULL, 2, 0);
            ChildLeaf(buff, Opti(dif(tree->left, str)), right);
            right = CreateLeafUFunc(0, NULL, 0, 11);
            ChildLeaf(right, CreateLeafNum(2, NULL, 0, 0), right);
            ChildLeaf(right, CopyTree(tree->left), NULL);
            return Opti(buff);
            break;
        case 12://tan
            buff = CreateLeafBFunc(0, NULL, 3, 0);
            right  = CreateLeafBFunc(0, NULL, 2, 0);
            ChildLeaf(buff, Opti(dif(tree->left, str)), right);
            left = CreateLeafUFunc(0, NULL, 0, 19);
            ChildLeaf(left, CopyTree(tree->left), NULL);
            ChildLeaf(right, left, left);
            return Opti(buff);
            break;
        case 13://tanh
            buff = CreateLeafBFunc(0, NULL, 3, 0);
            right  = CreateLeafBFunc(0, NULL, 2, 0);
            ChildLeaf(buff, Opti(dif(tree->left, str)), right);
            left = CreateLeafUFunc(0, NULL, 0, 20);
            ChildLeaf(left, CopyTree(tree->left), NULL);
            ChildLeaf(right, left, left);
            return Opti(buff);
            break;
        case 15://acos
            buff  = CreateLeafUFunc(0, NULL, 0, 1);
            left = CreateLeafBFunc(0, NULL, 3, 0);
            right = CreateLeafUFunc(0, NULL, 0, 11);
            ChildLeaf(left, Opti(dif(tree->left, str)), right);
            dleft = CreateLeafBFunc(0, NULL, 1, 0);
            ChildLeaf(right, dleft, NULL);
            dright = CreateLeafBFunc(0, NULL, 2, 0);
            ChildLeaf(dleft, CreateLeafNum(1, NULL, 0, 0), dright);
            ChildLeaf(dright, CopyTree(tree->left), CopyTree(tree->left));
            ChildLeaf(buff, Opti(left), NULL);
            return Opti(buff);
            break;
        case 16://asin
            buff = CreateLeafBFunc(0, NULL, 3, 0);
            right = CreateLeafUFunc(0, NULL, 0, 11);
            ChildLeaf(buff, Opti(dif(tree->left, str)), right);
            dleft = CreateLeafBFunc(0, NULL, 1, 0);
            ChildLeaf(right, dleft, NULL);
            dright = CreateLeafBFunc(0, NULL, 2, 0);
            ChildLeaf(dleft, CreateLeafNum(1, NULL, 0, 0), dright);
            ChildLeaf(dright, CopyTree(tree->left), CopyTree(tree->left));
            return Opti(buff);
            break;
        case 17://atan
            buff = CreateLeafBFunc(0, NULL, 3, 0);
            right = CreateLeafBFunc(0, NULL, 0, 0);
            ChildLeaf(buff, Opti(dif(tree->left, str)), right);
            dright = CreateLeafBFunc(0, NULL, 2, 0);
            ChildLeaf(right, CreateLeafNum(1, NULL, 0, 0), dright);
            ChildLeaf(dright, CopyTree(tree->left), CopyTree(tree->left));
            return Opti(buff);
            break;
        case 18://ceil
            return Opti(dif(tree->left, str));
            break;
        case 19://cos
            buff  = CreateLeafUFunc(0, NULL, 0, 1);
            left = CreateLeafBFunc(0, NULL, 2, 0);
            right = CreateLeafUFunc(0, NULL, 0, 9);
            ChildLeaf(left, Opti(dif(tree->left, str)), right);
            ChildLeaf(right, CopyTree(tree->left), NULL);
            ChildLeaf(buff, Opti(left), NULL);
            return Opti(buff);
            break;
        case 20: //cosh
            buff  = CreateLeafBFunc(0, NULL, 2, 0);
            right = CreateLeafUFunc(0, NULL, 0, 10);
            ChildLeaf(buff, Opti(dif(tree->left, str)), right);
            ChildLeaf(right, CopyTree(tree->left), NULL);
            return Opti(buff);
            break;
        }
    }
    return Opti(tree);
}
