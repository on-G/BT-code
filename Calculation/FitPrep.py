import numpy as np
import uproot
import os

# data file path
folder_name = "Data"
file_name = "dataNew_1_2.root"
tree_name = "treeList_0_24_0_24_Sgn"
file_path = os.path.join(folder_name, file_name)

# import data
data = uproot.open(file_path)
tree = data[tree_name]

# extract mass
massLc2K0Sp = tree["massLc2K0Sp"].array()
massLc2K0Sp_np = massLc2K0Sp.to_numpy()

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

mass = np.delete(massLc2K0Sp_np, index)

np.savetxt("Mass.txt", mass)