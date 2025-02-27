//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* fRojo = "shaders/shaderrojo.frag";
static const char* fAzul = "shaders/shaderazul.frag";
static const char* fVerde = "shaders/shaderverde.frag";
static const char* fVerdeOsc = "shaders/shaderverdeoscuro.frag";
static const char* fCafe = "shaders/shadercafe.frag";



float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		
		-0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f, 
		// back
		-0.5f, -0.5f,  -0.5f,
		0.5f, -0.5f,  -0.5f, 
		0.5f,  0.5f,  -0.5f, 
		-0.5f,  0.5f,  -0.5f
		//front
		
		
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/

	};
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 18);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);


	//Desde este punto se modifica CrearLetrasyFiguras
	//para agregar  cuadrafo cafe, cuadrado rojo, triangulo verde
	//y triangulo azul

	// ---- Cuadrado cafe (3)
	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,

	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	// ---- Cuadrado rojo (4)
	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};
	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	// ---- Triangulo verde (5)
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,

	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	// ---- Triangulo azul (6)
	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};
	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);


	// ---- Letra T (7)
	GLfloat vertices_T[] = {
		//X         Y        Z       R     G      B
		-0.8f,    0.3f,    0.0f,    0.0f,  1.0f,  1.0f,
		-0.4f,    0.3f,    0.0f,    0.0f,  1.0f,  1.0f,
		-0.4f,    0.15f,   0.0f,    0.0f,  1.0f,  1.0f,

		-0.8f,    0.3f,    0.0f,    0.0f,  1.0f,  1.0f,
		-0.8f,    0.15f,   0.0f,    0.0f,  1.0f,  1.0f,
		-0.4f,    0.15f,   0.0f,    0.0f,  1.0f,  1.0f,

		-0.675f,  0.15f,   0.0f,    0.0f,  1.0f,  1.0f,
		-0.525f,  0.15f,   0.0f,    0.0f,  1.0f,  1.0f,
		-0.525f, -0.275f,  0.0f,    0.0f,  1.0f,  1.0f,

		-0.675f,  0.15f,   0.0f,    0.0f,  1.0f,  1.0f,
		-0.675f, -0.275f,  0.0f,    0.0f,  1.0f,  1.0f,
		-0.525f, -0.275f,  0.0f,    0.0f,  1.0f,  1.0f,



	};
	MeshColor* letraT = new MeshColor();
	letraT->CreateMeshColor(vertices_T, 72);
	meshColorList.push_back(letraT);

	// ---- Letra B (8)
	GLfloat vertices_B[] = {
		//     X         Y         Z         R     G     B
		// ------ Letra B Vertical izquierdo
		-0.2f,    0.3f,    0.0f,    0.0f,  1.0f,  0.5f,
		-0.2f,   -0.275f,  0.0f,    0.0f,  1.0f,  0.5f,
		-0.05f,   0.3f,    0.0f,    0.0f,  1.0f,  0.5f,

		-0.05f,   0.3f,    0.0f,    0.0f,  1.0f,  0.5f,
		-0.2f,   -0.275f,  0.0f,    0.0f,  1.0f,  0.5f,
		-0.05f,  -0.275f,  0.0f,    0.0f,  1.0f,  0.5f,

		// ------ Letra B Horizontal central
		-0.05f,   0.3f,    0.0f,    0.0f,  1.0f,  0.5f,
		-0.05f,   0.175f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,   0.3f,    0.0f,    0.0f,  1.0f,  0.5f,

		-0.05f,   0.175f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,   0.175f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,   0.3f,    0.0f,    0.0f,  1.0f,  0.5f,

		-0.05f,   0.075f,  0.0f,    0.0f,  1.0f,  0.5f,
		-0.05f,  -0.05f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,   0.075f,  0.0f,    0.0f,  1.0f,  0.5f,

		-0.05f,  -0.05f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,  -0.05f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,   0.075f,  0.0f,    0.0f,  1.0f,  0.5f,

		-0.05f,  -0.15f,   0.0f,    0.0f,  1.0f,  0.5f,
		-0.05f,  -0.275f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,  -0.15f,   0.0f,    0.0f,  1.0f,  0.5f,

		-0.05f,  -0.275f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,  -0.275f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,  -0.15f,   0.0f,    0.0f,  1.0f,  0.5f,

		 // ------ Letra B Esquinas Derecha
		 0.05f,   0.3f,    0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,   0.175f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,    0.175f,  0.0f,    0.0f,  1.0f,  0.5f,

		 0.05f,  -0.275f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,  -0.15f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,   -0.15f,   0.0f,    0.0f,  1.0f,  0.5f,

		 // ------ Letra B Verticales Derecha
		 0.05f,   0.175f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,   0.075f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,    0.175f,  0.0f,    0.0f,  1.0f,  0.5f,

		 0.05f,   0.075f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,    0.075f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,    0.175f,  0.0f,    0.0f,  1.0f,  0.5f,

		 0.05f,  -0.05f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,  -0.15f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,   -0.05f,   0.0f,    0.0f,  1.0f,  0.5f,

		 0.05f,  -0.15f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,   -0.15f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,   -0.05f,   0.0f,    0.0f,  1.0f,  0.5f,

		 // ------ Letra B Centro Derecha
		 0.05f,   0.075f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,  -0.05f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,   -0.05f,   0.0f,    0.0f,  1.0f,  0.5f,

		 0.05f,  -0.05f,   0.0f,    0.0f,  1.0f,  0.5f,
		 0.05f,   0.075f,  0.0f,    0.0f,  1.0f,  0.5f,
		 0.2f,    0.075f,  0.0f,    0.0f,  1.0f,  0.5f,


	};
	MeshColor* letraB = new MeshColor();
	letraB->CreateMeshColor(vertices_B, 288);
	meshColorList.push_back(letraB);

	// ---- Letra G (9)
	GLfloat vertices_G[] = {
		 // X       Y       Z         R     G     B
		 // ------ Letra G Vertical izquierda
		 0.4f,    0.15f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.4f,   -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,

		 0.55f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.4f,    0.15f,   0.0f,    0.0f,  0.7f,  0.7f,

		// ------ Letra G Esquinas izquierda
		 0.4f,    0.15f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,   0.3f,    0.0f,    0.0f,  0.7f,  0.7f,

		 0.55f,  -0.275f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.4f,   -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,

		// ------ Letra G Centro horizontal
		 0.65f,   0.3f,    0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,   0.3f,    0.0f,    0.0f,  0.7f,  0.7f,

		 0.65f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.65f,   0.3f,    0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,

		 0.65f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,  -0.275f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,

		 0.65f,  -0.275f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.65f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.55f,  -0.275f,  0.0f,    0.0f,  0.7f,  0.7f,

		// ------ Letra G Esquinas derechas
		 0.65f,   0.3f,    0.0f,    0.0f,  0.7f,  0.7f,
		 0.65f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.80f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,

		 0.65f,  -0.275f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.65f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.80f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,

		// ------ Letra G Verticales derechas
		 0.65f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.65f,   0.10f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.80f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,

		 0.65f,   0.10f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.80f,   0.15f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.80f,   0.10f,   0.0f,    0.0f,  0.7f,  0.7f,

		 0.65f,   0.02f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.65f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.80f,   0.02f,   0.0f,    0.0f,  0.7f,  0.7f,

		 0.65f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.80f,   0.02f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.80f,  -0.125f,  0.0f,    0.0f,  0.7f,  0.7f,

		// ------ Letra G Palito del centro
		 0.60f,   0.02f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.60f,  -0.060f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.65f,   0.02f,   0.0f,    0.0f,  0.7f,  0.7f,

		 0.60f,  -0.060f,  0.0f,    0.0f,  0.7f,  0.7f,
		 0.65f,   0.02f,   0.0f,    0.0f,  0.7f,  0.7f,
		 0.65f,  -0.060f,  0.0f,    0.0f,  0.7f,  0.7f,
	};
	MeshColor* letraG = new MeshColor();
	letraG->CreateMeshColor(vertices_G, 288);
	meshColorList.push_back(letraG);


}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	// ------- Shaders Creados ------- //

	// ---- Rojo ----
	Shader* shader3 = new Shader(); 
	shader3->CreateFromFiles(vShader, fRojo);
	shaderList.push_back(*shader3);

	// ---- Azul ----
	Shader* shader4 = new Shader();
	shader4->CreateFromFiles(vShader, fAzul);
	shaderList.push_back(*shader4);

	// ---- Verde ----
	Shader* shader5 = new Shader();
	shader5->CreateFromFiles(vShader, fVerde);
	shaderList.push_back(*shader5);

	// ---- Verde Oscuro ----
	Shader* shader6 = new Shader();
	shader6->CreateFromFiles(vShader, fVerdeOsc);
	shaderList.push_back(*shader6);

	// ---- Cafe ----
	Shader* shader7 = new Shader();
	shader7->CreateFromFiles(vShader, fCafe);
	shaderList.push_back(*shader7);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	float aspectRatio = (float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight();
	//Aqui dividi el ancho de la ventana entre el alto para obtener la relacion de aspecto y que no saliera mal la relacion de aspecto
	//Logrando que se muestre bien el cubo y no parezca un rectangulo :)
    glm::mat4 projection = glm::ortho(-aspectRatio*2, aspectRatio*2, -2.0f, 2.0f, -2.0f, 100.0f);
    //glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -2.0f, 2.0f, -2.0f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad



	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		/*
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		
		// ---- // ---- // LETRAS (INICIALES TBG)  \\ ---- \\ ---- \\
		// ----Letra T----
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.00f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[7]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.00f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[8]->RenderMeshColor();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.00f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[9]->RenderMeshColor();
		*/
		// ---- // ---- // CASA FORMADA CON CUBOS Y PIRAMIDES \\ ---- \\ ---- \\

		// ---- Cubos Verdes ----
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.50f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.7f, 0.2f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.7f, 0.2f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		// ---- Cubos Cafes ----
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.1f, -1.7f, -4.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.1f, -1.7f, -4.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// ---- Cubo Rojo ----
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -4.0f));
		model = glm::scale(model, glm::vec3(3.00f, 3.00f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// ---- Piramides Verdes ----
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.1f, -0.6f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.1f, -0.6f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// ---- Piramide Azul ----
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.4f, -3.0f));
		model = glm::scale(model, glm::vec3(4.00f, 0.9f, 1.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/