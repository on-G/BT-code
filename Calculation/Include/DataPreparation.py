import uproot
import numpy                 as np
import pandas                as pd

class DataPreparation:

    def __init__(self, file_path, tree_name = "treeList_0_24_0_24_Sgn"):
        self.file_path = file_path
        self.tree_name = tree_name

    def load_data(self):
        # Open root file
        file = uproot.open(self.file_path)
        
        # Extract data tree
        tree = file[self.tree_name]

        # Load data from tree into Pandas DataFrames (df)
        self.df_data = tree.arrays(library = "pd")

    def prepare_data(self):
        # Select features
        X_data = self.df_data[["massK0S",
                                "tImpParBach",
                                "tImpParV0",
                                "CtK0S",
                                "cosPAK0S",
                                "nSigmapr",
                                "dcaV0"]]
        
        # Normalize data by dividing by the maximum value of each variable
        max_massK0S = X_data["massK0S"].max()
        max_tImpParBach = X_data["tImpParBach"].max()
        max_tImpParV0 = X_data["tImpParV0"].max()
        max_CtK0S = X_data["CtK0S"].max()
        max_cosPAK0S = X_data["cosPAK0S"].max()
        max_nSigmapr = X_data["nSigmapr"].max()
        max_dcaV0 = X_data["dcaV0"].max()

        self.X_data_normalized = pd.DataFrame()

        self.X_data_normalized["massK0S"] = X_data["massK0S"] / max_massK0S
        self.X_data_normalized["tImpParBach"] = X_data["tImpParBach"] / max_tImpParBach
        self.X_data_normalized["tImpParV0"] = X_data["tImpParV0"] / max_tImpParV0
        self.X_data_normalized["CtK0S"] = X_data["CtK0S"] / max_CtK0S
        self.X_data_normalized["cosPAK0S"] = X_data["cosPAK0S"] / max_cosPAK0S
        self.X_data_normalized["nSigmapr"] = X_data["nSigmapr"] / max_nSigmapr
        self.X_data_normalized["dcaV0"] = X_data["dcaV0"] / max_dcaV0