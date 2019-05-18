
import os
import argparse

class Node:
    def __init__(self,x,y,hash):
        self.x = x
        self.y = y
        self.fg = 0.0
        self.fh = 0.0
        self.fc = 0.0
        self.parent = ""
        self.hash = hash
        self.edges = dict()
        self.state = ""  


class Net:
    start = hash
    goal  = hash

    def __init__(self,w,h):
        self.initialized = 0
        self.map_file =""
        self.w = w
        self.h = h
        self.grid = dict()

#--- class NET implementation BEGIN

''' Initialize grid size parameters provided by TXT map
Input: map file
Output: grid size '''
def net_init(self,map_file):
    self.map_file = map_file

    fhn = open(self.map_file,"r")
    fmap = fhn.readlines()
    fhn.close()

    if( 0 == len(fmap) ):
        return Net(0,0)

    linenum=0
    while(linenum < 2):
        line = str(fmap[linenum]).strip()
        if( not line.isdecimal() ):
            print("Provided width is not a digit. Exiting")
            return Net(0,0)

        if(0 == linenum):
            self.h = int(line)
        else:
            self.w = int(line)
            break
        linenum +=1

''' Initialize network grid
Input: grid size
Output: grid '''
def net_create_grid(self):
    for x in range(0,self.w):
        for y in range(0,self.h):
            node = Node(x,y,gen_position_hash(x,y))
                
            if( x > 0 ): # x-1
                node.edges[gen_position_hash(x-1,y)] =1
            if( x < self.w-1 ): # x+1
                node.edges[gen_position_hash(x+1,y)] =1
            if( y > 0 ): # y-1
                node.edges[gen_position_hash(x,y-1)] =1
            if( y < self.h-1 ): # y+1
                node.edges[gen_position_hash(x,y+1)] =1
                
            self.grid[gen_position_hash(x,y)] = node

''' Create nodes network on given grid. Obstacle nodes are removed from network grid
Input: txt map, grid
Output: network '''
def net_create_network(self):

    fhn = open(self.map_file,"r")
    fmap = fhn.readlines()
    fhn.close()

    if( 0 == len(fmap) ):
        print("The Map file is empty. Exiting.")
        return 0

    if( self.h > len(fmap[2:]) ):
        print("Incorrect map. The number of map rows is lower then provided matrix height. Exiting.")
        return 0
    
    gridrow = 0
    for line in (fmap[2:]):
        if( gridrow  == self.h ):
            break

        line = str(line).strip()
        
        if(( (self.w * 2)  - 1 ) > len(line) ):
            print("Incorect map file. Matix width is bigger then map row nodes. Exiting")
            return 0

        gridcolumn = 0
        for x in range(0,len(line)):
            char = str(line)[x]

            if( gridcolumn == self.w ):
                break

            key = gen_position_hash(gridcolumn,gridrow)

            if( 's' == char):
                state = "START"
                self.start = key
            elif( 'g' == char):
                state = "GOAL"
                self.goal = key
            elif( '_' == char ):
                state = "NODE"
            elif( ' ' == char):
                continue
            else:
                self.net_remove_node(key)   ## <---- net = net_remove...
                gridcolumn +=1
                continue

            self.grid[key].state = state
            self.grid[key].x = gridcolumn
            self.grid[key].y = gridrow
            gridcolumn +=1
        
        gridrow +=1

''' Remove provided node from network, also from neighbours connection
Input: node to remove '''
def net_remove_node(self, key2rm):
    keys = self.grid[key2rm].edges.keys()
    for node in keys:
        del self.grid[node].edges[key2rm]

    del self.grid[key2rm]

''' Print general network structure in format; node[id] = [neighbours]
Input: network '''
def net_print(self):
    print("Map size [w,h] =["+str(self.w)+","+str(self.h)+"]")
    for node in self.grid:
        print("Node <"+str(self.grid[node].hash)+">="+ self.grid[node].state ,end='')
        print(self.grid[node].edges)

''' Print general information about network '''
def net_print_info(self):
    print("\nMap file :"+str(self.map_file)+"; ",end='')
    print(" net size ="+str(len(self.grid)))
    print("Path from (x,y) ["+str(self.grid[self.start].x)+","+str(self.grid[self.start].y)+"]--->",end='')
    print("["+str(self.grid[self.goal].x)+","+str(self.grid[self.goal].y)+"]")
    print()


Net.net_create_grid = net_create_grid
Net.net_init = net_init
Net.net_create_network = net_create_network
Net.net_remove_node = net_remove_node
Net.net_print = net_print
Net.net_print_info = net_print_info
#--- class NET implementation END

''' Generate key basd on node position
Input: x,y position of node
Return: custom hash '''
def gen_position_hash(x,y):
    return str(x)+"."+str(y)

''' Find and return node key/id
Input: node name
Output: node hash '''
def net_get_node_id(state,net):
    for node in net.grid:
        if( state == net.grid[node].state ):
            return net.grid[node].hash

    return ""

''' Find path from node A to node B in a recursive way (in backwards)
Input: end node GOAL and destination node START
Output: path between START and GOAL '''
def net_calculate_path(start, goal, net):
    path = []
    
    if( "" == net.grid[goal].parent ):
        return path
    
    node = goal
    while( node != start):
        path.append(node)
        node = net.grid[node].parent
        
    path.append(node)
    return path

def code_get_expands(list_closed, path):
    for node in path[1:len(path)]:
        list_closed.remove(node)

    return list_closed

''' A-star cost function factor 
Input: node 
Output: heuristic value '''
def core_w_factor(node, net):
    w = 0.0

    depth = len(net_calculate_path(net.start, node, net))
    N = len(net.grid)

    e=0.5
    if(N > 300):
        e=0.2
    
    w = float(1 + e - (e* (depth/N) ) )

    return w

''' A-star simple Manhattan 2d grid distance heuristic '''
def core_h_heuristic(node, net):

    h = abs(net.grid[net.goal].x - net.grid[node].x) + abs(net.grid[net.goal].y - net.grid[node].y)

    return h

''' Find the best neighbor from a given list.
Function implements simple tie-break selecting first node
with smallest Cost and next with smallest Heuristic

[!][]Implement an effitient search algorithm for below WHILE loop
    [!][]Implement an effitient sort algorithm for list_open

Input: list to search
Output: the best node '''
def core_next_best(list_open,net):

    list_index = len(list_open) - 1

    winner = net.grid[list_open[list_index]]
    while(list_index > 0):
        list_index -= 1
        node = net.grid[list_open[list_index]]

        if( node.fc < winner.fc ):
            winner = node
        if( node.fc == winner.fc ):
            if( node.fh < node.fh ):
                winner = node
        
    return winner.hash


''' The A-star algorithm '''
def core_get_path(net):
    list_open = []
    list_closed = []

    curr_node = net.start
    list_open.append(curr_node)

    while( curr_node != net.goal ):
        #print("Current Node :"+str(curr_node)+" PARENT="+str(net.grid[curr_node].parent)+", neighbours:"+ str(net.grid[curr_node].edges))
        list_open.remove(curr_node)
        list_closed.append(curr_node)

        for neighbor in net.grid[curr_node].edges:
            if( list_closed.count(neighbor) ):
                continue

            tmp_fg = net.grid[curr_node].fg + net.grid[curr_node].edges[neighbor]

            if(list_open.count(neighbor) ):                                         #if neighbor in the open list
                if( tmp_fg >= net.grid[neighbor].fg  ):                             #this is not a better path from start
                    continue

            net.grid[neighbor].parent = curr_node

            net.grid[neighbor].fh = core_h_heuristic(neighbor, net)
            net.grid[neighbor].fg = tmp_fg
            net.grid[neighbor].fc = net.grid[neighbor].fg + (core_w_factor(neighbor,net) * net.grid[neighbor].fh)
            
            list_open.append(neighbor)
        
        if( 0 == len(list_open) ):
            break    
        
        curr_node = core_next_best(list_open, net)

    path = net_calculate_path(net.start, net.goal, net)
    expands = code_get_expands(list_closed, path)
    return (path , expands)


def main_init(map_file):
    if(not os.path.isfile(map_file)):
        print("Map file :<"+str(map_file)+"> does not exists. Exiting.")
        quit()


def main():
    parser = argparse.ArgumentParser(description="Calculate route from A to B for given TXT map.")
    parser.add_argument("--map",help="A path to the text file containing map grid", required=True)
    args = vars(parser.parse_args())

    map_file = args["map"]

    main_init(map_file)

    net = Net(0,0)
    net.net_init(map_file)
    net.net_create_grid()
    net.net_create_network()
    net_print_info(net)
    #net.net_print()

    (path, expands) = core_get_path(net)

    if(not len(path)):
        print("Path NOT found.")
        quit()

    print("Path lenght ="+str(len(path)-1) )                                 
    print("Extends count ="+str(len(expands)))


if __name__ == "__main__":
    main()

