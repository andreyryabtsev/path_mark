# Import standard python libraries
import argparse
# from matplotlib import pyplot as plt
# from matplotlib import patches
# from matplotlib.collections import PatchCollection
import networkx as nx
import math
import random
# import numpy
# import seaborn as sns
LINK_LENGTH = 0.15
MIN_SEPARATION = 0.15

def getCoords(s):
    return list(map(float, s.split()))

def getDistance(a, b):
    sum = 0.0
    for i in range(len(a)):
        sum += (b[i] - a[i]) ** 2
    return math.sqrt(sum)

def ccw(Ax, Ay, Bx, By, Cx, Cy):
    return (Cy-Ay) * (Bx-Ax) > (By-Ay) * (Cx-Ax)

def linesCollide(line1, line2):
    return ccw(line1['x1'], line1['y1'], line2['x1'], line2['y1'], line2['x2'], line2['y2']) != \
        ccw(line1['x2'], line1['y2'], line2['x1'], line2['y1'], line2['x2'], line2['y2']) and \
        ccw(line1['x1'], line1['y1'], line1['x2'], line1['y2'], line2['x1'], line2['y1']) != \
        ccw(line1['x1'], line1['y1'], line1['x2'], line1['y2'], line2['x2'], line2['y2'])

def rectCollidesWithLine(rect, line):
    return linesCollide({'x1': rect['x1'], 'y1': rect['y1'], 'x2': rect['x2'], 'y2': rect['y1']}, line) or \
        linesCollide({'x1': rect['x1'], 'y1': rect['y1'], 'x2': rect['x1'], 'y2': rect['y2']}, line) or \
        linesCollide({'x1': rect['x1'], 'y1': rect['y2'], 'x2': rect['x2'], 'y2': rect['y2']}, line) or \
        linesCollide({'x1': rect['x2'], 'y1': rect['y1'], 'x2': rect['x2'], 'y2': rect['y2']}, line)

def rectsCollide(rect1, rect2):
    return not (rect1['x2'] < rect2['x1'] or rect1['x1'] > rect2['x2'] or rect1['y2'] < rect2['y1'] or rect1['y1'] > rect2['y2'])

def armCollides(obstacle, pos):
    curX = 0.5
    curY = 0.5
    curTheta = pos[0]
    for i in range(len(pos)):
        link = {'x1': curX, 'y1': curY, 'x2': curX + LINK_LENGTH * math.cos(curTheta), 'y2': curY + LINK_LENGTH * math.sin(curTheta)}
        if (rectCollidesWithLine(obstacle, link)):
            return True
        if i < len(pos) - 1:
            curX = link['x2']
            curY = link['y2']
            curTheta += pos[i + 1]
    return False


def appendObstacle(obstacles, pos1, pos2):
    obs = {'x1': 0, 'y1': 0, 'x2': 0, 'y2':0}
    while (True):
        w = random.uniform(0.05, 0.3)
        h = random.uniform(0.05, 0.3)
        x = random.uniform(0, 1 - w)
        y = random.uniform(0, 1 - h)
        obs = {'x1': x, 'y1': y, 'x2': x + w, 'y2': y + h}
        #verify this obstacle is well-separated, else try again
        if (armCollides(obs, pos1) or armCollides(obs, pos2)):
            continue
        # rectangular box expansion
        bigObs = {'x1': obs['x1'] - MIN_SEPARATION, 'x2': obs['x2'] + MIN_SEPARATION, 'y1': obs['y1'] - MIN_SEPARATION, 'y2': obs['y2'] + MIN_SEPARATION}
        if any(rectsCollide(bigObs, oldObs) for oldObs in obstacles):
            continue
        obstacles.append(obs)
        return


def randomizeStartAndTarget(graph_file, d, scale = False):
    graph = nx.read_graphml(graph_file)
    n = graph.number_of_nodes()
    # want to get two points that are 0.35 (arbitrary) * sqrt(d) away from each other in n-d for start and end
    min_d = 0.35 * math.sqrt(d)
    i1 = random.randint(0, n)
    i2 = i1
    n1 = getCoords(graph.nodes[str(i1)]["state"])
    n2 = n1
    while (getDistance(n1, n2) < min_d):
        i2 = random.randint(0, n)
        n2 = getCoords(graph.nodes[str(i2)]["state"])
    if scale:
        for i in range(d):
            n1[i] = (n1[i] - 0.5) * 2 * math.pi
            n2[i] = (n2[i] - 0.5) * 2 * math.pi
    return i1, i2, n1, n2

# Main Function
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='script for generating worlds files')
    
    parser.add_argument('-d', '--dimension', type=int, default=2,
                        help='Dimension of the planning space')

    parser.add_argument('-o', '--out', type=str, default='world_out.world',
                        help='Name of the graphml file')

    parser.add_argument('-f', '--graphFile', type=str, default='NONE',
                        help='Name of the graphml file')

    parser.add_argument('-n', '--count', type=int, default=3, 
                        help='Number of obstacles to generate')

    parser.add_argument('-s', '--separation', type=float, default=0.15, 
                        help='minimum spacing')


    args = parser.parse_args()

    MIN_SEPARATION = args.separation
    n = args.count
    out_location = '../resources/out/' + args.out

    # first come up with start and target
    start, target, startCoords, targetCoords = randomizeStartAndTarget(args.graphFile, args.dimension)
    obstacles = []
    for i in range(n):
        appendObstacle(obstacles, startCoords, targetCoords)

    with open(out_location, 'w') as f:
        f.write("%s %d\n" % (args.graphFile, args.dimension))
        f.write("%lf" % startCoords[0])
        for i in range(1, args.dimension):
            f.write(" %lf" % startCoords[i])
        f.write("\n")
        f.write("%lf" % targetCoords[0])
        for i in range(1, args.dimension):
            f.write(" %lf" % targetCoords[i])
        f.write("\n")
        f.write("%s\n" % n)
        for o in obstacles:
            f.write("%lf %lf %lf %lf\n" % (o['x1'], o['y1'], o['x2'], o['y2']))
    
