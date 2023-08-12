
#include "Angel.h"
#include <GLFW/glfw3.h> 
#include <algorithm>
#include <map>
#include <math.h>
#include <random>
#include <ctime>

typedef vec4  color4;
typedef vec4  point4;

const int NumVertices = 36;
double lastTime = glfwGetTime();

point4 pointsCube_1[NumVertices]; // middle center cube points
color4 colorsCube_1[NumVertices]; // middle center cube colors
mat4  Model_view[28]; // 29th cube is invisible and used for picking the correct face

GLfloat offsetBetweenCubes = 0.05;
GLfloat Edges = 0.4; // edge of one small cube

point4 Points[28][NumVertices] = {};
point4 VerticesCube[28][8] = {};

point4 Colors[28][NumVertices] = {};

/// globals

// Indices of cubes that are located on specific sides. Note that these indices change in each rotation.
int rightSide[9] = { 2,5,8,11,14,17,20,23,26 };
int leftSide[9] = { 0,3,6,9,12,15,18,21,24 };
int frontSide[9] = { 0,1,2,3,4,5,6,7,8 };
int backSide[9] = { 18,19,20,21,22,23,24,25,26 };
int topSide[9] = { 0,1,2,9,10,11,18,19,20 };
int bottomSide[9] = { 6,7,8,15,16,17,24,25,26 };
//Tempoaray array versions
int tempRightSide[9] = { 0,0,0,0,0,0,0,0,0 };
int tempLeftSide[9] = { 0,0,0,0,0,0,0,0,0 };
int tempFrontSide[9] = { 0,0,0,0,0,0,0,0,0 };
int tempBackSide[9] = { 0,0,0,0,0,0,0,0,0 };
int tempTopSide[9] = { 0,0,0,0,0,0,0,0,0 };
int tempBottomSide[9] = { 0,0,0,0,0,0,0,0,0 };

// Animation variables
double prevTime = glfwGetTime();
double currentTime = glfwGetTime();
double deltaTime = 0.0;
double animationTime = 0.0;
double animationDur = 0.7; // seconds
double angularVel = 200.0;
double totalRot = 0.0;


//Random Globals
int randCount = 0;
const int MAX_RAND_COUNT = 30;

//Key Flags

std::map<int, bool> keyPressed = {
    {GLFW_KEY_LEFT, false},
    {GLFW_KEY_RIGHT, false},
    {GLFW_KEY_UP, false},
    {GLFW_KEY_DOWN, false},
    {GLFW_KEY_L, false},
    {GLFW_KEY_O, false},
    {GLFW_KEY_E, false},
    {GLFW_KEY_W, false},
    {GLFW_KEY_KP_7, false},
    {GLFW_KEY_KP_8, false},
};


//----------------------------------------------------------------------------
//Timer Globals
int secs = 0;
int seq = 0;
bool check = true;

//----------------------------------------------------------------------------,
//Flags to control animation to not overlap
bool animationOngoing = false;
bool randomize = false;
bool anotherKeyIsActive = false; // player can press only one button at one time

/// 



/// point and color declaration


point4 distanceXToCentralCube[27] = { // distance x of all small cubes to the middle central cube.
    /* Front 9 */
    point4(-Edges - offsetBetweenCubes,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(+Edges + offsetBetweenCubes,0.0,0.0,0.0),
    point4(-Edges - offsetBetweenCubes,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(+Edges + offsetBetweenCubes,0.0,0.0,0.0),
    point4(-Edges - offsetBetweenCubes,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(+Edges + offsetBetweenCubes,0.0,0.0,0.0),

    /* Middle 9 */
    point4(-Edges - offsetBetweenCubes,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(+Edges + offsetBetweenCubes,0.0,0.0,0.0),
    point4(-Edges - offsetBetweenCubes,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(+Edges + offsetBetweenCubes,0.0,0.0,0.0),
    point4(-Edges - offsetBetweenCubes,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(+Edges + offsetBetweenCubes,0.0,0.0,0.0),

    /* Back 9 */
    point4(-Edges - offsetBetweenCubes,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(+Edges + offsetBetweenCubes,0.0,0.0,0.0),
    point4(-Edges - offsetBetweenCubes,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(+Edges + offsetBetweenCubes,0.0,0.0,0.0),
    point4(-Edges - offsetBetweenCubes,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(+Edges + offsetBetweenCubes,0.0,0.0,0.0)
};
point4 distanceYToCentralCube[27] = { // distance y of all small cubes to the middle central cube.
    /* Front 9 */
    point4(0.0,+Edges + offsetBetweenCubes,0.0,0.0), point4(0.0,+Edges + offsetBetweenCubes,0.0,0.0), point4(0.0,+Edges + offsetBetweenCubes,0.0,0.0),
    point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0),
    point4(0.0,-Edges - offsetBetweenCubes,0.0,0.0), point4(0.0,-Edges - offsetBetweenCubes,0.0,0.0), point4(0.0,-Edges - offsetBetweenCubes,0.0,0.0),

    /* Middle 9 */
    point4(0.0,+Edges + offsetBetweenCubes,0.0,0.0), point4(0.0,+Edges + offsetBetweenCubes,0.0,0.0), point4(0.0,+Edges + offsetBetweenCubes,0.0,0.0),
    point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0),
    point4(0.0,-Edges - offsetBetweenCubes,0.0,0.0), point4(0.0,-Edges - offsetBetweenCubes,0.0,0.0), point4(0.0,-Edges - offsetBetweenCubes,0.0,0.0),

    /* Back 9 */
    point4(0.0,+Edges + offsetBetweenCubes,0.0,0.0), point4(0.0,+Edges + offsetBetweenCubes,0.0,0.0), point4(0.0,+Edges + offsetBetweenCubes,0.0,0.0),
    point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0),
    point4(0.0,-Edges - offsetBetweenCubes,0.0,0.0), point4(0.0,-Edges - offsetBetweenCubes,0.0,0.0), point4(0.0,-Edges - offsetBetweenCubes,0.0,0.0)
};
point4 distanceZToCentralCube[27] = { // distance z of all small cubes to the middle central cube.
    /* Front 9 */
    point4(0.0,0.0,+Edges + offsetBetweenCubes,0.0), point4(0.0,0.0,+Edges + offsetBetweenCubes,0.0), point4(0.0,0.0,+Edges + offsetBetweenCubes,0.0),
     point4(0.0,0.0,+Edges + offsetBetweenCubes,0.0), point4(0.0,0.0,+Edges + offsetBetweenCubes,0.0), point4(0.0,0.0,+Edges + offsetBetweenCubes,0.0),
     point4(0.0,0.0,+Edges + offsetBetweenCubes,0.0), point4(0.0,0.0,+Edges + offsetBetweenCubes,0.0), point4(0.0,0.0,+Edges + offsetBetweenCubes,0.0),

     /* Middle 9 */
     point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0),
     point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0),
     point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0), point4(0.0,0.0,0.0,0.0),

     /* Back 9 */
     point4(0.0,0.0,-Edges - offsetBetweenCubes,0.0), point4(0.0,0.0,-Edges - offsetBetweenCubes,0.0), point4(0.0,0.0,-Edges - offsetBetweenCubes,0.0),
     point4(0.0,0.0,-Edges - offsetBetweenCubes,0.0), point4(0.0,0.0,-Edges - offsetBetweenCubes,0.0), point4(0.0,0.0,-Edges - offsetBetweenCubes,0.0),
     point4(0.0,0.0,-Edges - offsetBetweenCubes,0.0), point4(0.0,0.0,-Edges - offsetBetweenCubes,0.0), point4(0.0,0.0,-Edges - offsetBetweenCubes,0.0),
};

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 verticesCube_1[8] = {
    point4(-Edges / 2, -Edges / 2,  Edges / 2, 1.0),
    point4(-Edges / 2,  Edges / 2,  Edges / 2, 1.0),
    point4(Edges / 2,  Edges / 2,  Edges / 2, 1.0),
    point4(Edges / 2, -Edges / 2,  Edges / 2, 1.0),
    point4(-Edges / 2, -Edges / 2, -Edges / 2, 1.0),
    point4(-Edges / 2,  Edges / 2, -Edges / 2, 1.0),
    point4(Edges / 2,  Edges / 2, -Edges / 2, 1.0),
    point4(Edges / 2, -Edges / 2, -Edges / 2, 1.0)
};


// RGBA olors
color4 vertex_colors_cube_1[8] = {
    color4(0.0, 0.0, 0.0, 1.0),  // black
    color4(1.0, 0.0, 0.0, 1.0),  // red
    color4(1.0, 1.0, 0.0, 1.0),  // yellow
    color4(0.0, 1.0, 0.0, 1.0),  // green
    color4(0.0, 0.0, 1.0, 1.0),  // blue
    color4(1.0, 0.0, 1.0, 1.0),  // magenta
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(0.0, 1.0, 1.0, 1.0)   // cyan
};


// colors of cube
color4 ColorsCube[28][6];



///


// this function fills all our vertex arrays.
void fillVertexArrays() {

    for (int i = 0; i < 27; i++) {

        for (int j = 0; j < 9; j++) {

            VerticesCube[i][j] = verticesCube_1[j] + distanceXToCentralCube[i] + distanceYToCentralCube[i] + distanceZToCentralCube[i];

        }
    }
    // last cube is going to be the enlarged version of the middle center cube
    for (int j = 0; j < 9; j++) {
        VerticesCube[27][j] = VerticesCube[13][j];
    }

}




// Creates colors of the cube
void create_colors_cube(){

    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 6; j++) {
            if (j == 0) {
                ColorsCube[i][j] = color4(0.0, 0.0, 1.0, 1.0); // blue
            }
             else if (j == 1) {
              ColorsCube[i][j] = color4(1.0, 0.0, 0.0, 1.0); // red
            }
            else if (j == 2) {
             ColorsCube[i][j] = color4(0.0, 1.0, 0.0, 1.0); // green
            }
            else if (j == 3) {
             ColorsCube[i][j] = color4(1.0, 1.0, 0.0, 1.0); // yellow
            }
            else if (j == 4) {
             ColorsCube[i][j] = color4(1.0, 1.0, 1.0, 1.0); // white
            }
            else if (j == 5) {
             ColorsCube[i][j] = color4(1.0, 0.5, 0.0, 1.0); // orange
            }
        }
    }
    
}

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int  Axis = Xaxis;

GLfloat  Initial[NumAxes] = { 30.0, -40.0, 0.0 }; // initial rotation angles

GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 }; // Angles of ccw face rotations
GLfloat  Beta[NumAxes] = { 0.0, 0.0, 0.0 }; // Angles of cw face rotations



// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
// to the vertices

int Index = 0;
void quad(int a, int b, int c, int d, int cubeIndex, int face)
{
    // Initialize colors

    Colors[cubeIndex][Index] = ColorsCube[cubeIndex][face];
    Points[cubeIndex][Index] = VerticesCube[cubeIndex][a];
    Index++;
    Colors[cubeIndex][Index] = ColorsCube[cubeIndex][face];
    Points[cubeIndex][Index] = VerticesCube[cubeIndex][b];
    Index++;
    Colors[cubeIndex][Index] = ColorsCube[cubeIndex][face];
    Points[cubeIndex][Index] = VerticesCube[cubeIndex][c];
    Index++;
    Colors[cubeIndex][Index] = ColorsCube[cubeIndex][face];
    Points[cubeIndex][Index] = VerticesCube[cubeIndex][a];
    Index++;
    Colors[cubeIndex][Index] = ColorsCube[cubeIndex][face];
    Points[cubeIndex][Index] = VerticesCube[cubeIndex][c];
    Index++;
    Colors[cubeIndex][Index] = ColorsCube[cubeIndex][face];
    Points[cubeIndex][Index] = VerticesCube[cubeIndex][d];
    Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors for each cube
void
colorcube(int cubeIndex)
{
    quad(1, 0, 3, 2, cubeIndex, 0);
    quad(2, 3, 7, 6, cubeIndex, 1);
    quad(3, 0, 4, 7, cubeIndex, 2);
    quad(6, 5, 1, 2, cubeIndex, 3);
    quad(4, 5, 6, 7, cubeIndex, 4);
    quad(5, 4, 0, 1, cubeIndex, 5);
    Index = 0;
}



// OpenGL initialization
GLuint vaoArray[28];
GLuint buffer;
GLuint vPosition;
GLuint vColor;


//Initilaze function
void
init()
{
    create_colors_cube();
    fillVertexArrays();

    for (int i = 0; i < 28; i++) {
        colorcube(i);
    }

    GLuint program = InitShader("shaders/vshader.glsl", "shaders/fshader.glsl");
    const vec3 displacement(0.0, 0.0, 0.0);

    glGenVertexArrays(28, vaoArray);
    for (int i = 0; i < 28; i++) {

        glBindVertexArray(vaoArray[i]);
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * 36 + sizeof(Colors[i]), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * 36, Points[i]);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 36, sizeof(Colors[i]), Colors[i]);

        vPosition = glGetAttribLocation(program, "vPosition");

        glEnableVertexAttribArray(vPosition);
        glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

        vColor = glGetAttribLocation(program, "vColor");
        glEnableVertexAttribArray(vColor);
        glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4) * 36));

        Model_view[i] = identity();

        if (i == 27) {
            // this invisible cube will be slightly bigger than our rubik's cube so that when we click on faces, we get its color instead of
            // the smaller cubes of the rubik's cube.
            Model_view[i] = Scale(3.26, 3.26, 3.26) * RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * Model_view[i];
        }
        else {
            Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * Model_view[i];
        }

    }

    // Retrieve uniform variable locations
    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");

    // Set current program object
    glUseProgram(program);

    // Set projection matrix
    mat4  projection;
    projection = Ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // Ortho(): user-defined function in mat.h
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    // Enable hiddden surface removal
    glEnable(GL_DEPTH_TEST);

    // Set state variable "clear color" to clear buffer with.
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------


// helper function
int arrayMax(int arr[]) {
    int max = -999;
    for (int i = 0; i < 9; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// helper function
void printArray(int arr[]) {
    for (int i = 0; i < 9; i++) {
        std::cout << arr[i] << " ";
    }
}

// clears the arrays
void resetTempArrays() {
    for (int j = 0; j < 9; j++) {
        tempRightSide[j] = rightSide[j];
        tempLeftSide[j] = leftSide[j];
        tempFrontSide[j] = frontSide[j];
        tempBackSide[j] = backSide[j];
        tempTopSide[j] = topSide[j];
        tempBottomSide[j] = bottomSide[j];
    }
}

double easeOutQuad(double t, double duration, double finalVal) {
    return (4 * finalVal / pow(duration, 4)) * pow(duration - t, 3);
}

// Rotation functions
void rotateRightCW(int i) {
    animationOngoing = true;
    double rotationAngle = easeOutQuad(animationTime, animationDur, 91) * deltaTime;

    if (totalRot < 90) {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        if (i == arrayMax(rightSide)) {
            totalRot += rotationAngle;
            animationTime += deltaTime;
        }
    }
    else {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        if (i == arrayMax(rightSide)) {
        
            keyPressed[GLFW_KEY_L] = false;
            animationOngoing = false;
            totalRot = 0.0;
            anotherKeyIsActive = false;
            animationTime = 0.0;
            resetTempArrays();
            int indices1[] = {8, 5, 2};
            int indices3[] = {2, 5, 8};
            for (int j = 0; j < 3; j++) {
                frontSide[3*j + 2] = tempTopSide[indices1[j]];
                bottomSide[3*j + 2] = tempFrontSide[indices3[j]];
                backSide[3*j + 2] = tempBottomSide[indices1[j]];
                topSide[3*j + 2] = tempBackSide[indices3[j]];
            }
            
            int indices2[] = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
            for (int j = 0; j < 9; j++) {
                rightSide[j] = tempRightSide[indices2[j]];
            }
            
            

        }
        
    }
}

void rotateRightCCW(int i) {
    animationOngoing = true;
    double rotationAngle = -easeOutQuad(animationTime, animationDur, 91) * deltaTime;

    if (totalRot > -90) {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        if (i == arrayMax(rightSide)) {
            totalRot += rotationAngle;
            animationTime += deltaTime;
        }
    }
    else {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(-90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        if (i == arrayMax(rightSide)) {
        
            keyPressed[GLFW_KEY_O] = false;
            animationOngoing = false;
            totalRot = 0.0;
            anotherKeyIsActive = false;
            animationTime = 0.0;
            resetTempArrays();
            
            int indices1[] = {8, 5, 2};
            int indices3[] = {2, 5, 8};
            for (int j = 0; j < 3; j++) {
                frontSide[3*j + 2] = tempBottomSide[indices3[j]];
                bottomSide[3*j + 2] = tempBackSide[indices1[j]];
                backSide[3*j + 2] = tempTopSide[indices3[j]];
                topSide[3*j + 2] = tempFrontSide[indices1[j]];

            }
            
            int indices2[] = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };
            for (int j = 0; j < 9; j++) {
                rightSide[j] = tempRightSide[indices2[j]];
            }

        }
        
    }
}

void rotateTopCCW(int i) {
    animationOngoing = true;
    double rotationAngle = easeOutQuad(animationTime, animationDur, 91) * deltaTime;

    if (totalRot < 90) {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
         if (i == arrayMax(topSide)) {
            totalRot += rotationAngle;
            animationTime += deltaTime;
        }
    }
    else {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        if (i == arrayMax(topSide)) {
            keyPressed[GLFW_KEY_E] = false;
            animationOngoing = false;
            totalRot = 0.0;
            anotherKeyIsActive = false;
            animationTime = 0.0;
            resetTempArrays();
            int indices1[] = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
            for (int j = 0; j < 9; j++) {
                topSide[j] = tempTopSide[indices1[j]];
            }
            frontSide[0] = topSide[0];
            frontSide[1] = topSide[1];
            frontSide[2] = topSide[2];

            rightSide[0] = topSide[2];
            rightSide[3] = topSide[5];
            rightSide[6] = topSide[8];

            backSide[0] = topSide[6];
            backSide[1] = topSide[7];
            backSide[2] = topSide[8];

            leftSide[0] = topSide[0];
            leftSide[3] = topSide[3];
            leftSide[6] = topSide[6];          
        }
    }
}

void rotateTopCW(int i) {
    animationOngoing = true;
    double rotationAngle = -easeOutQuad(animationTime, animationDur, 91) * deltaTime;

    if (totalRot > -90) {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
         if (i == arrayMax(topSide)) {
            totalRot += rotationAngle;
            animationTime += deltaTime;
        }
    }
    else {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(-90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        if (i == arrayMax(topSide)) {
            keyPressed[GLFW_KEY_W] = false;
            animationOngoing = false;
            totalRot = 0.0;
            anotherKeyIsActive = false;
            animationTime = 0.0;
            resetTempArrays();
            int indices1[] = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };
            for (int j = 0; j < 9; j++) {
                topSide[j] = tempTopSide[indices1[j]];
            }
            frontSide[0] = topSide[0];
            frontSide[1] = topSide[1];
            frontSide[2] = topSide[2];

            rightSide[0] = topSide[2];
            rightSide[3] = topSide[5];
            rightSide[6] = topSide[8];

            backSide[0] = topSide[6];
            backSide[1] = topSide[7];
            backSide[2] = topSide[8];

            leftSide[0] = topSide[0];
            leftSide[3] = topSide[3];
            leftSide[6] = topSide[6];          
        }
    }
}

void rotateFrontCCW(int i) {
    animationOngoing = true;
    double rotationAngle = easeOutQuad(animationTime, animationDur, 91) * deltaTime;

    if (totalRot < 90) {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateZ(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        
         if (i == arrayMax(frontSide)) {
            totalRot += rotationAngle;
            animationTime += deltaTime;
        }
    }
    else {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateZ(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateZ(90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        if (i == arrayMax(frontSide)) {
            keyPressed[GLFW_KEY_KP_7] = false;
            animationOngoing = false;
            totalRot = 0.0;
            anotherKeyIsActive = false;
            animationTime = 0.0;
            resetTempArrays();
            int indices1[] = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };
            for (int j = 0; j < 9; j++) {
                frontSide[j] = tempFrontSide[indices1[j]];
            }

            topSide[0] = frontSide[0];
            topSide[1] = frontSide[1];
            topSide[2] = frontSide[2];  

            leftSide[0] = frontSide[0];
            leftSide[1] = frontSide[3];
            leftSide[2] = frontSide[6];

            bottomSide[0] = frontSide[6];
            bottomSide[1] = frontSide[7];
            bottomSide[2] = frontSide[8];

            rightSide[0] = frontSide[2];
            rightSide[1] = frontSide[5];
            rightSide[2] = frontSide[8];
            
        }
    }
}

void rotateFrontCW(int i) {
    animationOngoing = true;
    double rotationAngle = -easeOutQuad(animationTime, animationDur, 91) * deltaTime;

    if (totalRot > -90) {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateZ(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        
         if (i == arrayMax(frontSide)) {
            totalRot += rotationAngle;
            animationTime += deltaTime;
        }
    }
    else {
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateZ(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateZ(-90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
        if (i == arrayMax(frontSide)) {
            keyPressed[GLFW_KEY_KP_8] = false;
            animationOngoing = false;
            totalRot = 0.0;
            anotherKeyIsActive = false;
            animationTime = 0.0;
            resetTempArrays();
            int indices1[] = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
            for (int j = 0; j < 9; j++) {
                frontSide[j] = tempFrontSide[indices1[j]];
            }

            topSide[0] = frontSide[0];
            topSide[1] = frontSide[1];
            topSide[2] = frontSide[2];  

            leftSide[0] = frontSide[0];
            leftSide[1] = frontSide[3];
            leftSide[2] = frontSide[6];

            bottomSide[0] = frontSide[6];
            bottomSide[1] = frontSide[7];
            bottomSide[2] = frontSide[8];

            rightSide[0] = frontSide[2];
            rightSide[1] = frontSide[5];
            rightSide[2] = frontSide[8];
            
        }
    }
}

//Display Function
void
display()
{   
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < 27; i++) {
        glBindVertexArray(vaoArray[i]);

        const vec3 displacement(0.0, 0.0, 0.0);

        if ((std::find(std::begin(rightSide), std::end(rightSide), i) != std::end(rightSide))) { // if i is found in rightSide array
            // this if is to solve an (off by one bug). In my model, rotations always lack 2 degrees at the end, so I added these ifs. after the last update of the last cube, my side arrays updated. those updates are the most fundamental part of how I got the correct look.
            

            if (keyPressed[GLFW_KEY_L]) {

                rotateRightCW(i);
 
            }
            
            if (keyPressed[GLFW_KEY_O]) {
                rotateRightCCW(i);
            }

            // right check finishes
        }


        if ((std::find(std::begin(frontSide), std::end(frontSide), i) != std::end(frontSide))) { // if i is found in frontSide array


            if (keyPressed[GLFW_KEY_KP_7]) {
                rotateFrontCCW(i);
            }

            if (keyPressed[GLFW_KEY_KP_8]) {
                rotateFrontCW(i);
            }


          

            // front side check end
        }


        if ((std::find(std::begin(topSide), std::end(topSide), i) != std::end(topSide))) { // if i is found in topside array


            if (keyPressed[GLFW_KEY_E]) {
                rotateTopCCW(i);
            }

            if (keyPressed[GLFW_KEY_W]) {
                rotateTopCW(i);
            }


            // top check ends here
        }




        if (keyPressed[GLFW_KEY_RIGHT] && !keyPressed[GLFW_KEY_LEFT]) { // user rotates around Y
            animationOngoing = true;
            double rotationAngle = -easeOutQuad(animationTime, animationDur, 91) * deltaTime;
            if (totalRot > -90) {
                Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
                if (i == 26) {
                        totalRot += rotationAngle;
                        animationTime += deltaTime;
                }
            }
            else {
                Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
                Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(-90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
                if (i == 26) {
                    keyPressed[GLFW_KEY_RIGHT] = false;
                    animationOngoing = false;
                    totalRot = 0.0;
                    anotherKeyIsActive = false;
                    animationTime = 0.0;
                    resetTempArrays();
                    int indices1[] = {0, 3, 6, 1, 4, 7, 2, 5, 8 };
                    int indices2[] = {2, 5, 8, 1, 4, 7, 0, 3, 6 };
                    int indices3[] = { 6, 7, 8, 3, 4, 5, 0, 1, 2 };
                    for (int j = 0; j < 9; j++) {
                        rightSide[j] = tempBackSide[indices2[j]];
                        frontSide[j] = tempRightSide[indices1[j]];
                        backSide[j] = tempLeftSide[indices1[j]];
                        leftSide[j] = tempFrontSide[indices2[j]];
                        topSide[j] = tempTopSide[indices2[j]];
                        bottomSide[j] = tempBottomSide[indices2[j]];
                    }
                    
                }
            }
        }

       


        if (keyPressed[GLFW_KEY_DOWN] && !keyPressed[GLFW_KEY_UP]) {

            animationOngoing = true;
            double rotationAngle = easeOutQuad(animationTime, animationDur, 91) * deltaTime;
            if (totalRot < 90) {
                Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
                if (i == 26) {
                        totalRot += rotationAngle;
                        animationTime += deltaTime;
                }
            }
            else {
            Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
            Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
            if (i == 26) {
                    keyPressed[GLFW_KEY_DOWN] = false;
                    animationOngoing = false;
                    totalRot = 0.0;
                    anotherKeyIsActive = false;
                    animationTime = 0.0;
                    resetTempArrays();
                    int indices1[] = {6, 3, 0, 7, 4, 1, 8, 5, 2 };
                    int indices2[] = {2, 5, 8, 1, 4, 7, 0, 3, 6 };
                    int indices3[] = {6, 7, 8, 3, 4, 5, 0, 1, 2 };
                    for (int j = 0; j < 9; j++) {
                        bottomSide[j] = tempFrontSide[j];
                        frontSide[j] = tempTopSide[indices3[j]];
                        topSide[j] = tempBackSide[j];
                        backSide[j] = tempBottomSide[indices3[j]];
                        leftSide[j] = tempLeftSide[indices1[j]];
                        rightSide[j] = tempRightSide[indices1[j]];
                    }
                }
            }
            
        }


       

        if (keyPressed[GLFW_KEY_LEFT] && !keyPressed[GLFW_KEY_RIGHT]) {
            animationOngoing = true;
            double rotationAngle = easeOutQuad(animationTime, animationDur, 91) * deltaTime;
            
            if (totalRot < 90) {
                Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
                if (i == 26) {
                        totalRot += rotationAngle;
                        animationTime += deltaTime;
                        
                }
            }
            else {
                Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
                Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateY(90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
                if (i == 26) {
                    keyPressed[GLFW_KEY_LEFT] = false;
                    animationOngoing = false;
                    totalRot = 0.0;
                    anotherKeyIsActive = false;
                    animationTime = 0.0;
                    resetTempArrays();
                    int indices1[] = {0, 3, 6, 1, 4, 7, 2, 5, 8};
                    int indices2[] = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
                    int indices3[] = {6, 7, 8, 3, 4, 5, 0, 1, 2};
                    
                    for (int j = 0; j < 9; j++) {
                        rightSide[j] = tempFrontSide[indices1[j]];
                        backSide[j] = tempRightSide[indices2[j]];
                        leftSide[j] = tempBackSide[indices1[j]];
                        frontSide[j] = tempLeftSide[indices2[j]];
                        topSide[j] = tempTopSide[indices2[j]];
                        bottomSide[j] = tempBottomSide[indices2[j]];
                    }
                }
            }
        }


      

        if (keyPressed[GLFW_KEY_UP] && !keyPressed[GLFW_KEY_DOWN]) { // user rotates around X
            animationOngoing = true;
            double rotationAngle = -easeOutQuad(animationTime, animationDur, 91) * deltaTime;
            if (totalRot > -90) {
                Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(rotationAngle) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
                if (i == 26) {
                        totalRot += rotationAngle;
                        animationTime += deltaTime;
                }
            }
            else {
            Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(-totalRot) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
            Model_view[i] = RotateY(Initial[Yaxis]) * RotateX(Initial[Xaxis]) * RotateX(-90.0) * RotateX(-Initial[Xaxis]) * RotateY(-Initial[Yaxis]) * Model_view[i];
            if (i == 26) {
                    keyPressed[GLFW_KEY_UP] = false;
                    animationOngoing = false;
                    totalRot = 0.0;
                    anotherKeyIsActive = false;
                    animationTime = 0.0;
                    resetTempArrays();
                    int indices1[] = {0, 3, 6, 1, 4, 7, 2, 5, 8 };
                    int indices2[] = {2, 5, 8, 1, 4, 7, 0, 3, 6 };
                    int indices3[] = {6, 7, 8, 3, 4, 5, 0, 1, 2 };
                    for (int j = 0; j < 9; j++) {
                        frontSide[j] = tempBottomSide[j];
                        topSide[j] = tempFrontSide[indices3[j]];
                        backSide[j] = tempTopSide[j];
                        bottomSide[j] = tempBackSide[indices3[j]];
                        leftSide[j] = tempLeftSide[indices2[j]];
                        rightSide[j] = tempRightSide[indices2[j]];
                    }
                }
            }
        }
        


        glUniformMatrix4fv(ModelView, 1, GL_TRUE, Model_view[i]);

        glDrawArrays(GL_TRIANGLES, 0, NumVertices);

    }

   

    mat4  projection;
    projection = Ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0); // Ortho(): user-defined function in mat.h
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    glfwPollEvents();


}


//----------------------------------------------------------------------------

// Creates Random movements to randomization
void randomizeCube() {
    
    int keyArray[] = {GLFW_KEY_L, GLFW_KEY_O, GLFW_KEY_E, GLFW_KEY_W, GLFW_KEY_KP_7, GLFW_KEY_KP_8};
    int numOfFunctions = 6;
    
    int randomNum = std::rand() % numOfFunctions;
    if (!animationOngoing && randCount < MAX_RAND_COUNT) {
        animationDur = 0.2;
        keyPressed[keyArray[randomNum]] = true;
        randCount++;
    }
    if (randCount == MAX_RAND_COUNT) {
        randCount = 0;
        randomize = false;
        animationDur = 0.7;
    }
    
}


//----------------------------------------------------------------------------



// It prints the explanation of our input control (simply to the command line)
void printHelp() {
    printf("R: Randomize the cube\n");
    printf("Q: quit the program\n");
    printf("UP ARROW: Rotate the Cube around X Axis cw\n");
    printf("LEFT ARROW: Rotate the Cube around Y Axis cw\n");
    printf("DOWN ARROW: Rotate the Cube around X Axis ccw\n");
    printf("RIGHT ARROW: Rotate the Cube around Y Axis ccw\n");
    printf("W: Rotate top face clockwise\n");
    printf("E: Rotate top face counter-clockwise\n");
    printf("O: Rotate right face clockwise\n");
    printf("L: Rotate right face counter-clockwise\n");
    printf("8 (Numpad): Rotate front face clockwise\n");
    printf("7 (Numpad): Rotate front face counter-clockwise\n");
    printf("H: Help Menu\n");
    printf("Right mouse click: Rotate the selected face clokwise\n");
    printf("Left mouse click : Rotate the selected face counter-clockwise\n");
}


//Key Functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /*
    printf("Rght\tBack\tLeft\tFrnt\tBttm\tTop\n");
    for (int i = 0; i < 9; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t\n ", rightSide[i], backSide[i], leftSide[i], frontSide[i], bottomSide[i],topSide[i]);
    }
    printf("\n");
    */
    if (animationOngoing) {
        return;
    }
    if (action == GLFW_RELEASE) {
        keyPressed[key] = false;
        anotherKeyIsActive = false;
        return;
    }
    keyPressed[key] = true;
    switch (key) {

        // If user press q the windows closes
    case 'q': case 'Q':
        exit(EXIT_SUCCESS);
        break;
        // If user press h the game command will be printed to console
    case GLFW_KEY_LEFT:
        if (!anotherKeyIsActive) {
            anotherKeyIsActive = true;
            keyPressed[GLFW_KEY_LEFT] = true;

        }
        break;
        // IF user press C the color of the object changes
    case GLFW_KEY_RIGHT:
        if (!anotherKeyIsActive) {
            anotherKeyIsActive = true;
            keyPressed[GLFW_KEY_RIGHT] = true;
        }
        break;
    case GLFW_KEY_DOWN:
        if (!anotherKeyIsActive) {
            anotherKeyIsActive = true;
            keyPressed[GLFW_KEY_DOWN] = true;
        }
        break;
    case GLFW_KEY_UP:

        if (!anotherKeyIsActive) {
            anotherKeyIsActive = true;
            keyPressed[GLFW_KEY_UP] = true;
        }

        break;
    case GLFW_KEY_R:
        if (!anotherKeyIsActive) {
            anotherKeyIsActive = true;
            randomize = true;
        }
        break;
    case GLFW_KEY_H:
        printHelp();
        break;
    }


}


//---------------------------------------------------------------------
//timer function for animation
void setDeltaTime() {
    currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = glfwGetTime();
    // If the window is being moved or rendered very slowly, set deltaTime to zero.
    if (deltaTime > 0.5) {
        deltaTime = 0;
    }
}

//----------------------------------------------------------------------------


//Mouse functions
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

        glBindVertexArray(vaoArray[27]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(ModelView, 1, GL_TRUE, Model_view[27]);
        glDrawArrays(GL_TRIANGLES, 0, NumVertices);

        glFlush();

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        y = height - y;

        unsigned char pixel[4];
        glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

        if ((int)pixel[0] == 255 && (int)pixel[1] == 0 && (int)pixel[2] == 0) {
            //std::cout << "X Coord: " << x << std::endl;
            if (!anotherKeyIsActive) {
                anotherKeyIsActive = true;
                Theta[Zaxis] += 1.0;
                
                keyPressed[GLFW_KEY_L] = true;
            }
        }
        if ((int)pixel[0] == 255 && (int)pixel[1] == 255 && (int)pixel[2] == 0) {
            //std::cout << "X Coord: " << x << std::endl;
            if (!anotherKeyIsActive) {
                anotherKeyIsActive = true;
                Theta[Xaxis] += 1.0;
                keyPressed[GLFW_KEY_E] = true;
                
            }
        }
        if ((int)pixel[0] == 0 && (int)pixel[1] == 0 && (int)pixel[2] == 255) {
            if (!anotherKeyIsActive) {
                anotherKeyIsActive = true;
                Theta[Yaxis] += 1.0;
                keyPressed[GLFW_KEY_KP_7] = true;
                
                
            }

        }

        glfwPollEvents(); //redraw the scene with true colors
    }


    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {

        glBindVertexArray(vaoArray[27]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(ModelView, 1, GL_TRUE, Model_view[27]);
        glDrawArrays(GL_TRIANGLES, 0, NumVertices);

        glFlush();

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        y = height - y;


        unsigned char pixel[4];
        glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

        if ((int)pixel[0] == 255 && (int)pixel[1] == 0 && (int)pixel[2] == 0) {
            //std::cout << "X Coord: " << x << std::endl;

            if (!anotherKeyIsActive) {
                anotherKeyIsActive = true;
                Beta[Zaxis] -= 1.0;
                keyPressed[GLFW_KEY_O] = true;
                
            }
        }

        if ((int)pixel[0] == 255 && (int)pixel[1] == 255 && (int)pixel[2] == 0) {
            //std::cout << "X Coord: " << x << std::endl;
            if (!anotherKeyIsActive) {
                anotherKeyIsActive = true;
                Beta[Xaxis] -= 1.0;
                keyPressed[GLFW_KEY_W] = true;
            }
        }
        if ((int)pixel[0] == 0 && (int)pixel[1] == 0 && (int)pixel[2] == 255) {
            if (!anotherKeyIsActive) {
                anotherKeyIsActive = true;
                Beta[Yaxis] -= 1.0;
                
                keyPressed[GLFW_KEY_KP_8] = true;
            }

        }


        glfwPollEvents(); //redraw the scene with true colors
    }


}
//---------------------------------------------------------------------


// Screen reshape when the size changed code taken from lecture notes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); //may not need this since the default is usually the window size

    // Set projection matrix
    mat4  projection;
    if (width <= height)
        projection = Ortho(-1.0, 1.0, -1.0 * (GLfloat)height / (GLfloat)width,
            1.0 * (GLfloat)height / (GLfloat)width, -1.0, 1.0);
    else
        projection = Ortho(-1.0 * (GLfloat)width / (GLfloat)height, 1.0 *
            (GLfloat)width / (GLfloat)height, -1.0, 1.0, -1.0, 1.0);

    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);



    glfwPollEvents();
    glfwSwapBuffers(window);
}



int main(int argc, char** argv)
{
    // Initialize GLFW
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Initialize current window
    GLFWwindow* window = glfwCreateWindow(500, 500, "Homework 2", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Set the framebuffer size callback function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Set the  keycallback function
    glfwSetKeyCallback(window, key_callback);
    // Set the  mouse_button_callback function
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //comment out the following two lines for windows and linux (don't need glew in macos)
    glewExperimental = GL_TRUE;
    glewInit();

    //If windows not created and give error terminate and return error
    if (!window)
    {

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    //Initilaze firts object as Sphere
    init();

    while (!glfwWindowShouldClose(window))
    {
        //Starts animation
        setDeltaTime();
        
        if (randomize) {
            randomizeCube();
        }
        //Display and Screen events
        display();
        
        glfwSwapBuffers(window);
        glfwPollEvents(); // glfwWaitEvents
    }

    //Terminates the window and program
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
//----------------------------------------------------------------------------



