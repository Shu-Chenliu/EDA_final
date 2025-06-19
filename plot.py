import matplotlib.pyplot as plt
import matplotlib.patches as patches

# Set color
light_purple = '#8e0a74'
purple = '#731f7d'
dark_purple = '#4d1354'
light_blue = '#609ebc'
blue = '#5b6b9e'

# Load
f = open('testcase1/testcase1.txt')
input = []

for line in f.readlines():
  input.append(list(map(float, line.split())))

f.close


# Plot
ax = plt.subplot()

## Board: x, y, w, h
board = patches.Rectangle((0, 0), input[0][0], input[0][1], edgecolor=dark_purple, facecolor='none', linewidth=0.3)
ax.add_patch(board)

## Bin
xShift, yShift = input[1][0], input[1][1]
rows, cols = int(input[1][2]), int(input[1][3])
rowSpan, colSpan = input[1][4], input[1][5]

# for x in range(cols + 1):
#   X = xShift + x*colSpan
#   ax.plot([X, X], [yShift, yShift+rows*rowSpan], color='gray', linewidth=0.1)

# for y in range(rows + 1):
#   Y = yShift + y*rowSpan
#   ax.plot([xShift, xShift+cols*colSpan], [Y, Y], color='gray', linewidth=0.1)

## Pin
### inPins
inPins = int(input[2][0])
ip = []
for i in range(inPins):
  ip.append(patches.Circle((input[3+i][0], input[3+i][1]), radius=1, color=light_purple))
  ax.add_patch(ip[i])

### outPins
outPins = int(input[3+inPins][0])
op = []
for i in range(inPins):
  op.append(patches.Circle((input[3+inPins+1+i][0], input[3+inPins+1+i][1]), radius=1, color=light_blue))
  ax.add_patch(op[i])

## Cell
cells = int(input[3+inPins+1+outPins][0])
c = []
for i in range(cells):
  c.append(patches.Circle((input[3+inPins+1+outPins+1+i][0], input[3+inPins+1+outPins+1+i][1]), radius=1, color=purple))
  ax.add_patch(c[i])

print("...output -ing")

ax.set_xlim(-0.5, input[0][0]+0.5)
ax.set_ylim(-0.5, input[0][1]+0.5)
ax.set_aspect('equal')

plt.axis('off')
plt.savefig("plotInitial.png", dpi=500)

print("fin~~")
