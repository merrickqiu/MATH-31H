function coefficients = QuadFit(x,y)
n = size(x,2);
coefficients = zeros(n-1,3);

prevDeriv = 0;
for i = 1:n-1
    % Solve the system of equations Ax = b
    % ax_i^2     + bx_i     + c = y_i 
    % ax_{i+1}^2 + bx_{i+1} + c = y_{i+1}
    % 2ax_i      + b            = f'(x_i)
    A = [x(i)^2,  x(i),   1; 
        x(i+1)^2, x(i+1), 1; 
        2*x(i),   1,      0];
    b = [y(i); y(i+1); prevDeriv];
    currCoefficients =  (A\b)';
    coefficients(i, :) = currCoefficients;
    prevDeriv = 2*currCoefficients(1)*x(i+1) + currCoefficients(2);
end

