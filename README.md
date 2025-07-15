# **Search Analysis via Wikipedia Graphs**

This application provides an interactive way to explore the shortest paths between two Wikipedia articles using classic graph search algorithms: **Breadth-First Search (BFS)**, **Dijkstra's Algorithm**, and **Bellman-Ford**. The program features a simple and intuitive **GUI** for selecting source and target articles and visualizing their connections.

---

## 🔧 **Setup Instructions**

### 1. **Download Required Binaries**

Due to GitHub's 1 GB file size limitation, the required binary files are hosted externally.

👉 **Download `binaries.zip` from:**
[https://drive.google.com/file/d/1HBnSRNCwfCoaA9rP8cGIjFhIeS1TEznV/view?usp=sharing](https://drive.google.com/file/d/1HBnSRNCwfCoaA9rP8cGIjFhIeS1TEznV/view?usp=sharing)

* Extract the contents into the `bin/` folder of the project directory.
* The `.zip` includes `.bin` files required for the graph database and a `.txt` file listing all valid Wikipedia article titles.

### 2. **Compiling the Project**

* Use **CMake** to configure and build the project.
* Set your **IDE's working directory** to the project root (e.g., `Search-Analysis-via-Wikipedia-Graphs`).
  This ensures the program can correctly locate the relative paths to the binaries.

### 3. **Toolchain Requirements**

* Use the **Visual Studio toolchain** for proper dependency resolution.
* ⚠️ Using **MinGW** may cause build failures due to missing or incompatible dependencies.

---

## ⚠️ **Resource Requirements**

This project uses a **very large dataset** consisting of **163 million rows** (original `.csv` \~8.9 GB). As a result:

* The program may use up to **8 GB of RAM** during execution.
* Make sure your system meets these memory requirements to avoid crashes or performance issues.

---

## ✅ **Features**

* Graph traversal using **BFS**, **Dijkstra's**, or **Bellman-Ford**.
* Easy-to-use graphical interface.
* Real-time pathfinding across a large-scale Wikipedia graph.
* Includes full support for article title validation.