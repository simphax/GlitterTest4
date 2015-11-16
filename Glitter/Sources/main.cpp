// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>


const char *textFileRead( const char *fn, bool fatalError )
{
    /* Note: the `fatalError' thing is a bit of a hack, The proper course of
     * action would be to have people check the return values of textFileRead,
     * but lets avoid cluttering the lab-code even further.
     */
    
    FILE *fp;
    char *content = NULL;
    int count = 0;
    
    if( fn != NULL )
    {
        fp = fopen( fn, "rt" );
        if( fp != NULL )
        {
            fseek( fp, 0, SEEK_END );
            count = ftell( fp );
            fseek( fp, 0, SEEK_SET );
            
            if( count > 0 )
            {
                content = new char[count+1];
                count = fread( content, sizeof(char), count, fp );
                content[count] = '\0';
            }
            else
            {
                if( fatalError )
                {
                    char buffer[256];
                    sprintf( buffer, "File '%s' is empty\n", fn );
                }
            }
            
            fclose( fp );
        }
        else
        {
            if( fatalError )
            {
                char buffer[256];
                sprintf( buffer, "Unable to read file '%s'\n", fn );
            }
        }
    }
    else
    {
    }
    
    return content;
}

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    float vertices[] = {
        0.0f,  0.5f, // Vertex 1 (X, Y)
        0.5f, -0.5f, // Vertex 2 (X, Y)
        -0.5f, -0.5f  // Vertex 3 (X, Y)
    };
    
    GLuint vbo;
    glGenBuffers(1, &vbo); // Generate 1 buffer
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    const char *vs = textFileRead("../Shaders/simple.vert",false);
    const char *fs = textFileRead("../Shaders/simple.frag",false);
    
    glShaderSource(vertexShader, 1, &vs, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fs, NULL);
    glCompileShader(fragmentShader);
    
    delete [] vs;
    delete [] fs;
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);
        glUseProgram(shaderProgram);
        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
