/**
 * Note that this is created for A1.java for CSC3100
 * @Credit 袁常成 116010276
 */

import java.util.Arrays;

public class AS1_116010276_HeapSort {
    private int left;
    private int right;


    /**
     * Initializer. initialize the need to be sorted area
     * @param left: the left bound
     * @param right: the right bound
     */
    AS1_116010276_HeapSort(int left, int right) {
        this.left = left;
        this.right = right;
    }

    private int parent(int n) {
        return ((n - 1 - left) / 2) + left;
    }

    private int left(int n) {
        return (2 * (n - left) + 1) + left;
    }

    private int right(int n) {
        return (2 * (n - left) + 2) + left;
    }

    private void buildminHeap(int n, int[] data) {
        for(int i = 1 + left; i < n; i++) {
            int t = i;
            // 调整堆
            while(t != left && data[parent(t)] > data[t]) {
                int temp = data[t];
                data[t] = data[parent(t)];
                data[parent(t)] = temp;
                t = parent(t);
            }
        }
    }

    private void adjust(int i, int n, int[] data) {
        if(data[i] <= data[left]) {
            return;
        }

        int temp = data[i];
        data[i] = data[left];
        data[left] = temp;

        int t = left;
        while( (left(t) < n && data[t] > data[left(t)])
                || (right(t) < n && data[t] > data[right(t)]) ) {
            if(right(t) < n && data[right(t)] < data[left(t)]) {

                temp = data[t];
                data[t] = data[right(t)];
                data[right(t)] = temp;
                t = right(t);
            } else {

                temp = data[t];
                data[t] = data[left(t)];
                data[left(t)] = temp;
                t = left(t);
            }
        }
    }


    public void findTopN(int n, int[] data) {
        buildminHeap(n + left, data);
        for(int i = n + left; i <= right; i++) {
            adjust(i, n + left, data);
        }
    }

    private void buildmaxHeap(int n, int[] data) {
        for(int i = 1 + left; i < n; i++) {
            int t = i;
            while(t != left && data[parent(t)] < data[t]) {
                int temp = data[t];
                data[t] = data[parent(t)];
                data[parent(t)] = temp;
                t = parent(t);
            }
        }
    }

    private void adjustmax(int i, int n, int[] data) {
        if(data[i] >= data[left]) {
            return;
        }
        int temp = data[i];
        data[i] = data[left];
        data[left] = temp;
        int t = left;
        while( (left(t) < n && data[t] < data[left(t)])
                || (right(t) < n && data[t] < data[right(t)]) ) {
            if(right(t) < n && data[right(t)] > data[left(t)]) {
                temp = data[t];
                data[t] = data[right(t)];
                data[right(t)] = temp;
                t = right(t);
            } else {
                temp = data[t];
                data[t] = data[left(t)];
                data[left(t)] = temp;
                t = left(t);
            }
        }
    }

    public void findTailN(int n, int[] data) {
        buildmaxHeap(n + left, data);
        for(int i = n + left; i <= right; i++) {
            adjustmax(i, n + left, data);
        }
    }

    public void print(int[] data) {
        for(int i = 0; i < data.length; i++) {
            System.out.print(data[i] + " ");
        }
        System.out.println();
    }

    public static void findk(int arr[], int k, int left, int right) {
        int localk = k - left - 1;
        AS1_116010276_HeapSort topN = new AS1_116010276_HeapSort(left, right);

        if (right - k > k - left) {
            topN.findTailN(localk + 1, arr);
            System.out.println("e");
        } else {
            topN.findTopN(right - left - localk + 1, arr);
        }
        swap(arr, left, k - 1);
    }

    public static void findk(int arr[], int k) {
        findk(arr, k, 0, arr.length - 1);
    }

    private static void swap(int[] arr, int i, int j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }

    public static void main(String[] args) {

        // Note that the code here is only for testing the heap sort
        int k = 8;
        int[] arr1 = new int[]{0, 1, 24, 10, 9, 93, 8, 0, 20, 9, 68, 34};

        findk(arr1, k);
        System.out.println(arr1[k - 1]);

        Arrays.sort(arr1);
        System.out.println(arr1[k - 1]);

    }
}
