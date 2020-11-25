function [LU] = lu_gauss(A)
  Acp = A;
  [n,m] = size(A);
  assert(n == m);
  for k = 1:n-1
    rows = k+1:n;
    A(rows,k) = A(rows,k)/A(k,k);
    A(rows,rows) = A(rows,rows)-A(rows,k)*A(k,rows);
  end
  LU = A;
  
  for k = 1:n-1
    rows = k+1:n;
    Acp(rows,k) = Acp(rows,k)/Acp(k,k);
    for c=k+1:n
      for r=k+1:n
        Acp(r,c) = Acp(r,c) - Acp(r,k)*Acp(k,r);
      end
    end
    Acp
  end