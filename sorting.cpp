/**
  Shirley Li
  CS106B
  Assignment 5
  Implements mergeSort and quickSort with linked lists
  Citations: Lecture Slides and Lecture Code for Linked Lists and Sorting
  */
#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


void split(ListNode*& list, ListNode*& even, ListNode*& odd);
void merge(ListNode*& list, ListNode*& even, ListNode*& odd);
void printList(ListNode* front);
void partition(ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater, int pivot);
void concatenate(ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater);
void append(ListNode*& front, ListNode* node);

/**
 * @brief mergeSort
 *      sorts the given linked list via mergeSort
 * @param front
 *      the given linked list
 */
void mergeSort(ListNode*& front) {
    if (!front || !front->next) {
        return;
    }
    ListNode* even;
    ListNode* odd;
    split(front, even, odd);

    mergeSort(even);
    mergeSort(odd);

    front = nullptr;
    merge(front, even, odd);
}

/**
 * @brief split
 *      splits the list into even and odd linked lists (by order, not value lol)
 * @param list
 *      the main list
 * @param even
 *      the list containing listNodes of even indices
 * @param odd
 *      the list containing listNodes of odd indices
 */
void split(ListNode*& list, ListNode*& even, ListNode*& odd) {
    int idx = 0;

    odd = list;
    even = list->next;
    list = list->next->next;

    odd->next = nullptr;
    even->next = nullptr;

    ListNode* temp;
    ListNode* cur;
    while (list != nullptr) {
        if (idx % 2 == 0) {
            temp = odd;
            odd = list;
            cur = list->next;
            odd->next = temp;
        } else {
            temp = even;
            even = list;
            cur = list->next;
            even->next = temp;
        }
        idx++;
        list = cur;
    }
}

/**
 * @brief merge
 *      merges the even and odd linked lists into one
 * @param list
 *      the combined list
 * @param even
 *      the list containing the even links
 * @param odd
 *      the list containing the odd links
 */
void merge(ListNode*& list, ListNode*& even, ListNode*& odd) {
    ListNode* temp;
    while (even != nullptr && odd != nullptr) {
        if (even->data < odd->data) {
            temp = even;
            even = even->next;
            temp->next = nullptr;
            append(list, temp);
        } else {
            temp = odd;
            odd = odd->next;
            temp->next = nullptr;
            append(list, temp);
        }
    }

    if (even != nullptr) {
        append(list, even);
    }

    if (odd != nullptr) {
        append(list, odd);
    }

}

/**
 * @brief append
 *      adds a listNode to the end of another listNode
 * @param front
 *      the main listNode
 * @param node
 *      the listNode to be added
 */
void append(ListNode*& front, ListNode* node) {
    ListNode* end = front;
    while (end != nullptr && end->next != nullptr){
        end = end->next;
    }

    if (front == nullptr){
        front = node;
    } else {
        end->next = node;
    }
}

/**
 * @brief quickSort
 *      sorts given values with linked lists and quickSort algorithm
 * @param front
 *      the ListNode pointing to the front of the given linked list
 */
void quickSort(ListNode*& front) {
    if (!front || !front->next) {
        return;
    }

    ListNode* less;
    ListNode* equal;
    ListNode* greater;
    int pivot = front->data;
    partition(front, less, equal, greater, pivot);

    quickSort(less);
    quickSort(greater);

    front = nullptr;
    concatenate(front, less, equal, greater);
}

/**
 * @brief partition
 *      separates the main linked list into lesser, equal, and greater sublists
 * @param front
 *      the main linked list
 * @param less
 *      the sublist containing values less than pivot
 * @param equal
 *      the sublist containing values equal to pivot
 * @param greater
 *      the sublist containing values greater than pivot
 * @param pivot
 *      the pivot (first value of linked list)
 */
void partition(ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater, int pivot) {
    equal = front;
    less = nullptr;
    greater = nullptr;
    front = front->next;
    equal->next = nullptr;
    ListNode* temp = nullptr;

    while (front != nullptr) {
        if (front->data < pivot) {
            temp = less;
            less = front;
            front = front->next;
            less->next = temp;
        } else if (front->data == pivot) {
            temp = equal;
            equal = front;
            front = front->next;
            equal->next = temp;
        } else {
            temp = greater;
            greater = front;
            front = front->next;
            greater->next = temp;

        }
    }
}

/**
 * @brief concatenate
 *      concatenates the three sublists together into one
 * @param front
 *      the main linked list
 * @param less
 *      the sublist containing values less than pivot
 * @param equal
 *      the sublist containing values equal to pivot
 * @param greater
 *      the sublist containing values greater than pivot
 */
void concatenate(ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater) {

    ListNode* cur = less;
    ListNode* firstNode = less;
    while (cur != nullptr && cur->next != nullptr) {
        cur = cur->next;
    }

    if (cur == nullptr) {
        firstNode = equal;
        cur = equal;
    } else {
        cur->next = equal;
    }

    while (cur != nullptr && cur->next != nullptr) {
        cur = cur->next;
    }

    if (cur == nullptr){
        firstNode = equal;
        cur = greater;
    } else {
        cur->next = greater;
    }

    front = firstNode;
}

/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 *
 * As part of this process, you are also strongly recommedned to fill in
 * the function prototypes we have defined below.
 */

/*
 * We have provided this helper function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   while (front != nullptr) {
       cout << front->data;
       if (front->next != nullptr){
           cout << ", ";
       }
       front = front->next;
   }
   cout << "}" << endl;
}

/*
 * This helper function is provided a pointer to the front of the list
 * and is responsible for deallocating all of the memory being used
 * to store nodes in that list.
 */
void deallocateList(ListNode* front) {

    while (front != nullptr) {
        /* Store where to go next, since we're about to blow up our linked
         * list Node.
         */
        ListNode* next = front->next;
        delete front;
        front = next;
    }
}

/*
 * This helper function is provided a vector of integer values and
 * returns a pointer to the beginning of a linked list containing
 * those values in the specified order.
 */
ListNode* createList(Vector<int> values){
    if (values.isEmpty()) {
        error("Vector is empty; can't create linked list");
    }
    values.reverse();
    ListNode* head = new ListNode;
    head->data = values[0];
    head->next = nullptr;
    for (int idx = 1; idx < values.size(); idx++) {
        ListNode* node = new ListNode;
        node->data = values[idx];
        ListNode* temp = head;
        head = node;
        node->next = temp;
    }

    return head;
}

/*
 * This helper function is provided a vector of integer values
 * and a pointer to the beginning of a linked list. If these two structures
 * contain the same values in the same order, the function returns
 * true, otherwise it returns false.
 */
bool checkListVectorEquality(ListNode* front, Vector<int> v){
    bool same;
    int idx = 0;
    while (idx < v.size() && front != nullptr) {
        same = (front->data == v[idx]) ? true : false;
        if (!same) {
            break;
        }
        front = front->next;
        idx++;
    }
    if (front != nullptr && idx == v.size()) {
        same = false;
    }
    if (front == nullptr && idx < v.size()) {
        same = false;
    }
    return same;
}

STUDENT_TEST("This is a skeleton for what your overall student tests should look like."){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1, 2, 3, 4};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    mergeSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}

STUDENT_TEST("Empty list"){
    Vector<int> values = {};

    EXPECT_ERROR(createList(values));
}

STUDENT_TEST("Out of order list for mergeSort"){
    Vector<int> values = {3, 7, 4, 10, 1, 6};
    ListNode* list = createList(values);

    mergeSort(list);

    values.sort();

    EXPECT(checkListVectorEquality(list, values));

    deallocateList(list);
}

STUDENT_TEST("Out of order list with duplicate values (mergeSort)"){
    Vector<int> values = {3, 7, 4, 10, 1, 6, 9, 7, 15, 2, 3};
    ListNode* list = createList(values);

    mergeSort(list);

    values.sort();

    EXPECT(checkListVectorEquality(list, values));

    deallocateList(list);
}

STUDENT_TEST("Out of order list with duplicate values and negative values (mergeSort)"){
    Vector<int> values = {3, 7, 4, 10, 1, -15, 6, 9, 7, -3, 15, 2, 3, -15};
    ListNode* list = createList(values);

    mergeSort(list);

    values.sort();

    EXPECT(checkListVectorEquality(list, values));

    deallocateList(list);
}

STUDENT_TEST("Out of order list for quickSort"){
    Vector<int> values = {3, 7, 4, 10, 1, 6};
    ListNode* list = createList(values);

    quickSort(list);

    values.sort();

    EXPECT(checkListVectorEquality(list, values));

    deallocateList(list);
}

STUDENT_TEST("Out of order list with duplicate values (quickSort)"){
    Vector<int> values = {3, 7, 4, 10, 1, 6, 9, 7, 15, 2, 3};
    ListNode* list = createList(values);

    quickSort(list);

    values.sort();

    EXPECT(checkListVectorEquality(list, values));

    deallocateList(list);
}

STUDENT_TEST("Out of order list with duplicate values and negative values (quickSort)"){
    Vector<int> values = {3, 7, 4, 10, 1, -15, 6, 9, 7, -3, 15, 2, 3, -15};
    ListNode* list = createList(values);

    quickSort(list);

    values.sort();

    EXPECT(checkListVectorEquality(list, values));

    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(checkListVectorEquality(testList, {1, 2, 3}));
    EXPECT(!checkListVectorEquality(testList, {1, 2}));
    EXPECT(!checkListVectorEquality(testList, {1, 2, 3, 4}));
    EXPECT(!checkListVectorEquality(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v;
        ListNode* list = nullptr;

        /* Insert the same random values into linked list and vector. */
        for (int i = 0; i < n; i++) {
            int val = randomInteger(-10000, 10000);
            v.add(val);
            list = new ListNode(val, list);
        }

        /* The vector values are currently in the reverse order than the
         * linked list values – let's fix that so we have a fair comparison. */
        v.reverse();

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to generate a comparison. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

STUDENT_TEST("mergeSort crazy") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v;
        ListNode* list = nullptr;

        /* Insert the same random values into linked list and vector. */
        for (int i = 0; i < n; i++) {
            int val = randomInteger(-10000, 10000);
            v.add(val);
            list = new ListNode(val, list);
        }

        v.reverse();

        EXPECT(checkListVectorEquality(list, v));

        deallocateList(list);
    }
}




