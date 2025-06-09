import numpy as np

# Import probs
probs = np.loadtxt("probs.txt")

# Cut Off value
CutOff = 0.546

# Select events with prob above cut off value
i = 0
index = []
len = len(probs)

while i < len:
    if probs[i] < CutOff:
        index.append(i)
    i = i+1

newProbs = np.delete(probs, index)

np.savetxt("NewProbs.txt", newProbs, fmt='%2.3f')