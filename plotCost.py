import matplotlib.pyplot as plt

# Read the data from the file
iterations = []
costs = []

with open('cost.txt', 'r') as f:
    for line in f:
        i, c = map(int, line.strip().split())
        iterations.append(i)
        costs.append(c)

# Plotting
plt.figure(figsize=(8, 5))
plt.plot(iterations, costs, marker='o')
plt.xlabel('Iteration')
plt.ylabel('Cost')
plt.title('Cost over Iterations')
plt.grid(True)
plt.tight_layout()
plt.savefig("cost_plot.png")  # Save the plot as an image
plt.show()
