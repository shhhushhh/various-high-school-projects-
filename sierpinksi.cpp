/**
  Shirley Li
  CS106B
  Assignment 3
  Draws the Sierpinski's Triangle perpendicularly to order-0 Sierpinski Triangle's base
  **/

#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "error.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This 
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}


/**
 * @brief drawSierpinskiTriangle
 *      draws Sierpinski triangles up to the order given
 * @param window
 *      the graphics window
 * @param one
 *      one of the triangle points
 * @param two
 *      one of the triangle points
 * @param three
 *      one of the triangle points
 * @param order
 *      the order that the triangles should be drawn up to
 */
void drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order < 0) {
        error("Order is negative; cannot draw");
    }
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
    }
    else {
        double midpointX = (one.getX() + two.getX()) / 2.0;
        double midpointY = (one.getY() + two.getY()) / 2.0;
        GPoint midpoint = GPoint(midpointX, midpointY);
        drawSierpinskiTriangle(window, one, midpoint, GPoint((one.getX() + three.getX()) / 2.0, (one.getY() + three.getY()) / 2), order - 1);
        drawSierpinskiTriangle(window, midpoint, two, GPoint((two.getX() + three.getX()) / 2.0, (two.getY() + three.getY()) / 2.0), order - 1);
        drawSierpinskiTriangle(window, GPoint((one.getX() + three.getX()) / 2.0, (one.getY() + three.getY()) / 2), GPoint((two.getX() + three.getX()) / 2.0, (two.getY() + three.getY()) / 2.0), three, order - 1);

    }
}


/* * * * * * Test Cases * * * * * */

/* 
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo. 
 */

void runDemos();

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runDemos();
}

