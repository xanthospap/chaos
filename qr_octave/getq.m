function[Q] = getq(A, bvec, r)
    [m, n] = size(A);
    Q      = eye(m, n);
    u      = zeros(m,1);
    for j=r:-1:1
        u(j:m) = [1; A(j+1:m, j)];
        Q(j:m, j:n) = (eye(m-j+1,m-j+1) - bvec(j)*u(j:m)*transpose(u(j:m)))*Q(j:m,j:n);
    end