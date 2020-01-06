public class brando {
    private static void swap(int[] data, int i, int j) {
        int temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }

    private static int median3(int[] data, int leftIdx, int rightIdx) {
        int centerIdx = (leftIdx + rightIdx) / 2;
        if (data[centerIdx] < data[leftIdx]) {
            swap(data, leftIdx, centerIdx);
        }
        if (data[rightIdx] < data[leftIdx]) {
            swap(data, rightIdx, leftIdx);
        }
        if (data[rightIdx] < data[centerIdx]) {
            swap(data, centerIdx, rightIdx);
        }
        // place pivot at the head
        swap(data, centerIdx, leftIdx);
        return data[leftIdx];  // return pivot value
    }

    private static int partition(int[] data, int start, int end) {
        int pivot = median3(data, start, end - 1);
        while (start < end) {
            while (start < end && data[--end] >= pivot) ;
            data[start] = data[end];
            while (start < end && data[++start] <= pivot) ;
            data[end] = data[start];
        }
        data[start] = pivot;
        return start;
    }

    public static int quickSelectFind(int k, int[] data) {
        if (data == null || data.length == 0) {
            return Integer.MIN_VALUE;
        }
        int returnValue = Integer.MIN_VALUE, start = 0, end = data.length;
        while (start < end) {
            int pos = partition(data, start, end);
            if (pos == k - 1) {
                returnValue = data[pos];
                break;
            } else if (pos > k - 1) {
                end = pos;
            } else {
                start = pos + 1;
            }
        }
        return returnValue;
    }

    // calculate parent index
    private static int parentIdx(int idx) {
        return (idx - 1) / 2;
    }

    // calculate left child index
    private static int leftChildIdx(int idx) {
        return 2 * idx + 1;
    }

    // calculate right child index
    private static int rightChildIdx(int idx) {
        return 2 * idx + 2;
    }

    // build min heap structure
    private static void buildMinHeap(int n, int[] data) {
        for (int i = 1; i < n; i++) {
            int nodeIdx = i;
            // arrange min heap
            while (nodeIdx != 0 && data[parentIdx(nodeIdx)] > data[nodeIdx]) {
                swap(data, nodeIdx, parentIdx(nodeIdx));
                nodeIdx = parentIdx(nodeIdx);
            }
        }
    }

    // build max heap structure
    private static void buildMaxHeap(int n, int[] data) {
        for (int i = 1; i < n; i++) {
            int nodeIdx = i;
            // arrange max heap
            while (nodeIdx != 0 && data[parentIdx(nodeIdx)] < data[nodeIdx]) {
                swap(data, parentIdx(nodeIdx), nodeIdx);
                nodeIdx = parentIdx(nodeIdx);
            }
        }
    }

    // maintain the min heap while inserting values
    private static void adjustMinHeap(int insertIdx, int k, int[] data) {
        if (data[insertIdx] <= data[0]) {  // insert value should be greater than root
            return;
        }
        swap(data, insertIdx, 0);
        int parentIdx = 0;
        while (parentIdx < k) {  // index less than k is in the min heap
            int leftChildIdx = leftChildIdx(parentIdx);
            int rightChildIdx = rightChildIdx(parentIdx);
            int minIdx = parentIdx;  // the initial minimum value index is the same as parent's
            if (leftChildIdx < k && data[parentIdx] > data[leftChildIdx]) {
                minIdx = leftChildIdx;
            }
            if (rightChildIdx < k && data[minIdx] > data[rightChildIdx]) {
                minIdx = rightChildIdx;
            }
            if (minIdx == parentIdx) {
                break;
            } else {
                swap(data, parentIdx, minIdx);
                parentIdx = minIdx;
            }
        }
    }

    // maintain the max heap while inserting values
    private static void adjustMaxHeap(int insertIdx, int k, int[] data) {
        if (data[insertIdx] >= data[0]) {  // insert value should be less than root
            return;
        }
        swap(data, insertIdx, 0);
        int parentIdx = 0;
        while (parentIdx < k) {  // index less than k is in the max heap
            int leftChildIdx = leftChildIdx(parentIdx);
            int rightChildIdx = rightChildIdx(parentIdx);
            int maxIdx = parentIdx; // the initial maximum value index is the same as parent's
            if (leftChildIdx < k && data[parentIdx] < data[leftChildIdx]) {
                maxIdx = leftChildIdx;
            }
            if (rightChildIdx < k && data[maxIdx] < data[rightChildIdx]) {
                maxIdx = rightChildIdx;
            }
            if (maxIdx == parentIdx) {
                break;
            } else {
                swap(data, parentIdx, maxIdx);
                parentIdx = maxIdx;
            }
        }
    }

    public static int heapFind(int k, int[] data) {
        if (k < data.length / 2) {
            buildMaxHeap(k, data);
            for (int i = k; i < data.length; i++) {
                adjustMaxHeap(i, k, data);
            }
        } else {
            int numHeapElement = data.length - k + 1;  // number of heap elements
            buildMinHeap(numHeapElement, data);
            for (int i = numHeapElement; i < data.length; i++) {
                adjustMinHeap(i, numHeapElement, data);
            }
        }
        return data[0];
    }

    public static int find(int k, int[] data) {
        if (data == null || data.length == 0) {
            return Integer.MIN_VALUE;
        }
        int returnValue = Integer.MIN_VALUE, start = 0, end = data.length, offset = 0, subLength = 0, pos = 0;
        while (start < end) {
            pos = partition(data, start, end);
            offset = end - pos;
            subLength = end - start;
            if (offset < 0.5 * subLength && (k < 0.001 * data.length || k > 0.999 * data.length)) {  // change approach condition
                return heapFind(k, data);
            }
            if (pos == k - 1) {
                returnValue = data[pos];
                break;
            } else if (pos > k - 1) {
                end = pos;
            } else {
                start = pos + 1;
            }
        }
        return returnValue;
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

//        /* Test the program */
//        long startTime, endTime;
//        test = randomArray(n);
//        k = (int) (Math.random() * n) + 1;
//        startTime = System.currentTimeMillis();    //获取开始时间
//        System.out.printf("Trial: The %dth largest number is %d.\n", k, find(test, k));
//        endTime = System.currentTimeMillis();    //获取结束时间
//        System.out.println("平均程序运行时间：" + (endTime - startTime) + "ms");    //输出程序运行时间
//        /* Test ends */


        /* my own test in finding the average runtime */
        int round = 50;
        long totalq = 0;
        long startTimeq, endTimeq;
        for (int i = 0; i < round; i++) {

            test = AS1_116010276.randomArray(n);
            k = (int) (Math.random() * n);

            startTimeq = System.currentTimeMillis();    //获取开始时间
            find(k, test);
            endTimeq = System.currentTimeMillis();    //获取结束时间

            totalq += (endTimeq - startTimeq);
        }

        System.out.println("Quick程序运行时间：" + (totalq / round) + "ms");

//        /* Use the @baseCase to check if the result is right */
//        long startTimeB = System.currentTimeMillis();    //获取开始时间
//        System.out.printf("BaseSort: The %dth largest number is %d.\n", k, baseCase(test, k));
//        long endTimeB = System.currentTimeMillis();    //获取结束时间
//        System.out.println("基础运行时间：" + (endTimeB - startTimeB) + "ms");    //输出程序运行时间
//        /* Check ends */
    }

}
