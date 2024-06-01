function result = multiplyAB(A, B)
    [m, n] = size(A);
    [n2, p] = size(B);
    if n ~= n2
        error("Wrong dimensions");
    end
    result = zeros(m, p);

    for i = 1:m
        for j = 1:p
            sum = 0;
            for k = 1:n
                sum = sum + A(i, k)*B(k, j);
            end
            result(i, j) = sum;
        end
    end
end