#include <vector>
#include <cassert>
#include <iostream>

// forward decleration
class custom_it;

class A
{
public:
    A() {};
    A(const std::vector<int>& v) : ivec{v} {};

    std::vector<int> vec() const noexcept {return ivec;}
    std::vector<int>& vec() noexcept {return ivec;}
private:
    std::vector<int> ivec;
};

class B
{
public:
    B(A& a) : aref{a} {};
    B(A& a, const std::vector<char> v) : aref{a}, cvec{v} {};
    B(const B& b):aref{b.aref},cvec{b.cvec}{};
    B(B&& b):aref{b.aref},cvec{std::move(b.cvec)}{};
    B& operator=(const B& b)
    {
        if (this!=&b) {
            aref = b.aref;
            cvec = b.cvec;
        }
        return *this;
    }
    B& operator=(B&& b)
    {
        if (this!=&b) {
            aref = b.aref;
            cvec = std::move(b.cvec);
        }
        return *this;
    }

    custom_it begin();
    /*
    {
        custom_it it {*this};
        return it.begin();
    }*/
    custom_it end();
    /*
    {
        custom_it it {*this};
        return it.end();
    }*/
    
    std::vector<char>  vec() const noexcept {return cvec;}
    std::vector<char>& vec() noexcept {return cvec;}
    A a() const noexcept {return aref;}
    A& a() noexcept {return aref;}

private:
    A& aref;
    std::vector<char> cvec;
};

class custom_it
{
public:
    custom_it(B& b) : bref{b} {};
    ~custom_it() {};
    custom_it(const custom_it& other)
        : bref{other.bref}, a_it{other.a_it}, b_it{other.b_it}
    {};
    custom_it(custom_it&& other) // does the same as a copy c'tor
        : bref{other.bref}, a_it{other.a_it}, b_it{other.b_it}
    {};
    custom_it& operator=(const custom_it& other)
    {
        if (this != &other) {
            bref = other.bref;
            a_it = other.a_it;
            b_it = other.b_it;
        }
        return *this;
    }
    custom_it& operator=(custom_it&& other)
    {
        if (this != &other) {
            bref = other.bref;
            a_it = other.a_it;
            b_it = other.b_it;
        }
        return *this;
    }
    custom_it
    begin() const
    {
        custom_it tmp {*this};
        tmp.a_it = bref.a().vec().begin();
        tmp.b_it = bref.vec().begin();
        return tmp;
    }
    custom_it
    end() const
    {
        custom_it tmp {*this};
        tmp.a_it = bref.a().vec().end();
        tmp.b_it = bref.vec().end();
        return tmp;
    }
    custom_it& operator++()
    {
        ++a_it;
        ++b_it;
        return *this;
    }
    custom_it& operator--()
    {
        --a_it;
        --b_it;
        return *this;
    }
    custom_it operator++(int)
    {
        auto tmp {*this};
        this->operator++();
        return tmp;
    }
    custom_it operator+(std::size_t i) const
    {

    }
    bool operator==(const custom_it& i) const noexcept
    { return (&bref == &(i.bref) && (a_it == i.a_it && b_it == i.b_it)); }
    bool operator!=(const custom_it& i) const noexcept
    { return !(this->operator==(i)); }
    std::pair<int, char>
    get() const
    {
        return std::pair<int, char>(*a_it, *b_it);
    }
private:
    B& bref;
    std::vector<int>::iterator  a_it;
    std::vector<char>::iterator b_it;
};

custom_it B::begin()
{
    custom_it it {*this};
    return it.begin();
}
custom_it B::end()
{
    custom_it it {*this};
    return it.end();
}

int main()
{
    std::vector<int>  ivec {0,1,2,3,4,5,6,7,8,9};
    std::vector<char> cvec {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

    assert( ivec.size() == cvec.size() );

    A a{ivec};
    B b{a, cvec};

    std::cout<<"\nIterating in right order:";
    for (auto i = b.begin(); i != b.end(); ++i) {
        auto p = i.get();
        std::cout << "\n[" << p.first << ", " << p.second << "]";
    }
    
    std::cout<<"\nIterating in reverse order:";
    for (auto i = b.end(); i != b.begin(); --i) {
        auto p = i.get();
        std::cout << "\n[" << p.first << ", " << p.second << "]";
    }

    std::cout<<"\n";
    return 0;
}
