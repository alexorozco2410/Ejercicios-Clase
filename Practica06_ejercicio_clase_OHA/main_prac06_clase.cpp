/*---------------------------------------------------------*/
/* ----------------   Pr�ctica 6 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computaci�n gr�fica e interacci�n humano computadora ---------------*/
/*-------------  Orozco Hernandez Alexis  ----------------*/
/*----------------  Version VS 2017  ---------------------*/
/*----------------  EJERCICIO CLASE  ---------------------*/
/*-------------------  Grupo 04  -------------------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <iostream>

#define M_PI 3.14159265358979323846264338327950288
//#include "Sphere.h"

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
void animate(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

float	sol = 0.0f,
mercurio = 0.0f,
venus = 0.0f,
tierra = 0.0f,
marte = 0.0f,
jupiter = 0.0f,
saturno = 0.0f,
urano = 0.0f,
neptuno = 0.0f;

//To Buid the Sphere
//---------------------------------------------------------------------------
//const int na = 36;        // vertex grid size
//son el numero de divisiones que tendra la esfera, lo cual permite que parezca mas una esfera que otra cosa
const int meridianos = 36;			// vertex grid size   na -> meridianos 
const int paralelos = 8;			// nb -> paralelos
const int meridianos3 = meridianos * 3;     // line in grid size
const int nn = paralelos * meridianos3;    // whole grid size
GLfloat sphere_pos[nn]; // vertex
GLfloat sphere_nor[nn]; // normal
//GLfloat sphere_col[nn];   // color
GLuint  sphere_ix[meridianos*(paralelos - 1) * 6];    // indices
GLuint sphere_vbo[4] = { -1,-1,-1,-1 };
GLuint sphere_vao[4] = { -1,-1,-1,-1 };

void sphere_init()
{
	//con esta funcion se genera o dibuja la esfera
	// generate the sphere data
	GLfloat x, y, z, a, b, da, db, r = 1.0;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(meridianos);
	db = (GLfloat)M_PI / GLfloat(paralelos - 1);
	// [Generate sphere point data]
	// spherical angles a,b covering whole sphere surface
	for (ix = 0, b = (GLfloat)-0.5*M_PI, ib = 0; ib < paralelos; ib++, b += db)
		for (a = 0.0, ia = 0; ia < meridianos; ia++, a += da, ix += 3)
		{
			// unit sphere
			x = cos(b)*cos(a);
			y = cos(b)*sin(a);
			z = sin(b);
			sphere_pos[ix + 0] = x * r;
			sphere_pos[ix + 1] = y * r;
			sphere_pos[ix + 2] = z * r;
			sphere_nor[ix + 0] = x;
			sphere_nor[ix + 1] = y;
			sphere_nor[ix + 2] = z;
		}
	// [Generate GL_TRIANGLE indices]
	for (ix = 0, iy = 0, ib = 1; ib < paralelos; ib++)
	{
		for (ia = 1; ia < meridianos; ia++, iy++)
		{
			// first half of QUAD
			sphere_ix[ix] = iy;      ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + meridianos;   ix++;
			// second half of QUAD
			sphere_ix[ix] = iy + meridianos;   ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + meridianos + 1; ix++;
		}
		// first half of QUAD
		sphere_ix[ix] = iy;       ix++;
		sphere_ix[ix] = iy + 1 - meridianos;  ix++;
		sphere_ix[ix] = iy + meridianos;    ix++;
		// second half of QUAD
		sphere_ix[ix] = iy + meridianos;    ix++;
		sphere_ix[ix] = iy - meridianos + 1;  ix++;
		sphere_ix[ix] = iy + 1;     ix++;
		iy++;
	}
	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, sphere_vao);
	glGenBuffers(4, sphere_vbo);
	glBindVertexArray(sphere_vao[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_pos), sphere_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_ix), sphere_ix, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	i = 2; // normal
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_nor), sphere_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	/*
		i=3; // color
		glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_col),sphere_col,GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);
	*/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void sphere_exit()
{
	glDeleteVertexArrays(4, sphere_vao);
	glDeleteBuffers(4, sphere_vbo);
}
void sphere_draw()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glBindVertexArray(sphere_vao[0]);
	//  glDrawArrays(GL_POINTS,0,sizeof(sphere_pos)/sizeof(GLfloat));                   // POINTS ... no indices for debug
	glDrawElements(GL_LINE_LOOP, sizeof(sphere_ix) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)
	//glDrawElements(GL_TRIANGLES, sizeof(sphere_ix) / sizeof(GLuint), GL_UNSIGNED_INT, 0);   //GENERA LA ESFERA PERO SOLIDA
	glBindVertexArray(0);
}



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

//con esto se rota la esfera
void animate(void)
{
	sol += 0.1f;
	mercurio += 0.4f;
	venus += 0.35f;
	tierra += 0.3f;
	marte += 0.25f;
	jupiter += 0.2f;
	saturno += 0.15f;
	urano += 0.1f;
	neptuno += 0.05f;
	
}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 temp01 = glm::mat4(1.0f); //Temp

	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar c�digo aqu�
	model = glm::rotate(model, glm::radians(sol), glm::vec3(0.0f, 0.5f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	sphere_draw();	//Sun


	//mercurio
	model = glm::mat4(1.0f); //inicializa la matriz para comenzar desde el origen (en este caso coincide con el sol que esta en el origen)
	model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0, 1, 0));//genera el movimiento de traslacion, porque en este rotate model esta en el origen
	model = glm::translate(model, glm::vec3(3, 0, 0));//s4e mueve 3 unidades a la derecha
	model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0, 1, 0));//indica sobre que eje se hara la rotacion
	//suponiendo que le pongamos una luna a mercurio
	temp01 = model;

	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.6f, 0.0f, 0.3f));
	sphere_draw();	//mercurio
	//ahora se deben de crear sus animaciones como el sol, se crea la variable mercurio y se mete dentro de la funcion animate
	/*posteriormente se debe hacer el movimiento de traslacion alrededor del sol
	necesitamos aplicar una rotacion cuando el pivote todavia esta en el origen es decire en el sol*/

	//luna_mercurio
	//model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	//model = glm::translate(model, glm::vec3(1.5, 0, 0));
	//model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0, 1, 0)); //si no se pone el translate, con la escala podemos hacer una esfera muy aplastada para que parezca un anillo 
	//model = glm::scale(model, glm::vec3(1.2f, 0.1f, 1.2f));
	//projectionShader.setMat4("model", model);
	//projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//sphere_draw();	//luna o anillo

	//venus
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(venus), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(3.5, 0, 0));
	model = glm::rotate(model, glm::radians(venus), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8f, 0.2f, 0.0f));
	sphere_draw();	//venus

	//earth
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(4.3, 0, 0));
	model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.8f));
	sphere_draw();	//earth

	//luna tierra
	model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(2.2, 0, 0));
	model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.58f, 0.65f));
	sphere_draw();

	//marte
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(marte), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(5.3, 0, 0));
	model = glm::rotate(model, glm::radians(marte), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.13f));
	temp01 = model; //temporal esta en el centro de marte
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.7f, 0.3f, 0.1f));
	sphere_draw();	//marte

	//satelite PHOBOS
	model = temp01;
	model = glm::rotate(model, glm::radians(marte), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(2.0, -0.3, 0.0));
	model = glm::rotate(model, glm::radians(marte), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.58f, 0.65f));
	sphere_draw();

	//satelite DEIMOS
	model = temp01;
	model = glm::rotate(model, glm::radians(marte), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(-2.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(marte), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.58f, 0.65f));
	sphere_draw();

	//jupiter
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(jupiter), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.5, 0, 0));
	model = glm::rotate(model, glm::radians(jupiter), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	temp01 = model; //temporal esta en el centro de jupiter
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.8f, 0.2f));
	sphere_draw();

	//satelite IO
	model = temp01;
	model = glm::rotate(model, glm::radians(jupiter), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(1.5, 0.2, 0.0));
	model = glm::rotate(model, glm::radians(jupiter), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.63f, 0.0f));
	sphere_draw();

	//satelite EUROPA
	model = temp01;
	model = glm::rotate(model, glm::radians(jupiter), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(-1.1, 0.0, -1.3));
	model = glm::rotate(model, glm::radians(jupiter), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8f, 0.0f, 0.0f));
	sphere_draw();

	//saturno
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(9.0, 0, 0));
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0));
	temp01 = model; //temporal esta en el centro de saturno
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.55f, 0.65f));
	sphere_draw();

	//anillo_saturno
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(1.8f, 0.00f, 1.8f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();

	//satelite TITAN
	model = temp01;

	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(1.3, 0, 0));
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8f, 0.45f, 0.05f));
	sphere_draw();
	
	//satelite DIONE
	model = temp01;
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(-1.0, -0.2, 0.5));
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.58f, 0.65f));
	sphere_draw();

	//satelite RHEA
	model = temp01;
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(-0.7, 0.15, -0.8));
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.58f, 0.65f));
	sphere_draw();

	// urano
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(11.8, 0, 0));
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	temp01 = model; //temporal esta en el centro de urano
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.5f, 0.8f));
	sphere_draw();

	//satelite UMBRIEL
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(1.6, 0, 0));
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.35f, 0.85f));
	sphere_draw();

	//satelite MIRANDA
	model = temp01;
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(-1.7, 0.2, -1.0));
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.11f, 0.11f, 0.11f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.58f, 0.65f));
	sphere_draw();

	//satelite TITANIA
	model = temp01;
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(0.0, -0.3, -2.2));
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.58f, 0.65f));
	sphere_draw();

	//satelite OBERON
	model = temp01;
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(0.4, 0.0, 1.5));
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.24f, 0.13f));
	sphere_draw();

	//neptuno
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(neptuno), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(14.0, 0, 0));
	model = glm::rotate(model, glm::radians(neptuno), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	temp01 = model; //temporal esta en el centro de neptuno
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.0f, 0.8f));
	sphere_draw();

	//satelite TRITON
	model = temp01;
	model = glm::rotate(model, glm::radians(neptuno), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(2.1, -0.4, 0));
	model = glm::rotate(model, glm::radians(neptuno), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.58f, 0.65f));
	sphere_draw();

	//satelite PROTEUS
	model = temp01;
	model = glm::rotate(model, glm::radians(neptuno), glm::vec3(0, 1, 0)); //con estas lineas se crea la luna
	model = glm::translate(model, glm::vec3(0, +0.3, 2.5));
	model = glm::rotate(model, glm::radians(neptuno), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.16f, 0.16f, 0.16f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.52f, 0.58f, 0.65f));
	sphere_draw();

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
	sphere_init();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);
		animate();

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
	sphere_exit();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.08f;
	

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}