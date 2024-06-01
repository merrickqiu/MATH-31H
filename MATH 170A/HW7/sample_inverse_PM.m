rng('default')
[Q,R] = qr(randn(6));
D = diag([2, -1, -0.25, 0.5, 2, -5]);
A = Q*D*Q';
B = A - 0.25*eye(6);
C = inv(B);
q = ones(6,1); s = 1;
for j=1:12 
    q_old = q;
    q_new = C*q_old;
    [~, ind] = max(abs(q_new));
    s = q_new(ind(1));
    q = q_new/s;
end
q
A*q
norm(A*q)/norm(q)
norm((A-0.5*eye(6))*q)
