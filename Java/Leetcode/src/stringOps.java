import java.util.*;

public class stringOps {
    public static String reverseString(String s) {
//        StringBuilder sb = new StringBuilder(s);
//        sb.reverse();
//        return sb.toString();


        char[] data = s.toCharArray();
        for (int i  = 0; i < s.length() / 2; i++) {
            char tmp = data[s.length() - 1 - i];
            data[s.length() - 1 - i] = data[i];
            data[i] = tmp;
        }

        return new String(data);
    }

    public static int reverse(int x) {
        boolean flag = false;
        if (x < 0) {
            flag = true;
            x = -x;
        }
        String s = "" + x;
        char[] data = s.toCharArray();
        for (int i  = 0; i < s.length() / 2; i++) {
            char tmp = data[s.length() - 1 - i];
            data[s.length() - 1 - i] = data[i];
            data[i] = tmp;
        }

        if (data.length > 10) {
            return 0;
        } else if (data.length == 10) {
            int[] ref = new int[]{2,1,4,7,4,8,3,6,4,7};
            for (int j = 0; j < ref.length; j++) {

                if ((data[j] - '0') > ref[j]) {
                    return 0;
                } else if ((data[j] - '0') < ref[j]) {
                    break;
                }
            }
        }

        String  toReturn = new String(data);
        return flag? -Integer.parseInt(toReturn) : Integer.parseInt(toReturn);
    }

    public static int firstUniqChar(String s) {
        int[] ref = new int[26];
        for (int i = 0; i < s.length(); i++) {
            ref[s.charAt(i) - 97]++;
        }

        for (int i = 0; i < s.length(); i++) {
            if (ref[s.charAt(i) - 97] == 1) {
                return i;
            }
        }

        return -1;
    }

    public static boolean isAnagram(String s, String t) {
        char[] s1 = s.toCharArray();
        char[] s2 = t.toCharArray();
        Arrays.sort(s1);
        Arrays.sort(s2);
        return Arrays.equals(s1, s2);
    }

    public static boolean isPalindrome(String s) {
        s = s.toLowerCase();
        int indexHead = 0;
        int indexTail = s.length() - 1;
        boolean flag;
        while (indexHead < indexTail) {
            flag = false;
            if (!checkAlpha(s.charAt(indexHead))) {
                flag = true;
                indexHead++;
            }
            if (!checkAlpha(s.charAt(indexTail))) {
                flag = true;
                indexTail--;
            }

            if (!flag) {
                System.out.println(indexHead + " == " + indexTail);
                if (s.charAt(indexHead) != s.charAt(indexTail)) {

                    return false;
                }
                indexHead++;
                indexTail--;
            }
        }

        return true;
    }

    private static boolean checkAlpha(char n) {
        return (n >= 'A' && n <= 'Z') || (n >= 'a' && n <= 'z') || (n >= '0' && n <= '9');
    }

    public static int myAtoi(String str) {
        int start = -1;
        int end = -1;
        for (int i = 0; i < str.length(); ++i) {
            if (Character.isDigit(str.charAt(i))) {
                if (start == -1) {
                    start = i;
                }
                end = i;
            } else if (str.charAt(i) == ' ') {
                if (start == -1) {
                    continue;
                } else {
                    break;
                }
            } else if (str.charAt(i) == '+' || str.charAt(i) == '-') {
                if (start == -1 && i + 1 < str.length() && Character.isDigit(str.charAt(i + 1))) {
                    continue;
                } else {
                    break;
                }
            } else {
                break;
            }
        }

        if (start == -1) {
            return 0;
        }

        String toReturn = str.substring(start, end + 1);

        if (start > 0 && str.charAt(start - 1) == '-') {
            toReturn = "-" + toReturn;
            toReturn = checkFlow(toReturn) ? "-2147483648": toReturn;
        } else {
            toReturn = checkFlow(toReturn) ? "2147483647": toReturn;
        }

        return Integer.valueOf(toReturn);
    }

    private static boolean checkFlow(String str) {
        int index = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str.charAt(i) == '0') {
                index++;
            } else if (str.charAt(i) == '-') {
                continue;
            } else {
                break;
            }
        }

        if (index == str.length()) {
            str = "0";
        } else {
            str = str.substring(index);
        }

        char[] data = str.toCharArray();
        char[] ref;

        if (data[0] == '-') {
            ref = new char[]{'-','2','1','4','7','4','8','3','6','4','8'};
        } else {
            ref = new char[]{'2','1','4','7','4','8','3','6','4','7'};
        }

        if(data.length < ref.length) {
            return false;
        } else if (data.length > ref.length) {
            return true;
        }

        for (int j = 0; j < ref.length; j++) {

            if (data[j] > ref[j]) {
                return true;
            } else if (data[j] < ref[j]) {
                break;
            }
        }
        return false;
    }

    public static int strStr(String haystack, String needle) {
        if (needle == null ||needle.length() == 0) {
            return 0;
        }
        if (needle.length() > haystack.length()) {
            return -1;
        }

        int trackFlag = -1;
        for (int i = 0; i < haystack.length() - needle.length() + 1; i++) {
            if (haystack.charAt(i) == needle.charAt(0)) {
                trackFlag = i;
                for (int j = 1; j < needle.length(); j++) {
                    if (haystack.charAt(i + j) != needle.charAt(j)) {
                        trackFlag = -1;
                        break;
                    }
                }
                if (trackFlag != -1) break;
            }
        }

        return trackFlag;
    }

    public static String countAndSay(int n) {
        if (n < 0 || n > 30) {
            return "Invalid Input";
        }
        String start = "1";
        char type = 0;
        int count = 0;
        while (n > 1) {
            String next = "";
            count = 0;
            type = 0;
            for (int i = 0; i < start.length(); i++) {
                if (type != start.charAt(i)) {
                    if (type != 0) next = next + "" + count + "" + type;
                    type = start.charAt(i);
                    count = 1;
                } else {
                    count++;
                }
            }
            next = next + "" + count + "" + type;
            start = next;
            n--;
        }

        return start;
    }

    public static String longestCommonPrefix(String[] strs) {
        if (strs.length == 0) {
            return "";
        }

        int index = 0;
        boolean isEqual = true;
        while (isEqual) {
            if (index >= strs[0].length()) {
                isEqual = false;
            }

            for (int i = 0; i < strs.length - 1; i++) {
                if (index >= strs[i].length() || index >= strs[i + 1].length()) {
                    isEqual = false;
                    break;
                }

                if (strs[i].charAt(index) != strs[i + 1].charAt(index)) {
                    isEqual = false;
                    break;
                }
            }
            index++;
        }

        return strs[0].substring(0, index - 1);
    }

    public static void main(String[] args) {
//        System.out.println(reverseString("asdefg"));
//        System.out.println(reverse(-2147483412));
//        System.out.println(firstUniqChar("loveleetcode"));
//        System.out.println(isAnagram("qas", "scq"));
//        System.out.println(isPalindrome("0P"));
//        int[][][] giant = new int[1000][1000][1000];
//        System.out.println(myAtoi("+"));
//        System.out.println('9' + "");
//        System.out.println(strStr("mississippi", "p"));
//        System.out.println(countAndSay(6));
        String[] test = new String[]{"flower","flow"};
        System.out.println(longestCommonPrefix(test));
    }
}
