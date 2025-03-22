#include <iostream>
#include "kvector.h"

int main()
{
    KVector<int> vt;

    for (int i = 0; i < vt.getCap(); ++i)
    {
        vt.emplace_back(i + 1);
    }

    return 0;
}