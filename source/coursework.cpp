#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>
#include <common/light.hpp>


// prototype
void keyboardInput(GLFWwindow* window);
void mouseInput(GLFWwindow* window);

// Create camera object
Camera camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -2.0f));


// Frame timer
float previousTime = 0.0f;   
float deltaTime = 0.0f; 
float sensitivity = 5.0f;

// the zone
float borderRadius = 60; // will try to use a very basic circular border constraint
// rather than adding collisions to every rock on the border :P

glm::vec3 currentSunPosition = glm::vec3(0.0f, 1.0f, -10.0f);


// Object struct
struct Object
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(45.0f, 25.0f, 1.0f);

	unsigned int textureID;

    float angle = 0.0f;

	glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, 2.0f);
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
    float ka = 0.5f;
	float kd = 0.7f;
    std::string name;
};


struct Sun
{
    glm::vec3 position;
    glm::vec3 colour;
    
    float constant;
    float linear;
    float quadratic;
    int type;
};



int main( void )
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Computer Graphics Coursework", NULL, NULL);
    
    if( window == NULL ){
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================
    
    // capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // mouse inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // depth test
    glEnable(GL_DEPTH_TEST);
    // back face culling
    glEnable(GL_CULL_FACE);






    // verticies for mountain objects
    const float vertices[] = {
        // front
        -1.0f, -1.0f,  1.0f,   
         1.0f, -1.0f,  1.0f,    
         1.0f,  1.0f,  1.0f,    
        -1.0f, -1.0f,  1.0f,    
         1.0f,  1.0f,  1.0f,  
        -1.0f,  1.0f,  1.0f,    
        // right               
         1.0f, -1.0f,  1.0f,   
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         // back
          1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
          // left
          -1.0f, -1.0f, -1.0f,
          -1.0f, -1.0f,  1.0f,
          -1.0f,  1.0f,  1.0f,
          -1.0f, -1.0f, -1.0f,
          -1.0f,  1.0f,  1.0f,
          -1.0f,  1.0f, -1.0f,
          // bottom
          -1.0f, -1.0f, -1.0f,
           1.0f, -1.0f, -1.0f,
           1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f, -1.0f,
           1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f,  1.0f,
          // top
          -1.0f,  1.0f,  1.0f,
           1.0f,  1.0f,  1.0f,
           1.0f,  1.0f, -1.0f,
          -1.0f,  1.0f,  1.0f,
           1.0f,  1.0f, -1.0f,
          -1.0f,  1.0f, -1.0f,
    };

    // texture coords for mountains
    const float uv[] = {
        // front
        0.0f, 0.0f,     
        1.0f, 0.0f,     
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // right
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // back
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // left

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // bottom
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // top

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    // ebo indices
    unsigned int indices[] = {
        0, 1,  2,  3,  4,  5,     // front
        6, 7,  8,  9, 10, 11,     // right
        12, 13, 14, 15, 16, 17,     // back
        18, 19, 20, 21, 22, 23,     // left
        24, 25, 26, 27, 28, 29,     // bottom
        30, 31, 32, 33, 34, 35      // top
    };



    // for the sun diamond
    const float sunVertices[] = {
        // top point
         0.0f,  1.0f,  0.0f, // 0

         -1.0f,  0.0f,  1.0f, // 1
          1.0f,  0.0f,  1.0f, // 2
          1.0f,  0.0f, -1.0f,  // 3
         -1.0f,  0.0f, -1.0f,  // 4

         // long bottom
          0.0f, -2.0f,  0.0f    // 5
    };


    // sun diamond uv
    const float sunUv[] = {
        //top point
        0.5f, 1.0f,    // 0

        0.0f, 0.5f, // 1
        1.0f, 0.5f,// 2
        1.0f, 0.0f,  // 3
        0.0f, 0.0f,   // 4

        // Bottom apex
        0.5f, -0.5f    // 5
    };

    const unsigned int sunIndices[] = {
        0, 1, 2, // front
        0, 2, 3, // right
        0, 3, 4, // ack
        0, 4, 1, //left
        // bottom
        5, 2, 1,// front
        5, 3, 2,// right
        5, 4, 3, // back
        5, 1, 4  // left
    };


    //MOUNTAINS (first object type)
    // 
    // vao (mountains)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vbo 
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // uv
    unsigned int uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

    // ebo
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);



    // for the pyramid..

    unsigned int pyramidVAO, pyramidVBO, pyramidUVBO, pyramidEBO;
    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &pyramidVBO);
    glGenBuffers(1, &pyramidUVBO);
    glGenBuffers(1, &pyramidEBO);

    glBindVertexArray(pyramidVAO);
    //vertices
    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sunVertices), sunVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uv
    glBindBuffer(GL_ARRAY_BUFFER, pyramidUVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sunUv), sunUv, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sunIndices), sunIndices, GL_STATIC_DRAW);

    glBindVertexArray(0);




    
    // compile shaders

    unsigned int shaderID;
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

    // load texture
    unsigned int texture;
    texture = loadTexture("../assets/boxBorder.png");

    // Send the texture uniforms to the fragment shader
    glUseProgram(shaderID);
    unsigned int textureID;
    textureID = glGetUniformLocation(shaderID, "texture");
    glUniform1i(textureID, 0);



    ///






    // rock positions
    glm::vec3 positions[] = {
        glm::vec3(0.0f,  -3.0f,  0.0f),
        glm::vec3(2.0f,  -3.0f, -10.0f),
        glm::vec3(-3.0f, -3.0f, -3.0f),
        glm::vec3(-4.0f, -3.0f, -8.0f),
        glm::vec3(2.0f,  -3.0f, -6.0f),
        glm::vec3(-4.0f, -3.0f, -8.0f),
        glm::vec3(0.0f, -3.0f, -5.0f),
        glm::vec3(4.0f,  -3.0f, -4.0f),
        glm::vec3(2.0f,  -3.0f, -2.0f),
        glm::vec3(-1.0f,  -3.0f, -2.0f)
    };

    // Add cubes to objects vector
    std::vector<Object> objects;
    Object object;
    object.name = "rock";

    int numRocks = 32;
	float angleStep = 2.0f * glm::pi<float>() / float(numRocks);

    for (unsigned int i = 0; i < numRocks; i++)
    {
		float angle = i * angleStep;
		float x = borderRadius * cos(angle);
		float z = borderRadius * sin(angle);
        

        //object.position = positions[i];
		object.position = glm::vec3(x, -3.0f, z);

        glm::vec3 toCenter = glm::normalize(-object.position);
		object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        //object.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
        //object.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        //object.angle = Maths::radians(20.0f * i);

        object.angle = atan2(toCenter.z, toCenter.x);
        object.scale = glm::vec3(10.0f, 25.0f + rand() % 2, 10.0f);

		object.textureID = loadTexture("../assets/boxBorder.png");
        objects.push_back(object);
    }


    // The ground
    glm::vec3 floorPos(0.0f, -3.0f, 0.0f);
    glm::vec3 floorScale(250.0f, 0.1f, 250.0f);
    Object obj;
	obj.name = "floor";
	obj.position = floorPos;
	obj.scale = floorScale;
	obj.textureID = loadTexture("../assets/grassLayer.png");
    objects.push_back(obj);





    //--
    // 
    // 
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        
        // Get inputs
        keyboardInput(window);
        mouseInput(window);

        // Update timer
        float time = glfwGetTime();
        deltaTime = time - previousTime;
        previousTime = time;

        
        // Clear the window
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //--


		

        // !!! Model, View Projcetion and MVP matrices:
        // 
        // 
        // Calculate the model matrix
        float angle = Maths::radians(glfwGetTime() * 360.0f / 3.0f);
        glm::mat4 translate = Maths::translate(glm::vec3(0.0f, 0.0f, -2.0f));
        glm::mat4 scale = Maths::scale(glm::vec3(0.5f, 0.5f, 0.5f));
        glm::mat4 rotate = Maths::rotate(angle, glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 model = translate * rotate * scale;
        //glm::mat4 translate = Maths::translate(objects[i].position);
        //glm::mat4 rotate = Maths::rotate(objects[i].angle, objects[i].rotation); // rotates around Y
        //glm::mat4 scale = Maths::scale(objects[i].scale);
        //glm::mat4 model = translate * rotate * scale;



        // Calculate camera vectors
        camera.calculateCameraVectors();

        // Calculate the view matrix
        glm::mat4 view = glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f),  // eye
            glm::vec3(0.0f, 0.0f, -2.0f), // target
            glm::vec3(0.0f, 1.0f, 0.0f)); // worldUp

        //Calculate view and projection matrices
        camera.target = objects[0].position;
        camera.calculateMatrices();


        // Set sun as a bright point light
        glm::vec3 sunPos(0.0f, 5.0f, -10.0f); // or wherever your sun is
        glm::vec3 sunColor(2.0f, 2.0f, 1.6f); // bright yellowish

        glm::vec3 viewSpaceSunPos = glm::vec3(camera.view * glm::vec4(sunPos, 1.0f));
        glUniform3fv(glGetUniformLocation(shaderID, "lightPosition"), 1, &viewSpaceSunPos[0]);
        glUniform3fv(glGetUniformLocation(shaderID, "lightColour"), 1, &sunColor[0]);


        glBindVertexArray(VAO);

        // !! g
        //
        // Loop through cubes and draw each one
        for (int i = 0; i < static_cast<unsigned int>(objects.size()); i++)
        {
            // Calculate the model matrix
            glm::mat4 translate = Maths::translate(objects[i].position);
            glm::mat4 scale = Maths::scale(objects[i].scale);
            glm::mat4 rotate = Maths::rotate(objects[i].angle, objects[i].rotation);
            glm::mat4 model = translate * rotate * scale;

            // Calculate the MVP matrix
            glm::mat4 MVP = camera.projection * camera.view * model;

            // Send MVP matrix to the vertex shader
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

            //lighting
            glUniform1f(glGetUniformLocation(shaderID, "ka"), objects[i].ka);

            //mvp
            glm::mat4 MV = camera.view * model;
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MV"), 1, GL_FALSE, &MV[0][0]);
            glUniform1f(glGetUniformLocation(shaderID, "kd"), objects[i].kd);
            glUniform3fv(glGetUniformLocation(shaderID, "lightColour"), 1, &objects[i].lightColour[0]);
            glm::vec3 viewSpaceLightPosition = glm::vec3(camera.view * glm::vec4(objects[i].lightPosition, 1.0f));
            glUniform3fv(glGetUniformLocation(shaderID, "lightPosition"), 1, &viewSpaceLightPosition[0]);
            // Draw the triangles

			glBindTexture(GL_TEXTURE_2D, objects[i].textureID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }


        // FOR THE SUN
		//glm::mat4 sunModel = glm::mat4(1.0f);
		//model = glm::translate(sunModel, glm::vec3(0.0f, 8.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shaderID, "sunModel"), 1, GL_FALSE, &sunModel[0][0]);

  //      glDisableVertexAttribArray(0);
  //      glDisableVertexAttribArray(1);

        // Set up the model matrix for the pyramid
        glm::mat4 pyramidTranslate = Maths::translate(currentSunPosition); // Place it in front of the camera
        glm::mat4 pyramidScale = Maths::scale(glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 pyramidModel = pyramidTranslate * pyramidScale;
        glm::mat4 pyramidMVP = camera.projection * camera.view * pyramidModel;

        // Send MVP to shader
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &pyramidMVP[0][0]);

        // Optionally set lighting/material uniforms for the pyramid
        glUniform1f(glGetUniformLocation(shaderID, "ka"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderID, "kd"), 1.0f);
        glm::vec3 pyramidLightColour(1.0f, 1.0f, 0.8f);
        glUniform3fv(glGetUniformLocation(shaderID, "lightColour"), 1, &pyramidLightColour[0]);
        glm::vec3 pyramidLightPosition(0.0f, 10.0f, 0.0f);
        glm::vec3 viewSpaceLightPosition = glm::vec3(camera.view * glm::vec4(pyramidLightPosition, 1.0f));
        glUniform3fv(glGetUniformLocation(shaderID, "lightPosition"), 1, &viewSpaceLightPosition[0]);

        // Bind a texture (reuse one or load a new one)
        glBindTexture(GL_TEXTURE_2D, loadTexture("../assets/sun.png"));

        // Draw the pyramid
        glBindVertexArray(pyramidVAO);
        glDrawElements(GL_TRIANGLES, sizeof(sunIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);



        //--

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteProgram(shaderID);
    //sun.deleteBuffers();

    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // only moving on the x and z axis
    glm::vec3 flatFront = glm::normalize(glm::vec3(camera.front.x, 0.0f, camera.front.z));
    glm::vec3 flatRight = glm::normalize(glm::vec3(camera.right.x, 0.0f, camera.right.z));
    
    // if the new eye is outta range of the radius, will not update
	glm::vec3 newEye = camera.eye;

    // Move the camera using WSAD keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        newEye += sensitivity * deltaTime * flatFront;//camera.front;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        newEye -= sensitivity * deltaTime * flatFront;//camera.front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        newEye -= sensitivity * deltaTime * flatRight;//camera.right;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        newEye += sensitivity * deltaTime * flatRight;//camera.right;

    glm::vec2 centerToEye(newEye.x, newEye.z);
	if (Maths::length(centerToEye) < borderRadius -15)
	{
		camera.eye = newEye;
	}
	else
	{
		std::cout << "outta range.." << std::endl;
	}

	float sunDistance = Maths::length(camera.eye - currentSunPosition);
    if (sunDistance < 4.0f) // threshold for "close"
    {

		std::cout << "HEYY!!!" << std::endl;
        // getting random angle and radius for new position
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.1415926f;
        float radius = static_cast<float>(rand()) / RAND_MAX * 45.0f;
        float y = 1;

        currentSunPosition = glm::vec3(
            cos(angle) * radius,
            y,
            sin(angle) * radius
        );
    }
    else
    {
        std::cout << camera.eye << currentSunPosition << Maths::length(camera.eye - currentSunPosition) << std::endl;
    }
}

void mouseInput(GLFWwindow* window)
{
    // mouse cursor to center
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // updating yaw and pitch
    camera.yaw += 0.0005f * float(xPos - 1024 / 2);
    camera.pitch += 0.0005f * float(768 / 2 - yPos);
}