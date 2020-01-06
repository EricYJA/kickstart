from functools import total_ordering
from queue import PriorityQueue
import copy

def solution(puzzle):

    #logic
    def isGoal(puzzle):
        if puzzle == [[1, 2, 3], [4, 5, 6], [7, 8, 0]]:
            return True
        else:
            return False

    def hamming(puzzle):
        # if (hamming >= 0):
        #     return hamming
        hamming = 0
        for i in range(3):
            for j in range(3):
                if puzzle[i][j] != 0:
                    if i * 3 + j + 1 != puzzle[i][j]:
                        hamming += 1
        return hamming

    def manhattan(puzzle):
        # if (manhattan >= 0):
        #     return manhattan
        manhattan = 0
        for i in range(3):
            for j in range(3):
                if puzzle[i][j] != 0 and i * 3 + j + 1 != puzzle[i][j]:
                    ii = (puzzle[i][j] - 1) / 3
                    jj = puzzle[i][j] - ii * 3 - 1
                    manhattan += abs(ii - i) + abs(jj - j)
        return manhattan

    def exchange(x1, y1, x2, y2, puzzle):
        newPuzzle = copy.deepcopy(puzzle)
        temp = newPuzzle[x1][y1]
        newPuzzle[x1][y1] = newPuzzle[x2][y2]
        newPuzzle[x2][y2] = temp
        return newPuzzle

    @total_ordering
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
        
        def getPriority(self):
            return self.__priority

        # def setMoves(self, moves):
        #     self.__moves = moves
        #     self.__priority = manhattan(self.__board) + self.__moves

        def __eq__(self, other):
            return self.__priority == other.__priority

        def __lt__(self, other):
            return self.__priority < other.__priority

    #solve
    q = PriorityQueue()
    currentNode = gameTreeNode(puzzle, None, '')
    q.put(currentNode)
    while isGoal(currentNode.getBoard()) == False:
        if q.empty():
            return ''
        else:
            currentNode = q.get()
            currentBoard = currentNode.getBoard()
            # print(currentBoard, manhattan(currentBoard))
            for i in range(3):
                for j in range(3):
                    if currentBoard[i][j] == 0:
                        route = currentNode.getRoute()
                        if i > 0 and (len(route) == 0 or route[-1] != 'D'):
                            newPuzzle = exchange(i, j, i - 1, j, currentBoard)
                            newNode = gameTreeNode(newPuzzle, currentNode, route + 'U')
                            # print(1, newPuzzle)
                            q.put(newNode)
                        if i < 2 and (len(route) == 0 or route[-1] != 'U'):
                            newPuzzle = exchange(i, j, i + 1, j, currentBoard)
                            newNode = gameTreeNode(newPuzzle, currentNode, route + 'D')
                            # print(2, newPuzzle)
                            q.put(newNode)
                        if j > 0 and (len(route) == 0 or route[-1] != 'D'):
                            newPuzzle = exchange(i, j, i, j - 1, currentBoard)
                            newNode = gameTreeNode(newPuzzle, currentNode, route + 'L')
                            # print(3, newPuzzle)
                            q.put(newNode)
                        if j < 2 and (len(route) == 0 or route[-1] != 'L'):
                            newPuzzle = exchange(i, j, i, j + 1, currentBoard)
                            newNode = gameTreeNode(newPuzzle, currentNode, route + 'R')
                            # print(4, newPuzzle)
                            q.put(newNode)


    route = currentNode.getRoute()
    print('Route:', route)
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
            print("Puzzle if not solved")
            return False

    from test_ import get_matrix
    from time import time
    nameList = ["Puzzle_0_13.txt", "Puzzle_14_15.txt", "Puzzle_16_20.txt"]
    dataList = get_matrix(nameList[0])

    start = time()

    for i in dataList:
        check_solution(solution, i)

    end = time() 

    print(end - start)

    dataList = get_matrix(nameList[1])
    start = time()

    for i in dataList:
        check_solution(solution, i)

    end = time() 

    print(end - start)

    dataList = get_matrix(nameList[2])
    start = time()

    for i in dataList:
        check_solution(solution, i)

    end = time() 

    print(end - start)

### cases hard to run ###
''' 5 2 0 
    3 4 1 
    6 8 7 

    2 4 1 
    3 7 5 
    0 8 6 '''



