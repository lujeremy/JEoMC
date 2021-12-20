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
int vaoIndex = 0;
GLuint vao_arr[50] = {0};

/* General shape struct to track rgba, Triangle/Rectangle's vao, and Circle's x,y,radius */
struct Shape {
  GLuint vao;
  int indices;
  double x;
  double y;
  double radius;
  double r;
  double g;
  double b;
  double a;
};

/* Keep window, shape, and an index as global variables
   Shape array will be used as a queue to add shapes to be drawn.
   When user runs jeomcRunAndSave, all queued shapes in array will be rendered accordingly */
GLFWwindow* window;
int shapeIndex = 0;
struct Shape shape_arr[50] = {0};

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

/* Use x, y, and f (offset) coordinates to create a Triangle's VAO
   Insert vao into global array and increment index */
void drawTriangleOffset(double x, double y, double f) {

    double points[] = {
      x, y+f,
      x+f, y-f,
      x-f, y-f
    };

    GLuint VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_DOUBLE,GL_FALSE,0,NULL);
    

    struct Shape s = {VAO1, 3, 0, 0, 0, active_r, active_g, active_b, active_a};

    shape_arr[shapeIndex] = s;
    shapeIndex++;
    return;
}

void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3,
    double r, double g, double b) {

    double points[] = {
      x1, y1, 0.0,
      x2, y2, 0.0,
      x3, y3, 0.0,
    };

    GLuint VBO1, VAO1, EBO;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_DOUBLE,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(0);

    vao_arr[vaoIndex] = VAO1;
    vaoIndex++;

    const char* vertexSource =
       "#version 330\n"
       "in vec3 vp;"
       "void main() {"
       "    gl_Position = vec4(vp,1.0);"
       "}";

    const char* fragmentSource;
    char buf[256];
    sprintf(buf,"#version 330\n"
                "out vec4 frag_colour;"
                "void main() {"
                "    frag_colour = vec4(%0.1f,%0.1f,%0.1f,%0.1f);"
                "}",r,g,b,1.0);

    fragmentSource = buf;


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
    glfwSwapBuffers(window);

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
    double points[numPoints*3];

    while (theta<360){
        x = (double) rad * cos(theta * M_PI / 180.0);
        y = (double) rad * sin(theta * M_PI / 180.0);

        points[currentSize++] = x + centerx;
        points[currentSize++] = y + centery;
        points[currentSize++] = z;

        theta+=(360/numPoints);

    }

    GLuint VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_DOUBLE,GL_FALSE,0,NULL);

    struct Shape s = {VAO1, numPoints, 0, 0, 0, active_r, active_g, active_b, active_a};
    shape_arr[shapeIndex] = s;
    shapeIndex++;

    const char* vertexSource =
       "#version 330\n"
       "in vec3 vp;"
       "void main() {"
       "    gl_Position = vec4(vp,1.0);"
       "}";

    const char* fragmentSource;
    char buf[256];
    sprintf(buf,"#version 330\n"
                "out vec4 frag_colour;"
                "void main() {"
                "    frag_colour = vec4(%0.1f,%0.1f,%0.1f,%0.1f);"
                "}",r,g,b,1.0);

    fragmentSource = buf;


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

    
    glfwSwapBuffers(window);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numPoints);
    glfwSwapBuffers(window);
    return;
    
}

/* Queues a circle to be drawn with x and y center point and radius*/
void drawCircleLinux(double x, double y, double radius){
    struct Shape s = {0, 0, x, y, radius, active_r, active_g, active_b, active_a};
    shape_arr[shapeIndex] = s;
    shapeIndex++;
    return;
}

/* Actually draws a queued circle using many triangles */
void drawQueuedCircle(double x, double y, double radius) {
    int i;
    int triangleAmount = 30; //# of triangles used to draw circle

    GLfloat twicePi = 2.0f * M_PI;

    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(x, y); // center of circle
      for(i = 0; i <= triangleAmount;i++) {
        glVertex2f(
                  x + (radius * cos(i *  twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
      }
    glEnd();
    return;
}

/* Draws a rectangle with x and y as the bottom left point and h height, w width
   We use element buffers and drawElement instead to simplify the number of points
   needed (otherwise we would need to pass in 6 points for the 2 triangles).
*/
void drawRectangle(double x, double y, double h, double w,
    double r, double g, double b) {

    double points[] = {
      x, y+h,
      x+w, y+h,
      x+w, y,
      x, y
    };

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint VBO1, VAO1, ebo;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glGenBuffers(1, &ebo);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_DOUBLE,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    struct Shape s = {VAO1, 6, 0, 0, 0, active_r, active_g, active_b, active_a};

    shape_arr[shapeIndex] = s;
    shapeIndex++;

    const char* vertexSource =
       "#version 330\n"
       "in vec3 vp;"
       "void main() {"
       "    gl_Position = vec4(vp,1.0);"
       "}";

    const char* fragmentSource;
    char buf[256];
    sprintf(buf,"#version 330\n"
                "out vec4 frag_colour;"
                "void main() {"
                "    frag_colour = vec4(%0.1f,%0.1f,%0.1f,%0.1f);"
                "}",r,g,b,1.0);

    fragmentSource = buf;


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

    
    glfwSwapBuffers(window);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    return;
}

void drawLine(double startx, double starty, double endx, double endy,
    double r, double g, double b){
    double points[] = {
        startx, starty,
        endx, endy
    };

    GLuint VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_DOUBLE,GL_FALSE,0,NULL);
    glLineWidth(2);

    struct Shape s = {VAO1, 4, 0, 0, 0, active_r, active_g, active_b, active_a};
    shape_arr[shapeIndex] = s;
    shapeIndex++;



    const char* vertexSource =
       "#version 330\n"
       "in vec3 vp;"
       "void main() {"
       "    gl_Position = vec4(vp,1.0);"
       "}";

    const char* fragmentSource;
    char buf[256];
    sprintf(buf,"#version 330\n"
                "out vec4 frag_colour;"
                "void main() {"
                "    frag_colour = vec4(%0.1f,%0.1f,%0.1f,%0.1f);"
                "}",r,g,b,1.0);

    fragmentSource = buf;


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

    
    glfwSwapBuffers(window);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
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

        /* Render here */

        for (int i = 0; i < shapeIndex; i++) {
            struct Shape s = *(shape_arr + i);

            // Draw circles
            if (s.indices > 10) { 
                glBindVertexArray(s.vao);
                glDrawArrays(GL_TRIANGLE_FAN,0,30);
          // Draw Triangles
            } else if (s.indices == 3) {
                glBindVertexArray(s.vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);
          // Draw Rectangles
            } else if (s.indices == 6) {
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

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

  drawTriangleCoords(0.0,0.6,0.15,0.58,0.0,0.56,1.0,0.8,0.0);

  
  drawCircle(0.0, 0.1, 0.03,0.0,0.0,0.0);
  drawCircle(0.0, -0.1, 0.03,0.0,0.0,0.0);
  drawCircle(0.0, -0.3, 0.03,0.0,0.0,0.0);

  drawLine(-0.75,0.55,-0.18,0.3,0.5,0.25,0.0);
  drawLine(0.75,0.55,0.18,0.3,0.5,0.25,0.0);

  jeomcRunAndSave();
}
#endif
