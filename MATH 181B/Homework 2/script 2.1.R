getf = function() {
  # sampleX = rnorm(50, mean=1, sd=2)
  # sampleY = rnorm(50, mean=0, sd=2)
  sampleX = rexp(50, rate=10)
  sampleY = rexp(50, rate=10)
  varX = var(sampleX)
  varY = var(sampleY)
  f = varY/varX
  return(f)
}

fvalues = rep(0, 1000)

for (i in 1:1000) {
  fvalues[i] = getf()
}

hist(fvalues, breaks=30, prob=TRUE, main = "Histogram of F values")
curve(df(x, 49, 49), add=TRUE, col="red")

rejections = rep(0, 1000)

for (i in 1:1000) {
  fvalue = getf()
  if (fvalue <= qf(0.005,49,49) | fvalue >= qf(0.995,49,49)) {
    rejections[i] = 1;
  } 
}

sum(rejections)/1000
# > sum(rejections)/1000
# [1] 0.008
