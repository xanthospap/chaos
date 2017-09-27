function[Q] = getq(A, bvec)
    [m, n] = size(A);
    Q      = eye(n);
    u      = zeros(n,1);
    r      = n;
    for j = r:-1:1
        u(j:n) = [1; A(j+1:n, j)];
        u(j:n)
        Q(j:n, j:n) = (eye(n) - bvec(j)*u(j:n)*transpose(u(j:n)))*Q(j:n,j:n);
    end
