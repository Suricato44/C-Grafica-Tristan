/*
Pr�ctica 7: Iluminaci�n 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture octaTexture;

Model Blackhawk_M;
Model Cuerpo_coche;
Model Cofre;
Model Ojos;
Model Llanta_Delantera_Der;
Model Llanta_Delantera_Izq;
Model Llanta_Trasera_Izq;
Model Llanta_Trasera_Der;
Model Pistola;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];


SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];
SpotLight spotLights3[MAX_SPOT_LIGHTS];


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CrearOctaedro()
{
	unsigned int octaedro_indices[] = {
		// front
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,

		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat octaedro_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		//Numero 4
		-0.5f, 0.0f,  0.5f,		0.496f,	0.265f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, 0.0f,  0.5f,		0.496f,	0.737f,		0.0f,	0.0f,	-1.0f,	//1
		0.0f,  0.816f,  0.0f,	0.254f,	0.502f,		0.0f,	0.0f,	-1.0f,	//2
		//Numero 2
		0.5f, 0.0f,  -0.5f,		0.255f,	0.983f,		-1.0f,	0.0f,	0.0f,	//0
		0.5f, 0.0f,  0.5f,		0.497f,	0.745f,		-1.0f,	0.0f,	0.0f,	//1
		0.0f,  0.816f,  0.0f,	0.254f,	0.505f,		-1.0f,	0.0f,	0.0f,	//2
		//Numero 6
		-0.5f, 0.0f,  -0.5f,	0.257f,	0.021f,		1.0f,	0.0f,	0.0f,	//0
		-0.5f, 0.0f,  0.5f,		0.497f,	0.255f,		1.0f,	0.0f,	0.0f,	//1
		0.0f,  0.816f,  0.0f,	0.257f,	0.491f,		1.0f,	0.0f,	0.0f,	//2

		//Numero 8
		-0.5f, 0.0f,  -0.5f,	0.250f,	0.018f,		0.0f,	0.0f,	1.0f,	//0
		0.5f, 0.0f,  -0.5f,		0.009f,	0.255f,		0.0f,	0.0f,	1.0f,	//1
		0.0f,  0.816f,  0.0f,	0.250f,	0.493f,		0.0f,	0.0f,	1.0f,	//2


		//Numero 1
		-0.5f, 0.0f,  0.5f,	0.505f,	0.736f,			0.0f,	0.0f,	-1.0f,	//0
		0.5f, 0.0f,  0.5f,		0.505f,	0.274f,		0.0f,	0.0f,	-1.0f,	//1
		0.0f,  -0.816f,  0.0f,	0.742f,	0.500f,		0.0f,	0.0f,	-1.0f,	//2

		//Numero 2
		0.5f, 0.0f,  -0.5f,		0.747f,	0.972f,		-1.0f,	0.0f,	0.0f,	//0
		0.5f, 0.0f,  0.5f,		0.507f,	0.745f,		-1.0f,	0.0f,	0.0f,	//1
		0.0f,  -0.816f,  0.0f,	0.747f,	0.505f,		-1.0f,	0.0f,	0.0f,	//2

		//Numero 7
		-0.5f, 0.0f,  -0.5f,	0.747f,	0.018f,		1.0f,	0.0f,	0.0f,	//0
		-0.5f, 0.0f,  0.5f,		0.507f,	0.255f,		1.0f,	0.0f,	0.0f,	//1
		0.0f,  -0.816f, 0.0f,	0.747f,	0.491f,		1.0f,	0.0f,	0.0f,	//2


		-0.5f, 0.0f,  -0.5f,	0.751f,	0.018f,		0.0f,	0.0f,	1.0f,	//0
		0.5f, 0.0f,  -0.5f,		0.992f,	0.255f,		0.0f,	0.0f,	1.0f,	//1
		0.0f,  -0.816f,  0.0f,	0.754f,	0.491f,		0.0f,	0.0f,	1.0f,	//2

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24);
	meshList.push_back(dado);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearOctaedro();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	octaTexture = Texture("Textures/octaedro.png");
	octaTexture.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");


	Cuerpo_coche = Model();
	Cuerpo_coche.LoadModel("Models/Cuerpo_coche.obj");

	Ojos = Model();
	Ojos.LoadModel("Models/Ojos.obj");

	Cofre = Model();
	Cofre.LoadModel("Models/Cofre2.obj");

	Llanta_Delantera_Izq = Model();
	Llanta_Delantera_Izq.LoadModel("Models/Llanta_Delantera_Der.obj");

	Llanta_Delantera_Der = Model();
	Llanta_Delantera_Der.LoadModel("Models/Llanta_Delantera_Izq.obj");

	Llanta_Trasera_Izq = Model();
	Llanta_Trasera_Izq.LoadModel("Models/Llanta_Trasera_Der.obj");

	Llanta_Trasera_Der = Model();
	Llanta_Trasera_Der.LoadModel("Models/Llanta_Trasera_Izq.obj");

	Pistola = Model();
	Pistola.LoadModel("Models/Perkins2.obj");

	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 0.0f,
		0.5f, 1.0f,
		44.0f, 15.0f, 50.0f,
		0.3f, 0.1f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
		0.5f, 1.0f,
		44.0f, 15.0f, 30.0f,
		0.3f, 0.01f, 0.01f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	unsigned int spotLightCount2 = 0;
	unsigned int spotLightCount3 = 0;

	//Arreglo 1
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.01f, 0.01f,
		30.0f);
	spotLightCount++;
	

	//Arreglo 2

	spotLights2[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.8f, 0.8f,
		5.0f);
	spotLightCount2++;


	spotLights2[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.01f, 0.01f,
		30.0f);
	spotLightCount2++;

	//Arreglo 3 

	spotLights3[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.8f, 0.8f,
		5.0f);
	spotLightCount3++;

	spotLights3[1] = SpotLight(1.0f, 0.0f, 0.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.01f, 0.01f,
		30.0f);
	spotLightCount3++;


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz


		//---- Sigue Luz Coche
		glm::vec3 PosicionFaro = glm::vec3(2.05f, 0.75f, 3.2 + mainWindow.getmuevex());
		glm::vec3 PosicionFaro2 = glm::vec3(2.05f, 0.75f, -7.2 + mainWindow.getmuevex());
		glm::vec3 Direccion = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 Direccion2 = glm::vec3(0.0f, 0.0f, -1.0f);

		spotLights[1].SetFlash(PosicionFaro, Direccion);
		spotLights2[1].SetFlash(PosicionFaro2, Direccion2);


		//Luz Roja
		glm::vec3 PosicionFaro3 = glm::vec3(4.0f, 1.05f, 3.2 + mainWindow.getmuevex());
		glm::vec3 lowerLight = PosicionFaro3;
		glm::vec3 Direccion3 = glm::vec3(0.0f, 0.0f, mainWindow.getartcoche());

		lowerLight.y -= mainWindow.getartcoche()*0.07; //Modificaci�n en el valor X
		spotLights3[1].SetFlash(lowerLight, Direccion);     


		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);

		if (mainWindow.getb_delantera() == 1 && mainWindow.getb_trasera() == 0 && mainWindow.getb_cofre() == 0) {
			if (mainWindow.getapagao() == 1) {
				shaderList[0].SetSpotLights(spotLights, spotLightCount);
				shaderList[0].SetPointLights(pointLights, pointLightCount);

			}
			else {
				shaderList[0].SetSpotLights(spotLights, spotLightCount);
				shaderList[0].SetPointLights(pointLights, pointLightCount-1);
			}
			
		}
		else if (mainWindow.getb_delantera() == 0 && mainWindow.getb_trasera() == 1 && mainWindow.getb_cofre() == 0) {
			if (mainWindow.getapagao() == 1) {
				shaderList[0].SetSpotLights(spotLights2, spotLightCount2);
				shaderList[0].SetPointLights(pointLights, pointLightCount);
			}
			else {
				shaderList[0].SetSpotLights(spotLights2, spotLightCount2);
				shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
			}
		}
		else if (mainWindow.getb_delantera() == 0 && mainWindow.getb_trasera() == 0 && mainWindow.getb_cofre() == 1) {
			if (mainWindow.getapagao() == 1) {
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
				shaderList[0].SetPointLights(pointLights, pointLightCount);
			}
			else {
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
				shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
			}
		}
		else {
			if (mainWindow.getapagao() == 1) {
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3-1);
				shaderList[0].SetPointLights(pointLights, pointLightCount);
			}
			else {
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3-1);
				shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
			}
		}
		

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//dado octa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		octaTexture.UseTexture();
		meshList[4]->RenderMesh();

		//---- Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 1.0f, -1.5f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getmuevex()));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		Cuerpo_coche.RenderModel();
		modelaux = model;

		//---- Cofre
		model = glm::translate(model, glm::vec3(0.0f, 0.533f, 1.54f));
		model = glm::rotate(model, glm::radians(mainWindow.getartcoche()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		Cofre.RenderModel();

		//---- Ojos
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.05f, -0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ojos.RenderModel();

		//---- Llanta Delantera Der
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.212f, -1.345f, 3.257f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		Llanta_Delantera_Der.RenderModel();


		//---- Llanta Delantera Izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.193f, -1.299f, 3.239f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		Llanta_Delantera_Izq.RenderModel();


		//---- Llanta Trasera Der
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.175, -1.288f, -3.918f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		Llanta_Trasera_Der.RenderModel();


		//---- Llanta Trasera Izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.189f, -1.3f, -3.914f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		Llanta_Trasera_Izq.RenderModel();



		//---- Pistola Gungnir 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 6.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pistola.RenderModel();

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}