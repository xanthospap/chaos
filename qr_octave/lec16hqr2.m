function [A,tau] = lec16hqr2(A)
    % Compute the QR decomposition of an m-by-n matrix A using
    % Householder transformations, re-using the storage of A
    % for the Q and R factors.
    % Reference http://www.cs.cornell.edu/~bindel/class/cs6210-f12/notes/lec16.pdf

    [m,n] = size(A);
    tau = zeros(n,1);
    for j = 1:n
        % -- Find H = I-tau*w*w’ to put zeros below A(j,j)
        normx        = norm(A(j:end,j));
        s            = -sign(A(j,j));
        u1           = A(j,j) - s*normx;
        w            = A(j:end,j)/u1;
        w(1)         = 1;
        A(j+1:end,j) = w(2:end);       % Save trailing part of w
        A(j,j)       = s*normx;        % Diagonal element of R
        tau(j)       = -s*u1/normx;
        % -- R := HR
        A(j:end,j+1:end) = A(j:end,j+1:end)-...
        (tau(j)*w)*(transpose(w)*A(j:end,j+1:end));
    end
