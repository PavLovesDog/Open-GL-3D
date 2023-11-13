
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
//GLM specific includes for martix stuff
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//header files
#include "construct_mesh.h"

//VERTEX SHADER
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 2) in vec2 aTexCord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"   TexCoord = aTexCord;\n"
"}\0";

//FRAGMEBNT SHADER
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture1, TexCoord);\n"
"}\n\0";

//Function Definitions
unsigned int CompileShaders(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO();
unsigned int createVBO(const float* vertices, size_t size);
unsigned int createEBO(const unsigned int* indices, size_t size);
void setupVertexAttributes();
unsigned int LoadTexture(const char* filename);

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

//Global camera variables
float pitch = 0.0f;
float yaw = -90.0f; // Initialized to -90 because a yaw of 0 results in a direction vector pointing to the right
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

// Mouse callback function that matches GLFW's expected signature
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = 400, lastY = 300; // Assume the window starts at 800x600 resolution, so this is the center
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
 
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OPenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // build and compile our shader program
    // ------------------------------------
    unsigned int shaderProgram = CompileShaders(vertexShaderSource, fragmentShaderSource);
    if (shaderProgram == 0) {
        // Handle the error, perhaps by exiting the application
        return -1;
    }

    //Set up meshes
    // -------------
    Mesh cube_mesh = construct_cube();
    Mesh diamond_mesh = construct_diamond();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // Create VAO, VBO, and EBO's
    //CUBE
    unsigned int cubeVAO = createVAO();
    unsigned int cubeVBO = createVBO(cube_mesh.vertices.data(), cube_mesh.vertices.size() * sizeof(float));
    unsigned int cubeEBO = createEBO(cube_mesh.indices.data(), cube_mesh.indices.size() * sizeof(unsigned int));
    setupVertexAttributes();
    glBindVertexArray(0); // Unbind the VAO to prevent accidental changes to it.

    //PYRAMID
    unsigned int diamondVAO = createVAO();
    unsigned int diamondVBO = createVBO(diamond_mesh.vertices.data(), diamond_mesh.vertices.size() * sizeof(float));
    unsigned int diamondEBO = createEBO(diamond_mesh.indices.data(), diamond_mesh.indices.size() * sizeof(unsigned int));
    setupVertexAttributes();
    glBindVertexArray(0);

    //SPHERE

    //etc...
    
    // load and create a texture 
    // -------------------------
    unsigned int cubeTexture = LoadTexture("texture.crate.jpg");
    unsigned int diamondTexture = LoadTexture("texture.wall.jpg");

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    {
        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        //glBindVertexArray(0);

        // uncomment this call to draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    //Enable depth test for 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Matrix Transformstion Variables
    //camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float cameraSpeed = 0.05f; // Adjust as needed
    //cube
    float cubeRotationX = 0.5f;
    float cubeRotationY = 0.5f;
    glm::vec3 cubeScale = glm::vec3(0.5f,0.5f, 0.5f);
    //pyramid
    float diamondRotationX = 0.0f;
    float diamondRotationY = 0.0f;
    glm::vec3 diamondScale = glm::vec3(0.5f, 0.5f, 0.5f);

    // get model location
    glUseProgram(shaderProgram); // Use the shader program
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model"); // get model location from vert shader
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    //Mouse specifics for camera
    glfwSetCursorPosCallback(window, mouse_callback); // listen for mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // disable mouse on screen

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        // CAMERA
        {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                cameraPos += cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                cameraPos -= cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        // CUBE
        {
            // --- Rotations
            if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
                cubeRotationX += 1.0f; // Increase rotation angle around the X-axis
            if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
                cubeRotationX -= 1.0f; // Decrease rotation angle around the X-axis
            if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
                cubeRotationY += 1.0f; // Increase rotation angle around the Y-axis
            if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
                cubeRotationY -= 1.0f; // Decrease rotation angle around the Y-axis
            // --- Scale
            if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
                cubeScale += glm::vec3(0.01f); // Use smaller increments if 0.1 is too large
            if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
                cubeScale -= glm::vec3(0.01f); // Don't allow the scale to go negative!
        }
        //DIAMOND
        {
            // --- Rotations
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                diamondRotationX += 1.0f; // Increase rotation angle around the X-axis
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                diamondRotationX -= 1.0f; // Decrease rotation angle around the X-axis
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                diamondRotationY += 1.0f; // Increase rotation angle around the Y-axis
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                diamondRotationY -= 1.0f; // Decrease rotation angle around the Y-axis
        }

        //clear buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

        // draw our first triangle
        glUseProgram(shaderProgram);
        
        // Activate the texture unit and bind the texture
        glActiveTexture(GL_TEXTURE0); // Activate the first texture unit
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

        // Camera Matrix setup
        // -------------------
        // Create view matrix
        //glm::mat4 view = glm::mat4(1.0f);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // Move the camera "backwards" from the origin
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Create projection matrix
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Pass the view and projection matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // render
        // ------
        // Cube
        glBindVertexArray(cubeVAO); // Bind the cube's VAO
        glBindTexture(GL_TEXTURE_2D, cubeTexture); // Bind the cube's texture
        // Set the cube's transformations
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(0.0f,0.0f,0.0f)); // Position the cube (static for now)
        cubeModel = glm::rotate(cubeModel, glm::radians(cubeRotationX), glm::vec3(1.0f, 0.0f, 0.0f)); // set rotation
        cubeModel = glm::rotate(cubeModel, glm::radians(cubeRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        cubeModel = glm::scale(cubeModel, cubeScale); // Apply scaling
        // Set the uniform for the cube model matrix
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeModel)); // Set the cube model matrix uniform
        glDrawElements(GL_TRIANGLES, cube_mesh.num_of_indices, GL_UNSIGNED_INT, 0); // Draw the cube

        // Diamond
        glBindVertexArray(diamondVAO); // Bind the diamond's VAO
        glBindTexture(GL_TEXTURE_2D, diamondTexture); // Bind the diamond's texture
        // Set the diamond's transformations
        glm::mat4 pyramidModel = glm::mat4(1.0f); // model matrix
        pyramidModel = glm::translate(pyramidModel, glm::vec3(-1.0f, 0.0f, 0.5f)); // Position the diamond
        pyramidModel = glm::rotate(pyramidModel, glm::radians(diamondRotationX), glm::vec3(1.0f, 0.0f, 0.0f));// rotation
        pyramidModel = glm::rotate(pyramidModel, glm::radians(diamondRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        pyramidModel = glm::scale(pyramidModel, diamondScale); // Scale
        //Set the model matrix for each object right before you draw it.
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pyramidModel));
        glDrawElements(GL_TRIANGLES, diamond_mesh.num_of_indices, GL_UNSIGNED_INT, 0); // Draw the diamond
        
        //// Sphere
        //glBindTexture(GL_TEXTURE_2D, sphereTexture); // Bind the sphere's texture
        //glBindVertexArray(sphereVAO); // Bind the sphere's VAO
        //// Set the sphere's transformations, similar to cube
        //// ...
        //glDrawElements(GL_TRIANGLES, sphereIndicesCount, GL_UNSIGNED_INT, 0); // Draw the sphere

        // Unbind the VAO to prevent accidental changes to it
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO); // ---- Cube
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
    glDeleteVertexArrays(1, &diamondVAO); // ---- Diamond
    glDeleteBuffers(1, &diamondVBO);
    glDeleteBuffers(1, &diamondEBO);
    glDeleteProgram(shaderProgram); // ---- Shader Program

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

unsigned int CompileShaders(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0; // return 0 to indicate failure
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0; // return 0 to indicate failure
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return 0; // return 0 to indicate failure
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram; // return the shader program ID
}

unsigned int createVAO() {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    return VAO;
}

unsigned int createVBO(const float* vertices, size_t size) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return VBO;
}

unsigned int createEBO(const unsigned int* indices, size_t size) {
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    return EBO;
}

// this is set up specifically for the mesh structure of 3 floats for position and 2 floats for texture coordinates
void setupVertexAttributes() {
    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

//void setupVertexAttributes( int vertexStride, const void* positionOffset, const void* textureOffset) {
//    //vertexStride: This is the byte offset between consecutive vertex attributes.
//    // For example, if you have 3 floats for position and 2 floats for texture coordinates, and they are tightly packed, the stride would be (3 + 2) * sizeof(float).
//    //positionOffset: This is the offset of the position data from the start of the vertex data.
//    // It is provided as a void pointer.If the position data comes first in your vertex array, this would be(void*)0.
//    //textureOffset : This is the offset of the texture coordinate data from the start of the vertex data. It's also provided as a void pointer. 
//    //For example, if you have 3 floats for position data and then 2 floats for texture data, the texture offset would be 3 * sizeof(float) to skip the position data and reach the start of the texture data.
//    
//    // Position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, positionOffset);
//    glEnableVertexAttribArray(0);
//
//    // Texture coordinate attribute
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexStride, textureOffset);
//    glEnableVertexAttribArray(2);
//}

unsigned int LoadTexture(const char* filename) {
    // Generate a texture ID and load texture data
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        // If the loaded image has alpha channel, use GL_RGBA
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture: " << filename << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}



