# Trees

This is a project only for educational purposes in the Moscow Institute of Physics and Technology.

#### Description

The goal of the project is to implement two standard data structures: the AVL tree and the Red-Black tree, then to measure times of insertion, finding and deletion an element. Finally, build graphics and make sure that the asymptotics is logarithmic.

#### Building

To build the project just enter to the project root directory and type:

$ cmake .

$ make

Then you will have a binary file "tree" you can execute.

#### Usage

Execute the binary "tree". Than you will have the timing statistics in the files "out/avl.tsv", "/out/rb.tsv".
To show it in graphs use the python script "graph.py". It will save the graphs in the PNG format in the
directory "out/".
Also you can interactively play with the trees via:

$ ./tree --game avl|rb
