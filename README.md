# EDA_final

Team Members:  
b11901009, b11901052, b11901129

## 📦 Project Package

Download the `.zip` file from the provided link: https://drive.google.com/file/d/1oNhATUNTjZxUXb2liP4lebKZEjpHqYNC/view?usp=sharing

After unzipping, you will see a folder named `testcase1_0529` containing the `testcase1` folder.

**Directory structure should be organized as follows:**
```
your_directory/
├── EDA_final/ # Cloned from GitHub
└── testcase1/ # Located inside testcase1_0529
```

## 🔧 Setup

Clone the project from GitHub:

```bash
git clone https://github.com/Shu-Chenliu/EDA_final.git
```

Make sure the cloned EDA_final directory and the testcase1 folder are in the same directory level.

## How to Run
In the EDA_final directory, run:
```bash
make
./bin/test
```
After execution, the following output files will be generated:
```
results.txt
cost_all.txt
cost.txt
```
## How to Visualize Results
1. Initial Flip-Flop Placement
```bash
python plot.py
```
Output: plot.png

2. Final MBFF Placement
```bash
python plotResult.py
```
Output: plotResult.png

3. Cost per Iteration
```bash
python plotCost.py
```
Output: cost_plot.png

4. Detailed Cost Components per Iteration
```bash
python plotAllCost.py
```
Output: cost_comparison.png