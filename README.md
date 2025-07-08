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

To fit the data you first need to select the events with a probability value above the cut-off, to do so run:
```bash
python3 FitPrep.py
```

Then the actual fit is done by running the ROOT macro `fitTest.C`.