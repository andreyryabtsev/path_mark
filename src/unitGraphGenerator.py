# Import standard python libraries
import argparse
from matplotlib import pyplot as plt
from matplotlib import patches
from matplotlib.collections import PatchCollection
import networkx as nx
import numpy
import seaborn as sns
import math

# Required Colors
obstacleColor = sns.light_palette("red")[1]
collisionEdgeColor = sns.dark_palette("red")[4]
freeEdgeColor = sns.dark_palette("blue")[4]

# Halton Sequence Generator
def halton_sequence_value(index, base):
    
    result = 0
    f = 1

    while index > 0:
        f = f*1.0/base
        result = result + f*(index % base)
        index = index/base
    
    return result

# Wrap the values around 0 and 1.
def scale(coordinate):

    for i in range(numpy.size(coordinate)):
        if coordinate[i] > 1.0:
            coordinate[i] = coordinate[i] - 1.0
        if coordinate[i] < 0:
            coordinate[i] = 1.0 + coordinate[i]

    return (coordinate - 0.5) * math.pi * 2

# Checks validity of a state
def isValid(state, obstacleFile):

    # Saved as x1, y1, x2, y2 bottom-left and top-right corners
    obstacles = numpy.loadtxt(obstacleFile)

    # import IPython
    # IPython.embed()

    for i in range(numpy.shape(obstacles)[0]):
        xBound = obstacles[i][0] < state[0] < obstacles[i][2]
        yBound = obstacles[i][1] < state[1] < obstacles[i][3]

        if xBound and yBound:
            return False

    return True


# Computes binary prior for existence of edge
def computePrior(source, target, obstacleFile):

    stepSize = 0.1 # 9 states are selected in between
    for i in range(11):
        testState = source + i*stepSize*(target - source)
        if not isValid(testState, obstacleFile):
            return 0
    
    return 1

def visualize(graphFile, obstacleFile, saveFile):

    fig = plt.figure()
    ax = plt.gca()

    obs = []
    with open(obstacleFile) as file:
        for line in file:
            data = list(map(float, line.split()))
            rect = patches.Rectangle([data[0], data[1]], data[2]-data[0], data[3]-data[1])
            obs.append(rect)

    pc = PatchCollection(obs, facecolor=obstacleColor, edgecolor='None')
    ax.add_collection(pc)


    G = nx.read_graphml(riskmapFile)
    edges = G.edges()
    priors = nx.get_edge_attributes(G, 'prior')
    states = nx.get_node_attributes(G, 'state')
    for i in range(len(G.edges())):
        edge = edges[i]
        prior = int(priors[edge])
        
        sourceString = str(states[edge[0]])
        source = numpy.fromstring(sourceString, dtype=float, sep=' ')

        targetString = str(states[edge[1]])
        target = numpy.fromstring(targetString, dtype=float, sep=' ')

        if prior == 0:
            plt.plot([source[0], target[0]], [source[1], target[1]], linewidth=1, color=collisionEdgeColor)
        else:
            plt.plot([source[0], target[0]], [source[1], target[1]], linewidth=1, color=freeEdgeColor)

    plt.xlim(0, 1)
    plt.ylim(0, 1)
    plt.tight_layout()

    ax.get_xaxis().set_ticks([])
    ax.get_yaxis().set_ticks([])

    ax.set_aspect('equal', adjustable='box')
    plt.show()
    plt.savefig(saveFile)

def stringify(position, space_dim):
    v = `position[0]`
    for m in range(1, space_dim):
        v += ' ' + `position[m]`
    return v

# Halton Graph Generator
def euclidean_halton_graph(n, radius, bases, offset, space_dim, priors, obstacleFile):

    G = nx.Graph()

    position = {i-1 : scale(numpy.array([halton_sequence_value(i,base) for base in bases]) + offset) for i in range(1, n+1)}

    state = {i: stringify(position[i], space_dim) for i in position.keys()}

    for i in range(n):
        node_id = i
        G.add_node(node_id, state = state[i])

    for i in range(n-1):     
        for j in range(i+1,n):
            if numpy.linalg.norm(position[i]-position[j]) < radius:
                if priors:
                    pFree = computePrior(position[i], position[j], obstacleFile)
                    G.add_edge(i, j, prior = str(pFree))
                else:
                    G.add_edge(i, j)
    return G

# Main Function
if __name__ == "__main__":


    parser = argparse.ArgumentParser(description='script for generating graphml files')
    
    parser.add_argument('-d', '--dimension', type=int, default=2,
                        help='Dimension of the planning space')

    parser.add_argument('-n', '--vertices', type=int, default=100,
                        help='The number of vertices in the graph')

    parser.add_argument('-r', '--radius', type=float, default=-1.0,
                        help='Radius of connection in the graph')

    parser.add_argument('-f', '--graphFile', type=str, default='graph',
                        help='Name of the graphml file')

    parser.add_argument('-k', '--reps', type=int, default=1, 
                        help='Number of graphs to generate')

    parser.add_argument('-p', '--prior', action='store_true', 
                        help='To save edge priors in the graphml')

    parser.add_argument('-o', '--obstacleFile', type=str, default='', 
                        help='Path to the obstacle file. Must be specified if priors are required')

    parser.add_argument('-v', '--visualize', action='store_true',
                        help='Visualize the graph and the world. Must be true if priors is set to true')  

    args = parser.parse_args()

    space_dim = args.dimension

    halton_points = args.vertices
    disc_radius = args.radius

    default_graph_location = '../resources/out/'

    default_obstacle_location = '/home/andrey/workspaces/path_mark/resources/out/'
    obstacleFile = default_obstacle_location + args.obstacleFile

    savePrior = args.prior

    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197]
    bases = primes[0:space_dim]

    # Settings
    if disc_radius < 0:
        disc_radius = 2*halton_points**(-1.0/space_dim) * (2 * math.pi)

    for i in range(0, args.reps):
        print "[unitGraphGenerator]: Generating graph " + `i`
        numpy.random.seed()
        offset = numpy.random.random_sample(space_dim,)

        riskmapFile = default_graph_location + args.graphFile + '_graph_' + `i` + '.graphml'

        # Generate the graph
        G = euclidean_halton_graph(halton_points, disc_radius, bases, offset, space_dim, savePrior, obstacleFile)
        nx.write_graphml(G, riskmapFile)

        saveFile = default_graph_location + args.graphFile + '_graph_' + `i` + '.pdf'
        if args.visualize:
            visualize(riskmapFile, obstacleFile, saveFile)
