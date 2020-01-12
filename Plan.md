To do list

1. <<深入理解Nginx>> 每周2章 --> 毕业前阅读完毕

2. 学习shell脚本写法

3. 系统学习Go和Lua

4. 构建系统的vim配置环境

5. 学习Openresty



**Special Lecture Report**

Date: 19th Dec. 2019

Lecturer: Prof. John Wang from Griffith University  

Lecture theme: Subgraph Isomorphism Search (SIS): An Overview and the State-of-the-Art Tree Search Algorithms

**Summary:**

Prof. Wang kindly began his talk with an introduction of the beautiful Gold Coast in Australia where his research group of Griffith University is located. Nowadays, AI becomes increasingly popular, and more and more researchers start to work with it. But from my point of view, the study of traditional algorithms should not be taken lightly, like his research: Subgraph Isomorphism Search. In our daily life, graphs are everywhere, subgraph searching could be applied in many kinds of fields, such as social networks, biological networks, etc. 

The professor first gave us an overview of existing algorithms that used to solve such NP-complete problem: finding isomorphic subgraphs in the graph called G when given a query graph Q. Roughly, there are 3 types of algorithms, including contraint-based methods, backtracking algorithm methods and relational join methods. For the first method, constraint solving approach, it fit a SIS problem into constraint satisfaction (CSP) problem. Therefore, solving SIS problems can be taken as solving CSP problems, like shrinking the domains of all variables to 1. The second algorithm is based on backtracking, that is to match the node in Q one by one in a particular order, then the subgraphs can be found when all nodes in Q are perfectly matched. In each step of backtracing, there should be a set of candidate nodes in the original graph G, which shares the similar definition of the domain in CSP problem mentioned before. Originally, it seems that this is much like a brute-force search algorithm. However, some pruning methods have been proposed to optimize the backtracking. For example, CFL algorithm, proposed in SIGMOD 2016, decomposes the query graph Q into a core, a forest and a series of leaf nodes before matching. Of course, a more advanced algorithm named DAF which is claimed that can be 4-6 orders of magnitude faster than CFL was proposed later. On the other hand, the third method regards the query graph Q as muliple small tables based on edges, and G as a single table. Then, we can join the edges one by one and form subgraphs in the end. But it can be obviously slow in the cases of dense graphs.

Another optimization method for computing speed that worth mentioning is parallel computing. For instance, we can divide the calculation process of each candidate node (when backtracking) into parts and solve them seperately on the distributed system. In VLDB' 2019, Prof. Wang's group proposed their study of fast and robust distributed subgraph enumeration. Moreover, multi-query in SIS can naturally becomes parallel computing.

**Thinking:**

The BoostIso algorithm, proposed by Prof. Wang's research group, aims at boost the speed of SIS by exploring vertex relationships. Actually, it is a kind of pruning way in the process of backtracking mentioned before. The algorithm implements two ways of optimization based on the basic nature of subgraph matching: (1) aggregate nodes that share the same neighbours (2) define the containment of nodes in order to exclude some impossible nodes. According to the second idea, we can follow a particular order to check all candidate nodes: if node a0 contains node a1, check node a0 before node a1 and never check node a1 if node a0 failed to match the corresponding node in the graph Q we are currently focusing on. 

In my opinion, it is possible to extend the second optimization. In addition to defining the containment, we can calculate the superset of each node, that is to say, given a specific node A, we can find a set where each node contains the node A. By doing this, once the current node in Q sucessfully matched a candidate node K but failed in subsequent search processes, we can further try to match others nodes in the superset of node K. Because the current node in Q must match all nodes in this superset. In summary, defining containment is to exclude impossible searching paths, while defining superset of each node is aiming at finding candidate nodes faster. It is just a preliminary idea based on observation and thinking.


