import org.omg.PortableServer.LIFESPAN_POLICY_ID;

public class linkedList {
    static class ListNode {
        int val;
        ListNode next;
        ListNode(int x) { val = x; }
    }

    ListNode head;

    public void initHead(int[] elements) {
        head = new ListNode(elements[0]);
        ListNode pointer;
        pointer = head;
        for (int i = 1; i < elements.length; i++) {
            pointer.next = new ListNode(elements[i]);
            pointer = pointer.next;
        }
    }

    public void deleteNode(ListNode node) {
        if (head.val == node.val) {
            head = head.next;
            return;
        }

        ListNode pointer = head;
        while (pointer.next != null) {
            if (pointer.next.val == node.val) {
                pointer.next = pointer.next.next;
                return;
            }
            pointer = pointer.next;
        }

        System.out.println("Element not exist");
    }

    public ListNode removeNthFromEnd(ListNode head, int n) {
        ListNode toDelete = head;
        ListNode pointer = head;
        int count = 0;
        while (pointer != null) {
            if (count > n) {
                toDelete = toDelete.next;
            }

            count++;
            pointer = pointer.next;
        }

        System.out.println(count);

        if (count < 2) {
            head = null;
        } else if (n == count) {
            head = head.next;
        } else {
            toDelete.next = toDelete.next.next;
        }

        return head;
    }

    public ListNode reverseList(ListNode head) {
        if (head == null) {
            return null;
        }
        ListNode pointer;
        ListNode newHead;
        newHead = head;
        pointer = head.next;
        newHead.next = null;

        while (pointer != null) {
            ListNode tmp = pointer;
            pointer = pointer.next;

            tmp.next = newHead;
            newHead = tmp;
        }

        return newHead;
    }

    @Override
    public String toString() {
        String toReturn = "linkedList{ ";
        ListNode pointer = head;
        while (pointer != null) {
            toReturn += pointer.val + " ";
            pointer = pointer.next;
        }

        toReturn += "}";
        return toReturn;
    }

    public ListNode mergeKLists(ListNode[] lists) {
        if (lists.length == 0) return null;
        return mergeHelper(lists, lists.length);
    }

    public ListNode mergeHelper(ListNode[] lists, int n) {
        if (n == 1) {
            return lists[0];
        } else if (n == 0) {
            return null;
        }

        ListNode[] left = new ListNode[n / 2];
        ListNode[] right = new ListNode[n - n / 2];
        System.arraycopy(lists, 0, left, 0, n / 2);
        System.arraycopy(lists, n / 2, right, 0, n - n / 2);

        ListNode m_left = mergeHelper(left, left.length);
        ListNode m_right = mergeHelper(right, right.length);

        ListNode toReturn = new ListNode(0);
        ListNode ptr = toReturn;
        while (m_left != null && m_right != null) {
            int tmp;
            if (m_right.val > m_left.val) {
                tmp = m_left.val;
                m_left = m_left.next;
            } else {
                tmp = m_right.val;
                m_right = m_right.next;
            }
            ptr.next = new ListNode(tmp);
            ptr = ptr.next;
        }

        if (m_left != null) {
            ptr.next = m_left;
        } else {
            ptr.next = m_right;
        }

        toReturn = toReturn.next;

        return toReturn;
    }




    public static void main(String[] args) {
        linkedList test = new linkedList();
        test.initHead(new int[]{2});
        System.out.println(test);
        test.head = test.reverseList(test.head);
        System.out.println(test);
    }
}
