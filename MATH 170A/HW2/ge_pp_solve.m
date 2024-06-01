function x = ge_pp_solve(A,b)
n = size(A,1);

if (size(A,2) ~= n) || (size(b,1) ~= n) || (size(b,2) ~= 1)
    error('cannot solve this system')
end 

for i=1:n
    % Swap rows
    [pivot, i_star] = max(abs(A(i:n,i)));
    i_star = i_star + i - 1;
    if pivot == 0
        error('cannot do GE')
    end

    if i_star ~= i
        % Swap for A
        tempA = A(i, :);
        A(i,:) = A(i_star, :);
        A(i_star,:) = tempA;
        % Swap for B
        tempB = b(i);
        b(i) = b(i_star);
        b(i_star) = tempB;
    end

    % Row Operation
    for j=(i+1):n
        l = A(j,i)/A(i,i);
        A(j,i) = 0;
        for k = (i+1):n
            A(j,k) = A(j,k) - l*A(i,k);
        end
        b(j) = b(j) - l*b(i);
    end
end

U = A;

x = BackSub(U,b);
end

