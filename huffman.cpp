/**
  Shirley Li
  CS106B
  Assignment 6
  Citations: Section handout for areEqual
  Compresses and decompresses strings with Huffman coding trees
  */
#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

void encodeTextHelper(EncodingTreeNode* tree, string text, Vector<Bit> path, Map<char, Vector<Bit>>& map);

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * bits queue contains a valid sequence of encoded bits.
 *
 * @brief decodeText
 *      decodes the text
 * @param tree
 *      the tree
 * @param messageBits
 *      the queue of bits
 * @return s
 *      the text
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string s;
    EncodingTreeNode* root = tree;
    EncodingTreeNode* cur;
    while (!messageBits.isEmpty()) {
        cur = root;
        while (cur->zero) {
            Bit b = messageBits.dequeue();
            if (b == 0){
                cur = cur->zero;
            } else {
                cur = cur->one;
            }
        }
        s += cur->ch;
    }
    return s;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the input Queues are well-formed and represent
 * a valid encoding tree.
 *
 * @brief unflattenTree
 *      unflattens a tree with given nodes and leaves
 * @param treeBits
 *      queue of bits telling which nodes are/are not leaves
 * @param treeLeaves
 *      the chars of the leaves
 * @return root
 *      the root of the tree
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeBits, Queue<char>& treeLeaves) {
    Bit b = treeBits.dequeue();
    EncodingTreeNode* root;
    if (b == 0) {
        root = new EncodingTreeNode(treeLeaves.dequeue());
    } else {
        root = new EncodingTreeNode(unflattenTree(treeBits, treeLeaves), unflattenTree(treeBits, treeLeaves));
    }
    return root;
}


/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * @brief decompress
 *      decompresses the compressed message
 * @param data
 *      the struct encoding the queues needed for decyphering the message
 * @return msg
 *      the decompressed message
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* root = unflattenTree(data.treeBits, data.treeLeaves);
    string msg = decodeText(root, data.messageBits);
    deallocateTree(root);
    return msg;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * @brief buildHuffmanTree
 *      builds a Huffman coding tree for the given string
 * @param text
 *      the given string
 * @return root of resulting tree
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    Map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }
    Vector<char> keys = freqMap.keys();
    if (keys.size() < 2) {
        error("Cannot build Huffman coding tree for string with only one letter");
    }
    PriorityQueue<EncodingTreeNode*> branches;
    for (char key : keys) {
        EncodingTreeNode* leaf = new EncodingTreeNode(key);
        branches.enqueue(leaf, freqMap[key]);
    }
    while (branches.size() >= 2) {
        int zeroPriority = branches.peekPriority();
        EncodingTreeNode* zero = branches.dequeue();
        int onePriority = branches.peekPriority();
        EncodingTreeNode* one = branches.dequeue();
        EncodingTreeNode* parent = new EncodingTreeNode(zero, one);
        branches.enqueue(parent, zeroPriority + onePriority);
    }
    return branches.dequeue();
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * @brief encodeText
 *      encodes the given string according to tree
 * @param tree
 *      the given Huffman coding tree
 * @param text
 *      the message
 * @return code
 *      the priority queue containing the messageBits for text
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Map<char, Vector<Bit>> paths;
    Vector<Bit> path;
    Queue<Bit> code;
    encodeTextHelper(tree, text, path, paths);
    for (char ch : text) {
        path = paths[ch];
        for (Bit b : path) {
            code.enqueue(b);
        }
    }
    return code;
}

/**
 * @brief encodeTextHelper
 *      helps textHelper with finding a path for every character in tree
 * @param tree
 *      the given tree
 * @param text
 *      the given string
 * @param path
 *      the current path
 * @param map
 *      map of paths for all chars
 */
void encodeTextHelper(EncodingTreeNode* tree, string text, Vector<Bit> path, Map<char, Vector<Bit>>& map) {
    if (!tree->zero) {
        map[tree->ch] = path;
        return;
    }
    path.add(0);
    encodeTextHelper(tree->zero, text, path, map);
    path.remove(path.size() - 1);
    path.add(1);
    encodeTextHelper(tree->one, text, path, map);
    path.remove(path.size() - 1);

}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input Queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * @brief flattenTree
 *      takes a tree and turns it into a priority queue of bits and a priority queue of chars
 * @param tree
 *      the given tree
 * @param treeBits
 *      priority queue that contains bits demonstrating whether current node is leaf or not
 * @param treeLeaves
 *      priority queue that contains all characters of message in preorder order
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeBits, Queue<char>& treeLeaves) {
    if (!tree->zero) {
        treeLeaves.add(tree->ch);
        treeBits.add(0);
        return;
    }
    treeBits.add(1);
    flattenTree(tree->zero, treeBits, treeLeaves);
    flattenTree(tree->one, treeBits, treeLeaves);
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * @brief compress
 *      compresses the message into an EncodedData object
 * @param messageText
 *      the given message
 * @return eD
 *      EncodedData object containing queues needed for decompression
 */
EncodedData compress(string messageText) {
    EncodingTreeNode* root = buildHuffmanTree(messageText);
    Queue<Bit> messageBits = encodeText(root, messageText);
    Queue<Bit> treeBits;
    Queue<char> treeLeaves;
    flattenTree(root, treeBits, treeLeaves);
    deallocateTree(root);
    EncodedData eD = {treeBits, treeLeaves, messageBits};
    return eD;

}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    EncodingTreeNode* r = new EncodingTreeNode('R');
    EncodingTreeNode* s = new EncodingTreeNode('S');
    EncodingTreeNode* star = new EncodingTreeNode(r, s);
    EncodingTreeNode* e = new EncodingTreeNode('E');
    EncodingTreeNode* star2 = new EncodingTreeNode(star, e);
    EncodingTreeNode* t = new EncodingTreeNode('T');
    EncodingTreeNode* root = new EncodingTreeNode(t, star2);

    return root;
}

void deallocateTree(EncodingTreeNode* root) {
    EncodingTreeNode* tempLeft = root->zero;
    EncodingTreeNode* tempRight = root->one;
    delete root;
    if (!tempLeft && !tempRight) {
        return;
    }
    if (tempLeft) {
        deallocateTree(tempLeft);
    }
    if (tempRight) {
        deallocateTree(tempRight);
    }
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    if (!a->zero || !b->zero) {
        return a->ch == b->ch;
    }

    return areEqual(a->zero, b->zero) && areEqual(a->one, b->one);
}

/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("deallocateTree test") {
    EncodingTreeNode* root = createExampleTree();
    deallocateTree(root);
}

STUDENT_TEST("areEqual test equal") {
    EncodingTreeNode* root = createExampleTree();
    EncodingTreeNode* root2 = createExampleTree();
    EXPECT(areEqual(root, root2));
    deallocateTree(root);
    deallocateTree(root2);
}

STUDENT_TEST("areEqual test not equal") {
    EncodingTreeNode* root = createExampleTree();
    EncodingTreeNode* child = new EncodingTreeNode('W');
    EncodingTreeNode* child1 = new EncodingTreeNode('V');
    EncodingTreeNode* root2 = new EncodingTreeNode(child, child1);
    EXPECT(!areEqual(root, root2));
    deallocateTree(root);
    deallocateTree(root2);
}



/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeBits   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeBits, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeBits
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedBits   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeBits;
    Queue<char> treeLeaves;
    flattenTree(reference, treeBits, treeLeaves);

    EXPECT_EQUAL(treeBits,   expectedBits);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeBits    = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeBits, treeBits);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "The job requires extra pluck and zeal from every young wage earner.",
        ":-) :-D XD <(^_^)>",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(output.size(), input.size());

        /* Don't clobber the output with a huge string if there's a mismatch. */
        EXPECT(input == output);
    }
}
