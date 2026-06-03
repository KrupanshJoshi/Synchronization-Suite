import matplotlib.pyplot as plt
import time

plt.ion()   # interactive mode

while True:
    x = []
    y = []

    try:
        with open("rl_results.txt") as f:
            for line in f:
                parts = line.strip().split()
                if len(parts) == 2:
                    x.append(int(parts[0]))
                    y.append(int(parts[1]))
    except:
        pass

    plt.clf()  # clear previous graph
    plt.plot(x, y, marker='o', color='blue')

    plt.xlabel("Iterations")
    plt.ylabel("Deadlocks")
    plt.title("Deadlock Reduction (Live)")
    plt.grid()

    plt.pause(0.5)  # update every 0.5 sec