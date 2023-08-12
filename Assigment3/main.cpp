//Batuhan Yalcin
//Gokhan Sari
#include "Angel.h" // Angel.h is a header file containing prototypes for various functions and data types used in the Angel examples.
#include <GLFW/glfw3.h> // GLFW is a library for creating windows with OpenGL contexts and handling input and events.
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
//----------------------------------------------------------------------------

//Type Definitions
typedef vec4  color4;
typedef vec4  point4;
typedef vec3  point3;
//----------------------------------------------------------------------------



#define NUM_OF_FRAMES 379
#define NUM_OF_POINTS 33
#define NUM_OF_UZUV 12
#define FRAME_RATE 60

vec3 middlePoint[NUM_OF_UZUV] = { vec3(0, 0, 0) };
GLfloat dist[NUM_OF_UZUV] = { 0.0 };
GLfloat thetaZ[NUM_OF_UZUV] = { 0.0 };
GLfloat thetaX[NUM_OF_UZUV] = { 0.0 };
int p1Array[NUM_OF_UZUV] = { 11, 13, 12, 14, 23, 25, 24, 26, 11, 12, 24, 23 };
int p2Array[NUM_OF_UZUV] = { 13, 15, 14, 16, 25, 27, 26, 28, 12, 24, 23, 11 };

const int NumVertices = 36; 


point4 points[NUM_OF_UZUV][NumVertices];
vec3   normals[NUM_OF_UZUV][NumVertices];
vec3 positions[NUM_OF_FRAMES][NUM_OF_POINTS];
int currentFrameIdx = 0;

//----------------------------------------------------------------------------
void updateUzuv(int p1, int p2, int index) {
    middlePoint[index] = (positions[currentFrameIdx][p1] + positions[currentFrameIdx][p2]) / 2;
    dist[index] = sqrt(dot((positions[currentFrameIdx][p1] - positions[currentFrameIdx][p2]), (positions[currentFrameIdx][p1] - positions[currentFrameIdx][p2])));
    thetaZ[index] = -atan(((positions[currentFrameIdx][p1].x - positions[currentFrameIdx][p2].x) / (positions[currentFrameIdx][p1].y - positions[currentFrameIdx][p2].y))) * 180 / 3.14159265;
    thetaX[index] = atan((positions[currentFrameIdx][p1].z - positions[currentFrameIdx][p2].z) / sqrt(((positions[currentFrameIdx][p1].x - positions[currentFrameIdx][p2].x) * (positions[currentFrameIdx][p1].x - positions[currentFrameIdx][p2].x) + (positions[currentFrameIdx][p1].y - positions[currentFrameIdx][p2].y) * (positions[currentFrameIdx][p1].y - positions[currentFrameIdx][p2].y)))) * 180 / 3.14159265;


}

point4 vertices[8] = {
    point4(-0.5, -0.5,  0.5, 1.0),
    point4(-0.5,  0.5,  0.5, 1.0),
    point4(0.5,  0.5,  0.5, 1.0),
    point4(0.5, -0.5,  0.5, 1.0),
    point4(-0.5, -0.5, -0.5, 1.0),
    point4(-0.5,  0.5, -0.5, 1.0),
    point4(0.5,  0.5, -0.5, 1.0),
    point4(0.5, -0.5, -0.5, 1.0)
};



void update(void)
{

    for (int i = 0; i < NUM_OF_UZUV; i++) {
        updateUzuv(p1Array[i], p2Array[i], i);
    }
    /*
    middlePoint[0] = (positions[currentFrameIdx][13] + positions[currentFrameIdx][11]) / 2;
    dist[0] = sqrt(dot((positions[currentFrameIdx][13] - positions[currentFrameIdx][11]), (positions[currentFrameIdx][13] - positions[currentFrameIdx][11])));
    thetaZ[0] = - atan(((positions[currentFrameIdx][13].x - positions[currentFrameIdx][11].x) /  (positions[currentFrameIdx][13].y - positions[currentFrameIdx][11].y))) * 180 / 3.14159265;
    thetaX[0] = atan((positions[currentFrameIdx][13].z - positions[currentFrameIdx][11].z) / sqrt(((positions[currentFrameIdx][13].x - positions[currentFrameIdx][11].x) * (positions[currentFrameIdx][13].x - positions[currentFrameIdx][11].x)  + (positions[currentFrameIdx][13].y - positions[currentFrameIdx][11].y) * (positions[currentFrameIdx][13].y - positions[currentFrameIdx][11].y)))) * 180 / 3.14159265;

    middlePoint[1] = (positions[currentFrameIdx][13] + positions[currentFrameIdx][15]) / 2;
    dist[1] = sqrt(dot((positions[currentFrameIdx][13] - positions[currentFrameIdx][15]), (positions[currentFrameIdx][13] - positions[currentFrameIdx][15])));
    thetaZ[1] = - atan(((positions[currentFrameIdx][13].x - positions[currentFrameIdx][15].x) /  (positions[currentFrameIdx][13].y - positions[currentFrameIdx][15].y))) * 180 / 3.14159265;
    thetaX[1] = atan((positions[currentFrameIdx][13].z - positions[currentFrameIdx][15].z) / sqrt(((positions[currentFrameIdx][13].x - positions[currentFrameIdx][15].x) * (positions[currentFrameIdx][13].x - positions[currentFrameIdx][15].x)  + (positions[currentFrameIdx][13].y - positions[currentFrameIdx][15].y) * (positions[currentFrameIdx][13].y - positions[currentFrameIdx][15].y)))) * 180 / 3.14159265;
    */

    //currentFrameIdx = 1;
    //currentFrameIdx++;
    currentFrameIdx = (currentFrameIdx + 1) % (NUM_OF_FRAMES);
    //printf("Current frame: %d\n",currentFrameIdx);
}



vec3 displacement(0, 0, 0);
point4 light_position(0.0, 0.0, 2.0, 0.0);
color4 light_ambient(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0);
//color4 light_color(1.0, 1.0, 1.0, 1.0);
color4 material_ambient = color4(0.1, 0.1, 0.1, 1.0);
color4 material_diffuse = color4(0.4, 0.4, 0.4, 1.0);
color4 material_specular = color4(0.7, 0.7, 0.7, 1.0);
GLfloat material_shininess = 10;
color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product = light_diffuse * material_diffuse;
color4 specular_product = light_specular * material_specular;
float first = 0; 

// Texture objects and storage for texture image
GLuint textures[2];
const int  TextureSize = 64;
GLubyte* image;
GLubyte* image2;

GLuint default_textures;


// Constants
// Cube Constants
const int NumVerticesCube = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
// Sphere Constants
const int NumTimesToSubdivideSphere = 5;
const int NumTrianglesSphere = 4096;
const int NumVerticesSphere = 3 * NumTrianglesSphere;
vec3    tex_coords[NumVerticesSphere];
int Zclose = 3, Zaway = 7;
//----------------------------------------------------------------------------

// Global variables
// 
//For Cube shape Globals

std::string fshaders[] = { "shaders/fshader.glsl", "shaders/fshader_phong.glsl" };
std::string vshaders[] = { "shaders/vshader.glsl", "shaders/vshader_phong.glsl" };
int currentShaderIdx = 0;

std::string vshader = vshaders[currentShaderIdx];
std::string fshader = fshaders[currentShaderIdx];



//For Sphere shape Globals
point4 pointsSphere[NumVerticesSphere];
vec3 normalsSphere[NumVerticesSphere];
//color4 colorsSphere[NumVerticesSphere];
int indexSphere = 0;

float materail_products = 4;
//Color Global variables 
//vec4 colors[2] = {
//    {0.8, 0.0, 0.05, 0.0},
//    {0.05, 0.05, 0.8, 0.0}
//};
int currentColorIdx = 0;
//vec4 color = colors[currentColorIdx];
//Projection and vertex global variables
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLuint ModelView, Projection, ModelView2, Lightning;
GLuint program;
bool lighting = true; //enable lighting
GLfloat scaleFactor = 1.0;
//Pyhsic global variables
vec3 position(-0.7, 0.8, 0.0);
vec3 velocity(0.1, 0.0, 0.0);
//vec3 g(0.0, -9.81, 0.0);
vec3 g(0.0, -9.81/3.00, 0.0);
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
bool is_metal = false;
bool lightMoving = false;
int textureFlag = 0; //enable texture mapping
int shaderFlag = 0;
bool is_basketball = false;
GLuint  TextureFlagLoc; // texture flag uniform location
// Zoom Global vaiables
double zommingScale = 1.0;
// Display modes wireframe=1 shading 2 tecture = 3
float displaymode = 1;
//----------------------------------------------------------------------------


// Function prototypes
void drawSphere(int count);// drawSphere generates all necassary triangles for Sphere
void triangleSphere(const point4& a, const point4& b, const point4& c); //Divides the triangles gor sğhere given in book 
void divide_triangle(const point4& a, const point4& b, const point4& c, int count); //if not Divides the triangles gor sğhere given in book 
point4 unit(const point4& p);
void switchSphere(GLFWwindow* window);  // Switchs to sphere
void changeColor(GLFWwindow* window);  // Changes the current collor between red and blue
void display();                        // displays the objects 
void printHelp();                      // It prints the explanation of our input control (simply to the command line)
void initilazeTopLeft(GLFWwindow* window); // It initilaze any object to top left corner of the window 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);  // Key_callback function to define the keyboarc commands
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);    // mouse_button_callback function to define the mouse button commands
void moveObject(); // It moves the object and check each time to any collosion
void setDeltaTime(); // Calculates the time passed between current frame and previous frame.
void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Screen reshape when the size changed code taken from lecture notes
int main(int argc, char** argv); // Main Function of the program 
//----------------------------------------------------------------------------
void readPPMofBasketball() {

    FILE* fd;
    int k, n, m;
    char c;
    int i;
    char b[] = "basketball.ppm";
    int red, blue, green;

    fd = fopen(b, "r");
    fscanf(fd, "%[^\n] ", b);

    if (b[0] != 'P' || b[1] != '3') {
        printf("Not a ppm file.");
        exit(1);
    }
    printf("Valid file.");
    fscanf(fd, "%c", &c);

    while (c == '#') {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n", b);
        fscanf(fd, "%c", &c);
    }

    ungetc(c, fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    //printf("%d rows %d columns max value= %d\n",n,m,k);

    int nm = n * m;

    image = (GLubyte*)malloc(3 * sizeof(GLubyte) * nm);

    for (i = nm; i > 0; i--) {

        fscanf(fd, "%d %d %d", &red, &green, &blue);
        image[3 * nm - 3 * i] = red;
        image[3 * nm - 3 * i + 1] = green;
        image[3 * nm - 3 * i + 2] = blue;

    }
}

// earth.ppm read -----------------------------------------------------------------------------------------------------------------------

void readPPMofWorldMap() {

    FILE* fd;
    int k, n, m;
    char c;
    int i;
    char b[] = "earth.ppm";
    int red, blue, green;

    fd = fopen(b, "r");
    fscanf(fd, "%[^\n] ", b);

    if (b[0] != 'P' || b[1] != '3') {
        printf("Not a ppm file.");
        exit(1);
    }
    printf("Valid file.");
    fscanf(fd, "%c", &c);

    while (c == '#') {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n", b);
        fscanf(fd, "%c", &c);
    }

    ungetc(c, fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    //printf("%d rows %d columns max value= %d\n",n,m,k);

    int nm = n * m;

    image2 = (GLubyte*)malloc(3 * sizeof(GLubyte) * nm);

    for (i = nm; i > 0; i--) {

        fscanf(fd, "%d %d %d", &red, &green, &blue);
        image2[3 * nm - 3 * i] = red;
        image2[3 * nm - 3 * i + 1] = green;
        image2[3 * nm - 3 * i + 2] = blue;

    }
}



//----------------------------------------------------------------------------


//SPHERE Code taken from "Interactive Computer Graphics - A Top-Down Approach with Shader Based Opengl 6th edition"
//"https://www.cs.unm.edu/~angel/BOOK/INTERACTIVE_COMPUTER_GRAPHICS/SIXTH_EDITION/"

void triangleSphere(const point4& a, const point4& b, const point4& c) {
    vec3 norm = normalize(vec3(a.x, a.y, a.z));

    normalsSphere[indexSphere] = vec3(norm.x, norm.y, norm.z);
    pointsSphere[indexSphere] = a;
    tex_coords[indexSphere] = vec3(a.x, a.y, a.z); // TEXTURE POINTS for a
    indexSphere++;
    norm = normalize(vec3(b.x, b.y, b.z));
    normalsSphere[indexSphere] = vec3(norm.x, norm.y, norm.z);
    pointsSphere[indexSphere] = b;
    tex_coords[indexSphere] = vec3(b.x, b.y, b.z); // TEXTURE POINTS for a
    indexSphere++;
    norm = normalize(vec3(c.x, c.y, c.z));
    normalsSphere[indexSphere] = vec3(norm.x, norm.y, norm.z);
    pointsSphere[indexSphere] = c;
    tex_coords[indexSphere] = vec3(c.x, c.y, c.z); // TEXTURE POINTS for a
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



int Index = 0;

void
quad(int a, int b, int c, int d, int cubeIndex)
{
    // Initialize temporary vectors along the quad's edge to
    //   compute its face normal 
    vec4 u = vertices[b] - vertices[a];
    vec4 v = vertices[c] - vertices[b];

    vec3 normal = normalize(cross(u, v));

    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[a]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[b]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[c]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[a]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[c]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[d]; Index++;

    //Note that normals are fixed for a given face of the cube.
    //So the normal of a vertex is NOT computed based on neighboring faces.
    //which makes sense in this example since this is a cube with only 6 faces.
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    for (int i = 0; i < NUM_OF_UZUV; i++) {
        quad(1, 0, 3, 2, i);
        quad(2, 3, 7, 6, i);
        quad(3, 0, 4, 7, i);
        quad(6, 5, 1, 2, i);
        quad(4, 5, 6, 7, i);
        quad(5, 4, 0, 1, i);
        Index = 0;
    }

}



int IndexSphere = 0;

void
triangle(const point4& a, const point4& b, const point4& c)
{
    //normal vector is computed per vertex

    vec3 norm = normalize(vec3(a.x, a.y, a.z));
    normalsSphere[IndexSphere] = vec3(norm.x, norm.y, norm.z);  pointsSphere[IndexSphere] = a; IndexSphere++;
    norm = normalize(vec3(b.x, b.y, b.z));
    normalsSphere[IndexSphere] = vec3(norm.x, norm.y, norm.z);   pointsSphere[IndexSphere] = b; IndexSphere++;
    norm = normalize(vec3(c.x, c.y, c.z));
    normalsSphere[IndexSphere] = vec3(norm.x, norm.y, norm.z);   pointsSphere[IndexSphere] = c; IndexSphere++;

}
//----------------------------------------------------------------------------





void
tetrahedron(int count)
{
    point4 v[4] = {
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.942809, -0.333333, 1.0),
        vec4(-0.816497, -0.471405, -0.333333, 1.0),
        vec4(0.816497, -0.471405, -0.333333, 1.0)
    };

    divide_triangle(v[0], v[1], v[2], count);
    divide_triangle(v[3], v[2], v[1], count);
    divide_triangle(v[0], v[3], v[1], count);
    divide_triangle(v[0], v[2], v[3], count);
}


GLuint vao[NUM_OF_UZUV + 1];
GLuint buffer[NUM_OF_UZUV + 1];
const int NumTimesToSubdivide = 5;
void switchMotion(GLFWwindow* window) {

    glewExperimental = GL_TRUE;
    glewInit();
    colorcube();
    tetrahedron(NumTimesToSubdivide);


    // Open landmarks file
    FILE* positionsFile = fopen("landmarks.txt", "r");
    if (positionsFile == NULL) {
        printf("File cannot be opened\n");
    }

    // Read file content and save it to positions
    float x, y, z;
    int frameIdx;
    int locIdx;
    int i = 0;
    while (fscanf(positionsFile, "%f %f %f", &x, &y, &z) == 3) {
        locIdx = i % NUM_OF_POINTS;
        frameIdx = i / NUM_OF_POINTS;
        //positions[frameIdx][locIdx] = vec3((x-640.0)/640.0, (y-360.0)/640.0, z);
        positions[frameIdx][locIdx] = vec3((x - 640.0) / 360.0, (360.0 - y) / 360.0, z);
        //positions[frameIdx][locIdx] = vec3(x, y, z);
        i++;
    }
    printf("%d\n", frameIdx);
    fclose(positionsFile);

    GLuint program = InitShader("vshader_phong.glsl", "fshader_phong.glsl"); //use vshader_phong.glsl, fshader_phong.glsl for phong shading
    glUseProgram(program);

    // Create a vertex array object
    glGenVertexArrays(NUM_OF_UZUV + 1, vao);



    for (int i = 0; i < NUM_OF_UZUV; i++) {
        glBindVertexArray(vao[i]);

        // Create and initialize a buffer object

        glGenBuffers(1, &buffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points[i]) + sizeof(normals[i]), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points[i]), points[i]);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points[i]), sizeof(normals[i]), normals[i]);

        // set up vertex arrays
        GLuint vPosition = glGetAttribLocation(program, "vPosition");
        glEnableVertexAttribArray(vPosition);
        glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

        GLuint vNormal = glGetAttribLocation(program, "vNormal");
        glEnableVertexAttribArray(vNormal);
        glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points[i])));

    }

    glBindVertexArray(vao[NUM_OF_UZUV]);

    // Create and initialize a buffer object

    glGenBuffers(1, &buffer[NUM_OF_UZUV]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[NUM_OF_UZUV]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsSphere) + sizeof(normalsSphere), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsSphere), pointsSphere);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSphere), sizeof(normalsSphere), normalsSphere);


    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points[i])));

    // Load shaders and use the resulting shader program


    // set up vertex arrays


    // Initialize shader lighting parameters
    point4 light_position(0.0, 0.0, 1.0, 0.0); //directional light source
    color4 light_ambient(0.2, 0.2, 0.2, 1.0); // L_a
    color4 light_diffuse(1.0, 1.0, 1.0, 1.0); // L_d
    color4 light_specular(1.0, 1.0, 1.0, 1.0); // L_s

    color4 material_ambient(1.0, 0.0, 1.0, 1.0); // k_a
    color4 material_diffuse(1.0, 0.8, 0.0, 1.0); // k_d
    color4 material_specular(1.0, 0.8, 0.0, 1.0); // k_s
    float  material_shininess = 100.0;

    color4 ambient_product = light_ambient * material_ambient; // k_a * L_a
    color4 diffuse_product = light_diffuse * material_diffuse; // k_d * L_d
    color4 specular_product = light_specular * material_specular; // k_s * L_s
    glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
    glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
    glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);

    glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);

    glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);


    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");

    /*
    mat4  projection = Perspective( 45.0, 1.0, 0.5, 6.0 );
    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
    */
    mat4 projection = Ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // Ortho(): user-defined function in mat.h
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0, 1.0, 1.0, 1.0);





}






// Switchs to sphere
void switchSphere(GLFWwindow* window) {

    // Initilaze cube and bunny  false and sphere index the zero
    sphere = true;
    indexSphere = 0;

    // Draws sphere object and gives color the vertices
    drawSphere(NumTimesToSubdivideSphere);


    glGenTextures(2, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 256, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 1024, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (first == 0) {
        glBindTexture(GL_TEXTURE_2D, textures[0]);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, default_textures);
    }


    if (image == nullptr) {
        // Failed to load the image
        std::cout << "Failed to load the image." << std::endl;

    }
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsSphere) + sizeof(normalsSphere) + sizeof(tex_coords),
        NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsSphere), pointsSphere);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSphere), sizeof(normalsSphere), normalsSphere);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSphere) + sizeof(normalsSphere), sizeof(tex_coords), tex_coords);

    // Load shaders and use the resulting shader program
    program = InitShader(vshader.c_str(), fshader.c_str());
    glUseProgram(program);


    // Set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    //GLuint vColor = glGetAttribLocation(program, "vColor");
    //glEnableVertexAttribArray(vColor);
    //glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSphere)));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSphere)));

    //GLuint vCoords = glGetAttribLocation(program, "vCoords");
    //glEnableVertexAttribArray(vCoords);
    //glVertexAttribPointer(vCoords, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4) * 3 * NumTrianglesSphere + sizeof(vec3) * 3 * NumTrianglesSphere));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSphere) + sizeof(normalsSphere)));


    // Book page 635

    if (materail_products == 4) {
        glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
            1, ambient_product);
        glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
            1, diffuse_product);
        glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
            1, specular_product);
    }
    else if (materail_products == 3) {
        glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
            1, diffuse_product);
        glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
            1, diffuse_product);

    }
    else if (materail_products == 2) {
        glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
            1, ambient_product);
        glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
            1, specular_product);
    }
    else if (materail_products == 1) {
        glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
            1, diffuse_product);
        glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
            1, specular_product);
    }

    glUniform4fv(glGetUniformLocation(program, "LightPosition"),
        1, light_position);
    //glUniform4fv(glGetUniformLocation(program, "LightPosition2"),
      //  1, light_position2);



    glUniform1f(glGetUniformLocation(program, "Shininess"),
        material_shininess);

    glUniform1i(glGetUniformLocation(program, "L1_Toggle"), 1);
    glUniform1i(glGetUniformLocation(program, "TextureFlag"), textureFlag);
    glUniform1i(glGetUniformLocation(program, "ShaderFlag"), shaderFlag);
    //glUniform1i(glGetUniformLocation(program, "L2_Toggle"), 1);

    //GLuint DrawingMode = glGetUniformLocation(program, "DRAWING_MODE_GLOBAL");
    //GLuint Secondary_Mode = glGetUniformLocation(program, "SECONDARY_MODE_GLOBAL");
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");
    //ModelView2 = glGetUniformLocation(program, "ModelView2");


    // Set projection matrix

    mat4  projection;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height); // get current window size
    //displacement = vec3(0, 0, 2.5);
    //GLfloat aspect = GLfloat(width) / height;
    //projection = Perspective(45, aspect, Zclose, Zaway);
    
    if (width <= height)
        projection = Ortho(-1.0, 1.0, -1.0 * (GLfloat)height / (GLfloat)width,
            1.0 * (GLfloat)height / (GLfloat)width, -1.0, 1.0);
    else
        projection = Ortho(-1.0 * (GLfloat)width / (GLfloat)height, 1.0 *
            (GLfloat)width / (GLfloat)height, -1.0, 1.0, -1.0, 1.0);
    
    //projection = Perspective( 60.0, -1.0, -2.0, 6.0 );
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    // We already enable the DEPTH_TEST in assigment 1
    glEnable(GL_DEPTH_TEST);
    // Enabling culling edge
    glEnable(GL_CULL_FACE);
    glClearColor(1.0, 1.0, 1.0, 1.0);



}
//----------------------------------------------------------------------------



void moveLight(int offset, int axis) {
    if (axis == Xaxis) {
        light_position = RotateX(offset) * light_position;
        glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, Translate(position) * light_position);
    }
    else if (axis == Yaxis) {
        light_position = RotateY(offset) * light_position;
        glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, Translate(position) * light_position);
    }
    else if (axis == Zaxis) {
        light_position = RotateZ(offset) * light_position;
        glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, Translate(position) * light_position);
    }
}


// Changes the current collor between red and blue
void changeColor(GLFWwindow* window) {
    //currentColorIdx = (currentColorIdx + 1) % 2;
    //color = colors[currentColorIdx];
    //if (sphere) {
    //    switchSphere(window);
    //}

}
//----------------------------------------------------------------------------

// Changes the current collor between red and blue
void changeShader(GLFWwindow* window) {
    currentShaderIdx = !currentShaderIdx;
    std::string vshader = vshaders[currentShaderIdx];
    std::string fshader = fshaders[currentShaderIdx];
    switchSphere(window);

}

// displays the objects 
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    mat4  model_view = (Translate(position) * Scale(0.1 * zommingScale, 0.1 * zommingScale, 0.1 * zommingScale) *
        RotateX(Theta[Xaxis]) *
        RotateY(Theta[Yaxis]) *
        RotateZ(Theta[Zaxis]));

    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    glDrawArrays(GL_TRIANGLES, 0, NumVerticesSphere);

    glFlush();





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
    position = vec3(-0.9, 0.9, 0.0) * aspect;
    velocity = vec3(0.1, 0.0, 0.0) * aspect;
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
    case GLFW_KEY_R:
        initilazeTopLeft(window);
        zommingScale = 1.0;
        light_position = point4(0.0, 0.0, 2.0, 1.0);
        break;
        // IF user press C the color of the object changes
    case GLFW_KEY_C:
        changeColor(window);
        break;

    case GLFW_KEY_O:
        materail_products -= 1;
        if (materail_products == 0) {
            materail_products = 4;
        }
        break;
    case GLFW_KEY_M:
        if (is_metal) {
            material_ambient = color4(0.01, 0.01, 0.01, 1.0);
            material_diffuse = color4(0.4, 0.4, 0.4, 1.0);
            material_specular = color4(0.5, 0.5, 0.5, 1.0);
            material_shininess = 5;

            is_metal = false;
        }
        else {
            material_ambient = color4(0.1, 0.1, 0.8, 1.0);
            material_diffuse = color4(0.1, 0.1, 0.6, 1.0);
            material_specular = color4(0.2, 0.2, 0.8, 1.0);

            material_shininess = 10;

            is_metal = true;
        }
        ambient_product = light_ambient * material_ambient;
        diffuse_product = light_diffuse * material_diffuse;
        specular_product = light_specular * material_specular;


        break;
    case GLFW_KEY_Z:
        zommingScale += zommingScale * 0.1;
        break;
    case GLFW_KEY_W:
        zommingScale -= zommingScale * 0.1;
        break;

    case GLFW_KEY_S:
    shaderFlag = !shaderFlag;
        glUniform1i(glGetUniformLocation(program, "ShaderFlag"), shaderFlag);
        //changeShader(window);
        break;

    case GLFW_KEY_L:
        lightMoving = !lightMoving;
        break;

    case GLFW_KEY_T:
        displaymode -= 1;
        textureFlag = 0;
        if (displaymode == 2) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }
        else if (displaymode == 1) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else {
            textureFlag = 1;
            displaymode = 3;
        }
        break;

    case GLFW_KEY_I:

        is_basketball = !is_basketball;
        default_textures = textures[is_basketball];
        first = 1;
        break;

    case GLFW_KEY_UP:
        Theta[Xaxis] += 3.0;

        if (Theta[Xaxis] > 360.0) {
            Theta[Xaxis] -= 360.0;
        }
        break;

    case GLFW_KEY_DOWN:
        Theta[Xaxis] -= 3.0;

        if (Theta[Xaxis] < -360.0) {
            Theta[Xaxis] += 360.0;
        }
        break;

    case GLFW_KEY_LEFT:
        Theta[Zaxis] += 3.0;

        if (Theta[Zaxis] > 360.0) {
            Theta[Zaxis] -= 360.0;
        }
        break;

    case GLFW_KEY_RIGHT:
        Theta[Zaxis] -= 3.0;

        if (Theta[Zaxis] < -360.0) {
            Theta[Zaxis] += 360.0;
        }
        break;

    }

    if (displaymode != 0) {
        switchSphere(window);
    }
    glfwPollEvents();
    glfwSwapBuffers(window);
}
//----------------------------------------------------------------------------

// mouse_button_callback function to define the mouse button commands
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (button) {
            // If user press button right click it switchs the object
        case GLFW_MOUSE_BUTTON_RIGHT:

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


}
//----------------------------------------------------------------------------

// It moves the object and check eact time to any collosion
void moveObject() {

    // Update velocity and time
    velocity += g * deltaTime;
    // Update position
    position += velocity * deltaTime;

    if (lightMoving) {
        light_position = point4(position.x, position.y, position.z, 0.0);
        glUniform4fv(glGetUniformLocation(program, "LightPosition"),
        1, light_position);
    }

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


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    // Initialize current window
    GLFWwindow* window = glfwCreateWindow(500, 500, "Homework3", NULL, NULL);
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

    readPPMofBasketball();
    readPPMofWorldMap();
    
    //Initilaze firts object as Sphere
    switchMotion(window);

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