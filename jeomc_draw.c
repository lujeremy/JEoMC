#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image_write.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

/* MacOS imports */
#if __has_include(<OpenGL/gl.h>)
#include <OpenGL/gl.h>
#endif
#if __has_include(<GLUT/glut.h>)
#include <GLUT/glut.h>
#endif

/* Linux imports */
#if __has_include(<GL/gl.h>)
#include <GL/gl.h>
#endif
#if __has_include(<GL/glut.h>)
#include <GL/glut.h>
#endif

/* Keep window, vao array, and an index as global variables */
GLFWwindow* window;

/* Active rgba values are kept and saved into each shape when they are queued
   This allows for each shape to have a separate rbga value */
double active_r = 1.0;
double active_g = 0.894;
double active_b = 0.882;
double active_a = 1.0;

/* Change active rgba values to new ones */
void setActiveColor(double r, double g, double b, double a) {
  active_r = r;
  active_g = g;
  active_b = b;
  active_a = a;
}

/* Initialize glfw, creating the window to the global var */
void jeomcInit() {

    /* Initialize the library */
    if (!glfwInit())
        return;
    
#ifdef __APPLE__
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR,2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "JEoMC", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glClearColor( 0.1f, 0.1f, 0.1f, 0.0f );

    /* Enable transparency */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_COLOR_MATERIAL);

    glewExperimental = GL_TRUE;
    glewInit();
    return;
}

void compileShaders() {
    const char* vertexSource =
      "#version 330\n"
      "layout (location = 0) in vec3 vp;    // Position in attribute location 0\n"
      "layout (location = 1) in vec3 vc;  // Color in attribute location 1\n"
      "out vec3 theColor;                 // output a color to the fragment shader\n"
      "void main() {"
      "    gl_Position = vec4(vp,1.0);"
      "    theColor = vc;\n"
      "}";

    const char* fragmentSource =
      "#version 330\n"
      "in vec3 theColor;      // Color value came from the vertex shader (smoothed) \n"
      "out vec4 FragColor;    // Color that will be used for the fragment\n"
      "void main() {"
      "    FragColor = vec4(theColor, 1.0f);"
      "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vertexSource,NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fragmentSource,NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme,fs);
    glAttachShader(shader_programme,vs);
    glLinkProgram(shader_programme);
    glUseProgram(shader_programme);

    return;
}

void bindVertexBufferAndArray() {
    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6*sizeof(double), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6*sizeof(double), (void*)(3*sizeof(double)));
    glEnableVertexAttribArray(1);
    return;
}

void bindElementBuffer() {
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3,
    double r, double g, double b) {

    double points[] = {
      x1, y1, 0.0, 0.583,  0.771,  0.014,
      x2, y2, 0.0, 0.609,  0.115,  0.436,
      x3, y3, 0.0, 0.327,  0.483,  0.844
    };

    // bind vao and vbo, buffer points
    bindVertexBufferAndArray();
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // compile shaders and draw
    compileShaders();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    return;
}

void drawCircle(double centerx, double centery,double rad,
    double r, double g, double b){
    int theta = 0;
    double x;
    double y;
    double z = 0.0;
    int numPoints = 30;
    int currentSize = 0;
    double points[numPoints*6];

    while (theta<360){
        x = (double) rad * cos(theta * M_PI / 180.0);
        y = (double) rad * sin(theta * M_PI / 180.0);

        points[currentSize++] = x + centerx;
        points[currentSize++] = y + centery;
        points[currentSize++] = z;
        points[currentSize++] = r;
        points[currentSize++] = g;
        points[currentSize++] = b;

        theta+=(360/numPoints);

    }

    // bind vao and vbo, buffer points
    bindVertexBufferAndArray();
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // compile shaders and draw
    compileShaders();
    glDrawArrays(GL_TRIANGLE_FAN, 0, numPoints);

    glfwSwapBuffers(window);
    return;
    
}

/* Draws a rectangle with x and y as the bottom left point and h height, w width
   We use element buffers and drawElement instead to simplify the number of points
   needed (otherwise we would need to pass in 6 points for the 2 triangles).
*/
void drawRectangle(double x, double y, double h, double w,
    double r, double g, double b) {

    double points[] = {
      x, y+h, 0.0, r, g, b,
      x+w, y+h, 0.0, r, g, b,
      x+w, y, 0.0, r, g, b,
      x, y, 0.0, r, g, b
    };

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    // bind vao and vbo, buffer points
    bindVertexBufferAndArray();
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // bind ebo and buffer points
    bindElementBuffer();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    // compile shaders and draw
    compileShaders();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    return;
}

void drawLine(double startx, double starty, double endx, double endy,
    double r, double g, double b){
    double points[] = {
        startx, starty, 0, r, g, b,
        endx, endy, 0, r, g, b
    };

    // bind vao and vbo, buffer points
    bindVertexBufferAndArray();
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // set line width
    glLineWidth(10);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // compile shaders and draw
    compileShaders();
    glDrawArrays(GL_LINES, 0, 2);

    glfwSwapBuffers(window);
}

/* Saves the current GLFW window to a given filepath */
void saveImage(char *filepath, GLFWwindow *w){
    int width, height;
    glfwGetFramebufferSize(w,&width,&height);
    GLsizei nrChannels = 3;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4-stride % 4) : 0;
    GLsizei bufferSize = stride * height;
    char buffer[bufferSize];
    glPixelStorei(GL_PACK_ALIGNMENT,4);
    glReadBuffer(GL_BACK);
    glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,buffer);
    stbi_flip_vertically_on_write(1);
    stbi_write_png(filepath,width,height,nrChannels,buffer,stride);

    return;
}



/* Sets up the opening of a window:
    1. Creates the needed shaders
    2. Iterates through global vao array using index to bind and render all the vertices
    3. Saves window as an image to img.png
 */
void jeomcRunAndSave() {

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();
    }

    saveImage("img.png", window);
    glfwTerminate();
    return;
}

#ifdef BUILD_TEST
int main(int argc, char *argv[]) {

  jeomcInit();

  drawCircle(0.0, -0.5, 0.4,1.0,1.0,1.0);
  drawCircle(0.0, 0.1, 0.30,1.0,1.0,1.0);
  drawCircle(0.0, 0.6, 0.20,1.0,1.0,1.0);

  drawRectangle(-0.25,0.32,0.15,0.5,1.0,0.0,0.0);

  drawCircle(-0.08,0.65,0.03,0.0,0.0,0.0);
  drawCircle(0.08,0.65,0.03,0.0,0.0,0.0);

  drawTriangle(0.0,0.6,0.15,0.58,0.0,0.56,1.0,0.8,0.0);

  
  drawCircle(0.0, 0.1, 0.03,0.0,0.0,0.0);
  drawCircle(0.0, -0.1, 0.03,0.0,0.0,0.0);
  drawCircle(0.0, -0.3, 0.03,0.0,0.0,0.0);

  drawLine(-0.75,0.55,-0.18,0.3,0.5,0.25,0.0);
  drawLine(0.75,0.55,0.18,0.3,0.5,0.25,0.0);

  jeomcRunAndSave();
}
#endif
