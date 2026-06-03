import tkinter as tk
import threading
import time
import re

log_file = "buffer_log.txt"
buffer = []

WIDTH = 800
HEIGHT = 400

def read_log():
    global buffer

    while True:
        try:
            with open(log_file,"r") as f:
                line = f.readline()

                cells = re.findall(r'\[(.*?)\]', line)

                if cells:
                    buffer = cells

        except:
            pass

        time.sleep(0.5)


def draw():

    canvas.delete("all")

    canvas.create_text(WIDTH/2,40,
                       text="Producer Consumer Visualization",
                       font=("Arial",16))

    # Producer block
    canvas.create_rectangle(50,180,150,240,fill="lightblue")
    canvas.create_text(100,210,text="Producer")

    # Consumer block
    canvas.create_rectangle(WIDTH-150,180,WIDTH-50,240,fill="lightgreen")
    canvas.create_text(WIDTH-100,210,text="Consumer")

    # Buffer title
    canvas.create_text(WIDTH/2,140,text="Buffer",font=("Arial",12))

    if buffer:

        n = len(buffer)

        total_width = WIDTH - 350
        box_size = total_width / n

        start_x = 175
        y = 180

        for i,item in enumerate(buffer):

            x = start_x + i*box_size

            if item.strip() == "IO":
                color = "orange"
            elif item.strip() == "I":
                color = "green"
            elif item.strip() == "O":
                color = "red"
            else:
                color = "white"

            canvas.create_rectangle(x,y,x+box_size-10,y+50,fill=color)

            canvas.create_text(x+(box_size-10)/2,
                               y+25,
                               text=item.strip())

    root.after(400,draw)


root = tk.Tk()
root.title("Producer Consumer Visualization")

canvas = tk.Canvas(root,width=WIDTH,height=HEIGHT,bg="white")
canvas.pack()

threading.Thread(target=read_log,daemon=True).start()

draw()

root.mainloop()