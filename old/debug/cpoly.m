function [ ] = myfunction (order)

x   = [-10000:100:10000]; % row-vector of data x values
pts = size(x)(1,2);       % number of data points
c   = rand (1,order+1);   % coefficients of the polynomial; random
