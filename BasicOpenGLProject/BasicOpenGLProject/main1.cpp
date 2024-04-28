#include "../../freeglut/include/GL/freeglut.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Triangle position
float triangleX = 0.0f;
float triangleY = 0.0f;

// Square positions
float squarePositions[6][2] = {
    {-0.6f, 0.6f}, {0.0f, 0.6f}, {0.6f, 0.6f},
    {-0.6f, 0.0f}, {0.0f, 0.0f}, {0.6f, 0.0f}
};

// Square colors
bool squareColors[6] = {false, false, false, false, false, false}; // false: brown, true: red

// Triangle movement speed
float movementSpeed = 0.05f;

// Key states
bool keyW = false;
bool keyA = false;
bool keyS = false;
bool keyD = false;

// Timer interval for changing square colors (in milliseconds)
int timerInterval = 2000; // 2 seconds
// Timer ID
int timerID;

// Check if a point (x, y) is inside the square at (sx, sy)
bool isInsideSquare(float x, float y, float sx, float sy) {
    return (x >= sx - 0.1f && x <= sx + 0.1f && y >= sy - 0.1f && y <= sy + 0.1f);
}

// Randomly change the color of squares
void changeSquareColors(int value) {
    for (int i = 0; i < 6; ++i) {
        squareColors[i] = (rand() % 2 == 0); // Randomly set square color to red or brown
    }
    glutPostRedisplay();
    // Restart the timer for the next color change
    glutTimerFunc(timerInterval, changeSquareColors, 0);
}

// CALLBACKS
void idle_func() {
    glutPostRedisplay();
}

void reshape_func(int width, int height) {
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

void keyboard_func(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            keyW = true;
            break;
        case 'a':
            keyA = true;
            break;
        case 's':
            keyS = true;
            break;
        case 'd':
            keyD = true;
            break;
        case '\x1B': // Exit on escape key press
            exit(EXIT_SUCCESS);
            break;
    }
}

void key_released(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            keyW = false;
            break;
        case 'a':
            keyA = false;
            break;
        case 's':
            keyS = false;
            break;
        case 'd':
            keyD = false;
            break;
    }
}

void drawHalfCircle(float x, float y, float radius) {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP); // Draw a line loop to connect segments
    for (int i = 90; i <= 270; ++i) {
        float angle = 2.0f * M_PI * (float)i / 360.0f;
        float xPos = x + sin(angle) * radius;
        float yPos = y + cos(angle) * radius;
        glVertex2f(xPos, yPos);
    }
    glEnd();
}

void drawPeople(float x, float y, bool isRed){

    // Body
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(x - 0.05f, y - 0.1f);
    glVertex2f(x + 0.05f, y - 0.1f);
    glVertex2f(x + 0.05f, y + 0.1f);
    glVertex2f(x - 0.05f, y + 0.1f);
    glEnd();

    // Shirt
    glBegin(GL_QUADS);
    if (isRed) {
        glColor3f(1.0f, 0.0f, 0.0f); // Red color
    } else {
        glColor3f(0.6f, 0.3f, 0.0f); // Brown color
    }
    glVertex2f(x - 0.05f, y - 0.1f);
    glVertex2f(x + 0.05f, y - 0.1f);
    glVertex2f(x + 0.05f, y - 0.03f);
    glVertex2f(x - 0.05f, y - 0.03f);
    glEnd();

    // Left eye
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(x - 0.03f, y + 0.075f);
    glVertex2f(x - 0.01f, y + 0.075f);
    glVertex2f(x - 0.01f, y + 0.025f);
    glVertex2f(x - 0.03f, y + 0.025f);
    glEnd();

    // Right eye
    glBegin(GL_QUADS);
    glVertex2f(x + 0.03f, y + 0.075f);
    glVertex2f(x + 0.01f, y + 0.075f);
    glVertex2f(x + 0.01f, y + 0.025f);
    glVertex2f(x + 0.03f, y + 0.025f);
    glEnd();

    drawHalfCircle(x, y + 0.01f, 0.02f);

    // Raise hand
    if(isRed){
      glBegin(GL_LINES);
      glColor3f(1.0f, 1.0f, 0.0f);
      glVertex2f(x + 0.05f, y - 0.03f);
      glVertex2f(x + 0.1f, y + 0.03f);
      glLineWidth(8.0f);
      glEnd();
    }
}



void drawSquare(float x, float y, bool isRed) {
    glBegin(GL_QUADS);
    if (isRed) {
        glColor3f(1.0f, 0.0f, 0.0f); // Red color
    } else {
        glColor3f(0.6f, 0.3f, 0.0f); // Brown color
    }
    glVertex2f(x - 0.1f, y - 0.1f);
    glVertex2f(x + 0.1f, y - 0.1f);
    glVertex2f(x + 0.1f, y + 0.1f);
    glVertex2f(x - 0.1f, y + 0.1f);
    glEnd();
}

void display_func(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw squares
    for (int i = 0; i < 6; ++i) {
        drawPeople(squarePositions[i][0], squarePositions[i][1], squareColors[i]);
    }

    // Update triangle position based on key presses
    if (keyW) triangleY += movementSpeed;
    if (keyA) triangleX -= movementSpeed;
    if (keyS) triangleY -= movementSpeed;
    if (keyD) triangleX += movementSpeed;

    // Draw the triangle at its updated position
    glPushMatrix();
    glTranslatef(triangleX, triangleY, 0.0f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.1f, 0.1f);
    glVertex2f(0.4f, 0.1f);
    glVertex2f(0.25f, 0.4f);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

// INIT
void init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Seed random number generator
    srand(time(NULL));

	
	// Start the timer for changing square colors
	glutTimerFunc(timerInterval, changeSquareColors, 0);


    std::cout << "Finished initializing...\n\n";
}

// MAIN
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Basic OpenGL Example");

    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    glutReshapeFunc(reshape_func);
    glutKeyboardFunc(keyboard_func);
    glutKeyboardUpFunc(key_released);

    init();

    glutMainLoop();

    return EXIT_SUCCESS;
}
