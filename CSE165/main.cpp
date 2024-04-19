#include <QtWidgets>
#include <QtOpenGL>

class MyWidget : public QOpenGLWindow { // Inherit from QOpenGLWindow
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr) : QOpenGLWindow(parent) {}

protected:
    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
        glColor3f(1.0f, 0.0f, 0.0f); // Set color to red

        // Define triangle vertices
        GLfloat vertices[] = {
            0.0f, 1.0f, 0.0f,  // Top vertex
            -1.0f, -1.0f, 0.0f, // Bottom left vertex
            1.0f, -1.0f, 0.0f   // Bottom right vertex
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);  // Specify vertex data

        glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw a triangle

        glDisableClientState(GL_VERTEX_ARRAY);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MyWidget widget;
    widget.show();
    return app.exec();
}



/*#include "mainwindow.h"

#include <QApplication>
#include <QtOpenGL>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
*/
