#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h> 
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(-5.0f, 5.57f, 5.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


bool flashlightOn = false;
bool flashlightKeyPressed = false;
//Exercise 13
//lighting
glm::vec3 lightPos(1.2f, 2.8f, 2.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercise 15 Task 4", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // configure global opengl state
    //Exercise 11 Task 3
    // -----------------------------u
    glEnable(GL_DEPTH_TEST);

	    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("shaders/shader_exercise15t4_casters.vs", "shaders/shader_exercise15t4_casters.fs");
	Shader lightCubeShader("shaders/shader_exercise15_lightcube.vs", "shaders/shader_exercise15_lightcube.fs");

    Model ourModel("C:/Users/alejo/Documents/Visual Studio 2022/OpenGL/OpenGL/model/free_fire_clocktower_3d_model_by_ffxn/clocktower.obj"); //mapa
    Model ourModel1("C:/Users/alejo/Documents/Visual Studio 2022/OpenGL/OpenGL/model/oil_drums/oil.obj"); // barriles
    Model ourModel2("C:/Users/alejo/Documents/Visual Studio 2022/OpenGL/OpenGL/model/rusted_waste_container_low_poly/container.obj"); //contenedores
    //Model ourModel4("C:/Users/alejo/Documents/OpenGL/OpenGL/model/lampara/lamp.obj"); //lampara nueva
    //Model ourModel4("C:/Users/alejo/Documents/Visual Studio 2022/OpenGL/OpenGL/model/street_lantern/sin_nombre.obj"); //linternas viejas
    Model ourModel4("C:/Users/alejo/Documents/Visual Studio 2022/OpenGL/OpenGL/model/street_lantern/sin_nombre.obj"); //linternas nueva nueva

    Model ourModel5("C:/Users/alejo/Documents/Visual Studio 2022/OpenGL/OpenGL/model/mp5/mp5.obj"); // mp5 ahora es carro
    Model ourModel6("C:/Users/alejo/Documents/Visual Studio 2022/OpenGL/OpenGL/model/criminalbundle-free-fire/criminalbundle.obj"); // enemigo
    Model ourModel8("C:/Users/alejo/Documents/OpenGL/OpenGL/model/la_luna__viaje_virtual_guiado__3d___vr/luna.obj"); //luna
    //Model ourModel8("C:/Users/alejo/Documents/OpenGL/OpenGL/model/la_luna__viaje_virtual_guiado__3d___vr/luna.obj");



    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

 float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };


 // BARRILES
 glm::vec3 oilPositions[] = {
         glm::vec3(2.50f, 3.21f, -7.0f),
         glm::vec3(19.0f,  2.81f, 9.2f),
         glm::vec3(-6.30f, 4.97f, 9.4f),
         glm::vec3(-15.30f, 3.21f, 12.7f),

 };

//positions all containers
glm::vec3 cubePositions[] = {
    glm::vec3(15.0f,  3.0f, 10.2f),
    glm::vec3(-12.30f, 3.51f, 7.7f),
    glm::vec3(2.0f, 5.17f, 12.0f),
    glm::vec3(7.2f, 4.84f, 8.0f),
    glm::vec3(-7.0f,  5.2f, 13.5f),
    glm::vec3(-8.5f,  3.42f, 22.5f),
        //glm::vec3(4.4f, 3.4f, -6.0f),
        //glm::vec3(-3.8f, -2.0f, -12.3f),
//glm::vec3( 2.4f, -0.4f, -3.5f),
        
        //glm::vec3(-1.3f,  3.4f, -7.5f),
        //glm::vec3(-2.1f,  3.4f, -7.5f),
        //glm::vec3(-1.5f,  3.8f, -7.5f),
        //glm::vec3(-1.9f,  3.8f, -7.5f),

        
        //glm::vec3(4.4f,  3.4f, -3.0f),
        //glm::vec3(4.0f,  3.8f, -3.0f),
        //glm::vec3(4.4f,  3.8f, -3.0f),

        
        //glm::vec3(1.2f,  3.4f, -4.5f),
        
        //glm::vec3(-0.4f,  3.4f, -5.7f),
       // glm::vec3(-1.2f,  3.4f, -5.7f),
        glm::vec3(-2.0f,  3.4f, -5.7f)
    };
/*
glm::vec3 cubePositions1[] = {
        glm::vec3(20.0f, 45.0f, 8.0f),
        glm::vec3(18.99f,  2.96f, 8.90f),
        glm::vec3(19.08f,  3.87f, 8.90f),
        glm::vec3(19.0f,  3.18f, 8.90f),
        glm::vec3(20.0f, 45.0f, 8.0f),//bien
        glm::vec3(19.20f,  3.70f, 9.350f),
        glm::vec3(19.0f,  3.15f, 9.35f),
        glm::vec3(19.0f,  3.65f, 9.35f),
        glm::vec3(18.90f,  3.53f, 9.35f),
        glm::vec3(18.80f,  3.73f, 9.35f)
};*/
/*
glm::vec3 oilPositions[] = {
        glm::vec3(2.0f, 3.2f, -7.0f),
        glm::vec3(14.0f, 2.9f, 6.0f),
};*/

glm::vec3 containerPositions[] = {
        glm::vec3(9.0f, 3.2f, -0.5f),
        glm::vec3(15.5f, 2.9f, 5.1f),
};

//CUBOS DE LUZ
glm::vec3 pointLightPositions[] = {
        //luz de campanario
       /* glm::vec3(0.04f, 11.92f, 15.22f),
        glm::vec3(0.04f, 11.92f, 15.12f),
        glm::vec3(0.04f, 11.84f, 15.22f),
        glm::vec3(0.04f, 11.84f, 15.12f),*/

        glm::vec3(0.6f, 6.3f, 1.7f),
        glm::vec3(-3.5f, 6.3f, 1.7f),
        glm::vec3(-9.50f, 6.3f, 9.2f),
        glm::vec3(-9.50f, 6.3f, 13.1f),
        glm::vec3(-9.50f, 6.3f, 17.0f),
        glm::vec3(-7.4f, 6.3f, 17.0f),
        glm::vec3(-3.35f, 6.3f, 17.0f),
        glm::vec3(2.65f, 6.3f, 17.0f),

        glm::vec3(2.65f, 6.3f, 13.4f),
        glm::vec3(2.65f, 6.3f, 8.9f),
        glm::vec3(-9.5f, 6.3f, 1.7f),

        glm::vec3(2.65f, 6.3f, 1.7f)
};

// LAMPARA
glm::vec3 lamparaPositions[] = {        
        glm::vec3(0.6f, 5.85f, 1.7f),
        glm::vec3(-3.5f, 5.85f, 1.7f),
        glm::vec3(-9.50f, 5.85f, 9.2f),


        glm::vec3(-9.50f, 5.85f, 13.1f),
        glm::vec3(-9.50f, 5.85f, 17.0f),
        glm::vec3(-7.4f, 5.85f, 17.0f),
        glm::vec3(-3.35f, 5.85f, 17.0f),
        glm::vec3(2.65f, 5.85f, 17.0f),     
        glm::vec3(2.65f, 5.85f, 13.4f),
        glm::vec3(2.65f, 5.85f, 8.9f),
        glm::vec3(-9.5f, 5.85f, 1.7f),

        glm::vec3(2.65f, 5.85f, 1.7f)        
};

// POSICIÓN ARMAS - CARRO
glm::vec3 mp5Positions[] = {
        glm::vec3(-3.15f,  5.0f, 9.2f),
        glm::vec3(7.0f, 3.3f, -7.0f),

};


 // first, configure the cube's VAO (and VBO)
 unsigned int VBO, cubeVAO;
 glGenVertexArrays(1, &cubeVAO);
 glGenBuffers(1, &VBO);

 glBindBuffer(GL_ARRAY_BUFFER, VBO);
 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

 glBindVertexArray(cubeVAO);
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
 glEnableVertexAttribArray(0);
 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
 glEnableVertexAttribArray(1);
 glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
 glEnableVertexAttribArray(2);


 // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
 unsigned int lightCubeVAO;
 glGenVertexArrays(1, &lightCubeVAO);
 glBindVertexArray(lightCubeVAO);

 glBindBuffer(GL_ARRAY_BUFFER, VBO);
 
 // note that we update the lamp's position attribute's stride to reflect the updated buffer data
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
 glEnableVertexAttribArray(0);

 // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    unsigned int diffuseMap = loadTexture("textures/container2.png");
	unsigned int specularMap = loadTexture("textures/container2_specular.png");


    // shader configuration
    // --------------------
    lightingShader.use(); 
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

  
    camera.MovementSpeed = 5;
 // render loop
 // -----------
 while (!glfwWindowShouldClose(window))
 {
     // per-frame time logic
     // --------------------
     float currentFrame = glfwGetTime();
     deltaTime = currentFrame - lastFrame;
     lastFrame = currentFrame;

     // input
     // -----
     processInput(window);

     // render
     // ------
     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     if (flashlightOn)
     {
         lightingShader.setVec3("spotLight.position", camera.Position);
         lightingShader.setVec3("spotLight.direction", camera.Front);
     }

     float totalRotationRadians = 0.0f;
     float rotationSpeed = 50.0f; // Velocidad de rotación en grados por segundo
     totalRotationRadians += glm::radians(rotationSpeed) * deltaTime;

     float tiempo = glfwGetTime();
     float valorCambiante = cos(tiempo) / 2.0f + 0.5f;

     if (totalRotationRadians >= glm::radians(360.0f))
     {
         totalRotationRadians -= glm::radians(360.0f);
     }

     // be sure to activate shader when setting uniforms/drawing objects
     lightingShader.use();

     //Exercise 15 Task 
     lightingShader.setVec3("viewPos", camera.Position);
     lightingShader.setFloat("material.shininess", 25.0f);

     // directional light
     lightingShader.setVec3("dirLight.direction", 0.0f, -2.0f, 0.0f);
     lightingShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.3f);
     lightingShader.setVec3("dirLight.diffuse", 0.0f, 0.0f, 0.0f);
     lightingShader.setVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);
     /*
     // Luz de luna
     std::string lightName = "pointLights[" + std::to_string(0) + "].";

     lightingShader.setVec3(lightName + "position", pointLightPositions[0]);
     lightingShader.setVec3(lightName + "ambient", 0.5f, 0.5f, 0.5f);
     lightingShader.setVec3(lightName + "diffuse", 0.8f, 0.8f, 0.8f);
     lightingShader.setVec3(lightName + "specular", 1.0f, 1.0f, 1.0f);
     lightingShader.setFloat(lightName + "constant", 10.0f);
     lightingShader.setFloat(lightName + "linear", 0.09);
     lightingShader.setFloat(lightName + "quadratic", 0.032);

     */


     unsigned int cantidadLuces = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);

     for (unsigned int i = 0; i < 17; i++) {
         std::string lightName = "pointLights[" + std::to_string(i) + "].";

         lightingShader.setVec3(lightName + "position", pointLightPositions[i]);
         lightingShader.setVec3(lightName + "ambient", 0.05f, 0.05f, 0.05f);
         lightingShader.setVec3(lightName + "diffuse", 0.8f, 0.8f, 0.8f);
         lightingShader.setVec3(lightName + "specular", 1.0f, 1.0f, 1.0f);
         lightingShader.setFloat(lightName + "constant", 1.0f);
         lightingShader.setFloat(lightName + "linear", 0.09);
         lightingShader.setFloat(lightName + "quadratic", 0.032);
     }


     // spotLight
     lightingShader.setVec3("spotLight.position", camera.Position);
     lightingShader.setVec3("spotLight.direction", camera.Front);
     lightingShader.setVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
     lightingShader.setVec3("spotLight.diffuse", 0.2f, 0.2f, 0.2f);
     lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
     lightingShader.setFloat("spotLight.constant", 0.1f);
     lightingShader.setFloat("spotLight.linear", 0.09);
     lightingShader.setFloat("spotLight.quadratic", 0.032);
     lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
     lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


     

     // view/projection transformations
     glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
     glm::mat4 view = camera.GetViewMatrix();
     lightingShader.setMat4("projection", projection);
     lightingShader.setMat4("view", view);

     //Luna
     glm::mat4 model1 = glm::mat4(1.0f);
     model1 = glm::mat4(1.0f);
     model1 = glm::translate(model1, glm::vec3(20.0f, 45.0f, 8.0f));
     model1 = glm::rotate(model1, valorCambiante, glm::vec3(0.0f, 1.0f, 0.0f));
     model1 = glm::scale(model1, glm::vec3(10.0f, 10.0f, 10.0f));
     lightingShader.setMat4("model", model1);
     ourModel8.Draw(lightingShader);


     //mapa
     glm::mat4 model2 = glm::mat4(1.0f);
     model2 = glm::translate(model2, glm::vec3(-6.0f, -8.0f, -0.4f)); // translate it down so it's at the center of the scene
     model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
     lightingShader.setMat4("model", model2);
     ourModel.Draw(lightingShader);

     

     glBindVertexArray(cubeVAO);
     for (unsigned int i = 0; i < 7; i++) {
         //calculate the model matrix for each object and pass it to the shader before drawing
         glm::mat4 model3 = glm::mat4(1.0f);
         model3 = glm::translate(model3, cubePositions[i]);
         model3 = glm::scale(model3, glm::vec3(0.4f)); // Puedes ajustar el valor de escala según tus necesidade
         lightingShader.setMat4("model", model3);
         glDrawArrays(GL_TRIANGLES, 0, 36);
     }

     /*
     glBindVertexArray(cubeVAO);
     for (unsigned int i = 0; i < 10; i++) {
         //calculate the model matrix for each object and pass it to the shader before drawing
         glm::mat4 model4 = glm::mat4(1.0f);
         model4 = glm::translate(model4, cubePositions1[i]);
         float angle = totalRotationRadians + glm::radians(20.0f) * i; // Puedes ajustar el factor de rotación aquí
         model4 = glm::rotate(model4, angle, glm::vec3(1.0f, 0.3f, 0.5f));
         model4 = glm::scale(model4, glm::vec3(0.04f)); // Puedes ajustar el valor de escala según tus necesidade
         lightingShader.setMat4("model", model4);
         glDrawArrays(GL_TRIANGLES, 0, 36);
     }*/


     // BARRILES
     for (unsigned int i = 0; i <4 ; i++) {
         glm::mat4 model5 = glm::mat4(1.0f);
         model5 = glm::translate(model5, oilPositions[i]);
         model5 = glm::scale(model5, glm::vec3(0.5f, 0.5f, 0.5f));
         lightingShader.setMat4("model", model5);
         ourModel1.Draw(lightingShader);
     }
     /*
     // ENEMIGO
     for (unsigned int i = 0; i < 5; i++) {
         glm::mat4 model6 = glm::mat4(1.0f);
         if (i == 0) {
             model6 = glm::translate(model6, criminalRojoPositions[i]);
             model6 = glm::scale(model6, glm::vec3(0.2f, 0.2f, 0.2f));
             float angle = totalRotationRadians + glm::radians(20.0f) * i; ; // Ángulo de rotación en radianes (45 grados en este ejemplo)
             model6 = glm::rotate(model6, angle, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación alrededor del eje Y
             lightingShader.setMat4("model", model6);
             ourModel6.Draw(lightingShader);
         }
         else {
             model6 = glm::translate(model6, criminalRojoPositions[i]);
             model6 = glm::scale(model6, glm::vec3(0.2f, 0.2f, 0.2f));
             lightingShader.setMat4("model", model6);
             ourModel6.Draw(lightingShader);
         }
         
     }
     */
     for (unsigned int i = 0; i < 2; i++) {
         glm::mat4 model = glm::mat4(1.0f);
         model = glm::translate(model, mp5Positions[i]);
         model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
         lightingShader.setMat4("model", model);
         ourModel5.Draw(lightingShader);
     }

     // LAMPARA
     for (unsigned int i = 0; i < 12; i++) {
         glm::mat4 model = glm::mat4(1.0f);
         model = glm::translate(model, lamparaPositions[i]);
         model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
         lightingShader.setMat4("model", model);
         ourModel4.Draw(lightingShader);
     }
	 
     for (unsigned int i = 0; i < 2; i++) {
         glm::mat4 model = glm::mat4(1.0f);
         model = glm::translate(model, containerPositions[i]);
         model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
         lightingShader.setMat4("model", model);
         ourModel2.Draw(lightingShader);
     }


     lightCubeShader.use();
     lightCubeShader.setMat4("projection", projection);
     lightCubeShader.setMat4("view", view);


     //CUBOS DE LUZ
     glBindVertexArray(lightCubeVAO);
     for (unsigned int i = 0; i < 17; i++)
     {
         glm::mat4 model = glm::mat4(1.0f);
         model = glm::translate(model, pointLightPositions[i]);
         model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f)); // Make it a smaller cube
         lightCubeShader.setMat4("model", model);
         glDrawArrays(GL_TRIANGLES, 0, 36);
     }

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, diffuseMap);


     // bind specular map
     glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_2D, specularMap);
     glfwSwapBuffers(window);
     glfwPollEvents();
 }



 // we now draw as many light bulbs as we have point lights.


 
 // optional: de-allocate all resources once they've outlived their purpose:
 // ------------------------------------------------------------------------
 glDeleteVertexArrays(1, &cubeVAO);
 glDeleteVertexArrays(1, &lightCubeVAO);
 glDeleteBuffers(1, &VBO);

 // glfw: terminate, clearing all previously allocated GLFW resources.
 // ------------------------------------------------------------------
 glfwTerminate();
 return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

//Exercise 14 Task 2
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}