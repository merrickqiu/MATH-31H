#-----PART A-----
setwd("C:/Users/merri/Documents/MATH-31H/MATH 181B/Homework 5")
cars = read.csv("cars.csv")

model = lm(mpg ~ hp, data=cars)
summary(model)

#   Estimate Std. Error t value Pr(>|t|)    
# (Intercept) 30.09886    1.63392  18.421  < 2e-16 ***
#   hp          -0.06823    0.01012  -6.742 1.79e-07 ***

#PLUGGING IN THE ESTIMATES YIELDS THIS REGRESSION LINE:
# mpg = -0.06823*hp + 30.09886

#-----PART B-----
# Multiple R-squared:  0.6024,	Adjusted R-squared:  0.5892

# THE R^2 IS ABOUT 0.60, MEANING 60% OF THE VARIANCE IN THE mpg is explained by the hp.

#-----PART C-----
# R^2 CANNOT BE USED TO DETERMINE THE APPROPRIATENESS OF LINEAR REGRESSION

#-----PART D-----
# R = SQRT(0.6024) = 0.7761 
# THEREFORE THE PREDICTED MPG IS 3*0.7761=2.3283 STD BELOW THE MEAN

#-----PART E-----
predict(model,newdata = data.frame(hp=150),interval='confidence', level=0.9)
# fit        lwr      upr
# 1 19.86462 18.70419 21.02504

# THE 90% CI IS (18.70419, 21.02504)
# IF WE WERE TO REPEAT THIS PROCEDURE MANY TIMES, 
# 90% OF THE CI WILL CONTAIN THE TRUE AVERAGE MPG FOR 150HP.

#-----PART F-----
predict(model,newdata = data.frame(hp=150),interval='prediction', level=0.9)
#  fit      lwr      upr
#1 19.86462 13.20626 26.52297

# THE 90% PI IS (13.20626, 26.52297)
# IF WE WERE TO REPEAT THIS PROCEDURE MANY TIMES, 
# 90% OF THE PI WILL CONTAIN THE TRUE MPG OF A CAR FOR 150HP.