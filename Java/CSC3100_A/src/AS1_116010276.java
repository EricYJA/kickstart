/**
 * Note that this is for CSC3010 assignment 1
 * @Credit 袁常成 116010276
 * I used the @randomArray defined in this file
 * to generate the array and use a random
 * function to generate k and test.
 */

import java.util.Arrays;

public class AS1_116010276 {
    /**
     * Note that this is the base case to determine the base speed
     * @param nums
     * @param k
     * @return the kth biggest numeber
     */
    private static int baseCase(int[] nums, int k) {
        Arrays.sort(nums);
        return nums[k - 1];
    }

    /**
     *  Note that this function is the find function to
     *  find the kth number, this function would call @qSort
     * @param nums: the unsorted array
     * @param k: the number k
     * @return return the kth number
     */
    public static int find(int[] nums, int k) {
        qSort(nums, 0, nums.length - 1, k - 1);
        return nums[k - 1];
    }

    /**
     * This is the main part of the improved quick sort
     * @param arr: this the unsorted array
     * @param left: the starting index of the sorted part
     * @param right: the end index of the sorted part
     * @param k: the k
     */
    private static void qSort(int[] arr, int left, int right, int k) {
        /* recursion end */
        if (left >= right) {
            return;
        } else if ((right - left) <= 100) {
            insertionSort(arr, left, right);
            return;
        }

        /* Finding the pivot */
        int quar = (right - left);
        int tri = quar / 3;
        int lmid = (tri + left);
        int rmid = (right - tri);
        int mid = (quar / 2) + left;
        int rand = left + (int)(Math.random()*(quar+1));
        boolean swapflag = false;

        if (Math.abs(mid - k) > quar * 0.99) {
            AS1_116010276_HeapSort.findk(arr, k + 1, left, right);
            return;
        }

        if (k < lmid && (right - left) > (arr.length / 10)) {
            if (arr[rmid] > arr[mid] || arr[rmid] > arr[lmid]) {
                if (arr[mid] < arr[lmid]) {
                    swap_1(arr, mid, rmid);
                } else {
                    swap_1(arr, lmid, rmid);
                }
            }
        } else if (k > rmid  && (right - left) > (arr.length / 10)) {
            if (arr[rmid] < arr[mid] || arr[rmid] < arr[lmid]) {
                if (arr[mid] > arr[lmid]) {
                    swap_1(arr, mid, rmid);
                } else {
                    swap_1(arr, lmid, rmid);
                }
            }
        } else {
            if (arr[rmid] > arr[rand] && arr[rmid] > arr[lmid]) {
                if (arr[rmid] > arr[lmid]) {
                    swap_1(arr, rand, rmid);
                } else {
                    swap_1(arr, left, rmid);
                }
            } else if (arr[rmid] < arr[rand] && arr[rmid] < arr[lmid]) {
                if (arr[rand] < arr[lmid]) {
                    swap_1(arr, rand, rmid);
                } else {
                    swap_1(arr, lmid, rmid);
                }
            }
        }

        swap_1(arr, rmid, right);
        int pivot = arr[right];
        /* Finding ends */

        /* partition starts */
        int i = left;
        int j = right - 1;

        while (true) {
            while (arr[i] < pivot)
                i++;
            while (arr[j] >= pivot && j>0)
                j--;
            if (i < j) {
                swap_1(arr, i, j);
                swapflag = true;
            } else
                break;
        }

        if(!swapflag) {
            return;
        }

        swap_1(arr, i, right);
        /* partition ends */

        if (k < i) {
            qSort(arr, left, i - 1, k);
        } else {
            qSort(arr, i + 1, right, k);
        }
    }

    /**
     * This is to swap the values of the i index
     * and the j index in array arr
     * @param arr: the need-to-swap array
     * @param i: index
     * @param j: index
     */
    private static void swap_1(int[] arr, int i, int j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }

    /**
     * This is the insertion sort for the array
     * bounded by the left and right index.
     * @param arr: the need sorting array
     * @param left: the left bound
     * @param right: the right bound
     */
    private static void insertionSort(int[] arr, int left, int right) {
        int tmp;
        for (int i = left + 1; i <= right; i++) {
            tmp = arr[i];
            int j = i - 1;
            while (j >= left && tmp < arr[j]) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = tmp;
        }
    }


    private static int trial1(int[] nums, int k) {
        return 0;
    }

    /**
     * This is the random Array generating function
     * Choose to generate different arrays
     * @param n: the generated size, the random number range
     * @return return a random array size n, with value range
     *          from 0 to n
     */
    public static int[] randomArray(int n) {
        int[] toReturn = new int[n];
        for (int i = 0; i < toReturn.length; i++) {
            toReturn[i] = (int) (Math.random() * n); // total random
//            toReturn[i] = (int) ((Math.random()* 10) % 2); // random 0 or 1
//            toReturn[i] = i; // increasing order
//            toReturn[i] = n - i; // decreasing order
//            toReturn[i] = 0; // All 0
        }

        return toReturn;
    }

    public static void main(String[] args) {
        /* Create random numbers */
        long startT = System.currentTimeMillis();    //获取开始时间

        int n = 20000000; // define the size here
        int[] test;
        int k;

        long endT = System.currentTimeMillis();    //获取结束时间
        System.out.println("程序构建时间：" + (endT - startT) + "ms");    //输出程序运行时间
        /* Create ends */

        /* Test the program */
        long startTime, endTime;
        test = randomArray(n);
        k = (int) (Math.random() * n) + 1;
        startTime = System.currentTimeMillis();    //获取开始时间
        System.out.printf("Trial: The %dth largest number is %d.\n", k, find(test, k));
        endTime = System.currentTimeMillis();    //获取结束时间
        System.out.println("平均程序运行时间：" + (endTime - startTime) + "ms");    //输出程序运行时间
        /* Test ends */

        /* Use the @baseCase to check if the result is right */
        long startTimeB = System.currentTimeMillis();    //获取开始时间
        System.out.printf("BaseSort: The %dth largest number is %d.\n", k, baseCase(test, k));
        long endTimeB = System.currentTimeMillis();    //获取结束时间
        System.out.println("基础运行时间：" + (endTimeB - startTimeB) + "ms");    //输出程序运行时间
        /* Check ends */

        /* my own test in finding the average runtime */
//        int round = 50;
//        long totalq = 0;
//        long startTimeq, endTimeq;
//        for (int i = 0; i < round; i++) {
//
//            test = randomArray(n);
//            k = (int) (Math.random() * n);
//
//            startTimeq = System.currentTimeMillis();    //获取开始时间
//            find(test, k);
//            endTimeq = System.currentTimeMillis();    //获取结束时间
//
//            totalq += (endTimeq - startTimeq);
//        }
//
//        System.out.println("Quick程序运行时间：" + (totalq / round) + "ms");
    }
}
