function[u, b] = house(x)
    % x     = transpose(x);
    n     = length(x);
    sigma = transpose(x(2:n))*x(2:n);
    u     = [1 ; x(2:n) ];
    if sigma == 0e0
        b = 0e0;
    else
        mi = sqrt(x(1)*x(1)+sigma);
        if x(1) <= 0e0
            u(1) = x(1) - mi;
        else
            u(1) = -sigma / (x(1)+mi);
        end
        b = 2*u(1)*u(1) / (sigma+u(1)*u(1));
        u = u/u(1);
    end
