public class isValidsudoku {
    public static boolean isValidSudoku(char[][] board) {
        boolean rowsFlag, colsFlag, squaresFlag = true;
        for (int i = 0; i < board.length; i++) {
            rowsFlag = checkRow(i, board);
            colsFlag = checkCol(i, board);
            if (!rowsFlag || !colsFlag) {
                return false;
            }
        }

        for (int m = 0; m <= 6; m += 3) {
            for (int n = 0; n <= 6; n += 3) {
                squaresFlag = checkSquare(m, n, board);
                if (!squaresFlag) {
                    return false;
                }
            }
        }


        return true;
    }

    private static boolean checkSquare(int col, int row, char[][] board) {
        boolean[] squares = new boolean[10];
        for (int i = col; i <= col + 2; i++) {
            for (int j = row; j <= row + 2; j++) {

                if (board[i][j] ==  '.') {
                    continue;
                }

                if (!squares[board[i][j] - '0']) {
                    squares[board[i][j] - '0'] = true;
                } else {
                    return false;
                }
            }
        }

        return true;
    }

    private static boolean checkCol(int col, char[][] board) {
        boolean[] cols = new boolean[10];
        for (int i = 0; i < board.length; i++) {
            if (board[i][col] ==  '.') {
                continue;
            }

            if (!cols[board[i][col] - '0']) {
                cols[board[i][col] - '0'] = true;
            } else {
                return false;
            }
        }

        return true;
    }

    private static boolean checkRow(int row, char[][] board) {
        boolean[] rows = new boolean[10];
        for (int i = 0; i < board.length; i++) {
            if (board[row][i] ==  '.') {
                continue;
            }

            if (!rows[board[row][i] - '0']) {
                rows[board[row][i] - '0'] = true;
            } else {
                return false;
            }
        }

        return true;
    }


    public static void main(String[] args) {
        char[][] input = new char[][]{
            {'.', '.', '.', '.', '5', '.', '.', '1', '.'},
            {'.', '4', '.', '3', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '3', '.', '.', '1'},

            {'8', '.', '.', '.', '.', '.', '.', '2', '.'},
            {'.', '.', '2', '.', '7', '.', '.', '.', '.'},
            {'.', '1', '5', '.', '.', '.', '.', '.', '.'},

            {'.', '.', '.', '.', '.', '2', '.', '.', '.'},
            {'.', '2', '.', '9', '.', '.', '.', '.', '.'},
            {'.', '.', '4', '.', '.', '.', '.', '.', '.'}
        };

        System.out.println(isValidSudoku(input));

    }

}
