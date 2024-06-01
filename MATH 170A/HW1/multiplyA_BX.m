function result = multiplyA_BX(A, B, x)
    [n, n1] = size(A);
    [n2, n3] = size(B);
    n4 = size(x);
    if n ~= n1 | n ~= n2 | n ~= n3 | n ~= n4
        error("Wrong dimensions");
    end

    Bx = zeros(n, 1);
    for i = 1:n
        for j = 1:n
            Bx(i) = Bx(i) + B(i,j) * x(j);
        end
    end

    result = zeros(n, 1);
    for i = 1:n
        for j = 1:n
            result(i) = result(i) + A(i,j) * Bx(j);
        end
    end
end