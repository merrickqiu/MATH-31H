mu = c(1, 2, 3);
findRejections = function(beta) {
  df = data.frame();
  for (i in 1:3) {
    for (j in 1:3) {
      newRow = c(
        mu[i] + beta[j] + rnorm(1, 0, sqrt(0.4)),
        toString(i),
        toString(j)
      )
      df = rbind(df, newRow);
    }
  }
  colnames(df) = c('score','treatment','block');
  anova = summary(aov(score ~treatment, df));
  rbd = summary(aov(score ~treatment + block, df));

  anovaValue = anova[[1]][["Pr(>F)"]][1];
  rbdValue = rbd[[1]][["Pr(>F)"]][1];
  return(c(anovaValue < 0.05, rbdValue < 0.05))
}

# PART A
rejectionsA = matrix(NA, nrow=1000, ncol=2);
for (i in 1:1000) {
  rejectionsA[i,] = findRejections(c(1,1,1));
}
colSums(rejectionsA)/1000

# > colSums(rejectionsA)/1000
# [1] 0.990 0.945

#PART b
rejectionsB = matrix(NA, nrow=1000, ncol=2);
for (i in 1:1000) {
  rejectionsB[i,] = findRejections(c(1,2,3));
}
colSums(rejectionsB)/1000

# > colSums(rejectionsB)/1000
# [1] 0.058 0.943
