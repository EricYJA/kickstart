public class Tools {
    public static void printArray(char[] num) {
        System.out.println("===");
        for (char r : num) {
            System.out.print(r);
        }
        System.out.println("\n===");
    }

    public static void printMatrix(int[][] num) {
        for (int[] r : num) {
            String line = "";
            for (int n : r) {
                line = line + " " + n;
            }
            System.out.println(line);
        }
    }

    public static boolean isDigit(char i) {
        return i >= 48 && i <= 57;
    }

    public static boolean isAlpha(char i) {
        return i >= 65 && i <= 90;
    }
}


