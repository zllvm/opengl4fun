#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define PI 3.14159265
#define MAX_BUFF_SIZE 1024*256
#define NUMBER_OF_POINTS 4 // number of star verteces

// Functions declarations
char* readFile(const char* name, char* buffer);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window);
unsigned int addShader(const char* fileName, unsigned int shaderType); 
unsigned int addShaderProgram(unsigned int vertexShader, 
                              unsigned int fragmentShader); 

// Globals
const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;

    // Initialize GLFW
    if(!glfwInit()) {
        fprintf(stderr, "ERROR::Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    window = glfwCreateWindow(WIDTH, HEIGHT, "Shaders", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "ERROR::Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Print OpenGL information
    renderer = glGetString(GL_RENDERER);
    version = glGetString(GL_VERSION);
    printf("Renderer: %s\n",renderer);
    printf("OpenGL version supported: %s\n",version);

    // Define viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0,0,width,height);
    
    // Shaders
    unsigned int vertexShader = addShader("shader.vert", GL_VERTEX_SHADER);
    unsigned int fragmentShader = addShader("shader.frag", GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = addShaderProgram(vertexShader, fragmentShader); 

    // Window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    // Vertex data
    int points = 4;
    int total = points*2+2;
    float q = 360.0f/points;
    float r = 0.5f;
    float r2 = 0.15f;
    double angle1, angle2;
    int length = total*3+3;
    float *vertices = new float[length] {0.0f};

    for(int i=1,n=0;n<=points;i+=2,n++){
        angle1 = (90.0-q*(n))*PI/180.0;
        angle2 = (90.0-q*(n+1)+q/2)*PI/180.0f;
        vertices[3*i] = r*cos(angle1); 
        vertices[3*i+1] = r*sin(angle1); 
        vertices[3*(i+1)] = r2*cos(angle2); 
        vertices[3*(i+1)+1] = r2*sin(angle2); 
    }

    // Copy data to gpu memory 
    unsigned int VAO;
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 1. Bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. Copy vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, length*sizeof(float), vertices, GL_STATIC_DRAW);
    // 3. Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 4. Vertices no longer needed
    delete[] vertices;

    // Display settings
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        // Clear buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, total);

        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

// Functions implementations
// Read a shader from a file and creates its object
unsigned int addShader(const char* fileName,
                       unsigned int shaderType) {
    int success;
    char infoLog[512];
    char buffer[MAX_BUFF_SIZE];
    char* typeName = (char*)(shaderType == GL_VERTEX_SHADER 
                             ? "VERTEX" : "FRAGMENT");

    // read shader from file
    const GLchar* shaderSource = readFile(fileName, buffer);

    // create shader object
    int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::%s::COMPILATION_FAILED\n%s", typeName, infoLog);
    }

    return shader;
}

// Creates a shader program based on vertex and fragment shaders
unsigned int addShaderProgram(unsigned int vertexShader, 
                              unsigned int fragmentShader) 
{
    int success;
    char infoLog[512];
    unsigned int shaderProgram;

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n%s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram; 
}

// Updates viewport when window changes its size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    printf("width: %d heigth: %d\n", width, height);
    glViewport(0, 0, width, height);
}

// Captures ESC key and indicates that the main loop should be stopped
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Read a file from a filesystem
char* readFile(const char* name, char* buffer) {
    int fd, pos = 0;

    if((fd = open(name,O_RDONLY)) < 0) {
        fprintf(stderr, "Error in opening file %s\n", name);
    }

    while((pos = read(fd,buffer,MAX_BUFF_SIZE)) > 0) {
        buffer[pos] = '\0';
    }
    
    close(fd);

    int n = strlen(buffer);
    char* result = (char*) malloc(n+1);
    memcpy(result,buffer,n);

    return result; 
}
