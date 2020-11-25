#include <cstdio>

/*
  example from
  https://www.kau.edu.sa/Files/0060757/Subjects/Numerical%20Solution%20of%20Differential%20Equations.pdf
*/

/* d = A+c*B */
void adotcb(const double a[3], double c, const double b[3], double d[3]) {
  d[0] = a[0] + c * b[0];
  d[1] = a[1] + c * b[1];
  d[2] = a[2] + c * b[2];
}

void derivs(const double x0[3], double h, double dx[3]) {
  const double k1 = 1e0, k2 = 0e0, k3 = 3e0, k4 = 3e0;
  double Ca = x0[0];
  double Cb = x0[1];
  double Cc = x0[2];
  double dCa_dt = -k1 * Ca + k2 * Cb;
  double dCb_dt = k1 * Ca - k2 * Cb - k3 * Cb + k4 * Cc;
  double dCc_dt = k3 * Cb - k4 * Cc;
  dx[0] = dCa_dt * h;
  dx[1] = dCb_dt * h;
  dx[2] = dCc_dt * h;
  return;
}

void rk4(double x0, double x, double h, const double y0[3], double yn[3]) {
  double xn = x0;
  for (int i = 0; i < 3; i++)
    yn[i] = y0[i];
  double k1[3], k2[3], k3[3], k4[3], ynFac[3];
  while (xn <= x) {
    printf("\n%6.2f %+10.6f %+10.6f %+10.6f %+10.6f %+10.6f %+10.6f", xn, y0[0],
           yn[0], y0[1], yn[1], y0[2], yn[2]);
    // k1 = h*f(xn, Y(xn))
    derivs(yn, h, k1);
    // k2 = h*f(x_n+1/2h, Y(xn)+1/2k1)
    adotcb(yn, 1 / 2e0, k1, ynFac);
    derivs(ynFac, h, k2);
    // k3 = h*f(x_n+1/2h, Y(xn)+1/2k2)
    adotcb(yn, 1 / 2e0, k2, ynFac);
    derivs(ynFac, h, k3);
    // k4 = h*f(x_n+h, Y(xn)+k3)
    adotcb(yn, 1e0, k3, ynFac);
    derivs(ynFac, h, k4);
    for (int i = 0; i < 3; i++)
      yn[i] += (1 / 6e0) * (k1[i] + 2e0 * k2[i] + 2e0 * k3[i] + k4[i]);
    xn += h;
  }
}

int main() {
  double x0 = 0e0, x = 0.13e0, h = 0.01e0;
  double y0[3] = {1e0, 0e0, 0e0};
  double yn[3];
  rk4(x0, x, h, y0, yn);
  return 0;
}
