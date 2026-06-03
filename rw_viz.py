import tkinter as tk
import threading
import time
import os

reader_states = {}
writer_states = {}

WIDTH = 500
HEIGHT = 350


def read_status():
    global reader_states, writer_states

    while True:
        if os.path.exists("rw_status.txt"):
            try:
                with open("rw_status.txt", "r") as f:
                    lines = f.readlines()

                temp_readers = {}
                temp_writers = {}

                # 🔥 read latest states from bottom
                for line in reversed(lines):
                    parts = line.strip().split()

                    if len(parts) == 3:
                        role = parts[0]
                        pid = int(parts[1])
                        state = parts[2]

                        if role == "Reader" and pid not in temp_readers:
                            temp_readers[pid] = state

                        elif role == "Writer" and pid not in temp_writers:
                            temp_writers[pid] = state

                reader_states = temp_readers
                writer_states = temp_writers

            except:
                pass

        time.sleep(0.1)


def draw():
    canvas.delete("all")

    canvas.create_text(WIDTH/2, 30,
        text="Reader Writer Visualization",
        font=("Arial", 16))

    # 🔥 find active writer
    active_writer = None
    for w in writer_states:
        if writer_states[w] == "WRITING":
            active_writer = w
            break

    # ---------- Readers ----------
    canvas.create_text(100, 60, text="Readers", font=("Arial", 12))

    y = 80
    for r in sorted(reader_states):
        state = reader_states[r]

        if active_writer is not None:
            state = "WAITING"

        color = "green" if state == "READING" else "orange"

        canvas.create_rectangle(40, y, 160, y+40, fill=color)
        canvas.create_text(100, y+20, text=f"R{r}\n{state}")

        y += 60

    # ---------- Writers ----------
    canvas.create_text(400, 60, text="Writers", font=("Arial", 12))

    y = 80
    for w in sorted(writer_states):
        if w == active_writer:
            state = "WRITING"
            color = "red"
        else:
            state = "WAITING"
            color = "orange"

        canvas.create_rectangle(340, y, 460, y+40, fill=color)
        canvas.create_text(400, y+20, text=f"W{w}\n{state}")

        y += 60

    # ---------- Database ----------
    if active_writer is not None:
        db_color = "red"
    elif any(state == "READING" for state in reader_states.values()):
        db_color = "green"
    else:
        db_color = "lightblue"

    canvas.create_rectangle(210, 150, 290, 230, fill=db_color)
    canvas.create_text(250, 190, text="Database")

    root.after(100, draw)


# MAIN
root = tk.Tk()
root.title("Reader Writer Visualization")

canvas = tk.Canvas(root, width=WIDTH, height=HEIGHT, bg="white")
canvas.pack()

threading.Thread(target=read_status, daemon=True).start()

draw()
root.mainloop()