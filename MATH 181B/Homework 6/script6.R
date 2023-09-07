setwd("C:/Users/merri/Documents/MATH-31H/MATH 181B/Homework 6")
teen = read.csv("Adolescent.csv")$x
adult = read.csv("Adult.csv")$x
middle = read.csv("MiddleAge.csv")$x

People = data.frame(times = c(teen, adult, middle), 
                  Age= c(rep('Teen', length(teen)), rep('Adult', length(adult)), rep('Middle', length(middle))))
summary(aov(times~Age, People))

# > summary(aov(times~Age, People))
#           Df Sum Sq Mean Sq F value   Pr(>F)    
# Age       `2  173.6   86.80   22.46 3.08e-09 ***
# Residuals 147 568.1    3.86                     
# ---
#   Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1

# H0: mu_teen = mu_adult = mu_middle, H1: At least one mu is different
# Assume that Yij are independent, normal, and have the same variation for each j
# Since the p-value of 3.08e-09 is <0.05, we reject the null hypothesis and 
# the mean screen times of all the age groups are not the same.

t.test(teen, adult, var.equal = T) # p-value = 1.667e-05
t.test(teen, middle, var.equal = T) # p-value = 2.448e-09
t.test(adult, middle, var.equal = T) # p-value = 0.04485

# Using the Bonferroni correction we use alpha/n = 0.01667
# Since mean of teen is greater than adult and middle and the p-value is <alpha/n,
# we conclude that teens have more screen time on average than adults and middle.
# Since 0.04485 > alpha/n, we cannot conclude that adults watch more tv than middle.