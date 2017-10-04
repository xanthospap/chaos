function[Q] = getq(A, bvec)
    [m, n] = size(A);
    Q      = eye(m, m);
    u      = zeros(m,1);
    r      = n;
    for j=r:-1:1
        u(j:m) = [1; A(j+1:m, j)];
        Q(j:m, j:m) = (eye(m-j+1,m-j+1) - bvec(j)*u(j:m)*transpose(u(j:m)))*Q(j:m,j:m);
    end
