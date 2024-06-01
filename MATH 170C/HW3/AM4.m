function [x,t] = AM4(f,x0,a,b,h,TOL,MaxIters)
[x,t] = RK4(f,x0,a,b,h);
m = size(x,2);
for i = 4:m
    % Fixed point method
    for j = 1:MaxIters 
        k1 = 9/24*f(t(i), x(i));
        k2 = 19/24*f(t(i-1), x(i-1));
        k3 = -5/24*f(t(i-2), x(i-2));
        k4 = 1/24*f(t(i-3), x(i-3));

        prev_x = x(i);
        x(i) = x(i-1) + h*(k1+k2+k3+k4);
        if x(i) - prev_x < TOL
            break
        end
    end    
end
end

