import copy

def solution(puzzle):

    global d, p, manh, goal

    d =[[0,4,0,2],
    [0,5,1,3],
    [0,6,2,0],
    [1,7,0,5],
    [2,8,4,6],
    [3,9,5,0],
    [4,0,0,8],
    [5,0,7,9],
    [6,0,8,0]]

    p = ['U', 'D', 'L', 'R']

    manh = [[0, 1, 2, 1, 2, 3, 2, 3, 4],  
    [1, 0, 1, 2, 1, 2, 3, 2, 3],  
    [2, 1, 0, 3, 2, 1, 4, 3, 2],  
    [1, 2, 3, 0, 1, 2, 1, 2, 3],  
    [2, 1, 2, 1, 0, 1, 2, 1, 2],  
    [3, 2, 1, 2, 1, 0, 3, 2, 1],  
    [2, 3, 4, 1, 2, 3, 0, 1, 2],  
    [3, 2, 3, 2, 1, 2, 1, 0, 1],  
    [4, 3, 2, 3, 2, 1, 2, 1, 0]]

    class Node:
        def __init__(self, state, pos, route):
            self.state = state
            self.pos = pos
            self.route = route

    def manhattan(node):
        global manh
        m = 0
        pos = node.pos
        state = node.state
        for i in range(9):
            if pos != i:
                m += manh[(((state >> (3 * (8 - i))) & 7))][i]
        return m

    def convert(puzzle, node):
        ten = [1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000]
        state = 0
        data = 0
        count = 0  
        for i in range(3):
            for j in range(3):
                data += ten[3 * i + j] * puzzle[i][j]
                if puzzle[i][j] != 0:
                    node.state = (node.state | (puzzle[i][j] - 1) << (24 - 9 * i - 3 * j))
                else:
                    node.state = (node.state | puzzle[i][j] << (24 - 9 * i - 3 * j))
                    node.pos = 3 * i + j
        if data < 100000000:
            for i in range(8):
                for j in range(i + 1, 8):
                    if int(data/ten[i])%10 < int(data/ten[j])%10 and int(data/ten[i])%10 > 0:
                        count += 1
        else:
            for i in range(9):
                for j in range(i + 1, 9):
                    if int(data/ten[i])%10 < int(data/ten[j])%10 and int(data/ten[i])%10 > 0:
                        count += 1
        return count % 2 == 1

    def exchange(node, pos):
        s = Node(0, 0, '')
        node_state = node.state
        temp = 7 << ((9 - pos) * 3) & node_state
        temp = ((temp >> ((9 - pos) * 3)) << ((9 - node.pos - 1) * 3)) 
        s.state = (node_state | temp) & ( ~ (7 << ((9 - pos) * 3)))
        s.pos = pos - 1
        return s

    def search(node, depth, diff, maxh, prePos):
        global goal, d, p
        pos = node.pos
        if (pos == goal.pos) and (node.state == goal.state):
            global route
            route = node.route
            return True
        if depth > maxh:
            return False
        elif depth + diff > maxh:
            return False
        else:
            for i in range(4):
               if d[pos][i] > 0 and d[pos][i] != prePos + 1:
                   newNode = exchange(node, d[pos][i])
                   newNode.route = node.route + p[i]
                   if search(newNode, depth + 1, manhattan(newNode), maxh, pos) == True:
                       return True
            return False

    #solve
    a = Node(0, 0, '')
    if convert(puzzle, a) == True:
        return ''
    goal = Node(2739128, 8, '')
    maxh = manhattan(a)
    while search(a, 0, 0, maxh, -1) == False:
        maxh += 1

    return route

if __name__ == '__main__':

    GOAL = [[1, 2, 3], [4, 5, 6], [7, 8, 0]]
    MOVES = {'U': (-1, 0), 'D': (1, 0), 'L': (0, -1), 'R': (0, 1)}

    def check_solution(func, puzzle):
        route = func(puzzle)
        #test codes here
        goal = copy.deepcopy(puzzle)
        for i in route:
            move = False
            for j in range(3):
                for k in range(3):
                    if goal[j][k] == 0 and move == False:
                        goal[j][k] = goal[j + MOVES[i][0]][k + MOVES[i][1]]
                        goal[j + MOVES[i][0]][k + MOVES[i][1]] = 0
                        move = True

        if goal == GOAL:
            return True
        else:
            print("Puzzle is not solved")
            return False
    
    assert check_solution(solution, [[1, 2, 3], 
                                    [4, 6, 8],
                                    [7, 5, 0]]), "Example failed!"
