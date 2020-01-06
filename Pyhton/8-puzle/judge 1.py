from test_ import get_matrix

DATA_SOLVE = ['Puzzle_0_13.txt', 'Puzzle_14_15.txt', 'Puzzle_16_20.txt']
DATA_UNSOLVE = ['Puzzle_0_13_F.txt', 'Puzzle_14_15_F.txt', 'Puzzle_16_20_F.txt']

goal = [[1, 2, 3], 
    [4, 5, 6],
    [7, 8, 0]
    ]

def read(puzzle):
    ten = [1,10,100,1000,10000,100000,1000000,10000000,100000000]
    data = 0
    for i in range(3):
        for j in range(3):
            data += ten[3*i + j] * puzzle[i][j]
    return data

def judge(puzzle, goal):
    ten = [1,10,100,1000,10000,100000,1000000,10000000,100000000]
    src = read(puzzle)
    count = 0
    if src < 100000000:
        for i in range(8):
            for j in range(i+1, 8):
                if int(src/ten[i])%10 < int(src/ten[j])%10 and int(src/ten[i])%10 > 0:
                    count += 1
    else:
        for i in range(9):
            for j in range(i+1, 9):
                if int(src/ten[i])%10 < int(src/ten[j])%10 and int(src/ten[i])%10 > 0:
                    count += 1
    return count % 2 == 1


if __name__ == "__main__": 
    for matrix_name in DATA_UNSOLVE:
        for matrix_sample in get_matrix(matrix_name):
            if judge(matrix_sample, goal):
                print(' ')
            else:
                print('1')