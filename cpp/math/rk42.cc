#include <cstdio>

/* d = A+c*B */
void adotcb(const double a[2], double c, const double b[2], double d[2]) {
  d[0] = a[0] + c * b[0];
  d[1] = a[1] + c * b[1];
  // d[2] = a[2]+c*b[2];
}

void derivs(const double x0[2], const double h, double dx[2]) {
  dx[0] = h * (1.71 * x0[1]);
  dx[1] = h * (13.1 * x0[1] - 13.9 * x0[1] * x0[1]);
}

void rk4(double x0, double x, double h, const double y0[2], double yn[2]) {
  double xn = x0;
  for (int i = 0; i < 2; i++)
    yn[i] = y0[i];
  double k1[2], k2[2], k3[2], k4[2], ynFac[2];
  while (xn <= x) {
    printf("\n%3.1f %+8.5f %+8.5f", xn, yn[1], yn[0]);
    // k1 = h*f(xn, Y(xn))
    derivs(yn, h, k1);
    printf(" %+8.5f %+8.5f", k1[1], k1[0]);
    // k2 = h*f(x_n+1/2h, Y(xn)+1/2k1)
    adotcb(yn, 1 / 2e0, k1, ynFac);
    derivs(ynFac, h, k2);
    printf(" %+8.5f %+8.5f", k2[1], k2[0]);
    // k3 = h*f(x_n+1/2h, Y(xn)+1/2k2)
    adotcb(yn, 1 / 2e0, k2, ynFac);
    derivs(ynFac, h, k3);
    printf(" %+8.5f %+8.5f", k3[1], k3[0]);
    // k4 = h*f(x_n+h, Y(xn)+k3)
    adotcb(yn, 1e0, k3, ynFac);
    derivs(ynFac, h, k4);
    printf(" %+8.5f %+8.5f", k4[1], k4[0]);
    for (int i = 0; i < 2; i++)
      yn[i] += (1 / 6e0) * (k1[i] + 2e0 * k2[i] + 2e0 * k3[i] + k4[i]);
    xn += h;
  }
}

int main() {
  double x0 = 0e0, x = 1e0, h = 0.1e0;
  double y0[2] = {0e0, 0.03};
  double yn[2];
  rk4(x0, x, h, y0, yn);
  printf("\n%+8.5f %+8.5f", yn[1], yn[0]);
  return 0;
}
