function[QR, beta] = householder(A)
    [m, n] = size(A);
    bvec   = zeros(n,1);
    for j = 1:n
        [u, b] = house(A(j:m, j));
        bvec(j,1) = b;
        A(j:m, j:n) = (eye(m-j+1)-b*u*transpose(u))*A(j:m,j:n);
        if j < m
            A(j+1:m, j) = u(2:m-j+1);
        end
    end
    QR = A;
    beta = bvec;
    % Q = getq(A, bvec);
