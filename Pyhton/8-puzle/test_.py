

def get_matrix(route):
    matrix_file = open(route, "r")
    lines = matrix_file.readlines()
    lines = [i.split() for i in lines]
    matrix_file.close()

    for k in range(len(lines)):
        if lines[k]:
            lines[k] = [int(j) for j in lines[k]]
    
    matrix_list = list()
    
    mid_list = list()
    for i in range(len(lines)):
        if (i+1)%4 != 0:
            mid_list.append(lines[i])
        else:
            matrix_list.append(mid_list)
            mid_list = list()

    return matrix_list[:100]

if __name__ == "__main__":
    DATA_SET = ['Puzzle_0_13.txt', 'Puzzle_14_15.txt', 'Puzzle_16_20.txt', 'Puzzle_0_13_F.txt', 'Puzzle_14_15_F.txt', 'Puzzle_16_20_F.txt']
    DATA_SOLVE = ['Puzzle_0_13.txt', 'Puzzle_14_15.txt', 'Puzzle_16_20.txt']
    DATA_UNSOLVE = ['Puzzle_0_13_F.txt', 'Puzzle_14_15_F.txt', 'Puzzle_16_20_F.txt']

    for i in DATA_SET:
        print(i, len(get_matrix(i)))