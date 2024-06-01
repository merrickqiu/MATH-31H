function [x,t] = BVP_finitediff(u,v,w,a,b,alpha,beta,h)
    n = round((b-a)/h)-1;
    % Construct tridiagonal matrix, A
    A = zeros(n,n);
    for i=1:n-1
        A(i,i) = 2+h^2*v(a+i*h);           %d_i;
        A(i+1, i) = -1 - h*w(a+(i+1)*h)/2; %a_i
        A(i, i+1) = -1 + h*w(a+i*h)/2;     %c_i
    end
    A(n,n) = 2+h^2*v(n*h);
    % Construct RHS vector, B
    B = zeros(n,1);
    for i=1:n
        B(i) = -h^2*u(a+i*h);
    end
    B(1) = B(1) - (-1 - h*w(a+1*h)/2)*alpha;
    B(n) = B(n) - (-1 - h*w(a)/2)*beta;
    % Return answer
    t = linspace(a,b,n+2)';
    x = [alpha;A\B;beta];
end

