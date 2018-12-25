#include "stdafx.h"

ProdCons::ProdCons() : Index(0) {}

Product ProdCons::Porduce()
{
    return Product(Company((rand() + Index) % 4), 2000 + (rand() + Index) % 19);
}

void ProdCons::Consume(Product p, int num, int index)
{

    _tprintf(_T("I am consumer %d. I just bought %s laptop, %d year from %d\n"), num,
        Dell == p.company ? _T("Dell  ") : HP == p.company ? _T("HP    ") :
        Lenovo == p.company ? _T("Lenovo") : Asus == p.company ? _T("Asus  ") : _T("bad   "),
        p.year, index);
}

void ProdCons::PutIntoStorage(Product p, int num)
{
    _tprintf(_T("I am producer %d. I produce %s laptop, %d year into %d\n"), num,
        Dell == p.company ? _T("Dell  ") : HP == p.company ? _T("HP    ") :
        Lenovo == p.company ? _T("Lenovo") : Asus == p.company ? _T("Asus  ") : _T("bad   "),
        p.year, Index);
    Storage[Index++] = p;
}

Product ProdCons::RemoveFromStorage()
{
    Product p = Storage[--Index];
    Storage[Index] = Product();
    return p;
}

int ProdCons::GetIndex() { return Index; }