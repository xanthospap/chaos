function[x] = qr_solve(A, b)
    [m, n]     = size(A);
    [QR, beta, Q] = householder(A);
    %QR
    u = zeros(m,1);
    for j=1:n
        u(j) = 1e0;
        u(j+1:m) = QR(j+1:m, j);
        b(j:m) = b(j:m)-(beta(j)*u(j:m)*transpose(u(j:m)))*b(j:m);
    end
    %printf("\nNew b vector:");
    %transpose(b)
    %U = triu(QR)(1:n, 1:n);
    for j=n:-1:2
      %printf("b[%1d] = %7.5f / %7.5f",j, b(j), QR(j,j));
      b(j) = b(j)/QR(j,j);
      b(1:j-1) = b(1:j-1) - b(j)*QR(1:j-1,j);
      %b(1:n)
    end
    b(1) = b(1)/QR(1,1);
    x = b(1:n);
    % x = A(1:n, 1:n) \ b(1:n);