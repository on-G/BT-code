import os
import Include.DataPreparation as dp
import numpy as np
from tensorflow import keras

# data file path
folder_name = "Data"
file_name = "dataNew_1_2.root"
file_path = os.path.join(folder_name, file_name)

# Prepare data
dp = dp.DataPreparation(file_path)
dp.load_data()
dp.prepare_data()
X_data = dp.X_data_normalized

# Load model
model_path = os.path.join("..", "NeuralNetwork", "saved_models", "Sequential_NN.keras")

model = keras.models.load_model(model_path, compile=True)

# Run model
probas = model.predict(X_data)

# Save output
np.savetxt("probs.txt", probas, fmt='%2.3f')