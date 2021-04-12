/**
  Shirley Li
  CS106B
  Assignment 3
  This file deletes the non-operator items in a string and then
  checks if the string is balanced or not.
  **/

#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

string operatorsOnlyHelper(string s, string soFar);
/**
 * @brief operatorsOnly
 *      Calls a helper function to get the operators only string
 * @param s
 *      the string we want to examine for operators
 * @return result
 *      the string containing the operators in the original string
 */
string operatorsOnly(string s) {
    string result = operatorsOnlyHelper(s, "");
    return result;
}

/**
 * @brief operatorsOnlyHelper
 *      the helper function that recursively computes a string containing the operators
 *      from the original string
 * @param s
 *      the original string
 * @param soFar
 *      the string used to keep track of all the operators
 * @return soFar
 *      the string containing all operators
 */
string operatorsOnlyHelper(string s, string soFar) {
    if (s.length() == 0) {
        return soFar;
    }
    else {
        if (s[0] == '(' || s[0] == ')'
                || s[0] == '[' || s[0] == ']'
                || s[0] == '{' || s[0] == '}') {
            soFar += s[0];

        }
        s = s.substr(1);
        return operatorsOnlyHelper(s, soFar);
    }
}

bool checkOperatorsHelper(string s, string soFar);

/**
 * @brief checkOperators
 *      Calls helper function to check if string is balanced
 * @param s
 *      the input
 * @return result
 *      true or false
 */
bool checkOperators(string s) {
    string soFar = "";
    bool result = checkOperatorsHelper(s, soFar);   //had a surprised pikachu face moment here when I got to the bottom of the instructions webpage and I saw that I wasn't supposed to use any ADTs
    return result;
}

/**
 * @brief checkOperatorsHelper
 *      Checks if the string inputted is balanced in terms of operators
 * @param s
 *      the original string
 * @param soFar
 *      stack used to keep track of operators
 * @return true or false
 *      depending on whether or not string is balanced
 */
bool checkOperatorsHelper(string s, string soFar) {
    if (s.length() == 0 && soFar.length() != 0) {
        return false;
    }
    else if (s.length() == 0) {
        return true;
    }
    else {
        char mostRecent;
        if (soFar.length() != 0) {
            mostRecent = soFar[soFar.length() - 1];
        }
        char ch = s[0];
        if ((mostRecent == '(' && ch == ')')
                || (mostRecent == '{' && ch == '}')
                || (mostRecent == '[' && ch == ']')) {
            soFar = soFar.substr(0, soFar.length() - 1);
        }
        else {
            soFar += ch;
        }
        s = s.substr(1);
        return checkOperatorsHelper(s, soFar);
    }
}

/* 
 * This function assumes correct implementation of the previous 
 * two functions. It uses the operatorsOnly function to strip out
 * all characters from the provided string and then checks to see
 * whether the remaining operators are balanced by using the 
 * checkOperators function. All in all, this function is able to 
 * correctly determine whether a snippet of code has correctly
 * balanced bracketing operators. You should not need to modify this
 * function if the previous two functions have been implemented
 * correctly. 
 */
bool isBalanced(string str) {
    string ops = operatorsOnly(str);
    cout << ops << endl;
    return checkOperators(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsOnly on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    string only = "(){([])(())}";
    EXPECT_EQUAL(operatorsOnly(example), only);
}

PROVIDED_TEST("checkOperators on example from writeup") {
    string only = "(){([])(())}";
    EXPECT(checkOperators(only));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on illegal examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

STUDENT_TEST("operatorsOnly empty string") {
    string example ="";
    string only = "";
    EXPECT_EQUAL(operatorsOnly(example), only);
}

STUDENT_TEST("operatorsOnly numbers and punctuation") {
    string example ="123...{0//&%98";
    string only = "{";
    EXPECT_EQUAL(operatorsOnly(example), only);
}

STUDENT_TEST("checkOperators empty string") {
    EXPECT(isBalanced(""));
}

STUDENT_TEST("checkOperators numbers and punctuation") {
    EXPECT(!isBalanced("123...{0//&%98"));
}

STUDENT_TEST("checkOperators phrase-to-empty string true") {
    EXPECT(isBalanced("milk tea"));
}

STUDENT_TEST("checkOperators phrase plus operators true") {
    EXPECT(isBalanced("[milk tea]"));
}

STUDENT_TEST("checkOperators complicated and long false") {
    EXPECT(!isBalanced("(thelskn1493[{fhdslf}fhdk)dklsnm{sjds(*&%#nf]"));
}
