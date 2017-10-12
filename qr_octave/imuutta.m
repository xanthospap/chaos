function[] = imuutta(m,n)
  A=stdnormal_rnd(m,n);
  u=stdnormal_rnd(m,1);
  b=stdnormal_rnd(m,1);
  Acp = A; ucp = u; bcp = b;
  for j=1:n
    u(j) = 1;
    u(j+1:m) = A(j+1:m,j);
    %b(j:m) = (eye(m-j+1,m-j+1)-u(j:m)*transpose(u(j:m)))*b(j:m);
    b(j:m) = b(j:m) - (u(j:m)*transpose(u(j:m)))*b(j:m);
  end
  %% alternative
  for j=1:n
    sum = 0e0;
    ucp(j) = 1e0;
    ucp(j+1:m) = Acp(j+1:m,j);
    sum = bcp(j);
    for i=j+1:m
      % ucp(i) = Acp(i,j);
      sum += Acp(i,j)*bcp(i);
    end
    bcp(j) = bcp(j) - sum;
    for i=j+1:m
      bcp(i) = bcp(i) - sum*Acp(i,j);
      %bcp(i) = bcp(i) - sum*ucp(i);
    end
  end
  b
  bcp