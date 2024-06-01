function [x,t] = RK4(f,x0,a,b,h)
    m = round((b-a)/h)+1;
    t = linspace(a,b,m);
    x = zeros(1,m);
    t(1) = a;
    x(1) = x0;
    for i = 2:m
        t_n = t(i-1);
        x_n = x(i-1);

        k1 = f(t_n, x_n);
        k2 = f(t_n+h/2, x_n+h*k1/2);
        k3 = f(t_n+h/2, x_n+h*k2/2);
        k4 = f(t_n+h, x_n+h*k3);
        x(i) = x_n + h/6*(k1 + 2*k2 + 2*k3 + k4);
    end
end

