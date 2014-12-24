#include "intext.hpp"

/**
 * @details Given the arrays xa[1..n] and ya[1..n], which tabulate a function (with the 
 *          xa[i]’s in order), and given the array y2a[1..n], which is the output from 
 *          _preparecspline_, and given a value of x, this routine returns a cubic-spline 
 *          interpolated value y.
 *
 * @param[in]  xa  array of size N, containing the x values (\c double). Note that the
 *                 array is assumed to be unit-offset (i.e. xa[1..N]).
 * @param[in]  ya  array of size N, containing the y values (\c double). Note that the
 *                 array is assumed to be unit-offset (i.e. ya[1..N]).
 * @param[in]  y2a array of size N, containing the function's sedond derivative as
 *                 tabulated values (\c double). Note that the array is assumed to 
 *                 be unit-offset (i.e. ya[1..N]).
 * @param[in]  n   number of elements considered from nxa ya and y2a arrays; n<=N (\c int)
 * @param[in]  x   point where the interpolation is requested (\c double)
 * @param[out] y   value of interpolation (result) at given x (\c double)
 * @return         an integer denoting the function's status (\c int), which can be:
 *                 Exit Codes | Status
 *                 -----------|-------------------
 *                         0  | al ok
 *                         1  | memory error
 *                         2  | zero devision
 *
 * @attention Note that the input arrays are assumed to be unit-offset.
 *
 * Reference @cite nrc , Sec. 3.3
 *
 * @see csplineint _preparecspline_
 *
 */
int nmath::_csplineint_ (const double* xa,const double* ya,const double* y2a,
  const int& n,const double& x,double& y)
{

  // various variables
  double h,b,a;

  // We will find the right place in the table by means of
  // bisection. This is optimal if sequential calls to this
  // routine are at random values of x. If sequential calls
  // are in order, and closely spaced, one would do better
  // to store previous values of klo and khi and test if
  // they remain appropriate on the next call.
  // This attribute is controlled by the _SEQUENTIAL_CSPLINE_ definition

  static int klo = 1;
  static int khi = n;
  static int k;
  #ifdef _SEQUENTIAL_CSPLINE_
  if (!(xa[khi]<=x && xa[klo]>x)) {
  #endif
    while (khi-klo>1) {
      k = (khi+klo) >> 1;
      if (xa[k]>x) khi = k;
      else klo = k;
    }
  #ifdef _SEQUENTIAL_CSPLINE_
  }
  #endif

  // klo and khi now bracket the input value of x
  h = xa[khi] - xa[klo];

  if (h==0.0) return 2;

  a = (xa[khi]-x)/h;
  b = (x-xa[klo])/h;

  y=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)
    *y2a[khi])*(h*h)/6.0e0;

  // all done; return
  return 0;
}

/**
 * @details Given arrays x[1..n] and y[1..n] containing a tabulated function, 
 *          i.e., y[i] = f (x[i]), with x1 < x2 < . . . < xN, and given
 *          values yp1 and ypn for the first derivative of the interpolating
 *          function at points 1 and n , respectively, this routine returns
 *          an array y2[1..n] that contains the second derivatives of the
 *          interpolating function at the tabulated points x[i]. If yp1 and/or
 *          ypn are equal to 1e30 or larger, the routine is signaled to
 *          set the corresponding boundary condition for a natural spline,
 *          with zero second derivative on that boundary.
 *
 * @param[in]  x   array of size N, containing the x values (\c double). Note that the
 *                 array is assumed to be unit-offset (i.e. xa[1..N]).
 * @param[in]  y   array of size N, containing the y values (\c double). Note that the
 *                 array is assumed to be unit-offset (i.e. ya[1..N]).
 * @param[in]  n   number of elements considered from xa and ya arrays; n<=N (\c int)
 * @param[out] y2  computed array containing the values of the second derivative for
 *                 the given tabulated values (\c double)
 * @param[in]  yp1 (optional) defining value for left boundary condition for the computation of the
 *                 second derivative. If set to 1e30 or larger, the routine is signaled
 *                 to set the corresponding boundary condition for a natural spline,
 *                 with zero second derivative on that boundary (\c double).
 * @param[in]  ypn (optional) defining value for right boundary condition for the computation of the
 *                 second derivative. If set to 1e30 or larger, the routine is signaled
 *                 to set the corresponding boundary condition for a natural spline,
 *                 with zero second derivative on that boundary (\c double).
 * @return         an integer denoting the function's status (\c int), which can be:
 *                 Exit Codes | Status
 *                 -----------|-------------------
 *                         0  | al ok
 *                         1  | memory error
 *
 * @attention 
 * -# Note that the input arrays are assumed to be unit-offset (i.e. x, y).
 * -# The returned array (y2) is y2[1...n]
 * -# The memory for the y2 array,must be already allocated (size n)
 * -# It is important that the program spline is called only once to
 *    process an entire tabulated function in arrays x[i] and y[i].
 *    Once this has been done, values of the interpolated function for
 *    any value of x are obtained by calls (as many as desired) to a
 *    separate routine csplineint (for "spline interpolation")
 * -# For a unique solution, we need to specify two further conditions, typically
 *    taken as boundary conditions at x(1) and x(N). The most common ways of
 *    doing this are either:
 *     - set one or both of the second derivatives at y(1) and y(N) equal to zero,
 *       giving the so-called natural cubic spline, which has zero second 
 *       derivative on one or both of its boundaries, or
 *     - set either of y(1) and y(N) to values calculated from equation for first 
 *       derivative so as to make the first derivative of the interpolating function
 *       have a specified value on either or both boundaries.
 *
 * Reference @cite nrc , Sec. 3.3
 *
 * @see csplineint
 *
 */
int nmath::_preparecspline_ (const double* x,const double* y,
  const int& n,double* y2,double yp1,double ypn)
{

  // various variables
  double p,qn,sig,un;

  // allocate memory for u (unit offset)
  double* u = nmath::_nr_vector_<double> (1,n);
  if (u==nullptr) return 1;

  // The lower boundary condition is set either to be "natural"
  // or else to have a specified first derivative
  if (yp1>0.99e30) {
    y2[1] = u[1] = 0.0e0;
  } else {
    y2[1] = -.5e0;
    u[1] = (3.0/(x[2]-x[1]))*((y[2]-y[1])/(x[2]-x[1])-yp1);
  }

  // This is the decomposition loop of the tridiagonal algorithm
  // y2 and u are used for temporary storage of the decomposed factors
  for (int i=2;i<=n-1;i++) {
    sig   = (x[i]-x[i-1])/(x[i+1]-x[i-1]);
    p     = sig*y2[i-1]+2.0e0;
    y2[i] = (sig-1.0e0)/p;
    u[i]  = (y[i+1]-y[i])/(x[i+1]-x[i]) 
          - (y[i]-y[i-1])/(x[i]-x[i-1]);
    u[i]  = (6.0e0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
  }

  // The upper boundary condition is set either to be "natural"
  // or else to have a specified first derivative
  if (ypn>0.99e30) {
    qn = un = 0.0e0;
  } else {
    qn = .5e0;
    un=(3.0/(x[n]-x[n-1]))*(ypn-(y[n]-y[n-1])/(x[n]-x[n-1]));
  }

  y2[n] = (un-qn*u[n-1])/(qn*y2[n-1]+1.0e0);

  // This is the backsubstitution loop of the tridiagonal algorithm
  for (int k=n-1;k>=1;k--) {
    y2[k] *= y2[k+1];
    y2[k] += u[k];
  }

  // de-allocate u
  nmath::_free_nr_vector_<double> (u,1,n);

  // all done; return
  return 0;
}
