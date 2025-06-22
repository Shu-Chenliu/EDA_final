import matplotlib.pyplot as plt

# Read the data from the file
iterations = []
costs = []

with open('cost.txt', 'r') as f:
    for idx, line in enumerate(f):
        parts = line.strip().split()
        print(f"Line {idx} raw: {repr(line.strip())}, split: {parts}")
        if len(parts) != 2:
            print(f"❌ Skipping line {idx}, wrong format.")
            continue
        try:
            i, c = map(float, parts)
            iterations.append(i)
            costs.append(c)
        except ValueError as e:
            print(f"❌ Error parsing line {idx}: {e}")



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
