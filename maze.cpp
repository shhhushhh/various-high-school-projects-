/**
 * Shirley Li
 * CS106B
 * Assignment 2
 * Creates a maze and calculates the path from the start to the end of the maze without utilizing
 * exhaustive algorithms.
 */

#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "set.h"
#include "maze.h"
#include "mazegraphics.h"
#include "testing/SimpleTest.h"
using namespace std;


/**
 * @brief generateValidMoves
 *      Checks if there are valid one-step away moves
 * @param maze
 *      the maze
 * @param cur
 *      the current location
 * @return neighbors
 *      set of all valid moves
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    //cout << cur.row << ", " << cur.col << endl;


    if (maze.inBounds(cur.row-1, cur.col)) { //if south is in bounds and is true (meaning there's a path)
        if (maze.get(cur.row-1, cur.col)) {
            GridLocation gL = GridLocation(cur.row-1, cur.col);
            //cout << "N: " << gL.row << ", " << gL.col << endl;
            neighbors.add(gL);
        }
    }
    if (maze.inBounds(cur.row+1, cur.col)) { //if north is in bounds and is true
        if (maze.get(cur.row+1, cur.col)) {
            GridLocation gL = GridLocation(cur.row+1, cur.col);
            //cout << "S: " << gL.row << ", " << gL.col << endl;
            neighbors.add(gL);
        }
    }
    if (maze.inBounds(cur.row, cur.col+1)) { //if east is in bounds and is true
        if (maze.get(cur.row, cur.col+1)) {
            GridLocation gL = GridLocation(cur.row, cur.col+1);
            //cout << "E: " << gL.row << ", " << gL.col << endl;
            neighbors.add(gL);
        }
    }
    if (maze.inBounds(cur.row, cur.col-1)) { //if west is in bounds and is true
        if (maze.get(cur.row, cur.col-1)) {
            GridLocation gL = GridLocation(cur.row, cur.col-1);
            //cout << "W: " << gL.row << ", " << gL.col << endl;
            neighbors.add(gL);
        }
    }

    return neighbors;
}


/**
 * @brief checkSolution
 *      Checks if a path is valid
 * @param maze
 *      the maze
 * @param path
 *      the path
 */
void checkSolution(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows() - 1,  maze.numCols() - 1};

    if (path.isEmpty()) {
        error("Path is empty");
    }

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }

    Vector<GridLocation> path1; //using a vector to remember locations that have been travelled b/c it has the contains method
    GridLocation cell = path.pop(); //chose to pop the original location so
    path1.add(cell);                //I could compare it to the rest of the locations in the path
    int pastR;
    int pastC;

    while (!path.isEmpty()) {
        pastR = cell.row;
        pastC = cell.col;
        cell = path.pop();

        if (!maze.inBounds(cell)) {
            error("Path goes out of bounds");
        }

        else {

            if(!maze.get(cell.row, cell.col)) {
                error("Path goes through a wall");
            }

            if (path1.contains(cell)) {
                error("Path loops around");
            }

            if (!((cell.row == pastR + 1 && cell.col == pastC) ||  //if the cell location is not an adjacent step away
                  (cell.row == pastR - 1 && cell.col == pastC) ||
                  (cell.row == pastR && cell.col == pastC + 1) ||
                  (cell.row == pastR && cell.col == pastC - 1))) {
                error("Path does not follow one cardinal step rule");
            }
        }

        path1.add(cell);
    }
    if (cell.row != 0 || cell.col != 0) {
        error("Path does not begin at the maze entrance");
    }
}

/**
 * @brief solveMaze
 *      Figures out the best path
 * @param maze
 *      the maze
 * @return path
 *      the best path
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);  //graphics of maze
    //MazeGraphics::highlightPath;

    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1}; //added this here b/c for some reason,
    Stack<GridLocation> path = {{0, 0}};                           //if (path.peek().row == maze.numRows() - 1 &&
                                                                   //path.peek().col == maze.numCols() - 1) didn't work
    Queue<Stack<GridLocation>> allPaths;
    Vector<GridLocation> repeats;       //using vector b/c contains method

    allPaths.add(path);
    repeats.add(path.peek());

    while(true) {
        path = allPaths.dequeue();
        if (path.peek() == mazeExit) {
            break;
        }
        else {
            Set<GridLocation> moves = generateValidMoves(maze, path.peek());

            for (GridLocation move : moves) {    //goes through all the moves to see if any new ones show up
                if (!repeats.contains(move)) {
                    Stack<GridLocation> newPath = path;
                    newPath.push(move);
                    allPaths.add(newPath);
                    repeats.add(move); 
                }

            }
        }
    }
    return path;
}

/**
 * @brief readMazeFile
 *      checks for length of rows and character content; gives the first error encountered
 * @param filename
 *      the file
 * @param maze
 *      the maze
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following lines read in the data from the file into a Vector
     * of strings representing the lines of the file. We haven't talked
     * in class about what ifstreams are, so don't worry if you don't fully
     * understand what is going on here.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        int countOfCols = 0;
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            }
            else {
                error("Invalid character");
            }
            countOfCols++;
        }
        if (numCols != countOfCols) {   //placed inside forloop since every time a row is iterated through, we need to take attendance of columns
            error("There's a discrepancy in the number of columns");
        }
    }
}

/* This provided functions opens and read the contents of files ending
 * in a .soln extension and interprets the contents as a Stack of
 * GridLocations, populating the provided soln data structure.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    Vector<string> lines;
    readEntireFile(in, lines);

    if (lines.size() != 1){
        error("File contained too many or too few lines.");
    }

    istringstream istr(lines[0]); // Stack read does its own error-checking
    if (!(istr >> soln)) {// if not successfully read
        error("Solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateNeighbors on location in the center of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> neighbors = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

PROVIDED_TEST("generateNeighbors on location on the side of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> neighbors = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, side));
}

PROVIDED_TEST("generateNeighbors on corner of 2x2 grid with walls"){
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {{1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for medium maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for large maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}


PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
}


PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);
    MazeGraphics::highlightPath(soln, "pink");
    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);
    MazeGraphics::highlightPath(soln, "pink");
    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("Test readMazeFile on valid file 2x2.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/2x2.maze", maze));
}

PROVIDED_TEST("Test readMazeFile on valid file 5x7.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/5x7.maze", maze));
}

PROVIDED_TEST("readMazeFile on nonexistent file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/nonexistent_file", g));
}

PROVIDED_TEST("readMazeFile on malformed file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/malformed.maze", g));
}

STUDENT_TEST("generateValidMoves with walls 3x3 with out of bounds and no neighbors") {
    Grid<bool> maze = {{true, false, true},
                       {false, true, true},
                       {true, true, true}};
    GridLocation center = {};
    Set<GridLocation> neighbors = {};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

STUDENT_TEST("generateValidMoves with walls 3x3 with one neighbor") {
    Grid<bool> maze = {{true, false, true},
                       {false, true, true},
                       {true, true, true}};
    GridLocation center = {0, 2};
    Set<GridLocation> neighbors = {{1, 2}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

STUDENT_TEST("checkSolution all walls, wrong start") {
    Grid<bool> maze = {{false, false},
                       {false, false}};
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
}

STUDENT_TEST("checkSolution teleports and out of bounds") {
    Grid<bool> maze = {{true, false},
                       {false, false},
                       {true, true}};
    Stack<GridLocation> go_through_wall = { {0,0}, {1, 0}, {2, 1}, {0, 1}, {1, 2}, {1, 2} };

    EXPECT_ERROR(checkSolution(maze, go_through_wall));
}

STUDENT_TEST("checkSolution loopy doopy x2") {
    Grid<bool> maze = {{true, true, false},
                       {false, true, true},
                       {false, true, true}};
    Stack<GridLocation> looping = { {0,0}, {0, 1}, {1, 1}, {1, 2}, {1, 1}, {1, 2}, {2, 2} };

    EXPECT_ERROR(checkSolution(maze, looping));
}

STUDENT_TEST("solveMaze on file 19x11") {
    Grid<bool> maze;
    readMazeFile("res/19x11.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);
    MazeGraphics::highlightPath(soln, "pink");
    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("solveMaze on file 25x33") {
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);
    MazeGraphics::highlightPath(soln, "pink");
    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("readMazeFile more than 1 wrong col and more than 1 unaccepted character; should give first error encountered") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/12x12_onecoloff.maze", g));
}

STUDENT_TEST("readMazeFile wrong col then unaccepted char") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/12x12_onecoloff_copy.maze", g));
}

