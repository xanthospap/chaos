function[Q] = getq(A, bvec, r)
    [m, n] = size(A);
    Q      = eye(n, n);
    u      = zeros(n,1);
    for j=r:-1:1
        u(j:n) = [1; A(j+1:n, j)];
        Q(j:n, j:n) = (eye(n-j+1,n-j+1) - bvec(j)*u(j:n)*transpose(u(j:n)))*Q(j:n,j:n);
    end