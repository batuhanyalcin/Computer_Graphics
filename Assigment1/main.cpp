//Batuhan Yalcin
//Gokhan Sari
#include "Angel.h" // Angel.h is a header file containing prototypes for various functions and data types used in the Angel examples.
#include <GLFW/glfw3.h> // GLFW is a library for creating windows with OpenGL contexts and handling input and events.
#include <iostream>
#include <fstream>
#include <stdio.h>
//----------------------------------------------------------------------------

//Type Definitions
typedef vec4  color4;
typedef vec4  point4;
typedef vec3  point3;
//----------------------------------------------------------------------------

// Constants
// Cube Constants
const int NumVerticesCube = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
// Sphere Constants
const int NumTimesToSubdivideSphere = 5;
const int NumTrianglesSphere = 4096;
const int NumVerticesSphere = 3 * NumTrianglesSphere;
// Bunny Constants
const int NumVerticesBunny = 9840 * 3;
vec4 getWhite(1.0, 1.0, 1.0, 1.0);
//----------------------------------------------------------------------------

// Global variables
// 
//For Cube shape Globals
point4 pointsCube[NumVerticesCube];
color4 colorsCube[NumVerticesCube];
int IndexCube = 0;
//For Sphere shape Globals
point4 pointsSphere[NumVerticesSphere];
vec3 normalsSphere[NumVerticesSphere];
color4 colorsSphere[NumVerticesSphere];
int indexSphere = 0;
vec4 getColor = getWhite;
//For Bunny shape Globals
point4 pointsBunny[NumVerticesBunny];
color4 colorsBunny[NumVerticesBunny];
int indexBunnies = 0;
//Color Global variables 
vec4 colors[2] = {
    {0.8, 0.0, 0.05, 0.0},
    {0.05, 0.05, 0.8, 0.0}
};
int currentColorIdx = 0;
vec4 color = colors[currentColorIdx];
//Projection and vertex global variables
point4 vertices[8] = {
    point4(-0.1, -0.1,  0.1, 1.0),
    point4(-0.1,  0.1,  0.1, 1.0),
    point4(0.1,  0.1,  0.1, 1.0),
    point4(0.1, -0.1,  0.1, 1.0),
    point4(-0.1, -0.1, -0.1, 1.0),
    point4(-0.1,  0.1, -0.1, 1.0),
    point4(0.1,  0.1, -0.1, 1.0),
    point4(0.1, -0.1, -0.1, 1.0)
};
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLuint ModelView, Projection;
GLuint program;
//Pyhsic global variables
vec3 position(-0.7, 0.8, 0.0);
vec3 velocity(0.4, 0.0, 0.0);
vec3 g(0.0, -9.81, 0.0);
//Pyhsic Collosion global variables 
GLfloat collisionBoxWidth = 0.2;
GLfloat collisionBoxHeight = 0.2;
//Time global variables
double prevTime = glfwGetTime();
double currentTime = glfwGetTime();
double deltaTime = 0.0;
//Drawing mode global variables
bool wireframe = false;
bool cube = false;
bool sphere = true;
bool bunny = false;
//----------------------------------------------------------------------------


// Function prototypes
void quad(int a, int b, int c, int d); // quad generates two triangles for each face and assigns colors
void drawCube();// drawCube generates 12 triangles: 36 vertices and 36 colors to the vertices
void drawBunny();// drawCube generates all necassary triangles for Bunny
void drawSphere(int count);// drawSphere generates all necassary triangles for Sphere
void triangleSphere(const point4& a, const point4& b, const point4& c); //Divides the triangles gor sğhere given in book 
void divide_triangle(const point4& a, const point4& b, const point4& c, int count); //if not Divides the triangles gor sğhere given in book 
point4 unit(const point4& p);
void switchCube(GLFWwindow* window);     // Switchs to cube
void switchSphere(GLFWwindow* window);  // Switchs to sphere
void switchBunny(GLFWwindow* window);  // Switchs to Bunny
void changeColor(GLFWwindow* window);  // Changes the current collor between red and blue
void display();                        // displays the objects 
void printHelp();                      // It prints the explanation of our input control (simply to the command line)
void initilazeTopLeft(GLFWwindow* window); // It initilaze any object to top left corner of the window 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);  // Key_callback function to define the keyboarc commands
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);    // mouse_button_callback function to define the mouse button commands
void checkCollisions(); // It checks the collisons with wall and updates the velocity and position
void moveObject(); // It moves the object and check each time to any collosion
void setDeltaTime(); // Calculates the time passed between current frame and previous frame.
void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Screen reshape when the size changed code taken from lecture notes
int main(int argc, char** argv); // Main Function of the program 
//----------------------------------------------------------------------------


// quad generates two triangles for each face and assigns colors to the vertices
void quad(int a, int b, int c, int d)
{
    colorsCube[IndexCube] = color;
    pointsCube[IndexCube] = vertices[a];
    IndexCube++;
    colorsCube[IndexCube] = color;
    pointsCube[IndexCube] = vertices[b];
    IndexCube++;
    colorsCube[IndexCube] = color;
    pointsCube[IndexCube] = vertices[c];
    IndexCube++;
    colorsCube[IndexCube] = color;
    pointsCube[IndexCube] = vertices[a];
    IndexCube++;
    colorsCube[IndexCube] = color;
    pointsCube[IndexCube] = vertices[c];
    IndexCube++;
    colorsCube[IndexCube] = color;
    pointsCube[IndexCube] = vertices[d];
    IndexCube++;
}
//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void drawCube()
{
    quad(1, 0, 3, 2);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);
}
//----------------------------------------------------------------------------


//SPHERE Code taken from "Interactive Computer Graphics - A Top-Down Approach with Shader Based Opengl 6th edition"
//"https://www.cs.unm.edu/~angel/BOOK/INTERACTIVE_COMPUTER_GRAPHICS/SIXTH_EDITION/"

void triangleSphere(const point4& a, const point4& b, const point4& c) {
    vec3  normal = normalize(cross(b - a, c - b));

    normalsSphere[indexSphere] = normal;
    pointsSphere[indexSphere] = a;
    indexSphere++;
    normalsSphere[indexSphere] = normal;
    pointsSphere[indexSphere] = b;
    indexSphere++;
    normalsSphere[indexSphere] = normal;
    pointsSphere[indexSphere] = c;
    indexSphere++;
}
//----------------------------------------------------------------------------

/// look after
point4 unit(const point4& p) {
    float len = p.x * p.x + p.y * p.y + p.z * p.z;

    point4 t;
    if (len > DivideByZeroTolerance) {
        t = p / sqrt(len);
        t.w = 1.0;
    }

    return t;
}
//----------------------------------------------------------------------------

// It divides sphere point to triangle
void divide_triangle(const point4& a, const point4& b, const point4& c, int count) {
    if (count > 0) {
        point4 v1 = unit(a + b);
        point4 v2 = unit(a + c);
        point4 v3 = unit(b + c);
        divide_triangle(a, v1, v2, count - 1);
        divide_triangle(c, v2, v3, count - 1);
        divide_triangle(b, v3, v1, count - 1);
        divide_triangle(v1, v3, v2, count - 1);
    }
    else {
        triangleSphere(a, b, c);
    }
}
//----------------------------------------------------------------------------

// It generates all sphere triangles
void drawSphere(int count) {
    point4 v[4] = {
    vec4(0.0, 0.0, 1.0, 100.0),
    vec4(0.0, 0.942809, -0.333333, 100.0),
    vec4(-0.816497, -0.471405, -0.333333, 100.0),
    vec4(0.816497, -0.471405, -0.333333, 100.0)
    };

    divide_triangle(v[0], v[1], v[2], count);
    divide_triangle(v[3], v[2], v[1], count);
    divide_triangle(v[0], v[3], v[1], count);
    divide_triangle(v[0], v[2], v[3], count);
}
//----------------------------------------------------------------------------


// It generates all triangles for Bunny
void drawBunny() {
    std::ifstream bunnyFile("bunny.off");



    GLuint numTrianglesTemp = 0, numVerticesTemp = 0;
    point4 verticesBunny[4922];

    //reads the numbers
    std::string temp;
    bunnyFile >> temp
        >> numVerticesTemp
        >> numTrianglesTemp
        >> temp;

    // Creates vertices of the Bunny
    GLfloat x = 0, y = 0, z = 0;
    for (int i = 0; i < numVerticesTemp; i++) {
        bunnyFile >> x >> y >> z;
        verticesBunny[i] = point4(x * 0.1, y * 0.1, z * 0.1, 1.0);
    }

    //Generates triangles
    int v1 = 0, v2 = 0, v3 = 0;
    for (int i = 0; i < numTrianglesTemp; i++) {
        bunnyFile >> temp >> v1 >> v2 >> v3;
        pointsBunny[3 * i] = verticesBunny[v1];
        pointsBunny[3 * i + 1] = verticesBunny[v2];
        pointsBunny[3 * i + 2] = verticesBunny[v3];
    }
    
}
//----------------------------------------------------------------------------

// Switchs to Bunny
void switchBunny(GLFWwindow* window) {

    // Initilaze cube and shpere to false and Bunny index the zero
    cube = false;
    sphere = false;
    bunny = true;

    indexBunnies = 0;

    // Draws Bunny object and gives color the vertices
    drawBunny();
    for (int i = 0; i < NumVerticesSphere; i++) {
        colorsBunny[i] = colors[currentColorIdx];
    }


    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsBunny) + sizeof(colorsBunny),
        NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsBunny), pointsBunny);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsBunny), sizeof(colorsBunny), colorsBunny);

    // Load shaders and use the resulting shader program
    program = InitShader("shaders/vshader.glsl", "shaders/fshader.glsl");
    glUseProgram(program);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsBunny)));

    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation(program, "ModelView");
    // Rotate the bunny around the y-axis
   

    // Pass the model view matrix to the shader
    Projection = glGetUniformLocation(program, "Projection");

    // Set projection matrix
    mat4  projection;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height); // get current window size
    if (width <= height)
        projection = Ortho(-1.0, 1.0, -1.0 * (GLfloat)height / (GLfloat)width,
            1.0 * (GLfloat)height / (GLfloat)width, -1.0, 1.0);
    else
        projection = Ortho(-1.0 * (GLfloat)width / (GLfloat)height, 1.0 *
            (GLfloat)width / (GLfloat)height, -1.0, 1.0, -1.0, 1.0);

    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    printf("Iam here");
}
//----------------------------------------------------------------------------

// Switchs to cube
void switchCube(GLFWwindow* window) {

    // Initilaze sphere and bunny to false and cube index the zero
    cube = true;
    sphere = false;
    bunny = false;
    IndexCube = 0;

    // Draws cube object
    drawCube();

   
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(colorsCube), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsCube), pointsCube);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube), sizeof(colorsCube), colorsCube);

    // Set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube)));



}
//----------------------------------------------------------------------------


// Switchs to sphere
void switchSphere(GLFWwindow* window) {
    
    // Initilaze cube and bunny  false and sphere index the zero
    cube = false;
    sphere = true;
    bunny = false;
    indexSphere = 0;

    // Draws sphere object and gives color the vertices
    drawSphere(NumTimesToSubdivideSphere);
    for (int i = 0; i < NumVerticesSphere; i++) {
        colorsSphere[i] = colors[currentColorIdx];
    }
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsSphere) + sizeof(colorsSphere),
        NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsSphere), pointsSphere);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSphere), sizeof(colorsSphere), colorsSphere);

    // Load shaders and use the resulting shader program
    program = InitShader("shaders/vshader.glsl", "shaders/fshader.glsl");
    glUseProgram(program);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSphere)));

    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");

    // Set projection matrix
   
    mat4  projection;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height); // get current window size
    if (width <= height)
        projection = Ortho(-1.0, 1.0, -1.0 * (GLfloat)height / (GLfloat)width,
            1.0 * (GLfloat)height / (GLfloat)width, -1.0, 1.0);
    else
        projection = Ortho(-1.0 * (GLfloat)width / (GLfloat)height, 1.0 *
            (GLfloat)width / (GLfloat)height, -1.0, 1.0, -1.0, 1.0);

    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);

}
//----------------------------------------------------------------------------



// Changes the current collor between red and blue
void changeColor(GLFWwindow* window) {
    currentColorIdx = (currentColorIdx + 1) % 2;
    color = colors[currentColorIdx];
    if (sphere) {
        switchSphere(window);
    }
    else if (cube) {
        switchCube(window);
    }
    else {
        switchBunny(window);
    }
   
}
//----------------------------------------------------------------------------


// displays the objects 
void display()
{
    if (cube) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4  model_view = (Translate(position) * Scale(1.0, 1.0, 1.0) *
            RotateX(Theta[Xaxis]) *
            RotateY(Theta[Yaxis]) *
            RotateZ(Theta[Zaxis]));

        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        glDrawArrays(GL_TRIANGLES, 0, NumVerticesCube);
    }
    else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4  model_view = (Translate(position) * Scale(0.1, 0.1, 0.1) *
            RotateX(Theta[Xaxis]) *
            RotateY(Theta[Yaxis]) *
            RotateZ(Theta[Zaxis]));

        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        glDrawArrays(GL_TRIANGLES, 0, NumVerticesSphere);
    }




}
//----------------------------------------------------------------------------

// It prints the explanation of our input control (simply to the command line)
void printHelp() {
    printf("i: initialize the pose\n");
    printf("c: switch color of the object\n");
    printf("q: quit the program\n");
    printf("Right mouse click: Switch object type\n");
    printf("Left mouse click : Switch drawing mode\n");
}
//----------------------------------------------------------------------------


// It initilaze any object to top left corner of the window 
void initilazeTopLeft(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height); // get current window size
    //Get current aspect ratios
    GLfloat aspect = GLfloat(width) / height;
    //Updates object to top left what ever size is
    position = vec3(-0.9, 0.9, 0.0)*aspect;
    velocity = vec3(0.4, 0.0, 0.0)*aspect;
}
//----------------------------------------------------------------------------

// Key_callback function to define the keyboarc commands
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE) {
        return;
    }

    switch (key) {

    // If user press q the windows closes
    case 'q': case 'Q':
        exit(EXIT_SUCCESS);
        break;
    // If user press h the game command will be printed to console
    case GLFW_KEY_H:
        printHelp();
        break;
    // If user press I the object initilazes to the initial position 
    case GLFW_KEY_I:
        initilazeTopLeft(window);
        break;
    // IF user press C the color of the object changes
    case GLFW_KEY_C:
        changeColor(window);
        break;
    }
}
//----------------------------------------------------------------------------

// mouse_button_callback function to define the mouse button commands
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{   
    if (action == GLFW_PRESS) {
        switch (button) {
        // If user press button right click it switchs the object
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (sphere) {
                
                switchCube(window);
                glfwPollEvents();
            }
            else if (cube) {
                switchBunny(window);
                glfwPollEvents();
            }
            else {
                switchSphere(window);
                glfwPollEvents();
            }
            ;  break;
        // If user press left click it switchs the fill option to get wireframe or the solid
        case GLFW_MOUSE_BUTTON_LEFT:
            
            if (wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            wireframe = !wireframe;
            break;
        }
    }
}
//----------------------------------------------------------------------------
 

// It checks the collisons with wall and updates the velocity and position
void checkCollisions() {
    // Bottom collision
    if (position.y - collisionBoxHeight / 2 < -1) {
        position.y = -1 + collisionBoxHeight / 2;
        double energyLoss = velocity.y / 10;
        velocity.y = -velocity.y + energyLoss;
    }

    // Top collision
    if (position.y + collisionBoxHeight / 2 > 1) {
        position.y = 1 - collisionBoxHeight / 2;
        velocity.y = -velocity.y;
    }

    //Since the description writes either continues from window we erase this part to see collision un comment this lines 
    //Also donot for get the take window as input 
    
    //glfwGetFramebufferSize(window, &width, &height); // get current window size
    //  GLfloat aspect = GLfloat(width) / height;
        // Right collision
    //if ((position.x + collisionBoxWidth / 2)*aspect > 1) {
      //  position.x = (1 - collisionBoxWidth / 2)*aspect;
      //  velocity.x = -velocity.x;
    //}

    // Left collision
    //if ((position.x - collisionBoxWidth / 2)*aspect < -1) {
      //  position.x = (-1 + collisionBoxWidth / 2)*aspect;
      //  velocity.x = -velocity.x;
    //}

}
//----------------------------------------------------------------------------

// It moves the object and check eact time to any collosion
void moveObject() {
    
    // Update velocity and time
    velocity += g * deltaTime;
    // Update position
    position += velocity * deltaTime;

    // Check collisions
    checkCollisions();

}
//----------------------------------------------------------------------------

// Calculates the time passed between current frame and previous frame.
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

    // Recalculate the collision box dimensions may not need if you do not want the collide with the right corner
    //GLfloat aspect = GLfloat(width) / height;
    //GLfloat collisionBoxWidth = 0.2 * aspect;
    //GLfloat collisionBoxHeight = 0.2;

    glfwPollEvents();
    glfwSwapBuffers(window);
}
//----------------------------------------------------------------------------



int main(int argc, char** argv)
{
    // Initialize GLFW
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Initialize current window
    GLFWwindow* window = glfwCreateWindow(500, 500, "Homework1", NULL, NULL);
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
    switchSphere(window);

    while (!glfwWindowShouldClose(window))
    {
        //Starts animation
        setDeltaTime();
        moveObject();

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