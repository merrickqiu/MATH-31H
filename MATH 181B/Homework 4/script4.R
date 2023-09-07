getAB = function(x, y) {
  n = length(x)
  aNumerator = n*sum(1/(x*y))  - sum(1/x)*sum(1/y)
  aDenominator = n*sum(1/x^2) - sum(1/x)^2
  a = aNumerator/aDenominator
  
  b = (sum(1/y) - a*sum(1/x))/n
  
  return(list(a=a, b=b))
}

# x = 1:10
# y = x/(2+3*x)
x = 1:3
y = 4:6

ab = getAB(x, y)
ab$a # [1] 2
ab$b # [1] 3

x2 <- 1/x
y2 <- 1/y
model = lm(y2 ~ x2)
coef(model)[1] #intercept = b = 3
coef(model)[2] #slope = a = 2


