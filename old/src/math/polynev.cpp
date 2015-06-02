#include "intext.hpp"

/**
 * @details Polynomial interpolation using Neville's algorithm. Given arrays xa[1..n] and
 *          ya[1..n] , and given a value x, this routine returns a value y, and an error
 *          estimate dy. If P(x) is the polynomial of degree N − 1 such that 
 *          P(xa[i]) = ya[i], i = 1, . . . , n , then the returned value y = P (x).
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
 * @note Quite often you will want to call polint with the dummy arguments xa
 *       and ya replaced by actual arrays with offsets. For example, the construction
 *       polyneville(&xx[14],&yy[14],4,x,y,dy) performs 4-point interpolation on the tab-
 *       ulated values xx[15..18], yy[15..18].
 *
 * @attention Note that the input arrays are assumed to be unit-offset.
 *
 * Reference @cite nrc , Sec. 3.1
 *
 */
int nmath::_polyneville_ (const double* xa,const double* ya,
  const int& n,const double& x,double& y,double& dy)
{

  // various variables
  double den,dif,dift,ho,hp,w;

  // c and d are vectors of size n (unit offset)
  double* c = nmath::_nr_vector_<double> (1,n);
  double* d = nmath::_nr_vector_<double> (1,n);
  if (c==nullptr || d==nullptr) return 1;

  // find the index ns of the closest entry
  int ns = 1;
  dif = std::fabs (x-xa[1]);
  for (int i=1;i<=n;i++) {
    if ((dift=std::fabs(x-xa[i]))<dif) {
      ns  = i;
      dif = dift;
    }
    c[i] = ya[i];
    d[i] = ya[i];
  }

  // initial estimate
  y = ya[ns--];

  for (int m=1;m<n;m++) {
    for (int i=1;i<=n-m;i++) {
      ho = xa[i]   - x;
      hp = xa[i+m] - x;
      w  = c[i+1] - d[i];
      // guard against zero division
      // This error can occur only if two input xa’s are (to within roundoff) identical.
      if ((den=ho-hp)==0.0) {
        nmath::_free_nr_vector_<double> (d,1,n);
        nmath::_free_nr_vector_<double> (c,1,n);
        return 2;
      }
      den = w/den;
      d[i] = hp * den;
      c[i] = ho * den;
    }
    // note that the last dy added is thus the error indication
    y += (dy=(2*ns<(n-m)? c[ns+1]:d[ns--]));
  }

  // de-allocate c and d
  nmath::_free_nr_vector_<double> (d,1,n);
  nmath::_free_nr_vector_<double> (c,1,n);

  // return ok status
  return 0;
}
