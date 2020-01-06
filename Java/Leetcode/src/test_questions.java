public class test_questions {

//    class Tree {
//        public int x;
//        public Tree l;
//        public Tree r;
//    }
//
//    public int solution(Tree T) {
//        return helper(T, T.x, 0);
//    }
//
//    public int helper(Tree node, int currMax, int count) {
//        if (node == null) {
//            return 0;
//        }
//
//        if (node.x >= currMax) {
//            currMax = node.x;
//            count++;
//        }
//
//        return helper(node.l, currMax, count) + helper(node.r, currMax, count);
//    }

    public int solution(int N, String S) {
        int[][] matrix = new int[N][3];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < 3; j++) {
                matrix[i][j] = -1;
            }
        }

        int init = N * 3;

        for (int i = 0; i < S.length() - 2; i += 3) {
            int row = S.charAt(i) - 49;

            if (S.charAt(i + 1) == 71 || S.charAt(i + 1) == 68) {
                matrix[row][1]++;
                if (matrix[row][1] == 1) {
                    System.out.println("mid " + row);
                    init--;
                }



            } else  if (S.charAt(i + 1) == 70 || S.charAt(i + 1) == 69) {
                if (matrix[row][1] < 1) {
                    System.out.println("mid - mid " + row);
                    init--;
                }
                matrix[row][1] = 1;



            } else if (S.charAt(i + 1) <= 67 && S.charAt(i + 1) >= 65) {
                if (matrix[row][0] < 1) {
                    System.out.println("left " + row);
                    init--;
                }
                matrix[row][0] = 1;



            } else if (S.charAt(i + 1) <= 75 && S.charAt(i + 1) >= 72) {
                if (matrix[row][2] < 1) {
                    System.out.println("right " + row);
                    init--;
                }
                matrix[row][2] = 1;


            }
        }

        return init;
    }


    public static void main(String[] args) {
        test_questions a = new test_questions();
        System.out.println(a.solution(3, "1A 2F 1C 3D 3E 3F 3G"));
    }
}
