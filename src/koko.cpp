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
        
        inline std::string toString() const
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

    //i1.add_obs( E1::a, E2::b, E3{'i'}, E1::b,E2::c,E3{'x'} );
    //i1.print();

    std::cout <<"\n";
    return 0;
}
