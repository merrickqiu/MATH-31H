#get a random value for d
getd = function() {
  probs = c(0.02,0.01,0.97)
  observed = rmultinom(1, 50, probs)
  expected = 20*probs

  D = sum((observed-expected)^2/expected)
}

# create vector of 1000 samples of D
dvalues = rep(0, 1000)
for (i in 1:1000) {
  dvalues[i] = getd()
}

# Plot histogram
hist(dvalues, breaks = 20, freq=FALSE, main="D histogram")
curve(dchisq(x, df = 2), add = TRUE, col = "red", lwd = 2)

#get a random value for d


# create vector of rejections
# rejections = rep(0, 1000)
# for (i in 1:1000) {
#   dvalue = getd()
#   pvalue = pchisq(dvalue, df=2, lower.tail=FALSE)
#   if (pvalue < 0.05) {
#     rejections[i] = 1
#   }
# }
# 
# sum(rejections)/1000
#0.057
