import java.util.Arrays;

public class Interview {

    private static void TLcar(String cars, int h) {
        String[] list_s = cars.split(" ");
        int[] list_i = new int[cars.length()];
        for (int i = 0; i < list_s.length; i++) {
            list_i[i] = Integer.parseInt(list_s[i]);
        }

        for (int i = 0; i < list_s.length; i++) {
            int tmpj = -1;
            for (int j = 0; j < i; j++) {
                if (tan(h, list_i[i], i + 1) >= tan(h, list_i[j], j + 1)) {
                    tmpj = j;
                }
            }
            System.out.println(tmpj + 1);
        }
    }

    private static double tan(int h, int a, int dist) {
        return (double)(h - a) / (double) dist;
    }


    public static void main(String[] args) {
        System.out.println("Hello world");

        char a = '5';

        System.out.println('5' - 48);

//        TLcar("5 4 3 4 3 3 3 3 3", 5);
    }
}
