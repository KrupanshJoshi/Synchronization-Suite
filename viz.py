import tkinter as tk
import math
import time
import threading
import os

RADIUS = 200
CENTER = 250

states = []
N = 0
deadlock = False   # GUI freeze flag

def detect_processes():
    global N, states

    while True:
        if os.path.exists("status.txt"):
            with open("status.txt","r") as f:
                lines = f.readlines()

            max_pid = -1

            for line in lines:
                parts = line.strip().split()
                if len(parts) >= 3 and parts[0] == "Process":
                    pid = int(parts[1])
                    if pid > max_pid:
                        max_pid = pid

            if max_pid >= 0:
                N = max_pid + 1
                states = ["WAITING"] * N

        time.sleep(1)

def read_status():
    global states, deadlock

    while True:
        if os.path.exists("status.txt") and N > 0:
            with open("status.txt", "r") as f:
                lines = f.readlines()

            # reset states each time
            new_states = ["WAITING"] * N

            for line in lines:
                if "DEADLOCK OCCURRED" in line:
                    deadlock = True

                parts = line.strip().split()
                if len(parts) >= 3 and parts[0] == "Process":
                    pid = int(parts[1])
                    state = parts[2]

                    if pid < N:
                        new_states[pid] = state   # 🔥 always keep latest

            states = new_states   # 🔥 update once

        time.sleep(0.5)
        
def draw():
    canvas.delete("all")

    if N == 0:
        canvas.create_text(250,250,text="Waiting for C program...",font=("Arial",16))
        root.after(1000, draw)
        return

    canvas.create_oval(150,150,350,350,fill="steelblue")

    for i in range(N):
        angle = 2*math.pi*i/N
        x = CENTER + RADIUS*math.cos(angle)
        y = CENTER + RADIUS*math.sin(angle)

        if states[i] == "USING":
            color = "green"
        elif states[i] == "DEADLOCK":
            color = "red"
        else:
            color = "orange"

        canvas.create_oval(x-40,y-40,x+40,y+40,fill=color)
        canvas.create_text(x,y,text=f"P{i}\n{states[i]}")

    # STOP GUI when deadlock occurs
    if not deadlock:
        root.after(1000, draw)
    else:
        canvas.create_text(250,30,text="SYSTEM HALTED (DEADLOCK)",fill="red",font=("Arial",16))

root = tk.Tk()
root.title("Deadlock Halt Visualization")

canvas = tk.Canvas(root,width=500,height=500,bg="white")
canvas.pack()

threading.Thread(target=detect_processes,daemon=True).start()
threading.Thread(target=read_status,daemon=True).start()

draw()
root.mainloop()