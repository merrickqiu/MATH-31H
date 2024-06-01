% load image
A = imread('street2.jpg');   
% convert to grascale
A = rgb2gray(A); 
% B uses doubles instead of ints
B = double(A);                                  

% print size of B
size(B)               
% print rank of B
r = rank(B)            
% compute SVD of B
[U,S,V] = svd(B);                               

% number of ranks for each approximation
ranks = [1 2 4 8 16 32 64 r];    
% number of ranks to test
l = length(ranks);                              

% loop through each rank
for i = 1:l        
    % get current rank
    k = ranks(i);       
    % use the first k singular values for approximation
    approxB = U(:,1:k)*S(1:k,1:k)*V(:,1:k)';    
    % round to nearest integer
    approxA = uint8(approxB);                   
    
    % make figure window
    figure(1)                 
    % go to the ith place in grid
    subplot(2,4,i)         
    % show the approximated matrix
    imshow(approxA);                         
    % set title
    title(sprintf('rank %d approximation', k))  
end
