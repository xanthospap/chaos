function [U,R] = qrstanfrd(A)
    % http://statweb.stanford.edu/~candes/acm106a/Handouts/qr.pdf
    [m, n] = size(A);
    R = A;
    for k = 1:n,
        x = R(k:m,k);
        e = zeros(length(x),1); e(1) = 1;
        u = sign(x(1))*norm(x)*e + x;
        u = u./norm(u);
        R(k:m, k:n) = R(k:m, k:n) -2*u*transpose(u)*R(k:m, k:n);
        U(k:m,k) = u;
    end
