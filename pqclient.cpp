/**
  Shirley Li
  CS106B
  Assignment 4
  Finds the k largest elements in a stream using a priority queue and stores them into a vector
  */
#include "pqclient.h"
#include "pqsortedarray.h"
#include "stack.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include <sstream>
#include "testing/SimpleTest.h"
using namespace std;

/* Using the Priority Queue data structure as a tool to sort, neat! */
void pqSort(Vector<DataPoint>& v) {
    PQHeap pq;

    /* Add all the elements to the priority queue. */
    for (int i = 0; i < v.size(); i++) {
        pq.enqueue(v[i]);
    }

    /* Extract all the elements from the priority queue. Due
     * to the priority queue property, we know that we will get
     * these elements in sorted order, in order of increasing priority
     * value.
     */
    for (int i = 0; i < v.size(); i++) {
        v[i] = pq.dequeue();
    }
}

/**
 * @brief topK
 *      Stores the k largest priority DataPoints from the stream into a vector
 * @param stream
 *      the stream
 * @param k
 *      the number of largest elements
 * @return largestValues
 *      the vector holding the k largest values in decreasing order
 */
Vector<DataPoint> topK(istream& stream, int k) {
    Vector<DataPoint> largestValues;
    PQHeap pq;

    if (k > 0) {
        DataPoint point;
        while (stream >> point) {
            if (pq.size() < k) {
                pq.enqueue(point);
            } else if (pq.size() == k){
                DataPoint smallest = pq.peek();
                if (point.priority > smallest.priority) {
                    pq.dequeue();
                    pq.enqueue(point);
                }
            }
        }
        Stack<DataPoint> dPStack;   //stack was most efficient ADT that I could think of for this type of problem
        while (!pq.isEmpty()) {
            dPStack.push(pq.dequeue());
        }
        while (!dPStack.isEmpty()) {
            largestValues.add(dPStack.pop());
        }
    }
    return largestValues;
}



/* * * * * * Test Cases Below This Point * * * * * */

/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* Helper function that, given range start and stop, produces a stream of values. */
stringstream asStream(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = { "", i };
        result << " " << pt << " ";
    }
    return result;
}

/* TODO: Add your own custom tests here! */
/**
    for (int idx = 0; idx < numItems; idx++) {
        if (elements[(2*idx) + 1].priority > elements[idx].priority ||
                elements[(2*idx) + 2].priority > elements[idx].priority) {
            if (elements[(2*idx) + 1].priority > elements[(2*idx) + 2].priority) {
                DataPoint temp = elements[idx];
                elements[idx] = elements[(2*idx) + 1];
                elements[(2*idx) + 1] = temp;
            } else {
                DataPoint temp = elements[idx];
                elements[idx] = elements[(2*idx) + 2];
                elements[(2*idx) + 2] = temp;
            }
        }
    }
  */








/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Test pqSort") {
    Vector<DataPoint> input;
    Vector<int> expectedOutput;
    for (int i = 0; i < 100; i++) {
        int randomValue = randomInteger(1, 10);
        input.add({ "elem" + integerToString(randomValue), randomValue});
        expectedOutput.add(randomValue);
    }
    expectedOutput.sort();
    pqSort(input);
    for (int i = 0; i < 100; i++) {
        EXPECT_EQUAL(input[i].priority, expectedOutput[i]);
    }
}

PROVIDED_TEST("Time operation pqSort") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        for (int i = 0; i < n; i++) {
            v.add({ "", randomInteger(1, n) });
        }
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

PROVIDED_TEST("Provided Test: Stream 0 elements, ask for top 0") {
    stringstream stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 0), expected);
}

PROVIDED_TEST("Provided Test: Stream 0 elements, ask for top 1") {
    stringstream stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("Provided Test: Stream 0 elements, ask for top many") {
    stringstream stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, kMany), expected);
}

PROVIDED_TEST("Provided Test: Stream 1 element, ask for top 0") {
    stringstream stream = asStream({ { "A" , 1 } });
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 0), expected);
}

PROVIDED_TEST("Provided Test: Stream 1 element, ask for top 1") {
    stringstream stream = asStream({ { "A" , 1 } });
    Vector<DataPoint> expected = { { "A" , 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("Provided Test: Stream 1 element, ask for top many") {
    stringstream stream = asStream({ { "A" , 1 } });
    Vector<DataPoint> expected = { { "A" , 1 } };
    EXPECT_EQUAL(topK(stream, kMany), expected);
}

PROVIDED_TEST("Provided Test: Works in a simple case.") {
    /* Build a stream with some simple elements in it. */
    Vector<DataPoint> input = {
        { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 }
    };
    stringstream stream = asStream(input);

    /* What we should see. */
    Vector<DataPoint> expected = {
        { "D", 4 }, { "C", 3 }
    };

    EXPECT(topK(stream, 2) == expected);


    stream = asStream(input);
    expected = {
        { "D", 4 }, { "C", 3 }, { "B", 2 }
    };

    EXPECT(topK(stream, 3) == expected);

    stream = asStream(input);
    expected = {
        { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 }
    };

    EXPECT(topK(stream, 4) == expected);

    stream = asStream(input);
    EXPECT(topK(stream, 6) == expected);
}

PROVIDED_TEST("Provided Test: Stream contains duplicate elements") {
    Vector<DataPoint> vec = {
        { "" , 1 },
        { "" , 3 },
        { "" , 2 },
        { "" , 1 },
        { "" , 5 },
        { "" , 4 },
        { "" , 2 },
        { "" , 3 },
        { "" , 4 },
        { "" , 5 },
    };
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = { vec[4] };
    EXPECT_EQUAL(topK(stream, 1), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4] };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4], vec[5] };
    EXPECT_EQUAL(topK(stream, 3), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4], vec[5], vec[5] };
    EXPECT_EQUAL(topK(stream, 4), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4], vec[5], vec[5], vec[1] };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("Provided Test: Stream contains negative elements") {
    Vector<DataPoint> vec = {
        { "" , 1 },
        { "" , 3 },
        { "" , 2 },
        { "" , -1 },
        { "" , -5 },
        { "" , 4 },
        { "" , -2 },
        { "" , 3 },
        { "" , -4 },
        { "" , 5 },
    };
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = { vec[9] };
    EXPECT_EQUAL(topK(stream, 1), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5] };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5], vec[1] };
    EXPECT_EQUAL(topK(stream, 3), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5], vec[1], vec[1] };
    EXPECT_EQUAL(topK(stream, 4), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5], vec[1], vec[1], vec[2] };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("Provided Test: Stream many elements, ask for top 0") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 0), expected);
}

PROVIDED_TEST("Provided Test: Stream many elements, ask for top 1") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = { { "" , kMany - 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("Provided Test: Stream many elements, ask for top 5") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = {
        { "" , kMany - 1 },
        { "" , kMany - 2 },
        { "" , kMany - 3 },
        { "" , kMany - 4 },
        { "" , kMany - 5 }
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("Provided Test: Stress Test – many elements with random values") {
    Vector<int> sorted;
    Vector<DataPoint> points;
    for (int i = 0; i < 10000; i++) {
        int weight = randomInteger(0, 100000);
        sorted.add(weight);
        points.add({ "" , weight});
    }

    stringstream stream = asStream(points);
    sort(sorted.begin(), sorted.end(), greater<int>());
    Vector<DataPoint> result = topK(stream, 10);

    EXPECT_EQUAL(result.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(result[i].priority, sorted[i]);
    }
}

//PROVIDED_TEST("Provided Test: Stress Test – Stream many elements, ask for top half. May take >5 minutes on some computers.") {
//    stringstream stream = asStream(1, kMany);
//    Vector<DataPoint> result = topK(stream, kMany/2);
//    EXPECT_EQUAL(result.size(), kMany/2);
//    EXPECT_EQUAL(result[0].priority, kMany);
//    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
//}

PROVIDED_TEST("Provided Test: Time operation top-k") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("Negative Stress Test – many elements with random negative values") {
    Vector<int> sorted;
    Vector<DataPoint> points;
    for (int i = 0; i < 10000; i++) {
        int weight = randomInteger(0, 100000);
        sorted.add(weight * -1);
        points.add({ "" , weight * -1});
    }

    stringstream stream = asStream(points);
    sort(sorted.begin(), sorted.end(), greater<int>());
    Vector<DataPoint> result = topK(stream, 10);

    EXPECT_EQUAL(result.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(result[i].priority, sorted[i]);
    }
}

STUDENT_TEST("Negative Stress Test – k as a negative int") {
    Vector<int> sorted;
    Vector<DataPoint> points;
    for (int i = 0; i < 10000; i++) {
        int weight = randomInteger(0, 100000);
        sorted.add(weight);
        points.add({ "" , weight});
    }

    stringstream stream = asStream(points);
    sort(sorted.begin(), sorted.end(), greater<int>());
    Vector<DataPoint> result = topK(stream, -1);

    EXPECT_EQUAL(result.size(), 0);
}

STUDENT_TEST("Changing n Test 1 - Time operation top-k") {
    int startSize = 100000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("Changing n Test 2 - Time operation top-k") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("Changing n Test 3 - Time operation top-k") {
    int startSize = 300000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("Changing n Test 4 - Time operation top-k") {
    int startSize = 400000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("Changing k Test 1 - Time operation top-k") {
    int startSize = 1000000;
    int k = 50;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("Changing k Test 2 - Time operation top-k") {
    int startSize = 1000000;
    int k = 20;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("Changing k Test 3 - Time operation top-k") {
    int startSize = 1000000;
    int k = 5;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("Changing k Test 4 - Time operation top-k") {
    int startSize = 1000000;
    int k = 2;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}
