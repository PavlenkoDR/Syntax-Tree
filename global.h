#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

class priora
{
private:
    int _priority[30];
public:
    priora()
    {
        _priority[0] = 2;
        _priority[1] = 2;
        _priority[2] = 3;
        _priority[3] = 3;
        _priority[4] = 1;
        _priority[5] = 0;
        int i;
        for (i = 6; i <= 29; i++)
            _priority[i] = 4;
    }
    int &operator [](int index){return _priority[index];}
};
#endif // GLOBAL_H_INCLUDED
