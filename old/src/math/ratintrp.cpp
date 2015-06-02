#include "intext.hpp"

#define TINY 1.0e-25 ///< A small number.

/**
 * @details Rational Function Interpolation and Extrapolation. Given arrays xa[1..n] and
 *          ya[1..n] , and given a value x, this routine returns a value y, and an error
 *          estimate dy. The value returned is that of the diagonal rational function,
 *          evaluated at x , which passes through the n points (xa[i], ya[i]),
 *          i = 1... n.
 *
 * @param[in]  xa array of size N, containing the x values (\c double). Note that the
 *                array is assumed to be unit-offset (i.e. xa[1..N]).
 * @param[in]  ya array of size N, containing the y values (\c double). Note that the
 *                array is assumed to be unit-offset (i.e. ya[1..N]).
 * @param[in]  n  number of elements considered from nxa and ya arrays; n<=N (\c int)
 * @param[in]  x  point where the interpolation is requested (\c double)
 * @param[out] y  value of interpolation (result) at given x (\c double)
 * @param[out] dy error estimate for interpolated value (\c double)
 * @return        an integer denoting the function's status (\c int), which can be:
 *                 Exit Codes | Status
 *                 -----------|-------------------
 *                         0  | al ok
 *                         1  | memory error
 *                         2  | zero devision
 *
 * @note
 * -# Rational functions are sometimes superior to polynomials, roughly 
 * speaking, because of their ability to model functions with poles
 * -# This algorithm can fail if the interpolating function has a pole at the 
 * requested value of x.
 *
 * @attention Note that the input arrays are assumed to be unit-offset.
 *
 * Reference @cite nrc , Sec. 3.2
 *
 */
int nmath::_rationalinterp_ (const double* xa,const double* ya,
  const int& n,const double& x,double& y,double& dy)
{

  // various variables
  double w,t,hh,h,dd;

  // c and d are of size n (unit offset)
  double* c = nmath::_nr_vector_<double> (1,n);
  double* d = nmath::_nr_vector_<double> (1,n);
  if (c==nullptr || d==nullptr) return 1;

  hh = std::fabs (x-xa[1]);

  int ns = 1;
  for (int i=1;i<=n;i++) {
    h = std::fabs (x-xa[i]);
    if (h==0.0) {
      y = ya[i];
      dy = 0.0e0;
      nmath::_free_nr_vector_<double> (d,1,n);
      nmath::_free_nr_vector_<double> (c,1,n);
      return 0;
    } else if (h<hh) {
      ns = i;
      hh = h;
    }
    c[i] = ya[i];
    // The TINY part is needed to prevent a rare zero-over-zero condition
    d[i] = ya[i]+TINY;
  }

  y = ya[ns--];
  for (int m=1;m<n;m++) {
    for (int i=1;i<=n-m;i++) {
      w = c[i+1]-d[i];
      h = xa[i+m]-x;
      t = (xa[i]-x)*d[i]/h;
      dd= t-c[i+1];
      if (dd==0.0) {
        nmath::_free_nr_vector_<double> (d,1,n);
        nmath::_free_nr_vector_<double> (c,1,n);
        return 2;
      }
      dd = w/dd;
      d[i] = c[i+1]*dd;
      c[i] = t*dd;
    }
    y += (dy=(2*ns<(n-m) ? c[ns+1] : d[ns--]));
  }

  // de-allocate c and d
  nmath::_free_nr_vector_<double> (d,1,n);
  nmath::_free_nr_vector_<double> (c,1,n);

  // return ok status
  return 0;
}
