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

GLFWwindow* jeomcInit() {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return;

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

    glewExperimental = GL_TRUE;
    glewInit();
    return window;
}

GLuint createShaders() {

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

    return shader_programme;
}

GLuint* drawTriangle(GLFWwindow* window, float x, float y, float f) {

    float points[] = {
      x, y+f, 0.0f,
      x+f, y-f, 0.0f,
      x-f, y-f, 0.0f,
    };

    float points2[] = {
      -x, -y+f, 0.0f,
      -x+f, -y-f, 0.0f,
      -x-f, -y-f, 0.0f,
    };

    GLuint VBO1, VAO1, EBO;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO);

    GLuint VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(0);

    GLuint* vaos = malloc(2);
    if (!vaos) {
      return NULL;
    }

    vaos[0] = VAO1;
    vaos[1] = VAO2;
    return vaos;
}


void jeomcRun(GLFWwindow* window, GLuint* vaos, GLuint shader) {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);

        glBindVertexArray(*vaos);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(vaos[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    saveImage("img.png", window);
    glfwTerminate();
    return;
}

#ifdef BUILD_TEST
int main(int argc, char *argv[]) {

  GLFWwindow* window = jeomcInit();
  GLuint shader = createShaders();

  GLuint* vaos = drawTriangle(window, 0.5f, 0.5f, 0.25f);
  jeomcRun(window, vaos, shader);

  if (vaos) {
    free(vaos);
  }
}
#endif
