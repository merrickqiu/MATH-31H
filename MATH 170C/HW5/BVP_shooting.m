function [x,t] = BVP_shooting(f,a,b,alpha,beta,h,TOL,MaxIters)
    % Rewrite y'' = f(t,y,y') using x_0 = t,x_1 = y,x_2 = y'
    % x_0' = 1
    % x_1' = x_2
    % x_2' = f(x_0, x_1, x_2)

    % Function phi using RK4
    steps = round((b-a)/h);
    F = @(X) [1, X(3), f(X(1), X(2), X(3))];
    function out = phi(z)
        X = [0, alpha, z];
        for i = 1:steps
            F1 = h*F(X);
            F2 = h*F(X + F1/2);
            F3 = h*F(X + F2/2);
            F4 = h*F(X + F3);
            X = X + (F1 + 2*F2 + 2*F3 + F4)/6;
        end
        out = X(2) - beta;
    end 
    % Initial Guesses for x'
    z1 = 0;
    z2 = 1;
    phi_z1 = phi(z1);
    phi_z2 = phi(z2);
    % Secant Method
    for i = 1:MaxIters
        if phi_z2-phi_z1 == 0
            error("Divide by zero");
        end
        z3 = z2 - phi_z2*(z2-z1)/(phi_z2-phi_z1);
        z1 = z2;
        z2 = z3;
        phi_z1 = phi_z2;
        phi_z2 = phi(z2);
        if abs(phi_z2) < TOL
            break
        end
    end
    % Use RK4 with z2 as initial velocity
    x = zeros(steps+1,1);
    t = linspace(a,b,steps+1)';
    x(1) = alpha;
    X = [0, alpha, z2];
    for i = 2:steps+1
        F1 = h*F(X);
        F2 = h*F(X + F1/2);
        F3 = h*F(X + F2/2);
        F4 = h*F(X + F3);
        X = X + (F1 + 2*F2 + 2*F3 + F4)/6;
        x(i) = X(2);
    end

end

