#pragma once
#define STORAGE_CAPACITY 100                // this shows how many items can Storage hold
#define M _T("M")                           // the name of the mutex for Strage
#define CONSOLE_COLOR _T("ConsoleColor")    // the name of the mutex for console color
#define FULL _T("Full")                     // the name of the fullness semaphore
#define EMPTY _T("Empty")                   // the name of the emtiness emaphore
#define PRODCNT 10                          // count of producers
#define CONSCNT 12                          // count of consumers

enum Company
{
    Dell,
    HP,
    Lenovo,
    Asus,
    UnknownCompany
};

struct Product
{
    Company company;
    int year;
    Product(Company c = UnknownCompany, int y = -1) : company(c), year(y) {}
};

class ProdCons
{
private:
    Product Storage[STORAGE_CAPACITY];
    int Index;
public:
    ProdCons();
    Product Porduce();
    void Consume(Product, int);
    void PutIntoStorage(Product, int);
    Product RemoveFromStorage();
};