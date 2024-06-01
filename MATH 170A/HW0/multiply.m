function result = multiply(A, x)
    [m, n] = size(A);
    if n ~= length(x)
        error("Wrong dimensions");
    end
    result = zeros(m, 1);

    for i = 1:m
        sum = 0;
        for j = 1:n
            sum = sum + A(i, j)*x(j);
        end
        result(i) = sum;
    end
end