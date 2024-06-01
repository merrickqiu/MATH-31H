function x = BackSub(A,b)
    n = size(A,1);
    if (size(A,2) ~= n) || (size(b,1) ~= n) || (size(b,2) ~= 1)
        error('cannot solve')
    end
    if tril(A) ~= zeros(n)
        error('input matrix not upper triangular')
    end

    x = b;
    for i = n:-1:1
        for j = (i+1):n
            x(i) = x(i) - A(i,j)*x(j);
        end
        if (A(i,i) == 0)
            error('input matrix not invertible')
        end
        x(i) = x(i)/A(i,i);
    end
end

