# Import files
setwd("C:/Users/merri/Documents/MATH-31H/MATH 181B/Homework 2")
regular = unlist(read.csv("regular.csv"))
fast = unlist(read.csv("fast.csv"))

# Calculate F value
varX = var(regular)
varY = var(fast)
#H0 assumes varX = varY, and so f is just varY/varX
f = varY/varX 

# Do rejection check by using quantiles for alpha=0.05
# Two sided since H1 says varX != varY
if (f <= qf(0.025, 9, 9) | f >= qf(0.975, 9, 9)) {
  print("Reject, variances are not equal")
} else {
  print("Fail to reject, variances can be assumed to be equal")
}
# Printed "Reject, variances are not equal"
# Therefore we reject the null hypothesis and cannot assume the variances are equal.

# Do a HT on H0: mu_X = mu_Y and H1: mu_X > mu_Y.
# We cannot assume that sigma_x = sigma_y,
# so we use Welch's approximation for this calculation.

# Calculate mean and std
Xbar = mean(regular)
Ybar = mean(fast)
Sx = sd(regular)
Sy = sd(fast)
n = length(regular)
m = length(fast)

# The test statistic is 
Tv = (Xbar - Ybar)/sqrt(Sx^2/n + Sy^2/m)

# v degrees of freedom
v = round((Sx^2/n + Sy^2/m)^2/(Sx^4/n^2/(n-1) + Sy^4/m^2/(m-1)))

# Find P(t_12 > 0.0726)
pt(Tv, v, lower=F)

# Since p = 0.4716732 > 0.05, we fail to reject the null hypothesis and both glues dry at about the same rate

#Part a
var.test(regular, fast)$p.value # 0.01754406 so reject null hypothesis
var.test(regular, fast)$conf.int #1.381679 22.395125 so 1 is not in the interval
t.test(regular, fast, alternative = "greater")$p.value

