#include <stdio.h>

class A
{
public:
    int* p;
    A()
    {
        p = new int{};
    }

    ~A()
    {
        printf("Deconstructor of A called.\n");
        printf("Value of A.p: %d\n", *p);
        if(p)
        {
            printf("Deleting pointer.\n");
            delete p;
        }
        printf("Value of A.p after delete: %d\n", *p);
        printf("Address of A.p after delete: %p\n", p);
    }
};

class B
{
public:
    int* p;
    ~B()
    {
        printf("Deconstructor of B called.\n");
        printf("Value of B.p: %d\n", *p);
        if(p)
        {
            printf("Deleting pointer.\n");
            delete p;
        }
        printf("Value of B.p after delete: %d\n", *p);
        printf("Address of B.p after delete: %p\n", p);
    }
};

int main()
{
    A a;
    B b;
    printf("%d\n", *a.p);

    b.p = a.p;
    return 0;
}
