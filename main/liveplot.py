import matplotlib.pyplot as plt
import matplotlib.animation as anim
from matplotlib import style

style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

def animate(i):
    try:
        graph_data = open('datafile.txt', 'r').read()
        lines = graph_data.split('\n')
        xs = []
        ys = []
        for line in lines:
            if len(line) > 1:
                x, y = line.split(',')
                xs.append(float(x))
                ys.append(float(y))

        ax1.clear()
        if xs and ys:
            ax1.set_xlim(min(xs) - 1, max(xs) + 1)
            ax1.set_ylim(min(ys) - 1, max(ys) + 1)
        ax1.plot(xs, ys)
    
    except Exception as e:
        print(f'Error: {e}')

global ani
ani = anim.FuncAnimation(fig, animate, interval=200, save_count=100)

plt.show()

#with open('datafile.txt', 'r') as f:
#    print (f.read())
