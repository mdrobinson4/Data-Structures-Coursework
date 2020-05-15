# DSA Coursework
 Contains programming assignments from my Data Structures and Algorithms course.
 
### Assignment 0
 Consists of a dynamic array, circular dynamic array, stack, and queue class. The stack class is built upon the dynamic array class, while the queue class is built upon the circular dynamic array class. The language of implementation is Portable C99. Each of the four classes need to be implemented in its own module.
 The classes are generic. Meaning that the type of the value stored in a node object will not be specified by the any of the classes. The user of the array, stack, and queue classes can determine the type of value stored in these data structures.
 
 ### Assignment 1
Implementation of a maze creater and solver algorithm using the previously created stack and queue classes. 
To build the maze, a depth-first traversal of a completely walled-in maze is performed. Where the wall between the current and a randomly-chosen, univisited neighbor is removed. Then it traverses further from the chosen neighbor (and so on) until there are no more unvisited neighbors. At this point, it goes back to the previous spot and trys (and so on).
A breadth-first search is performed to solve the maze
 
 ### Assignment 2
Reads in a corpus of text and stores the words in red-black tree and green tree. A green tree is identical to a regular binary search tree except that it can store duplicates in a more efficient manner. Once the words are added the trees, actions such as adding more words, deleting words, or showing the frequency of a specific word are performed to compare the efficiency of the different implementations.
 
 ### Assignment 3
Implements Kruskal's algorithm for undirected graphs, using a disjoint-set data structure.
