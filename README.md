## Bimatrix Generators

A library of bimatrix game generators to complement the GAMUT generator with
instances of games focusing on approximation algorithms. The game classes
implemented are as follows:

- **Colonel Blotto Games**: The players in this game have an army with equal number
  of troops T, which they can then divide and dispatch to n battlefields (hills). Each
  player has a personal valuation for each hill which is received if the
  player has more troops than the opponent on that hill. In case of ties, the
  winner of the hill is choosen uniformly at random.
- **Ranking Games**: In ranking games, each player puts in an effort which comes at
  a cost and results in a score. The values of cost and score are monotone
  increasing in the amount of effort put in. The players are then ranked based
  on their score where the player with the higher score recieves a prize p, and
  in cases where a tie occurs, the players split the prize between them. Each
  players payoff is given by the value of the prize earned minus the cost of the
  effort put in.
- **SGC Games**: These games were created as a worst case scenario for support
  enumeration as it has a unique equilibrium where each player uses half of his
  actions in his support profile.
- **Tournament Games**: Starting from an arbitrary tournament graph with n nodes, an
  asymmetric bipartite graph is created where the nodes on one side (partition
  R) correspond to the nodes of the tournament graph, and the nodes on the other
  (partition C)side correspond to the k-sized subsets of nodes in the tournament 
  graph. The bipartite graph is then transformed into a win-lose game where the 
  actions of each player are nodes on their side of the graph.
- **Unit vector Games**: The payoffs of the column player are selected uniformly at
  random from the range [0, 1], while in the row player has each column of his
  payoff matrix being a unit vector.

### Generating Instances:

- **Global parameters**:

        -f filename: Output the generated instance into the specified file using
        the gamebit game format.
        -g game : Specifies which game to generate
        -r seed : Specify a randomseed for the generator

- **Colonel Blotto Games**:

        $ ./blotto.py -[n:T:r:f:c:]
        -n hills: Indicates the number of hills
        -T troops: Indicates the number of troops
        -c covariance : The covariance value of the multivariate distriubution

- **Ranking Games**:

        $ ./gen -g Ranking -[s:]
        -s m : Generates a ranking game where both players have m effort levels

- **SGC Games**:

        $ ./gen -g SGC -s
        -s m : Generates an instance of the SGC game where both players use
        supports of size m.

- **Tournament Games**:

        $./gen -g Tournament -[s:k:]
        -s n : Sets the number of nodes in the tournament graph to n
        -k k : Sets the size of the subset of nodes to k

- **Unit vector Games**:

        $ ./gen -g Unit -[s:k:]
        -s m : Generates a Unit vector game of size m x m
        -k i : If the argument is 0 then generates a Unit vector game which
        avoids having a pure, otherwise, generates a random unit vector game
