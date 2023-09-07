reject <- function(p) {
  first = rnorm(50, 1, 1);
  second = rnorm(50, 1, 1);
  third = rnorm(50, 1, 1);
  p1 = t.test(first, second, var.equal = T)$p.value;
  p2 = t.test(second, third, var.equal = T)$p.value; 
  p3 = t.test(first, third, var.equal = T)$p.value;
  return (p1<p | p2<p | p3<p);
}

regular_errors = rep(0, 1000);
bonferroni_errors = rep(0, 1000);
for(i in 1:1000) {
  regular_errors[i] = reject(0.05)
  bonferroni_errors[i] = reject(0.05/3)
}
sum(regular_errors)/1000
sum(bonferroni_errors)/1000
# > sum(regular_errors)/1000
# [1] 0.118
# 
# > sum(bonferroni_errors)/1000
# [1] 0.042

# The proportion of type 1 errors is indeed <0.05 for the bonferroni correction,
# but it is more than double 0.05 without the correction.