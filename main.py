import tkinter as tk
from tkinter import ttk
import subprocess
import os

visual = None
graph_process = None 
def start():

    global visual
    algo = algo_box.get()

    open("status.txt","w").close()
    open("buffer_log.txt","w").close()
    open("rw_status.txt","w").close()

    if algo == "Dining Philosophers":
        subprocess.Popen(["xterm","-hold","-e","./ai"])   
        visual = subprocess.Popen(["python3","viz.py"])


    elif algo == "Producer Consumer":

        subprocess.Popen(["xterm","-e","./pc"])
        visual = subprocess.Popen(["python3","visualizer.py"])


    elif algo == "Reader Writer":

        subprocess.Popen(["xterm","-e","./rw"])
        visual = subprocess.Popen(["python3","rw_viz.py"])


def stop():

    try:
        os.system("pkill dp")
        os.system("pkill pc")
        os.system("pkill rw")

        if visual:
            visual.terminate()

    except:
        pass

def show_graph():
    subprocess.Popen(["python3", "graph.py"])

def stop_graph():
    global graph_process
    try:
        if graph_process:
            graph_process.terminate()
            graph_process.wait()
            graph_process = None
        else:
            os.system("pkill -f graph.py") 
    except:
        pass

root = tk.Tk()
root.title("Process Synchronization Simulator")
root.geometry("340x230")

tk.Label(root,
         text="OS Synchronization Simulator",
         font=("Arial",14)).pack(pady=10)

algo_box = ttk.Combobox(root,
                        values=[
                        "Dining Philosophers",
                        "Producer Consumer",
                        "Reader Writer"],
                        state="readonly")

algo_box.pack(pady=10)
algo_box.current(0)

tk.Button(root,
          text="Start",
          width=15,
          bg="lightgreen",
          command=start).pack(pady=5)

tk.Button(root,
          text="Stop",
          width=15,
          bg="salmon",
          command=stop).pack(pady=5)

tk.Button(root,
          text="Show Graph",
          width=15,
          bg="lightblue",
          command=show_graph).pack(pady=5)

tk.Button(root,
          text="Stop Graph",
          width=15,
          bg="orange",
          command=stop_graph).pack(pady=5)

root.mainloop()