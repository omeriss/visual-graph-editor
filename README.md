# visual-graph-editor
this project was made in c++ using the simple graphics library sfml.
in the program you can create graphs and see them visually.
this also lets you try graph algorithems on the graph you created, like: Dijkstra and Min Spanning Tree.
# User Guide
when you enter the program you will see a black screen
* Left click- Create a new node
* Right click- RClick on 2 nodes one after the other to connect them with an edge.
  the starting dis will be the dis in pixels, you can change it with backspace and the digit buttons after you create the edge.
* S- set the node the mouse is on to the start node.
* E- set the node the mouse is on to the end node
* Enter- shows you the shortest path from the start node to the end node using dijkstra.
* Shift + Enter- shows you the shortest path from the start node to every other node step by step using dijkstra (press enter to move to the next step).
* M- shows you the Min Spanning Tree with the node that your mouse is on.
* Shift + M- shows you how the Min Spanning Tree is created using kruskal algorithem step by step (press enter to move to the next step).
