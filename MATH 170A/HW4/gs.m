function [Q,R]=gs(A)                  % classical Gram-Schmidt
n = size(A,2);                        % number of columns; this formulation
for i=1:n                             % does not need the number of rows
Q(:,i) = A(:,i);                      % initialization
    for j=1:(i-1)
        R(j,i)=(A(:,i))'*Q(:,j);      % computing R(j,i) by going down the column
        Q(:,i)=Q(:,i)-R(j,i)*Q(:,j);  % updating Q(:,j)
    end
    R(i,i) = norm(Q(:,i));            % computing R(i,i) 
    Q(:,i)=Q(:,i)/R(i,i);             % making Q(:,i) a unit vector
end
