#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image_write.h"
#include <stdio.h>

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

struct Shape {
  GLuint vao;
  int indices;
  double r;
  double g;
  double b;
  double a;
};

/* Keep window, vao array, and an index as global variables */
GLFWwindow* window;
int shapeIndex = 0;
struct Shape shape_arr[50] = {0};

double active_r = 1.0;
double active_g = 0.894;
double active_b = 0.882;
double active_a = 1.0;

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
    glfwWindowHint (GLFW_OPEN_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "JEoMC", NULL, NULL);
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

    glewExperimental = GL_TRUE;
    glewInit();
    return;
}

/* Use x, y, and f (offset) coordinates to create a Triangle's VAO
   Insert vao into global array and increment index */
void drawTriangle(double x, double y, double f) {

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
    glVertexAttribPointer(0,2,GL_DOUBLE,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(0);

    struct Shape s = {VAO1, 3, active_r, active_g, active_b, active_a};

    shape_arr[shapeIndex] = s;
    shapeIndex++;
    return;
}

void drawRectangle(double x, double y, double h, double w) {

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

    struct Shape s = {VAO1, 6, active_r, active_g, active_b, active_a};

    shape_arr[shapeIndex] = s;
    shapeIndex++;
    return;
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
}

/* Sets up the opening of a window:
    1. Creates the needed shaders
    2. Iterates through global vao array using index to bind and render all the vertices
    3. Saves window as an image to img.png
 */
void jeomcRunAndSave() {
    const char *vertex_shader =
      "#version 330\n"
      "in vec3 vp;"
      "void main() {"
      "    gl_Position = vec4(vp,1.0);"
      "    gl_PointSize = 10.0;"
      "}";

    const char *fragment_shader =
      "#version 330\n"
      "out vec4 frag_colour;"
      "void main() {"
      "    frag_colour = vec4(1.0,0.0,0.0,0.0);"
      "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vertex_shader,NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fragment_shader,NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme,fs);
    glAttachShader(shader_programme,vs);
    glLinkProgram(shader_programme);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_programme);

        for (int i = 0; i < shapeIndex; i++) {
          struct Shape s = *(shape_arr + i);
          glBindVertexArray(s.vao);
          glColor4f( s.r, s.g, s.b, s.a );

          if (s.indices == 3) {
            glDrawArrays(GL_TRIANGLES, 0, 3);
          } else if (s.indices == 6) {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
          }
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    saveImage("img.png", window);
    glfwTerminate();
    return;
}

#ifdef BUILD_TEST
int main(int argc, char *argv[]) {

  jeomcInit();

  // default
  drawTriangle(0.5, 0.5, 0.25);

  // blue
  setActiveColor(0.686, 0.933, 0.933, 1.0);
  drawRectangle(-0.5, -0.5, 0.25, 0.25);

  // violet
  setActiveColor(0.867, 0.627, 0.867, 1.0);
  drawTriangle(-0.5, 0.5, 0.5);

  jeomcRunAndSave();
}
#endif
