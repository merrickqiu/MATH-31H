# Do a HT on H0: mu_X = mu_Y and H1: mu_X > mu_Y.
# We cannot assume that sigma_x = sigma_y,
# so we use Welch's approximation for this calculation.

# Import files
setwd("C:/Users/merri/Documents/MATH-31H/MATH 181B/Homework 1")
phone = unlist(read.csv("Phone.csv"))
noPhone = unlist(read.csv("NoPhone.csv"))

# Calculate mean and std
Xbar = mean(phone)
Ybar = mean(noPhone)
Sx = sd(unlist(phone))
Sy = sd(unlist(noPhone))
n = length(phone)
m = length(noPhone)

# The test statistic is 
Tv = (Xbar - Ybar)/sqrt(Sx^2/n + Sy^2/m)

# v degrees of freedom
v = round((Sx^2/n + Sy^2/m)^2/(Sx^4/n^2/(n-1) + Sy^4/m^2/(m-1)))

# Find P(t_56 > 2.44)
pt(Tv, v, lower=F)

# Since p = 0.008970693 < 0.05, we reject the null hypothesis

# Calculate Confidence interval
diffMean = Xbar - Ybar
ME = qt(0.005, df = v, lower.tail=FALSE)*sqrt(Sx^2/n + Sy^2/m)
cat("(", diffMean - ME, ", ", diffMean + ME, ")")
# ( -2.976387 ,  66.66539 )

# Verify
t.test(phone, noPhone, alternative = "greater")$p.value
t.test(phone, noPhone, conf.level = 0.99)$conf.int
# Output
# 0.008956362
# -2.965777 66.654780



