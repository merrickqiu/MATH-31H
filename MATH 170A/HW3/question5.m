function question5()
    q = zeros(50,1);
    for i = 1:50 
        A = randn(500);
        b = randn(500,1);
        x = A\b;
        delta_b = 10^(-1)*randn(500,1);
        x_tilde = A\(b + delta_b);
        rel_error_sol = norm(x_tilde-x,1)/norm(x,1);
        rel_error_b = norm(delta_b,1)/norm(b,1);
        q(i) = rel_error_sol/(cond(A,1)*rel_error_b);
    end
    y = log10(q);
    plot(y);
end

