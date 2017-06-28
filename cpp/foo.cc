#include <iostream>
#include <vector>

class A
{
public:
    A(std::size_t n = 1) : v(n) {};
    
    void
    populate(int val = 0) 
    { for (auto it=v.begin(); it!=v.end(); ++it) *it = val++; }

    A(const A& a, std::size_t start=0, std::size_t end=0)
    : v{ a.v.cbegin()+start,
          (!end)
          ?(a.v.cend())
          :(a.v.cbegin()+end)
        }
    {};

    void print() const
    { for (auto it=v.begin(); it!=v.end(); ++it) std::cout<<" "<<*it;
      std::cout<<"\n";}

    std::vector<int> v;
};

int main()
{
    A a(10);
    a.populate();
    a.print();

    A b {a, 1, 5};
    b.print();

    std::vector<int> *pa, *pb;
    pa = &(a.v);
    pb = &(a.v[5]);


    std::cout<<"\n";
    return 0;
}
