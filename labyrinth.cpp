/**
  Shirley Li
  CS106B
  Assignment 5
  Checks if an inputted path is valid in a given labyrinth
  */
#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "testing/SimpleTest.h"
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
const string kYourName = "Shirley Li";

/* Change this constant to the path out of your labyrinth. */
const string kPathOutOfNormalMaze = "ESWSSEEWNENSENN";

string getValidMoves(MazeCell* current);

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to find all items in the needed set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */
bool isPathToFreedom(MazeCell* start, string moves, Set<string> needs) {
    string allMoves;
    if (needs.contains(start->contents)) {
        needs.remove(start->contents);
    }
    for (int idx = 0; idx < moves.length(); idx++) {
        if (moves[idx] != 'N' && moves[idx] != 'S'
                && moves[idx] != 'E' && moves[idx] != 'W') {
            error("Input path contains invalid characters");
        }
        if (needs.isEmpty()) {
            break;
        }
        allMoves = getValidMoves(start);
        for (char move : allMoves) {
            if (moves[idx] == move) {
                start = (move == 'N') ? start->north : start;
                start = (move == 'S') ? start->south : start;
                start = (move == 'E') ? start->east : start;
                start = (move == 'W') ? start->west : start;
                if (needs.contains(start->contents)) {
                    needs.remove(start->contents);
                }
                break;
            } else if (move == allMoves.length() - 1) {
                return false;
            }
        }
    }
    if (!needs.isEmpty()) {
        return false;
    }
    return true;
}

/**
 * @brief getValidMoves
 *      gets all valid moves for a given location in the maze
 * @param current
 *      the current location
 * @return moves
 *      string with all valid directions to move in
 */
string getValidMoves(MazeCell* current) {
    string moves = "";
    if (current->north != nullptr) {
        moves += "N";
    }
    if (current->south != nullptr) {
        moves += "S";
    }
    if (current->east != nullptr) {
        moves += "E";
    }
    if (current->west != nullptr) {
        moves += "W";
    }
    return moves;
}

/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("Check paths in the sample from writeup") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES", allThree));

    /* This path works to pick up just potion */
    EXPECT(isPathToFreedom(maze[2][2], "ESNW", {"Potion"}));

    /* This path doesn't work, since doesn't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW", allThree));

    /* This path doesn't work, cannot walk through walls. */
    EXPECT(!isPathToFreedom(maze[2][2], "NNWWSSSEEE", allThree));
}

PROVIDED_TEST("Works when start on an item") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("Reports errors if given illegal characters") {
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Q", {"Wand"}));
    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Ee", {"Wand"}));
}

PROVIDED_TEST("Escape from your personal labyrinth!") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName != "TODO");
    MazeCell* startLocation = mazeFor(kYourName);

    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathOutOfNormalMaze != "TODO");
    EXPECT(isPathToFreedom(startLocation, kPathOutOfNormalMaze, allThree));
}

