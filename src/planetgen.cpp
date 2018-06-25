//============================================================================
// Name        : planetgen.cpp
// Author      : David Linus Briemann
// Version     :
// Copyright   :
// Description :
//============================================================================

//#include "primitives.h"
//#include <QGLViewer/qglviewer.h>
//#include <QApplication>
//
//static Sphere globe = Sphere(9, 6000, 0.0003);
//
//class Viewer: public QGLViewer
//{
//
//protected:
//    virtual void draw()
//    {
//        Vec3 normal;
//
//        glBegin(GL_TRIANGLES);
//        for (unsigned int i = 0; i < globe.polies.size(); i++) {
//            normal = globe.polies[i].getNormal();
//            glNormal3f(normal.x, normal.y, normal.z);
//
//            glColor3f(globe.polies[i].c0.r, globe.polies[i].c0.g, globe.polies[i].c0.b);
//            glVertex3f(globe.polies[i].p0.x, globe.polies[i].p0.y, globe.polies[i].p0.z);
//
//            glColor3f(globe.polies[i].c1.r, globe.polies[i].c1.g, globe.polies[i].c1.b);
//            glVertex3f(globe.polies[i].p1.x, globe.polies[i].p1.y, globe.polies[i].p1.z);
//
//            glColor3f(globe.polies[i].c2.r, globe.polies[i].c2.g, globe.polies[i].c2.b);
//            glVertex3f(globe.polies[i].p2.x, globe.polies[i].p2.y, globe.polies[i].p2.z);
//        }
//        glEnd();
//
//    }
//
//    virtual void init()
//    {
//        glEnable(GL_CULL_FACE);
//    }
//
//};
//
//int main(int argc, char** argv)
//{
//    QApplication application(argc, argv);
//    Viewer viewer;
//    viewer.show();
//
//    return application.exec();
//}


#include <iostream>
#include <vector>
#include <GL/glut.h>

#include "primitives.h"

static int width = 800;
static int height = 800;
static int globe_iterations = 8;
static Sphere globe = Sphere(globe_iterations, 2000, 0.0002);

static float cam_x = 0.0;
static float cam_y = 0.0;
static float cam_z = 3.0;
static bool mousedown = false;

static float LightPos[4] = { cam_x - 10.0, cam_y, cam_z + 10.0, 0.0f };
const GLfloat light_ambient[4]  = {1.0, 1.0, 1.0, 1.0};
const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
const GLfloat light_diffuse[4]  = {1.0, 1.0, 1.0, 1.0};

float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;

void idleScene(void)
{
    glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

void processSpecialKeys(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP:
            cam_z -= 0.05;
            break;
        case GLUT_KEY_DOWN:
            cam_z += 0.05;
            break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mousedown = true;

        xdiff = x - yrot;
        ydiff = -y + xrot;
    } else
        mousedown = false;
}

void mouseMotion(int x, int y)
{
    if (mousedown) {
        yrot = x - xdiff;
        xrot = y + ydiff;

        glutPostRedisplay();
    }
}

void renderScene(void)
{
    Vec3 normal;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(cam_x, cam_y, cam_z, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);
    LightPos[0] = cam_x - 10.0;
    LightPos[1] = cam_y;
    LightPos[2] = cam_z + 100.0;
    LightPos[3] = 0.0f;
    glLightfv(GL_LIGHT1, GL_POSITION, LightPos);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < globe.polies.size(); i++) {
        normal = globe.polies[i].getNormal();
        glNormal3f(normal.x, normal.y, normal.z);

        glColor3f(globe.polies[i].c0.r, globe.polies[i].c0.g, globe.polies[i].c0.b);
        glVertex3f(globe.polies[i].p0.x, globe.polies[i].p0.y, globe.polies[i].p0.z);

        glColor3f(globe.polies[i].c1.r, globe.polies[i].c1.g, globe.polies[i].c1.b);
        glVertex3f(globe.polies[i].p1.x, globe.polies[i].p1.y, globe.polies[i].p1.z);

        glColor3f(globe.polies[i].c2.r, globe.polies[i].c2.g, globe.polies[i].c2.b);
        glVertex3f(globe.polies[i].p2.x, globe.polies[i].p2.y, globe.polies[i].p2.z);
    }
    glEnd();

    glutSwapBuffers();
}

void changeSize(int w, int h)
{

    // Prevent a divide by zero, when window is too short
    if (h == 0)
        h = 1;

    float ratio = 1.0 * w / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cam_x, cam_y, cam_z, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);
    LightPos[0] = cam_x - 10.0;
    LightPos[1] = cam_y;
    LightPos[2] = cam_z + 100.0;
    LightPos[3] = 0.0f;
    glLightfv(GL_LIGHT1, GL_POSITION, LightPos);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
}

int main(int argc, char **argv)
{
    /*
     * init glut
     */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Planet Gen");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

//    glMaterialfv(GL_FRONT, GL_AMBIENT, Ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
    glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 3.0);
    glLightf( GL_LIGHT1, GL_SPOT_CUTOFF,   10.0);
    glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION,  0.1f);
    glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION,    0.3f);
    glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3f);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);

    //    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glutDisplayFunc(renderScene);
    glutIdleFunc(idleScene);
    glutReshapeFunc(changeSize);

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    std::cout << "Created Sphere with " << globe.polies.size() << " Triangles" << std::endl;

    glutMainLoop();

    return 0;
}
