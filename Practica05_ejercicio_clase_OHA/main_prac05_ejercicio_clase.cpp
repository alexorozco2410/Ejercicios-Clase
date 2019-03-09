/*---------------------------------------------------------*/
/* ----------------   Pr�ctica 5 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computaci�n gr�fica e interacci�n humano computadora ---------------*/
/*--------------  Orozco Hernandez Alexis  ---------------*/
/*-----------------  Version VS 2017  --------------------*/
/*--------------------  Grupo 04  ------------------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f,
rotY = 0.0f;
//brazo
float angHom = 0.0f;
float angCo = 0.0f;
float mu = 0.0f;
float angPu = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection; AQUI ESTAN LAS MATRICES TEMPORALES Temp1 y Temp2
	glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
	glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	
	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view,glm::radians(rotY),glm::vec3(0.0f,1.0f,0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	model = glm::rotate(model, glm::radians(angHom), glm::vec3(0,0,1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor",glm::vec3(1,1,0));
	glDrawArrays(GL_QUADS,0,24);

	//bicep
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(2, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	/*COMO LAS ESCALAS SON ACUMULATIVAS, HACEMOS UNA MATRIZ TEMPORAL PARA GUARDAR UNA ESCALA Y QUE PODAMOS TRABAJAR UNIFORMEMENTE 
	CADA QUE HAGAMOS LA CREACION DE CADA CUBO (LINEAS 122, 123)
	DEBEMOS AGREGAR " model = modelTemp " PARA REGRESAR LA ESCALA A LO ESTABLECIDO EN LA MATRIZ TEMPORAL
	VIENE SIENDO ALGO PARECIDO A UN CHECKPOINT
	*/
	//antebrazo
	model = glm::translate(model, glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(angCo), glm::vec3(0, 1, 0));//articualacion de codo
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(3, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//palma
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(mu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.5, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(1, 1, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;
	
	//PULGAR
	modelTemp2 = model;
	model = glm::translate(model, glm::vec3(-0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(angPu), glm::vec3(1, 0, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//pulgar2
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	model = glm::rotate(model, glm::radians(angPu), glm::vec3(1, 0, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.6f));
	glDrawArrays(GL_QUADS, 0, 24);
	

	//indice
	model = modelTemp2;
	model = glm::translate(model, glm::vec3(0.5, 0.38, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//2
	model = glm::translate(model, glm::vec3(0.15, 0.0, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//3
	model = glm::translate(model, glm::vec3(0.15, 0.0, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//medio
	model = modelTemp2;
	model = glm::translate(model, glm::vec3(0.5, 0.12, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.2, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//2
	model = glm::translate(model, glm::vec3(0.2, 0.0, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.2, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//3
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.2, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.4, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//anular
	model = modelTemp2;
	model = glm::translate(model, glm::vec3(0.5, -0.12, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.6f, 0.9f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//2
	model = glm::translate(model, glm::vec3(0.15, 0.0, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.6f, 0.9f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//3
	model = glm::translate(model, glm::vec3(0.15, 0.0, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.6f, 0.9f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//menique
	model = modelTemp2;
	model = glm::translate(model, glm::vec3(0.5, -0.38, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.2f, 0.1f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//2
	model = glm::translate(model, glm::vec3(0.1, 0.0, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.2f, 0.1f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	//3
	model = glm::translate(model, glm::vec3(0.1, 0.0, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));//articulacion de munieca
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	projectionShader.setMat4("model", model);
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.25, 0.3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.2f, 0.1f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = modelTemp;

	glBindVertexArray(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi funci�n de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.15f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.15f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		movY -= 0.15f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		movY += 0.15f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.15f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.15f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.6f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.6f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY -= 0.6f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY += 0.6f;
	if (glfwGetKey(window, GLFW_KEY_T)) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
			if (angHom < 90)
				angHom += 0.8;
		}
		else {
			if (angHom > -90)
				angHom -= 0.8;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_Y)) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
			if (angCo <0)
				angCo += 0.8;
		}
		else {
			if (angCo > -140)
				angCo -= 0.8;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_U)) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
			if (mu <20)
				mu += 0.8;
		}
		else {
			if (mu > -40)
				mu -= 0.8;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_I)) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
			if (angPu <80)
				angPu += 0.8;
		}
		else {
			if (angPu > 0)
				angPu -= 0.8;
		}
	}
	/*if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		if (mode == GLFW_MOD_SHIFT) {
			hombro += 2.0f;
		}
		else {
			//lowercase
			hombro -= 2.0f;
		}
	}*/

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}