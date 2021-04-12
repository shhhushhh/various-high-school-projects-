/**
  Shirley Li
  CS106B
  Assignment 3
  Merges queues with three methods: binary merging, multiple iterating merging, and multiple recursive merging.
  **/
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;


Queue<int> mergeHelper(Queue<int> queue); //chose to use a helper function b/c decomposition

/**
 * @brief merge
 *      Merges the two queues entered in order
 * @param a
 *      one of the queues
 * @param b
 *      one of the queues
 * @return result
 *      the resultant queue of the inputted queues; sorted in order
 */
Queue<int> merge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    mergeHelper(a);
    mergeHelper(b);
    if (a.size() < b.size()) {
        while (!b.isEmpty()) {
            if (!a.isEmpty()) {
                int firstA = a.peek();
                int firstB = b.peek();
                if (firstA < firstB) {
                    result.add(firstA);
                    a.dequeue();
                }
                else {
                    result.add(firstB);
                    b.dequeue();
                }
            }
            else {
                result.enqueue(b.dequeue());
            }
        }
    }
    else if (a.size() > b.size()){
        while (!a.isEmpty()) {
            if (!b.isEmpty()) {
                int firstA = a.peek();
                int firstB = b.peek();
                if (firstA < firstB) {
                    result.add(firstA);
                    a.dequeue();
                }
                else {
                    result.add(firstB);
                    b.dequeue();
                }
            }
            else {
                result.enqueue(a.dequeue());
            }
        }

    }
    else {
        while (!a.isEmpty() && !b.isEmpty()) {
            int firstA = a.peek();
            int firstB = b.peek();
            if (firstA < firstB) {
                result.add(firstA);
                a.dequeue();
            }
            else {
                result.add(firstB);
                b.dequeue();
            }
        }

    }
    if (!a.isEmpty()) {
        while (!a.isEmpty()) {
            result.add(a.dequeue());
        }
    }
    else {
        while (!b.isEmpty()) {
            result.add(b.dequeue());
        }
    }
    return result;
}

/**
 * @brief mergeHelper
 *      Checks if the two queues are in order
 * @param queue
 *      the queue being examined
 * @return queue
 *      if in order; otherwise, will raise an error
 */
Queue<int> mergeHelper(Queue<int> queue) {
    if (queue.size() > 1) {
        int first = queue.dequeue();
        while (!queue.isEmpty()) {
            if (first > queue.peek()) {
                error("Input queues are not in order");
            }
            else {
                first = queue.dequeue();
            }
        }
    }
    return queue;
}

/* 
 * This function assumes correct functionality of the previously
 * defined merge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented – 
 * it does not need to modified at all.
 */
Queue<int> multiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    for (Queue<int>& q : all) {
        result = merge(q, result);
    }
    return result;
}

/**
 * @brief recMultiMerge
 *      Multi-merges recursively
 * @param all
 *      the entered vector
 * @return merged result
 *      the merged version of all the queues in all
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    if (all.size() == 1) {
        return merge(result, all[0]);
    }
    else {
        Vector<Queue<int>> left = all.subList(0, all.size() / 2);
        Vector<Queue<int>> right = all.subList(all.size() / 2);
        return merge(recMultiMerge(left), recMultiMerge(right));
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("Test binary merge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

PROVIDED_TEST("Test multiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(multiMerge(all), expected);
}

PROVIDED_TEST("Test recMultiMerge by compare to multiMerge") {
    int n = 1000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(multiMerge(all), recMultiMerge(all));
}

PROVIDED_TEST("Time binary merge operation") {
    int n = 1500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

PROVIDED_TEST("Time multiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("merge empty queues") {
    Queue<int> a = {};
    Queue<int> b = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

STUDENT_TEST("merge one empty queue with a filled queue") {
    Queue<int> a = {};
    Queue<int> b = {4, 5, 6};
    Queue<int> expected = {4, 5, 6};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

STUDENT_TEST("merge one empty queue with an incorrently ordered filled queue") {
    Queue<int> a = {};
    Queue<int> b = {5, 7, 2};
    EXPECT_ERROR(merge(a, b));
    EXPECT_ERROR(merge(b, a));
}

STUDENT_TEST("merge two filled queues of different lengths") {
    Queue<int> a = {1};
    Queue<int> b = {4, 5, 6};
    Queue<int> expected = {1, 4, 5, 6};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

STUDENT_TEST("merge two filled queues of different lengths") {
    Queue<int> a = {6, 9, 10, 24, 32};
    Queue<int> b = {4, 5, 6};
    Queue<int> expected = {4, 5, 6, 6, 9, 10, 24, 32};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

STUDENT_TEST("merge two filled queues of different lengths incorrently ordered") {
    Queue<int> a = {6, 9, 32, 24, 1};
    Queue<int> b = {4, 5, 6};
    EXPECT_ERROR(merge(a, b));
}

STUDENT_TEST("merge two filled queues of different lengths with shorter queue containing largest number") {
    Queue<int> a = {6, 9, 10, 24, 32};
    Queue<int> b = {1500};
    Queue<int> expected = {6, 9, 10, 24, 32, 1500};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

STUDENT_TEST("merge two filled queues of different lengths with shorter queue containing smallest number") {
    Queue<int> a = {6, 9, 10, 24, 32};
    Queue<int> b = {1};
    Queue<int> expected = {1, 6, 9, 10, 24, 32};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

STUDENT_TEST("Time binary merge operation 4 of 'em") {
    int n = 150;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
    n = 1500;
    a = createSequence(n);
    b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
    n = 15000;
    a = createSequence(n);
    b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
    n = 150000;
    a = createSequence(n);
    b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

STUDENT_TEST("multimerge empty queues") {
    Vector<Queue<int>> all = { {}, {} };
    Queue<int> expected = {};
    EXPECT_EQUAL(multiMerge(all), expected);
}

STUDENT_TEST("multimerge many empty queues with several numbers") {
    Vector<Queue<int>> all = { {}, {}, {14, 76}, {}, {95}, {}, {}, {14, 17} };
    Queue<int> expected = {14, 14, 17, 76, 95};
    EXPECT_EQUAL(multiMerge(all), expected);
}

STUDENT_TEST("Time multiMerge operation different sizes of n") {
    int n = 1000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));

}
STUDENT_TEST("Time multiMerge operation different sizes of n") {
    int n = 2000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));

}
STUDENT_TEST("Time multiMerge operation different sizes of n") {
    int n = 3000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));

}

STUDENT_TEST("Time multiMerge operation different sizes of n") {
    int n = 4000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));

}

STUDENT_TEST("Time multiMerge operation different sizes of n") {
    int n = 5000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));

}

STUDENT_TEST("Time multiMerge operation different sizes of k") {
    int n = 10000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time multiMerge operation different sizes of k") {
    int n = 10000;
    int k = n/5;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time multiMerge operation different sizes of k") {
    int n = 10000;
    int k = n/2;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time multiMerge operation different sizes of k") {
    int n = 10000;
    int k = n/4;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time multiMerge operation different sizes of k") {
    int n = 10000;
    int k = n/25;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 100000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 200000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 300000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 400000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 500000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 200000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 200000;
    int k = n/2;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 200000;
    int k = n/5;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 200000;
    int k = n/20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 200000;
    int k = n/4;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].add(input.dequeue());
    }
}
