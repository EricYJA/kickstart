import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

public class dynamicProgramming_1 {

    /**
     * Given a man who can walk one stair or two stairs per time,
     * calculate how many possibilities for a man to walk on some
     * given number stairs.
     * @param n : the number of stairs count
     * @return : the number of combinations
     */

    public static int climbStairs(int n) {

        // This is only the recursive way to solve this
        // However, recursion is time wasting. Thus I comment
        // it just for the record
//        if (n == 1) {
//            return 1;
//        } else if (n == 2) {
//            return 2;
//        }
//
//        return climbStairs(n - 1) + climbStairs(n - 2);


        if (n <= 2) {
            return n;
        }

        int[] ref = new int[n + 1];
        ref[0] = 0;
        ref[1] = 1;
        ref[2] = 2;

        int count = 3;
        while (count <= n) {
            int toAdd = ref[count - 1] + ref[count - 2];
            ref[count] = toAdd;
            count++;
        }

        return ref[n];

    }

    /**
     * This is for finding the max profit in a single stock trade.
     * That is, finding the max difference of two days' price in
     * increasing index direction.
     * @param prices : the array of prices arranged by time
     * @return : the max profit
     */

    public static int maxProfit(int[] prices) {
        // This is the O(N^2) way to solve this.
        // There is a better way to do it, though.
        // However, I still keep this one for the record.
//        int profit = 0;
//        int tmp;
//        for (int i = 0; i < prices.length; i++) {
//            for (int j = i; j < prices.length; j++) {
//                tmp = prices[j] - prices[i];
//                if (tmp > profit) {
//                    profit = tmp;
//                }
//            }
//        }
//
//        return profit;


        if (prices == null || prices.length == 0) {
            return 0;
        }

        int profit = 0;
        int start = 0;
        for (int i = 0; i < prices.length; i++) {
            profit = Math.max(profit, prices[i]-prices[start]);
            start = Math.min(prices[start], prices[i]) == prices[start] ? start : i;
        }

        return profit;
    }

    public static int[] intersect(int[] nums1, int[] nums2) { // Note that the nums1 is the short one
        if (nums1.length < 1 || nums2.length < 1) {
            return new int[0];
        }

        Arrays.sort(nums1);
        Arrays.sort(nums2);

        if (nums1.length > nums2.length) {
            int[] temp = nums2;
            nums2 = nums1;
            nums1 = temp;
        }

        List<Integer> intersect = new ArrayList<>();

        int n1 = 0;
        int n2 = 0;

        while (n1 < nums1.length && n2 < nums2.length) {
            if (nums2[n2] > nums1[n1]) {
                n1++;
            } else if (nums2[n2] < nums1[n1]) {
                n2++;
            } else {
                n1++;
                intersect.add(nums2[n2]);
                n2++;
            }
        }

        int[] toReturn = new int[intersect.size()];
        for (int k = 0; k < toReturn.length; k++) {
            toReturn[k] = intersect.get(k);
        }

        return toReturn;
    }

    public static int numSqroot(int A, int B) {
        int result = 0;

        for (int i = 2; i < 31622; i++) {
            boolean flag = false;
            int pow = i;
            int cnt = 0;

            while (pow <= B) {
                if (pow >= A) {
                    flag = true;
                }
                pow = (int) Math.pow(pow, 2);
                cnt++;
            }

            if (pow > B) {
                cnt--;
            }

            if (cnt > result && flag) {
                result = cnt;
            }
        }

        return result;
    }

    public int rob(int[] nums) {
        int n = nums.length;
        if (n == 0) return 0;
        else if (n == 1) return nums[1];
        else if (n == 2) return nums[0] > nums[1] ? nums[0] : nums[1];

        int[] f = new int[n];


        f[0] = nums[0];
        f[1] = nums[0] > nums[1] ? nums[0] : nums[1];

        for (int i = 2; i < f.length; i++) {
            f[i] = Math.max(f[i - 2] + nums[i], f[i -1]);
        }
        return f[n - 1];
    }

    public static void main(String[] args) {
        dynamicProgramming_1 test = new dynamicProgramming_1();
        String[] a = new String[]{"a", "c", "b"};
        Arrays.sort(a, Comparator.comparing(o -> o.charAt(0)));
        for (int i = 0; i < a.length; i++) {
            System.out.print(a[i]);
        }
        System.out.println(test.rob(new int[]{1, 7, 9, 4}));
    }
}
