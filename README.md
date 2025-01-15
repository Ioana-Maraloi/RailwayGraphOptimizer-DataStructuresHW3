# Assignment 3 - Data Structures and Algorithms (DSA)

## Project Description

This assignment tackles problems related to graphs and the implementation of efficient data structures. The context of the problem involves modeling a European railway system from the 19th century.The project uses adjacency lists for graph representation, as required. The assignment is divided into two major requirements, each with a specific goal:
## Requirement 1: Modeling railway track wear and tear
**Objective**: Simulate the wear level of each railway segment after a given number of years, considering interactions between neighboring segments.

Input:

- Number of routes (R), years (K), and acceptable wear level (L).

- List of routes with city names, segments, and initial wear levels.

Output:

- Updated wear levels for each route and indices of preserved routes.
  
Details: Each route is divided into segments, each with an initial wear level.
Wear propagates between segments based on their proximity and doubles annually.
After K years, generate a list of routes worth preserving, based on an acceptable average wear level.

## Requirement 2: Reducing the number of railways
**Objective**: Determine an optimal subnetwork of railways to maximize profitability while preserving at most a specified number of railways.
Profitability is defined by the number of cities for which the shortest path costs remain unchanged.

Input:

- Starting city, maximum railways to retain (K), and total railways (M).

- List of railways with connected cities and distances.

Output:

- Number of railways retained and their details.
  
### Key implementation highlights include:

**Propagation Logic**: For Requirement 1, the wear propagation between segments is handled iteratively. The maximum wear from neighboring segments is used to compute updates, ensuring accuracy in the simulation.

**Graph Traversals**: Breadth-First Search (BFS) and Depth-First Search (DFS) are used for analyzing graph connectivity and updating component states.

**Shortest Path Preservation**: For Requirement 2, a priority queue-based Dijkstra algorithm ensures efficient computation of shortest paths and evaluates the impact of removing railways.

**Memory Management**: Dynamic memory allocation is managed carefully to avoid leaks, and the implementation is tested with Valgrind.
