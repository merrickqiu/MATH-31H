# Import files
setwd("C:/Users/merri/Documents/MATH-31H/MATH 181B/Homework 3")
x = unlist(read.csv("Question.csv"))

#Estimate MLE for Poisson
lambda_hat = mean(x)

# Put data into bins
bins = c(0,1,2,3,4,5,6,7,Inf)
observed = table(cut(x, bins, right=F))

# Expected probabilities
probabilities = dpois(c(0,1,2,3,4,5,6,Inf), lambda_hat)
probabilities[8] = 1 - ppois(6, lambda_hat)
expected = probabilities*length(x)

#Calculate D
D = sum((observed-expected)^2/expected)

#8-1-1 degrees of freedom
pchisq(D, df=6, lower.tail=FALSE)

#P-value is 0.1231678>0.03
