/**
  Shirley Li
  CS106B
  Assignment 3
  Totals up the possible scores for one Boggle scoreboard.
  */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * @brief points
 *      Calculates the amount of points for one word
 * @param str
 *      the word
 * @return total
 *      the total amount of points for the input
 */
int points(string str) {
    int total = 0;
    if (str.length() >= 4) {
        for (int count = 3; count < str.length(); count++) {
            total++;
        }
    }
    return total;
}

void scoreBoardHelper(Grid<char>& board, Lexicon& lex, GridLocation current,
                     string soFar, Set<GridLocation> visited,
                     Set<string>& wordsFound);

Set<GridLocation> generateValidMoves(Grid<char>& board, GridLocation cur);

/**
 * @brief scoreBoard
 *      Goes through every character on the board and finds the total amount of points for them
 * @param board
 *      the Boggle board
 * @param lex
 *      the dictionary
 * @return total
 *      the total amount of points for this board
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    int total = 0;
    if (board.size() > 0) {
        Set<string> wordsFound;
        for (int row = 0; row < board.numRows(); row++) {
            for (int col = 0; col < board.numCols(); col++) {
                Set<GridLocation> visited;
                GridLocation current = GridLocation(row, col);
                scoreBoardHelper(board, lex, current,
                                 "", visited,
                                 wordsFound);
            }
        }
        for (string word : wordsFound) {
            total += points(word);
        }
    }
    return total;
}

/**
 * @brief generateValidMoves
 *      Generates the valid adjacent moves for this position
 * @param board
 *      the boggle board
 * @param cur
 *      the current location
 * @param visited
 *      a set of gridlocations showing visited
 * @return validMoves
 *      set of valid moves
 */
Set<GridLocation> generateValidMoves(Grid<char>& board, GridLocation cur, Set<GridLocation> visited) {
    Set<GridLocation> validMoves;
    for (int row = cur.row - 1; row <= cur.row + 1; row++) {
        for (int col = cur.col - 1; col <= cur.col + 1; col++) {
            if (board.inBounds(row, col) && !visited.contains(GridLocation(row, col))) {
                validMoves.add(GridLocation(row, col));
            }
        }
    }
    return validMoves;
}

/**
 * @brief scoreBoardHelper
 *      Finds all the possible words and adds it into a set
 * @param board
 *      the boggle board
 * @param lex
 *      the dictionary
 * @param current
 *      the current position on the board
 * @param soFar
 *      the string so far
 * @param visited
 *      the GridLocations that have already been visited
 * @param wordsFound
 *      set used to add all possible words in the boggle board
 */
void scoreBoardHelper(Grid<char>& board, Lexicon& lex, GridLocation current,
                     string soFar, Set<GridLocation> visited,
                     Set<string>& wordsFound) {

    Set<GridLocation> validMoves = generateValidMoves(board, current, visited);
    if (!lex.containsPrefix(soFar)) {
        return;
    } else if (lex.contains(soFar)) {
        if (!wordsFound.contains(soFar)) {
            wordsFound.add(soFar);
        }
        if (validMoves.isEmpty()) {
            return;
        }

    }
    for (GridLocation move : validMoves) {
        char ch = board[move.row][move.col];
        soFar += ch;
        visited.add(move);
        scoreBoardHelper(board, lex, move, soFar, visited, wordsFound);
        soFar = soFar.substr(0, soFar.length() - 1);
        visited.remove(move);
    }

}


/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("Test scoreBoard, empty board") {
    Grid<char> board = {};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

STUDENT_TEST("Test scoreBoard, rectangular board") { //no need to test a mangled board since Grids are always rectangular
    Grid<char> board = {{'C','_','_' },
                        {'Z','_','_' },
                        {'_','A','_' },
                        {'_','_','R' }};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

