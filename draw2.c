/*melody's draw test*/

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image_write.h"
//#include <stb/stb_image_write.h>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

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

void draw2(int c)
{
    GLFWwindow* window;

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
    window = glfwCreateWindow(640, 480, "GLFW CMake starter", NULL, NULL);
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

    //uncomment below to draw individual points
    /*
    float points[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
    };
    */
    
    //uncomment below to draw lines
    
    float points[] = {
    0.0f,0.125f,0.0f,
    0.0625f,0.0f,0.0f,

    0.0625f,0.0f,0.0f,
    -0.0625f,0.0f,0.0f,

    -0.0625f,0.0f,0.0f,
    0.0f,0.125f,0.0f,

    0.0f,0.25f,0.0f,
    0.125f,0.0f,0.0f,

    0.125f,0.0f,0.0f,
    -0.125f,0.0f,0.0f,

    -0.125f,0.0f,0.0f,
    0.0f,0.25f,0.0f,

	0.0f,0.5f,0.0f,
	0.25f,0.0f,0.0f,

	0.25f,0.0f,0.0f,
	-0.25f,0.0f,0.0f,

	-0.25f,0.0f,0.0f,
	0.0f,0.5f,0.0f,
	
	0.0f,1.0f,0.0f,
	0.5f,0.0f,0.0f,

	0.5f,0.0f,0.0f,
	-0.5f,0.0f,0.0f,

	-0.5f,0.0f,0.0f,
	0.0f,1.0f,0.0f
	
    };
    

    GLuint vbo = 0;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);

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
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shader_programme);
	glBindVertexArray(vao);

	//use to draw a triangle---------------
	//glDrawArrays(GL_TRIANGLES,0,3);
	//-------------------------------------
	
	//use to draw points------------------
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	//glDrawArrays(GL_POINTS,0,3);
	//------------------------------------

	//use to draw lines--------------------
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glDrawArrays(GL_LINES,0,24);
	//-------------------------------------

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    saveImage("img.png",window);

    glfwTerminate();
    return;
}



#ifdef BUILD_TEST
int main(int argc, char *argv[])
{
  draw2(1);
}
#endif
