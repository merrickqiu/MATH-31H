function [Q,R]=mgs(A)
n = size(A,2);                        % number of columns; this formulation
for i=1:n                             % does not need the number of rows
    Q(:,i) = A(:,i);                  % initialization
end
for i=1:n
    R(i,i) = norm(Q(:,i));            % computing R(i,i) 
    Q(:,i)=Q(:,i)/R(i,i);             % making Q(:,i) a unit vector
    for j=(i+1):n
        R(i,j)=(Q(:,i))'*Q(:,j);      % computing R(i,j) by going right on the ith row
        Q(:,j)=Q(:,j)-R(i,j)*Q(:,i);  % updating Q(:,j)
    end
end
