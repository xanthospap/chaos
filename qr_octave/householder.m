function[B] = householder(A)
    [m, n] = size(A);
    for j = 1:n
        [u, b] = house(A(j:m, j));
        A(j:m, j:n) = (eye(m-j+1)-b*u*transpose(u))*A(j:m,j:n)
    end
