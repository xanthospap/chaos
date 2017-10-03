function[x] = qr_solve(A, b)
    [m, n]     = size(A);
    [QR, beta] = householder(A);
    for j=1:n
        u(j) = 1e0;
        u(j+1:m) = A(j+1:m, j);
        b(j:m) = (eye(m-j+1, m-j+1)-beta(j)*u*transpose(u))*b(j:m);
    end
    x = A(1:n, 1:n) \ b(1:n);
