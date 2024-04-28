#include "../../freeglut/include/GL/freeglut.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

void drawHalfCircle(float x, float y, float radius) {
   glColor3f(0.8588f, 0.6824f, 0.5804f);
   glBegin(GL_LINE_LOOP); // Draw a line loop to connect segments
   for (int i = 90; i <= 270; ++i) {
       float angle = 2.0f * M_PI * (float)i / 360.0f;
       float xPos = x + sin(angle) * radius;
       float yPos = y + cos(angle) * radius;
       glVertex2f(xPos, yPos);
   }
   glEnd();
}

void drawPeople(float x, float y, bool isRed, bool isTeacher){

   // Body
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.7961f, 0.4118f);
    glVertex2f(x - 0.05f, y - 0.1f);
    glVertex2f(x + 0.05f, y - 0.1f);
    glVertex2f(x + 0.05f, y + 0.1f);
    glVertex2f(x - 0.05f, y + 0.1f);
    glEnd();

    // Shirt
    glBegin(GL_QUADS);
    if(isTeacher){
        glColor3f(0.8588f, 0.6824f, 0.5804f);
    }else{
        glColor3f(0.4706f, 0.4902f, 0.3843f);

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
    glColor3f(0.8588f, 0.6824f, 0.5804f);
    glVertex2f(x + 0.05f, y - 0.03f);
    glVertex2f(x + 0.1f, y + 0.03f);
    glLineWidth(8.0f);
    glEnd();
   }
}



class Shape {
public:
    virtual void draw() = 0; // Pure virtual function to draw the shape
};

class Triangle : public Shape {
private:
    float x;
    float y;
public:
    Triangle(float x, float y) : x(x), y(y) {}

    void draw() override {
        /*
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(x, y);
        glVertex2f(x + 0.3f, y);
        glVertex2f(x + 0.15f, y + 0.3f);
        glEnd();*/

        drawPeople(x, y, false, true);
    }
};

class Square : public Shape {
private:
    float x;
    float y;
    bool isRed;
public:
    Square(float x, float y, bool isRed) : x(x), y(y), isRed(isRed) {}

    void draw() override {
        glBegin(GL_QUADS);
        if (isRed) {
            glColor3f(1.0f, 0.0f, 0.0f); // Red color
        } else {
            glColor3f(0.6000f, 0.4824f, 0.4000f); // Brown color
        }
        glVertex2f(x - 0.1f, y - 0.1f);
        glVertex2f(x + 0.1f, y - 0.1f);
        glVertex2f(x + 0.1f, y + 0.1f);
        glVertex2f(x - 0.1f, y + 0.1f);
        glEnd();
    }
};

// Triangle position
float triangleX = 0.0f;
float triangleY = -0.3f;

// Square positions
float squarePositions[6][2] = {
    {-0.6f, 0.6f}, {0.0f, 0.6f}, {0.6f, 0.6f},
    {-0.6f, 0.0f}, {0.0f, 0.0f}, {0.6f, 0.0f}
};

// Square colors
bool squareColors[6] = {false, false, false, false, false, false}; // false: brown, true: red

// Triangle movement speed
float movementSpeed = 0.02f;

// Key states
bool keyW = false;
bool keyA = false;
bool keyS = false;
bool keyD = false;
bool keySpace = false;

// Timer interval for changing square colors (in milliseconds)
int timerInterval = 500; // half a second
// Timer ID
int timerID;
int maxTime = 20000; // 20 seconds


// Counter for squares changed back to brown
int brownCount = 0;

bool squareHover[6] = {false, false, false, false, false, false};

// Check if a point (x, y) is inside the square at (sx, sy)
// bool isInsideSquare(float x, float y, float sx, float sy) {
//     return (x >= sx - 0.1f && x <= sx + 0.1f && y >= sy - 0.1f && y <= sy + 0.1f);
// }

// bool isInsideTriangle(float x, float y, float x1, float y1, float x2, float y2, float x3, float y3) {
//     // Compute barycentric coordinates
//     float denominator = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
//     float alpha = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / denominator;
//     float beta = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / denominator;
//     float gamma = 1.0f - alpha - beta;

//     // Check if the point is inside the triangle
//     return alpha > 0 && beta > 0 && gamma > 0;
// }

bool isInsideTriangle(float x, float y, float rectX, float rectY){
    bool answer = false;
    // x and y are inside the bounded region of the rectangle's x and y values
    if(x*2 >= rectX - 0.15f && x*2 <= rectX + 0.15f && y*2 >= rectY - 0.12f && y*2 <= rectY + 0.12f){
        answer = true;
    }
    return answer;
}

void terminateCode(int value){
    exit(0);
}

void renderText(const std::string& text, int xPos, int yPos) {
    // std::cout << text << std::endl;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white

    for (unsigned int i = 0; i < text.size(); ++i) {
        if (text[i] == '\n') {
            yPos -= 20; // Move to the next line
            xPos = 30; // Reset X position
        } else if (text[i] == '\t') {
            xPos += 40; // Move to the next tab position
        } else {
            glRasterPos2i(xPos, yPos); // Set position for the text
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]); // Render each character
            xPos += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, text[i]); // Move X position by character width
        }
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Randomly change the color of squares
void changeSquareColors(int value) {
    maxTime -= timerInterval;
    for (int i = 0; i < 6; ++i) {
        squareColors[i] = (rand() % 2 == 0); // Randomly set square color to red or brown
    }
    glutPostRedisplay();
    // Restart the timer for the next color change
    if (maxTime > 0) {
        glutTimerFunc(timerInterval, changeSquareColors, 0);
    } else {
        // Delay before terminating
        glutTimerFunc(8000, terminateCode, 0);
    }
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
        case ' ':
            keySpace = true;
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
        case ' ':
            keySpace = false;
            break;
    }
}

void drawSquare(float x, float y, bool isRed, bool isHovering) {

    drawPeople(x, y + 0.1f, isRed, false);

    glBegin(GL_QUADS);
    if (isHovering && isRed) {
        glColor3f(1.0f, 0.7961f, 0.4118f); // Green color when hovering (&& if it is Red!)
    } else {
        if (isRed) {
            glColor3f(1.0f, 0.0f, 0.0f); // Red color
        } else {
            glColor3f(0.6000f, 0.4824f, 0.4000f); // Brown color
        }
    }
    glVertex2f(x - 0.1f, y - 0.1f);
    glVertex2f(x + 0.1f, y - 0.1f);
    glVertex2f(x + 0.1f, y + 0.02f);
    glVertex2f(x - 0.1f, y + 0.02f);
    glEnd();
}

void display_func(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw squares
    for (int i = 0; i < 6; ++i) {
        drawSquare(squarePositions[i][0], squarePositions[i][1], squareColors[i], squareHover[i]);
    }

    // Update triangle position based on key presses
    if (keyW && triangleY + movementSpeed <= 0.5f) triangleY += movementSpeed;
    if (keyA && triangleX - movementSpeed >= -0.5f) triangleX -= movementSpeed;
    if (keyS && triangleY - movementSpeed >= -0.5f) triangleY -= movementSpeed;
    if (keyD && triangleX + movementSpeed <= 0.5f) triangleX += movementSpeed;

    // Check if the space bar is pressed and handle interactions
    /*
    if (keySpace) {
        std::cout << "space pressed" << std::endl;
        bool squareChanged = false; // Flag to track if any square color was changed
    
        for (int i = 0; i < 6; ++i) {
            squareHover[i] = isInsideTriangle(triangleX, triangleY, 
                                           squarePositions[i][0], squarePositions[i][1]);
            // if (isInsideTriangle(triangleX, triangleY, 
            //                      squarePositions[i][0] - 0.1f, squarePositions[i][1] - 0.1f, 
            //                      squarePositions[i][0] + 0.1f, squarePositions[i][1] - 0.1f, 
            //                      squarePositions[i][0], squarePositions[i][1] + 0.1f)) {
            if(squareHover[i]){
                std::cout << "is in square: (" << squarePositions[i][0] << ", " <<  squarePositions[i][1] << ")" << std::endl;
    
                drawSquare(squarePositions[i][0], squarePositions[i][1], false, squareHover[i]);
                std::cout << "square color changed" << std::endl; 
                brownCount++;
                std::cout << "count: " << brownCount << std::endl; 
    
                // If the square is red, change its color to brown
                if (squareColors[i]) {
                    squareColors[i] = false;
                    std::cout << "color changed" << std::endl;
                    squareChanged = true;
                    break; // Exit the loop after changing the color of the first touched square
                }
            }
        }
    
        // Increment the brown count only if a square color was changed
        if (squareChanged) {
            brownCount++;
            std::cout << "count: " << brownCount << std::endl;
        }
    }*/

    for (int i = 0; i < 6; ++i) {
        squareHover[i] = isInsideTriangle(triangleX, triangleY, 
                                        squarePositions[i][0], squarePositions[i][1]);
        // if (isInsideTriangle(triangleX, triangleY, 
        //                      squarePositions[i][0] - 0.1f, squarePositions[i][1] - 0.1f, 
        //                      squarePositions[i][0] + 0.1f, squarePositions[i][1] - 0.1f, 
        //                      squarePositions[i][0], squarePositions[i][1] + 0.1f)) {
        if(squareHover[i]){
            if (keySpace) {
                // std::cout << "space pressed" << std::endl;
                bool squareChanged = false; // Flag to track if any square color was changed

                // If the square is red, change its color to brown
                if (squareColors[i]) {
                    squareColors[i] = false;
                    // std::cout << "color changed" << std::endl;
                    squareChanged = true;
                    // break; // Exit the loop after changing the color of the first touched square
                }

                if (squareChanged) {
                    brownCount++;
                    drawSquare(squarePositions[i][0], squarePositions[i][1], false, squareHover[i]);
                    std::cout << "Score: " << brownCount << std::endl;
                }
            }
        }
    }

    // for (int i = 0; i < 6; ++i) {
    //     // squareHover[i] = isInsideTriangle(triangleX, triangleY, 
    //     //                                    squarePositions[i][0] - 0.1f, squarePositions[i][1] - 0.1f, 
    //     //                                    squarePositions[i][0] + 0.1f, squarePositions[i][1] - 0.1f, 
    //     //                                    squarePositions[i][0], squarePositions[i][1] + 0.1f);
    //     squareHover[i] = isInsideTriangle(triangleX, triangleY, 
    //                                        squarePositions[i][0], squarePositions[i][1]);
    // }

    // Draw the triangle at its updated position
    glPushMatrix();
    glTranslatef(triangleX, triangleY, 0.0f);
    /*
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.1f, 0.1f);
    glVertex2f(0.4f, 0.1f);
    glVertex2f(0.25f, 0.4f);
    glEnd();*/
    drawPeople(triangleX, triangleY, false, true);
    glPopMatrix();

    // Render text
    int xPos = 30; // Initial X position
    int yPos = 100; // Initial Y position

    std::string text = "Goal: Help as many students within 20 seconds!\nAbilities!\n\tMovement: W,A,S,D keys\n\tHelp students: Press space key\nNumber of students you've helped: " + std::to_string(brownCount);

    renderText(text, xPos, yPos);

    int timexPos = 30;
    int timeyPos = 200;
    std::string timetext = "Time: " + std::to_string(maxTime/1000) + " seconds left!";
    renderText(timetext, timexPos, timeyPos);

    if(maxTime == 0){
        std::string outOfTimeMessage = std::string("Ran out of time!\n") +" Number of students you've helped: " + std::to_string(brownCount);
        int endx = 300;
        int endy = 300;
        renderText(outOfTimeMessage, endx, endy);
    }

    glutSwapBuffers();
}


// INIT
void init(void) {
    glClearColor(0.6078f, 0.6078f, 0.4706f, 0.0f);

    // Seed random number generator
    srand(time(NULL));

    // Start the timer for changing square colors
    glutTimerFunc(timerInterval, changeSquareColors, 0);

    std::cout << "Finished initializing...\n\n";
}




// MAIN
int main(int argc, char** argv) {
    std::cout << "Goal: Help as many students within 30 seconds!" << std::endl;
    std::cout << "Abilities!" << std::endl;
    std::cout << "Movement: W,A,S,D keys" << std::endl;
    std::cout << "Help students: Press space key" << std::endl;

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Tutoring Center Game");

    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    glutReshapeFunc(reshape_func);
    glutKeyboardFunc(keyboard_func);
    glutKeyboardUpFunc(key_released);
    
    init();

    glutMainLoop();

    return EXIT_SUCCESS;
}
