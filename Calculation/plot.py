import uproot
import numpy as np
import matplotlib.pyplot as plt
import os

# set output folder
output_folder = "plots"

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

massK0S = tree["massK0S"].array()
tImpParBach = tree["tImpParBach"].array()
tImpParV0 = tree["tImpParV0"].array()
CtK0S = tree["CtK0S"].array()
cosPAK0S = tree["cosPAK0S"].array()
nSigmapr = tree["nSigmapr"].array()
dcaV0 = tree["dcaV0"].array()

massK0S_np = massK0S.to_numpy()
tImpParBach_np = tImpParBach.to_numpy()
tImpParV0_np = tImpParV0.to_numpy()
CtK0S_np = CtK0S.to_numpy()
cosPAK0S_np = cosPAK0S.to_numpy()
nSigmapr_np = nSigmapr.to_numpy()
dcaV0_np = dcaV0.to_numpy()

# correlation matrixes
XmassK0S = np.vstack((massLc2K0Sp_np, massK0S_np)).T
XtImpParBach = np.vstack((massLc2K0Sp_np, tImpParBach_np)).T
XtImpParV0 = np.vstack((massLc2K0Sp_np, tImpParV0_np)).T
XCtK0S = np.vstack((massLc2K0Sp_np, CtK0S_np)).T
XcosPAK0S = np.vstack((massLc2K0Sp_np, cosPAK0S_np)).T
XnSigmapr = np.vstack((massLc2K0Sp_np, nSigmapr_np)).T
XdcaV0 = np.vstack((massLc2K0Sp_np, dcaV0_np)).T

massK0S_cm = np.corrcoef(XmassK0S, rowvar=False)
tImpParBach_cm = np.corrcoef(XtImpParBach, rowvar=False)
tImpParV0_cm = np.corrcoef(XtImpParV0, rowvar=False)
CtK0S_cm = np.corrcoef(XCtK0S, rowvar=False)
cosPAK0S_cm = np.corrcoef(XcosPAK0S, rowvar=False)
nSigmapr_cm = np.corrcoef(XnSigmapr, rowvar=False)
dcaV0_cm = np.corrcoef(XdcaV0, rowvar=False)

massK0S_names = ['massLc2K0Sp', 'massK0S']
tImpParBach_names = ['massLc2K0Sp', 'tImpParBach']
tImpParV0_names = ['massLc2K0Sp', 'tImpParV0']
CtK0S_names = ['massLc2K0Sp', 'CtK0S']
cosPAK0S_names = ['massLc2K0Sp', 'cosPAK0S']
nSigmapr_names = ['massLc2K0Sp', 'nSigmapr']
dcaV0_names = ['massLc2K0Sp', 'dcaV0']

plt.figure(figsize=(15, 10))
plt.subplot(2, 4, 1)
plt.imshow(massK0S_cm, cmap='coolwarm', aspect='auto')
plt.title('massK0S Correlation Matrix')
plt.xticks(ticks=np.arange(len(massK0S_names)), labels=massK0S_names, rotation=45)
plt.yticks(ticks=np.arange(len(massK0S_names)), labels=massK0S_names)
plt.colorbar()

plt.subplot(2, 4, 2)
plt.imshow(tImpParBach_cm, cmap='coolwarm', aspect='auto')
plt.title('tImpParBach Correlation Matrix')
plt.xticks(ticks=np.arange(len(tImpParBach_names)), labels=tImpParBach_names, rotation=45)
plt.yticks(ticks=np.arange(len(tImpParBach_names)), labels=tImpParBach_names)
plt.colorbar()

plt.subplot(2, 4, 3)
plt.imshow(tImpParV0_cm, cmap='coolwarm', aspect='auto')
plt.title('tImpParV0 Correlation Matrix')
plt.xticks(ticks=np.arange(len(tImpParV0_names)), labels=tImpParV0_names, rotation=45)
plt.yticks(ticks=np.arange(len(tImpParV0_names)), labels=tImpParV0_names)
plt.colorbar()

plt.subplot(2, 4, 4)
plt.imshow(CtK0S_cm, cmap='coolwarm', aspect='auto')
plt.title('CtK0S Correlation Matrix')
plt.xticks(ticks=np.arange(len(CtK0S_names)), labels=CtK0S_names, rotation=45)
plt.yticks(ticks=np.arange(len(CtK0S_names)), labels=CtK0S_names)
plt.colorbar()

plt.subplot(2, 4, 5)
plt.imshow(cosPAK0S_cm, cmap='coolwarm', aspect='auto')
plt.title('cosPAK0S Correlation Matrix')
plt.xticks(ticks=np.arange(len(cosPAK0S_names)), labels=cosPAK0S_names, rotation=45)
plt.yticks(ticks=np.arange(len(cosPAK0S_names)), labels=cosPAK0S_names)
plt.colorbar()

plt.subplot(2, 4, 6)
plt.imshow(nSigmapr_cm, cmap='coolwarm', aspect='auto')
plt.title('nSigmapr Correlation Matrix')
plt.xticks(ticks=np.arange(len(nSigmapr_names)), labels=nSigmapr_names, rotation=45)
plt.yticks(ticks=np.arange(len(nSigmapr_names)), labels=nSigmapr_names)
plt.colorbar()

plt.subplot(2, 4, 7)
plt.imshow(dcaV0_cm, cmap='coolwarm', aspect='auto')
plt.title('dcaV0 Correlation Matrix')
plt.xticks(ticks=np.arange(len(dcaV0_names)), labels=dcaV0_names, rotation=45)
plt.yticks(ticks=np.arange(len(dcaV0_names)), labels=dcaV0_names)
plt.colorbar()

plt.savefig(os.path.join(output_folder, "correlation_matrixes.png"))
plt.tight_layout()

# import probs
probs = np.loadtxt("probs.txt")

# plot probs-massLc2K0Sp
plt.figure(figsize=(15, 10))
plt.plot(probs, massLc2K0Sp, '.', ms=1)
plt.xlabel("probs")
plt.ylabel("massLc2K0Sp")

plt.savefig(os.path.join(output_folder, "probs-massLc2K0Sp_plot.png"))
plt.tight_layout()