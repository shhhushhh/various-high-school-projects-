/**
  Shirley Li
  CS106B
  Assignment 4
  Creates a PQHeap that does everything a priority queue does, but in O(log n) time.
  */
#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/**
 * @brief PQHeap::PQHeap
 *      creates a PQHeap
 */
PQHeap::PQHeap() {
    numItems = 0;
    allocatedCapacity = INITIAL_CAPACITY;
    elements = new DataPoint[allocatedCapacity];
}

/**
 * @brief PQHeap::~PQHeap
 *      deletes the array, elements
 */
PQHeap::~PQHeap() {
    delete[] elements;
}

/**
 * @brief PQHeap::enqueue
 *      adds an element to the array
 * @param elem
 *      the element to be added
 */
void PQHeap::enqueue(DataPoint elem) {
    if (numItems == allocatedCapacity) {
        expand();
    }
    numItems++;
    elements[numItems - 1] = elem;
    bubbleUp();
}

/**
 * @brief PQHeap::bubbleUp
 *      bubbles up the heap
 */
void PQHeap::bubbleUp() {
    int idx = numItems - 1;
    int parentIdx = getParentIndex(idx);
    while (parentIdx > -1 && elements[idx].priority < elements[parentIdx].priority) {
        DataPoint temp = elements[idx];
        elements[idx] = elements[parentIdx];
        elements[parentIdx] = temp;
        idx = parentIdx;
        parentIdx = getParentIndex(idx);
    }
}

/**
 * @brief PQHeap::bubbleDown
 *      bubbles down the heap
 */
void PQHeap::bubbleDown() {
    int parent = 0;
    while (true) {
        int left = getLeftChildIndex(parent);
        int right = getRightChildIndex(parent);
        int smallest = parent;

        if (left > -1 && left < numItems && elements[left].priority < elements[smallest].priority) {
            smallest = left;
        }
        if (right > -1 && right < numItems && elements[right].priority < elements[smallest].priority) {
            smallest = right;
        }
        if (smallest != parent) {
            swap(smallest, parent);
            parent = smallest;
        } else {
            break;
        }
    }
}

/**
 * @brief PQHeap::swap
 *      swaps two elements
 * @param currentIdx
 *      index of one element
 * @param otherIdx
 *      index of another element
 */
void PQHeap::swap(int currentIdx, int otherIdx) {
    DataPoint temp = elements[currentIdx];
    elements[currentIdx] = elements[otherIdx];
    elements[otherIdx] = temp;
}

/**
 * @brief PQSortedArray::expand
 *      expands elements into an array double its original size
 */
void PQHeap::expand() {
    DataPoint* newElements = new DataPoint[allocatedCapacity * 2];
    for (int idx = 0; idx < numItems; idx++) {
        newElements[idx] = elements[idx];
    }
    this->~PQHeap();
    elements = newElements;
    allocatedCapacity = allocatedCapacity * 2;
}

/**
 * @brief PQHeap::peek
 *      peeks at the element with the highest priority
 * @return elements[0]
 *      the element with the highest priority
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("Cannot peek empty pqueue");
    }
    return elements[0];
}

/**
 * @brief PQHeap::dequeue
 *      dequeues the first element in the array
 * @return elements[0]
 *      the element with the highest priority
 */
DataPoint PQHeap::dequeue() {
    if (isEmpty()) {
        error("Cannot deqeuue empty pqueue");
    }
    DataPoint temp = elements[0];
    elements[0] = elements[numItems - 1];
    numItems--;
    bubbleDown();
    return temp;
}

/**
 * @brief PQHeap::isEmpty
 *      checks if the array is empty
 * @return size of array
 *      if empty, then true; if not empty, then false
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/**
 * @brief PQHeap::size
 *      gives the size of the array
 * @return numItems
 *      the number of elements in the array
 */
int PQHeap::size() const {
    return numItems;
}

/**
 * @brief PQHeap::clear
 *      "clears the array" by setting numItems back to zero
 */
void PQHeap::clear() {
    numItems = 0;
}

/**
 * @brief PQHeap::printDebugInfo
 *      prints the debug info
 */
void PQHeap::printDebugInfo() {
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << elements[i] << endl;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState(){
    /*
     * If the array only has zero or one elements in it, then they
     * must necessarily be in sorted order.
     */
    if (size() == 0 || size() == 1){
        return;
    }

    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (size() > allocatedCapacity) error("Too many elements in not enough space!");


    /* This for loop walks over the elements in the array, comparing
     * neighboring pairs to one another. If the current element has larger
     * priority than the last element, then the array has not been maintained
     * in decreasing sorted order, and we should throw an error.
     */
    DataPoint lastElem = elements[0];
    for (int i = 1; i < size(); i++){
        DataPoint cur = elements[i];
        if (cur.priority < lastElem.priority){
            error("Internal state of the array is out of order!");
        }
        lastElem = cur;
    }
}

/* 
 * We strongly recommend implementing this helper function, which 
 * should calculate the index of the parent of the element with the
 * provided index.
 */
int PQHeap::getParentIndex(int curIndex){
    int parent = (curIndex - 1) / 2;
    parent = (parent >= 0) ? parent : -1;
    return parent;
}

/* 
 * We strongly recommend implementing this helper function, which 
 * should calculate the index of the left child of the element with the
 * provided index.
 */
int PQHeap::getLeftChildIndex(int curIndex){
    int leftChild = (2 * curIndex) + 1;
    leftChild = (leftChild < numItems) ? leftChild : -1;
    return leftChild;
}

/* 
 * We strongly recommend implementing this helper function, which 
 * should calculate the index of the right child of the element with the
 * provided index.
 */
int PQHeap::getRightChildIndex(int curIndex){
    int rightChild = (2 * curIndex) + 2;
    rightChild = (rightChild < numItems) ? rightChild : -1;
    return rightChild;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */




/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Newly-created heap is empty.") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    EXPECT(pq.size() == 0);
}

PROVIDED_TEST("Provided Test: Enqueue / dequeue single element (two cycles)") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };
    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.peek(), point);
    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.peek(), point);
    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Test clear operation works with single element."){
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Dequeue / peek on empty priority queue throws error") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Dequeue / peek on recently-cleared priority queue throws error") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Enqueue elements in sorted order.") {
    PQHeap pq;
    for (int i = 0; i < 25; i++) {
        pq.enqueue({ "elem" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 25);
    for (int i = 0; i < 25; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            "elem" + integerToString(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Enqueue elements in reverse-sorted order.") {
    PQHeap pq;
    for (int i = 25; i >= 0; i--) {
        pq.enqueue({ "elem" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 26);
    for (int i = 0; i <= 25; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            "elem" + integerToString(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Insert ascending and descending sequences.") {
    PQHeap pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), 2 * i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), 2 * i + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 40; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Insert random sequence of elements.") {
    Vector<DataPoint> sequence = {
        { "A", 0 },
        { "D", 3 },
        { "F", 5 },
        { "G", 6 },
        { "C", 2 },
        { "H", 7 },
        { "I", 8 },
        { "B", 1 },
        { "E", 4 },
        { "J", 9 },
    };

    PQHeap pq;
    for (DataPoint elem: sequence) {
        pq.enqueue(elem);
    }

    EXPECT_EQUAL(pq.size(), sequence.size());

    for (int i = 0; i < 10; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            charToString('A' + i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}


PROVIDED_TEST("Provided Test: Insert duplicate elements.") {
    PQHeap pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 20; i++) {
        DataPoint one = pq.dequeue();
        DataPoint two = pq.dequeue();

        EXPECT_EQUAL(one.priority, i);
        EXPECT_EQUAL(two.priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Handles data points with empty string name.") {
    PQHeap pq;
    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 10);
}

PROVIDED_TEST("Test enqueue/dequeue of longer random sequence") {
    PQHeap pq;

    for (int i = 0; i < 100; i++) {
        int randomValue = randomInteger(0, 100);
        DataPoint pt = {"elem" + integerToString(randomValue), randomValue};
        pq.enqueue(pt);
    }
    EXPECT_EQUAL(pq.size(), 100);
    DataPoint last = {"", -1};
    for (int i = 0; i < 100; i++) {
        DataPoint cur = pq.dequeue();
        EXPECT(cur.priority >= 0 && cur.priority <= 100 && cur.priority >= last.priority);
        last = cur;
    }
    EXPECT_EQUAL(pq.size(), 0);
}


PROVIDED_TEST("Provided Test: Handles data points with negative weights.") {
    PQHeap pq;
    for (int i = -10; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 20);
    for (int i = -10; i < 10; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
}

PROVIDED_TEST("Provided Test: Interleave enqueues and dequeues.") {
    PQHeap pq;
    int n = 100;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

static void fillAndEmpty(int n) {
    PQHeap pq;
    DataPoint max = {"max", 106106106};
    DataPoint min = {"min", -106106106};

    pq.enqueue(min);
    pq.enqueue(max);
    for (int i = 0; i < n; i++) {
        int randomPriority = randomInteger(-10000, 10000);
        pq.enqueue({ "", randomPriority });
    }
    EXPECT_EQUAL(pq.size(), n + 2);

    EXPECT_EQUAL(pq.dequeue(), min);
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.dequeue(), max);
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("Provided Test: Stress Test. Time the amount of time it takes to cycle many elements in and out. Should take at most about 5-10 seconds.") {
    TIME_OPERATION(20000, fillAndEmpty(20000));
}

STUDENT_TEST("Crazy random elements") {
    PQHeap pq;

    for (int i = 0; i < 10000000; i++) {
        int randomValue = randomInteger(0, 10000000);
        DataPoint pt = {"elem" + integerToString(randomValue), randomValue};
        pq.enqueue(pt);
    }
    EXPECT_EQUAL(pq.size(), 10000000);
    DataPoint last = {"", -1};
    for (int i = 0; i < 10000000; i++) {
        DataPoint cur = pq.dequeue();
        EXPECT(cur.priority >= 0 && cur.priority <= 10000000 && cur.priority >= last.priority);
        last = cur;
    }
    EXPECT_EQUAL(pq.size(), 0);
}

