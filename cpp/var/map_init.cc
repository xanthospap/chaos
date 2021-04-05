#include <map>
#include <cstdio>

const double inar[4] = {0e0, 1e0, 2e0, 3e0};
const char car[4] = {'a', 'b', 'c', 'd'};
double outar[4];

struct inout {
  const double* inar;
  double* outar;
  void print_in() const {
    for (int i=0; i<4; i++) printf("\t%5.1f", inar[i]);
  }
  void print_out() const {
  for (int i=0; i<4; i++) printf("\t%5.1f", outar[i]);
  }
};

typedef std::map<const char, inout> map_def;

void foo(map_def& m, double fac=1e0) {
  for (int i=0; i<4; i++) m['a'].outar[i] = m['a'].inar[i] * fac;
  return;
}

int main() {

  inout io = {inar, outar};
  map_def m = { {car[0], io} };
  foo(m, 2e0);

  printf("\nHere is what we did:");
  printf("\n");
  m['a'].print_in();
  
  printf("\n");
  m['a'].print_out();
  
  foo(m, 5e0);
  printf("\n");
  m['a'].print_out();
  
  printf("\n");
  return 0;
}
