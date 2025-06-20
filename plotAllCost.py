import matplotlib.pyplot as plt

# Initialize lists
iterations = []
cost1 = []
cost2 = []
cost3 = []
cost4 = []

# Read the file
with open('cost_all.txt', 'r') as f:
    for line in f:
        i, c1, c2, c3, c4 = map(float, line.strip().split())
        iterations.append(i)
        cost1.append(c1)
        cost2.append(c2)
        cost3.append(c3)
        cost4.append(c4)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(iterations, cost1, label='Cost 1: TNS estimated cost', marker='o')
plt.plot(iterations, cost2, label='Cost 2: Power estimated cost', marker='s')
plt.plot(iterations, cost3, label='Cost 3: Area estimated cost', marker='^')
plt.plot(iterations, cost4, label='Total weighted estimated cost', marker='x')

plt.xlabel('Iteration')
plt.ylabel('Cost')
plt.title('Comparison of Costs during iterations')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("cost_comparison.png")
plt.show()
