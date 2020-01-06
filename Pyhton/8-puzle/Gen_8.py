import random
import math

### Generate random matrix ###

### fail example ###
_goal_state = [[1,2,3],
               [4,5,6],
               [7,8,0]]

# def shuffle(matx, num):
#     numlist = [i for i in range(3)]

#     for i in range(num):
#         col_1 = random.choice(numlist)
#         row_1 = random.choice(numlist)

#         col_2 = random.choice(numlist)
#         row_2 = random.choice(numlist)

#         f_num = matx[row_1][col_1]
#         s_num = matx[row_2][col_2]

#         matx[row_2][col_2] = f_num
#         matx[row_1][col_1] = s_num

#     return matx

def gen_ran():
    numlist = [i for i in range(9)]
    matx = [[0, 0 ,0], [0, 0, 0], [0, 0, 0]]
    for j in range(3):
        for k in range(3):
            num = random.choice(numlist)
            matx[j][k] = num
            numlist.remove(num)

    return matx


### Test manhattan ###

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


### Test solvable ###

def read(puzzle):
    ten = [1,10,100,1000,10000,100000,1000000,10000000,100000000]
    data = 0
    for i in range(3):
        for j in range(3):
            data += ten[3*i + j] * puzzle[i][j]
    return data

def judge(puzzle, goal):
    src = read(puzzle)
    dest = read(goal)
    ten = [1,10,100,1000,10000,100000,1000000,10000000,100000000]
    dist=[[0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0],]
    for i in range(9):
        for j in range(9):
            dist[i][j] = abs(int(i/3)-int(j/3))+abs(i%3-j%3)

    arc = [0,0,0,0,0,0,0,0,0]
    for k in range(9):
        arc[int(dest/ten[k])%10] = k

    p = []
    for i in range(9):
        p.append(arc[int(src/ten[i]%10)])
    b1 = 0
    b2 = 0
    count = 0
    vis = [False, False, False, False, False, False, False, False, False]
    while (int(src/ten[b1])%10!=0):
        b1 += 1
    while (int(dest/ten[b2])%10!=0):
        b2 += 1
    for i in range(9):
        if vis[i] == False:
            count += 1
            z = i
            while vis[z]==False:
                vis[z] = True
                z = p[z]
    return (count-dist[b1][b2])%2 == 0

### Main funx ###

def main():
    n = 10000


    puzzle_f = open("Puzzle_0_13.txt", "w+")
    puzzle_s = open("Puzzle_14_15.txt", "w+")
    puzzle_t = open("Puzzle_16_20.txt", "w+")
    puzzle_f_f = open("Puzzle_0_13_F.txt", "w+")
    puzzle_s_f = open("Puzzle_14_15_F.txt", "w+")
    puzzle_t_f = open("Puzzle_16_20_F.txt", "w+")
    Mix = open("Puzzle_mix.txt", "w+")

    sample_0_13 = 0
    sample_14_15 = 0
    sample_16_20 = 0
    sample_0_13_f = 0
    sample_14_15_f = 0
    sample_16_20_f = 0

    for i in range(n):
        _adj_matrix = gen_ran()

        
        for i in _adj_matrix:
            mid_str = ""
            for j in i:
                mid_str = mid_str + str(j) + " "
            mid_str = mid_str + "\n"
            Mix.write(mid_str)
        Mix.write("\n")

        # print(_adj_matrix)
        # _adj_matrix = [
        #     [1,3,2],
        #     [4,5,6],
        #     [7,8,0]
        # ]

        if judge(_adj_matrix, _goal_state) == False:

            if 0 <= manhattan(_adj_matrix) <= 13:
                for i in _adj_matrix:
                    mid_str = ""
                    for j in i:
                        mid_str = mid_str + str(j) + " "
                    mid_str = mid_str + "\n"
                    puzzle_f.write(mid_str)
                puzzle_f.write("\n")

                sample_0_13 = sample_0_13 + 1


            elif 14 <= manhattan(_adj_matrix) <= 16:
                for i in _adj_matrix:
                    mid_str = ""
                    for j in i:
                        mid_str = mid_str + str(j) + " "
                    mid_str = mid_str + "\n"
                    puzzle_s.write(mid_str)
                puzzle_s.write("\n")

                sample_14_15 = sample_14_15 + 1

            else:
                for i in _adj_matrix:
                    mid_str = ""
                    for j in i:
                        mid_str = mid_str + str(j) + " "
                    mid_str = mid_str + "\n"
                    puzzle_t.write(mid_str)
                puzzle_t.write("\n")

                sample_16_20 = sample_16_20 + 1

        else:
            
            if 0 <= manhattan(_adj_matrix) <= 13:
                for i in _adj_matrix:
                    mid_str = ""
                    for j in i:
                        mid_str = mid_str + str(j) + " "
                    mid_str = mid_str + "\n"
                    puzzle_f_f.write(mid_str)
                puzzle_f_f.write("\n")

                sample_0_13_f = sample_0_13_f + 1

            elif 14 <= manhattan(_adj_matrix) <= 16:
                for i in _adj_matrix:
                    mid_str = ""
                    for j in i:
                        mid_str = mid_str + str(j) + " "
                    mid_str = mid_str + "\n"
                    puzzle_s_f.write(mid_str)
                puzzle_s_f.write("\n")

                sample_14_15_f = sample_14_15_f + 1

            else:
                for i in _adj_matrix:
                    mid_str = ""
                    for j in i:
                        mid_str = mid_str + str(j) + " "
                    mid_str = mid_str + "\n"
                    puzzle_t_f.write(mid_str)
                puzzle_t_f.write("\n")

                sample_16_20_f = sample_16_20_f + 1

    print(" 0-13 true ", sample_0_13)
    print("14-15 true ", sample_14_15)
    print("16-20 true ", sample_16_20)
    print(" 0-13_f false ", sample_0_13_f)
    print("14-15_f false ", sample_14_15_f)
    print("16-20_f false ", sample_16_20_f)

if __name__ == "__main__":
    main()
