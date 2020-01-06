from functools import total_ordering
from queue import PriorityQueue
import copy

def solution(puzzle):

    global d, manh, goal

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
    r = ['D', 'U', 'R', 'L']

    manh = [[0, 1, 2, 1, 2, 3, 2, 3, 4],  
    [1, 0, 1, 2, 1, 2, 3, 2, 3],  
    [2, 1, 0, 3, 2, 1, 4, 3, 2],  
    [1, 2, 3, 0, 1, 2, 1, 2, 3],  
    [2, 1, 2, 1, 0, 1, 2, 1, 2],  
    [3, 2, 1, 2, 1, 0, 3, 2, 1],  
    [2, 3, 4, 1, 2, 3, 0, 1, 2],  
    [3, 2, 3, 2, 1, 2, 1, 0, 1],  
    [4, 3, 2, 3, 2, 1, 2, 1, 0]]

    @total_ordering
    class Node:
        def __init__(self, state, pos, moves, priority, route):
            self.__state = state
            self.__pos = pos
            self.__moves = moves
            self.__priority = priority
            self.__route = route

        def getState(self):
            return self.__state

        def getPos(self):
            return self.__pos 

        def getMoves(self):
            return self.__moves

        def getPriority(self):
            return self.__priority

        def getRoute(self):
            return self.__route

        def setState(self, state):
            self.__state = state

        def setPos(self, pos):
            self.__pos = pos

        def setPriority(self, priority):
            self.__priority = priority

        def setRoute(self, route):
            self.__route = route

        def __eq__(self, other):
            return self.__priority == other.__priority

        def __lt__(self, other):
            return self.__priority < other.__priority

    goal = Node(0, 0, 0, 0, '')

    def manhattan(node):
        global manh
        m = 0
        pos = node.getPos()
        state = node.getState()
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
                    node.setState(node.getState() | (puzzle[i][j] - 1) << (24 - 9 * i - 3 * j))
                else:
                    node.setState(node.getState() | puzzle[i][j] << (24 - 9 * i - 3 * j))
                    node.setPos(3 * i + j)
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
    
    convert([[1, 2, 3], [4, 5, 6], [7, 8, 0]], goal)

    def exchange(node, pos):
        s = Node(0, 0, node.getMoves() + 1, 0, '')
        node_state = node.getState()
        temp = 7 << ((9 - pos) * 3) & node_state
        temp = ((temp >> ((9 - pos) * 3)) << ((9 - node.getPos() - 1) * 3)) 
        s.setState((node_state | temp) & ( ~ (7 << ((9 - pos) * 3))))
        s.setPos(pos - 1)
        s.setPriority(s.getMoves() + manhattan(s))
        return s

    #solve
    a = Node(0, 0, 0, 0, '')
    if convert(puzzle, a):
        return ''
    a.setPriority(manhattan(a))
    q = PriorityQueue()
    q.put(a)
    pos = a.getPos()
    while (pos != goal.getPos()) or (a.getState() != goal.getState()):
        # if q.empty():
        #     return ''
        # else:
        #     a = q.get()
        #     pos = a.getPos()
        a = q.get()
        pos = a.getPos()
        route = a.getRoute()
        for i in range(4):
            if d[pos][i] > 0 and (len(route) == 0 or route[-1] != r[i]):
                newNode = exchange(a, d[pos][i])
                newNode.setRoute(route + p[i])
                q.put(newNode)

    route = a.getRoute()
    # print('Route:', route)
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
            # print("Puzzle if not solved")
            return False
    
    # assert check_solution(solution, [[1, 2, 3], 
    #                                 [4, 5, 6],
    #                                 [7, 8, 0]]), "Example failed!"

    from test_ import get_matrix
    from time import time
    nameList = ["Puzzle_0_13_F.txt", "Puzzle_0_13.txt", "Puzzle_14_15.txt", "Puzzle_16_20.txt", "Puzzle_mix.txt"]
    dataList = get_matrix(nameList[4])

    start = time()

    for i in dataList:
        check_solution(solution, i)

    end = time()

    print(end - start)