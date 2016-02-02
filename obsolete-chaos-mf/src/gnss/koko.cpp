#include <iostream>
#include <cfenv>
#include <cmath>
#include <vector>

    enum class E1 : char { a, b, c };
    char toChar(E1 e) { 
        if (e==E1::a) return 'a';
        else if (e==E1::b) return 'b';
        else return 'c';
    }

    enum class E2 : char { a, b, c };
    char toChar(E2 e) { 
        if (e==E2::a) return 'a';
        else if (e==E2::b) return 'b';
        else return 'c';
    }

    struct E3 { 
        char c_; 
    };

    struct RawObs { 
        E1 e1_; E2 e2_; char c_;
        
        explicit RawObs(E1 a,E2 b,char c):
            e1_(a),e2_(b),c_(c){ };
        
        std::string toString() const
        {
            std::string str { '[' };
            str += toChar(e1_);
            str += toChar(e2_);
            str += c_;
            str += ']';
            return str;
        }
    };

    class A {
        private:
            std::vector<RawObs> obs_;
        public:
            A (std::initializer_list<RawObs>&& il)
            {
                for (auto i : il) obs_.emplace_back(i);
            }
            
            void addObs(E1 e1,E2 e2,char c)
            {
                obs_.emplace_back( RawObs {e1,e2,c} );
                RawObs r {e1,e2,c};
                std::cout<<"\n::called addObs with RO="<<r.toString();
            }
            template<typename... Args>
            void addObs(E1 e1,E2 e2,char c,Args... args)
            {
                std::cout<<"\n__PRETTY_FUNCTION__";
                RawObs r {e1,e2,c};
                std::cout<<"\n::called Template::addObs RO="<<r.toString();
                obs_.emplace_back( RawObs {e1,e2,c} );
                addObs(args...);
            }
            void print() { 
                std::cout << "\nObs -> ";
                for ( auto& i : obs_ ) std::cout << i.toString();
            }
    };

int main ()
{
/*    
    E3 c { 'x' };
    A a1 { E1::a,E2::b,c };
    a1.print();

    A b1 { E1::a,E2::b,c,E1::b,E2::c,c };
    b1.print();
*/
    A i1 { RawObs{ E1::a,E2::b,'i' }, RawObs{ E1::b,E2::c,'x' } };
    i1.print();

    i1.addObs( E1::a, E2::b, 'i', E1::b, E2::c, 'x', 
               E1::a, E2::b, 'i', E1::a, E2::b, 'l' );
    i1.print();

    std::cout <<"\n";
    return 0;
}
