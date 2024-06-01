function ydd = SecDeriv(x, y)
    h = x(2) - x(1);
    n = length(x);
    ydd = zeros(n,1);
    
    % forward difference approx of O(h^2)
    ydd(1) = (2*y(1)-5*y(2) + 4*y(3) - y(4))/h^3;
    % backward difference approx of O(h^2)
    ydd(n) = (2*y(n)-5*y(n-1) + 4*y(n-2) - y(n-3))/h^3;
    for i = 2:length(x)-1
        % center difference approx of O(h^2)
        ydd(i) = (y(i+1) - 2*y(i) + y(i-1))/h^2;
    end

