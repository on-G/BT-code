# Instructions to run the code

---
- [Instructions to run the code](#instructions-to-run-the-code)
    - [Analysis](#analysis)
    - [Neural network](#neural-network)
    - [Prediction](#prediction)
    - [Fit](#fit)


## Analysis

Move to the folder 'main/Analysis' and run the command:
```bash
python3 analysis.py
```

Once done you will find the correlation matrixes and the histograms of the variables that will be used in the folder `main/Analysis/plots`.

## Neural network
Now move to the folder 'main/NeuralNetwork'. To train the model run the command:
```bash
python3 main.py Neural_Network
```
In `main/NeuralNetwork/evaluation_results` will be stored the roc curve, the confusion matrix and a feature importance plot.

## Prediction

To apply the trained model on real data move to the folder `main/Calculation` and run the command:
```bash
python3 Prediction.py
```

## Fit

To fit the data run the ROOT macro `fitTest.C`. Keep in mind that the cutoff value to be applied to the data is represented by the iMin variable in the argument list of the fitTest function.