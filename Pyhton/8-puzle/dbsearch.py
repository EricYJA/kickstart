import copy
from queue import Queue

def solution(puzzle):

    global d, manh, goal, allState

    MAX = 400000
    allState = [list() for i in range(MAX)]

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

    manh = [[-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],  
    [-1, 0, 1, 2, 1, 2, 3, 2, 3, 4],  
    [-1, 1, 0, 1, 2, 1, 2, 3, 2, 3],  
    [-1, 2, 1, 0, 3, 2, 1, 4, 3, 2],  
    [-1, 1, 2, 3, 0, 1, 2, 1, 2, 3],  
    [-1, 2, 1, 2, 1, 0, 1, 2, 1, 2],  
    [-1, 3, 2, 1, 2, 1, 0, 3, 2, 1],  
    [-1, 2, 3, 4, 1, 2, 3, 0, 1, 2],  
    [-1, 3, 2, 3, 2, 1, 2, 1, 0, 1],  
    [-1, 4, 3, 2, 3, 2, 1, 2, 1, 0]]

    class Node:
        def __init__(self, state, pos, visit, route):
            self.__state = state
            self.__pos = pos
            self.__visit = visit
            self.__route = route

        def getState(self):
            return self.__state

        def getPos(self):
            return self.__pos
        
        def getVisit(self):
            return self.__visit 

        def getRoute(self):
            return self.__route

        def setState(self, state):
            self.__state = state

        def setPos(self, pos):
            self.__pos = pos

        def setRoute(self, route):
            self.__route = route

    goal = Node(0, 0, 1, '')

    def manhattan(node):
        global manh
        m = 0
        pos = node.getPos()
        state = node.getState()
        for i in range(9):
            if pos != i:
                m += manh[(((state >> (3 * (8 - i))) & 7) + 1)][i + 1]
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
        temp = 7 << ((9 - pos) * 3)
        s = Node(0, 0, node.getVisit(), None)
        s.setState(node.getState())
        temp = temp & node.getState()
        temp = ((temp >> ((9 - pos) * 3)) << ((9 - node.getPos() - 1) * 3))
        s.setState(s.getState() | temp)
        s.setState(s.getState() & ( ~ (7 << ((9 - pos) * 3))))
        s.setPos(pos - 1)
        return s

    def search(node):
        global allState, goal
        index = node.getState() % MAX
        flag = True
        while (flag == True):
            if allState[index] == []:
                allState[index] = node
                flag = False
            elif (node.getPos() == allState[index].getPos()) and (node.getState() == allState[index].getState()):
                flag = False
            else:
                index = (index + 1) % MAX
        return index

    #solve
    a = Node(0, 0, 0, '')
    convert(puzzle, a)
    q = Queue()
    b = search(a)
    q.put(b)
    c = search(goal)
    q.put(c)
    # if (a.getPos() == goal.getPos()) and (a.getState() == goal.getState()):
    if convert(puzzle, a) == True:
        return ''
    else:
        isGoal = False
        while (q.empty() == False and isGoal == False):
            index = q.get()
            route = allState[index].getRoute()
            # print(route,end = ' '+str(allState[index].getVisit())+" ")
            pos = allState[index].getPos()
            for i in range(4):                
                if d[pos][i] > 0:
                    if (allState[index].getVisit() == 0) and (len(route) == 0 or route[-1] != r[i]):
                        newIndex = search(exchange(allState[index], d[pos][i]))
                        if allState[newIndex].getRoute() is None:
                            allState[newIndex].setRoute(allState[index].getRoute() + p[i])
                            q.put(newIndex)
                        elif allState[index].getVisit() != allState[newIndex].getVisit():
                            route = allState[index].getRoute() + p[i] + allState[newIndex].getRoute()
                            isGoal = True
                            break
                    elif (allState[index].getVisit() == 1) and (len(route) == 0 or route[0] != p[i]):
                        newIndex = search(exchange(allState[index], d[pos][i]))
                        if allState[newIndex].getRoute() is None:
                            allState[newIndex].setRoute(r[i] + allState[index].getRoute())
                            q.put(newIndex)
                        elif allState[index].getVisit() != allState[newIndex].getVisit():
                            route = allState[newIndex].getRoute() + r[i] + allState[index].getRoute()
                            isGoal = True
                            break
                    
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
    
    # assert check_solution(solution, [[5, 0, 3], 
    #                                 [6, 8, 4],
    #                                 [7, 2, 1]]), "Example failed!"

    from test_ import get_matrix
    from time import time
    nameList = ["Puzzle_0_13.txt", "Puzzle_14_15.txt", "Puzzle_16_20.txt", "Puzzle_mix.txt"]
    dataList = get_matrix(nameList[3])

    start = time()

    for i in dataList:
        check_solution(solution, i)

    end = time()

    print(end - start)