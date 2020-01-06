import copy

def solution(puzzle):

    #logic
    def isGoal(puzzle):
        if puzzle == [[1, 2, 3], [4, 5, 6], [7, 8, 0]]:
            return True
        else:
            return False

    # def hamming(puzzle):
    #     # if (hamming >= 0):
    #     #     return hamming
    #     hamming = 0
    #     for i in range(3):
    #         for j in range(3):
    #             if puzzle[i][j] != 0:
    #                 if i * 3 + j + 1 != puzzle[i][j]:
    #                     hamming += 1
    #     return hamming

    global manh, r
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

    def manhattan(puzzle):
        global manh
        # if (manhattan >= 0):
        #     return manhattan
        manhattan = 0
        for i in range(3):
            for j in range(3):
                if puzzle[i][j] != 0:
                    manhattan += manh[puzzle[i][j]][3 * i + j + 1]
        return manhattan

    def exchange(x1, y1, x2, y2, puzzle):
        newPuzzle = copy.deepcopy(puzzle)
        temp = newPuzzle[x1][y1]
        newPuzzle[x1][y1] = newPuzzle[x2][y2]
        newPuzzle[x2][y2] = temp
        return newPuzzle

    class gameTreeNode:
        def __init__(self, board, father, route):
            self.__board = board
            self.__father = father
            self.__route = route
            if father is not None:
                self.__moves = father.getMoves() + 1
            else:
                self.__moves = 0
            self.__priority = manhattan(self.__board) + self.__moves

        def getRoute(self):
            return self.__route

        def getFather(self):
            return self.__father

        def getBoard(self):
            return self.__board

        def getMoves(self):
            return self.__moves

        # def setMoves(self, moves):
        #     self.__moves = moves
        #     self.__priority = manhattan(self.__board) + self.__moves

    class Node:
        def __init__(self, state, pos, route):
            self.__state = state
            self.__pos = pos
            self.__route = route

        def getState(self):
            return self.__state

        def getPos(self):
            return self.__pos 

        def getRoute(self):
            return self.__route

        def setState(self, state):
            self.__state = state

        def setPos(self, pos):
            self.__pos = pos

        def setRoute(self, route):
            self.__route = route

    goal = Node(0, 0, '')


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

    def search(currentNode, depth, diff, maxh):
        currentBoard = currentNode.getBoard()
        if isGoal(currentBoard) == True:
            global r
            r = currentNode.getRoute()
            return True
        if depth > maxh:
            return False
        elif depth + diff > maxh:
            return False
        else:
            for i in range(3):
                for j in range(3):
                    if currentBoard[i][j] == 0:
                        route = currentNode.getRoute()
                        if i > 0 and (len(route) == 0 or route[-1] != 'D'):
                            newPuzzle = exchange(i, j, i - 1, j, currentBoard)
                            newNode = gameTreeNode(newPuzzle, currentNode, route + 'U') 
                            if search(newNode, depth + 1, manhattan(newPuzzle), maxh) == True:
                                return True
                        if i < 2 and (len(route) == 0 or route[-1] != 'U'):
                            newPuzzle = exchange(i, j, i + 1, j, currentBoard)
                            newNode = gameTreeNode(newPuzzle, currentNode, route + 'D')
                            if search(newNode, depth + 1, manhattan(newPuzzle), maxh) == True:
                                return True
                        if j > 0 and (len(route) == 0 or route[-1] != 'R'):
                            newPuzzle = exchange(i, j, i, j - 1, currentBoard)
                            newNode = gameTreeNode(newPuzzle, currentNode, route + 'L')
                            if search(newNode, depth + 1, manhattan(newPuzzle), maxh) == True:
                                return True
                        if j < 2 and (len(route) == 0 or route[-1] != 'L'):
                            newPuzzle = exchange(i, j, i, j + 1, currentBoard)
                            newNode = gameTreeNode(newPuzzle, currentNode, route + 'R')
                            if search(newNode, depth + 1, manhattan(newPuzzle), maxh) == True:
                                return True
                        return False


    #solve
    a = Node(0, 0, '')
    if convert(puzzle, a) == True:
        return ''
        
    maxh = manhattan(puzzle)
    currentNode = gameTreeNode(puzzle, None, '')
    while search(currentNode, 0, 0, maxh) == False:
        maxh += 1
    print('Route:', r)
    return r

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
            print("Puzzle if not solved")
            return False
    
    # assert check_solution(solution, [[0, 5, 7], 
    #                                 [6, 1, 2],
    #                                 [3, 8, 4]]), "Example failed!"

    from test_ import get_matrix
    from time import time
    nameList = ["Puzzle_0_13_F.txt", "Puzzle_0_13.txt", "Puzzle_14_15.txt", "Puzzle_16_20.txt", "Puzzle_mix.txt"]
    dataList = get_matrix(nameList[4])

    start = time()

    for i in dataList:
        check_solution(solution, i)

    end = time()

    print(end - start)
