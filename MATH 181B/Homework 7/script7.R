score = c(5, 6.1, 7.2, 6, 5.9, 7, 6.5, 6.3, 7.3)
data = data.frame(score=score,
                  block = c(rep('Sit', 3), rep('Wait', 3), rep('Drop it',3)),
                  treatment = c(rep(c('Dog 1', 'Dog 2', 'Dog 3'), 3)))
summary(aov(score ~treatment + block, data))

#             Df Sum Sq Mean Sq F value Pr(>F)  
# treatment    2 2.9867  1.4933   8.145 0.0389 *
# block        2 0.5600  0.2800   1.527 0.3215  
# Residuals    4 0.7333  0.1833       
