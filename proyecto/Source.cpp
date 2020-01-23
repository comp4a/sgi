#include <iostream> // Biblioteca de entrada salida
#include <sstream> // Biblioteca de manejo de strings
#include <cmath> // Biblioteca matematica de C
#include <GL/freeglut.h> // Biblioteca grafica
#include <ctime> //Biblioteca tiempo
#include <Utilidades.h>
#include <FreeImage.h>


#define POS_X 0
#define POS_Y 1
#define POS_Z 1

#define COLOR_FONDO 0.5, 0.5, 0.5,
#define FOV 45
#define CERCA 1
#define LEJOS 500

#define PROYECTO 0

using namespace std;

/**********************************
TODOS
- reparar luces
- cambiar generacion carretera
**********************************/


static float angulo = 0; // Angulo de travelling inicial de la camara
static const float velocidad = 6.0 * PI / 180; // radianes/segundo
static const int tasaFPS = 40; // tasa que se quiere como maximo
GLfloat streetLights[] = { GL_LIGHT2, GL_LIGHT3,GL_LIGHT4,GL_LIGHT5 };


static float girox = 0, giroy = 90;
static int xanterior, yanterior;

void init();
void loadLights();
void loadTextures();
	static GLuint texFondo;
	static GLuint texCarretera;
	static GLuint texSuelo;
	static GLuint texCar;
	static GLuint texCarDark;
	static GLuint texCarTop;
	static GLuint texCarTopDark;
	static GLuint texBar;
	static GLuint texArrow;
	static GLuint texPublicidad;
	static GLuint texPublicidad2;
	static GLuint texPole;
	static GLuint texCarretaraLight;


void display();

void rotation();
	static float x = 0;
	static float z = 0;
	static float speed = 0;
	int vista = 0;

void drawGUI();
void drawFloor();
void drawStreetlights();
void drawBackground();
void drawRoad();
void drawComercial();
	float A = 4, T = 100; // Amplitud & Periodo
	float anchuraCarretera = 2;
	GLfloat v0[3] = {0,0,0}, v1[3] = {0,0,0}, v2[3] = {0,0,0}, v3[3] = {0,0,0};
	GLfloat xLight, zLight;

void drawParticle();
	int particle_mode = 0;
	int dia = 0;
	int solidarios = 1;
	int texturas = 1;

void dynamicLights();

int main(int argc, char** argv);
void onTimer(int valor);
void onClick(int boton, int estado, int x, int y);
void onDrag(int x, int y);
void onKey(int key, int x, int y);
void onTimer(int valor);


/**********************************
**********************************/

float absolute(float height, float window_height){
	return (height/window_height -1); 
}



void drawGUI()
{
	// Dibuja elementos solidarios a la camara
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Cambio de camara a una ortografica
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	// Cambio la situacion de la camara 
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Dibujar un poligono que ocupe todo el viewport
	glDisable(GL_LIGHTING);
	float window_width = glutGet(GLUT_WINDOW_WIDTH);
	float window_height = glutGet(GLUT_WINDOW_HEIGHT);
	// coche
	if(vista == 0){
		float xini_abs = absolute(0,window_width);
		float yini_abs = absolute(-100,window_height);
		float xfin_abs = absolute(800,window_width);
		float yfin_abs = absolute(300,window_height);
		GLfloat v0[] = { xini_abs+1-(xfin_abs-xini_abs)/2,yini_abs,0 };
		GLfloat v1[] = { xfin_abs+1-(xfin_abs-xini_abs)/2,yini_abs,0 };
		GLfloat v2[] = { xfin_abs+1-(xfin_abs-xini_abs)/2,yfin_abs,0 };
		GLfloat v3[] = { xini_abs+1-(xfin_abs-xini_abs)/2,yfin_abs,0 };
		if(dia == 1){
			glBindTexture(GL_TEXTURE_2D, texCar);
		}else{
			glBindTexture(GL_TEXTURE_2D, texCarDark);
		}
		quadtex(v0, v1, v2, v3, 0, 1, 0, 1);
	}else{
		float xini_abs = absolute(0,window_width);
		float yini_abs = absolute(100,window_height);
		float xfin_abs = absolute(500,window_width);
		float yfin_abs = absolute(600,window_height);
		GLfloat v0[] = { xini_abs+1-(xfin_abs-xini_abs)/2,yini_abs,0 };
		GLfloat v1[] = { xfin_abs+1-(xfin_abs-xini_abs)/2,yini_abs,0 };
		GLfloat v2[] = { xfin_abs+1-(xfin_abs-xini_abs)/2,yfin_abs,0 };
		GLfloat v3[] = { xini_abs+1-(xfin_abs-xini_abs)/2,yfin_abs,0 };
		if(dia == 1){
			glBindTexture(GL_TEXTURE_2D, texCarTop);
		}else{
			glBindTexture(GL_TEXTURE_2D, texCarTopDark);
		}

		quadtex(v0, v1, v2, v3, 0, 1, 0, 1);
	}

	// velocidad
	float xini = absolute(50,window_width);
	float yini = absolute(50,window_height);
	float xfin = absolute(200,window_width);
	float yfin = absolute(speed*1000+60,window_height);
	GLfloat v0c[] = { xini,yini,0 };
	GLfloat v1c[] = { xfin,yini,0 };
	GLfloat v2c[] = { xfin,yfin,0 };
	GLfloat v3c[] = { xini,yfin,0 };
	glBindTexture(GL_TEXTURE_2D, texBar);
	quadtex(v0c, v1c, v2c, v3c, 0, 1, 0, 1);
	
	// direccion
	 xini = absolute(0,window_width);
	 yini = absolute(0,window_height);
	 xfin = absolute(100,window_width);
	 yfin = xfin;
	GLfloat v0v[] = { xini+1-(xfin-xini)/2,yini+1-(yfin-yini)/2,0 };
	GLfloat v1v[] = { xfin+1-(xfin-xini)/2,yini+1-(yfin-yini)/2,0 };
	GLfloat v2v[] = { xfin+1-(xfin-xini)/2,yfin+1-(yfin-yini)/2,0 };
	GLfloat v3v[] = { xini+1-(xfin-xini)/2,yfin+1-(yfin-yini)/2,0 };
	glPushMatrix();
	glRotatef(-giroy+180, 0, 0, 1);		
	glBindTexture(GL_TEXTURE_2D, texArrow);
	quadtex(v0v, v1v, v2v, v3v, 0, 1, 0, 1);
	glPopMatrix();

	// Volver a la camara original
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();

}



float* pos_abs(float window_width, float window_height, float width, float height){
	//asumimos que mide 100 de ancho la pantalla
	float max = 100;
	float ratio = height / width;
	float return_width = (window_width / max)*width;
	float return_height = return_width * ratio;
	float ret[2] = {return_width,return_height}; 
 	return ret;
}

float fsin(float u) {
	return A * sin((2 * PI * u) / T);
}
float dfsin(float u) {
	return ((2 * PI * A) / T) * cos((2 * PI * u) / T);
}

void rotation(){
	x += sin(giroy * PI / 180) * speed;
	z += cos(giroy * PI / 180) * speed;
	if(vista == 0){
		gluLookAt(x, 1, z, sin(giroy * PI / 180) + x, 1, cos(giroy * PI / 180) + z, 0, 1, 0);
	}else{
		gluLookAt(x, 10, z, sin(giroy * PI / 180) + x, 0, cos(giroy * PI / 180) + z, 0, 1, 0);
	}
}

void drawStreetlight(float x, float z, int index){
	GLfloat p02[] = { x, 3, z, 1 };
	cout << index << " - " << streetLights[index] << endl;
	glLightfv(streetLights[index], GL_POSITION, p02);
}

void drawParticle(){
	
	if(particle_mode == 0){	//nada
		float color[] = {0.3,0.3,0.3};
		glFogfv(GL_FOG_COLOR, color);
		glFogf(GL_FOG_DENSITY, 0);
	}else if(particle_mode == 1){	// niebla
		if(dia == 1){
			float color[] = {0.7,0.7,0.7};
			glFogfv(GL_FOG_COLOR, color);
			glFogf(GL_FOG_DENSITY, 0.1);
		}else{
			float color[] = {0.2,0.2,0.2};
			glFogfv(GL_FOG_COLOR, color);
			glFogf(GL_FOG_DENSITY, 0.1);
		}
	}else if(particle_mode == 2){	// arena
			if(dia == 1){
			float color[] = {1,0.8,0.4};
			glFogfv(GL_FOG_COLOR, color);
			glFogf(GL_FOG_DENSITY, 0.1);
		}else{
			float color[] = {0.4,0.2,0};
			glFogfv(GL_FOG_COLOR, color);
			glFogf(GL_FOG_DENSITY, 0.1);
		}
	}
}

void drawBackground(){
	float elements = 30;
	float alpha = 360 / elements;
	float height = 300;
	float distance = 400;
	float a2 =  PI/180.0 * alpha/2;
	float width = 2*sin(a2)*distance / sin(PI/2 - a2);
	for (int i = 0; i < elements; i++) {

		glPushMatrix();
		glRotatef(180, 1, 0, 0);		
		glTranslatef(x, 0, -z);
		glRotatef(alpha*i, 0, 1, 0);
		glTranslatef(0, -height*2/3 , distance);
		GLfloat v0[3] = { -width / 2,0,0 }, v1[3] = { width/2,0,0 }, v2[3] = { width/2,height,0 }, v3[3] = { -width / 2,height,0 };
		glBindTexture(GL_TEXTURE_2D, texFondo);
		quadtex(v3, v2, v1, v0, i*(1.0/elements),(i+1)*(1.0/elements),0,1,10,10);
		glPopMatrix();
	}
}

void loadLights(){

	GLfloat a00[] = { 0.2,0.2,0.2 };
	GLfloat d00[] = { 0.1,0.1,0.1};
	GLfloat e00[] = { 0.0,0.0,0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, a00);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, d00);
	glLightfv(GL_LIGHT0, GL_SPECULAR, e00);
	glEnable(GL_LIGHT0);

	//luz faro
	GLfloat a01[] = { 1,1,1,1};
	GLfloat d01[] = { 0,0,0,1};
	GLfloat e01[] = { 0,0,0,1};
	glLightfv(GL_LIGHT1, GL_AMBIENT, a01);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, d01);
	glLightfv(GL_LIGHT1, GL_SPECULAR, e01);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0);
	glEnable(GL_LIGHT1);

	//luz FAROLA 1
	GLfloat a02[] = { 1,1,1,1.0 };
	GLfloat d02[] = { 0.5,0.5,0.2,1.0 };
	GLfloat e02[] = { 0.0,0.0,0.0,1.0 };

	glLightfv(GL_LIGHT2, GL_AMBIENT, a02);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, d02);
	glLightfv(GL_LIGHT2, GL_SPECULAR, e02);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10);
	glEnable(GL_LIGHT2);

	glLightfv(GL_LIGHT3, GL_AMBIENT, a02);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, d02);
	glLightfv(GL_LIGHT3, GL_SPECULAR, e02);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10);
	glEnable(GL_LIGHT3);

	glLightfv(GL_LIGHT4, GL_AMBIENT, a02);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, d02);
	glLightfv(GL_LIGHT4, GL_SPECULAR, e02);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10);
	glEnable(GL_LIGHT4);

	glLightfv(GL_LIGHT5, GL_AMBIENT, a02);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, d02);
	glLightfv(GL_LIGHT5, GL_SPECULAR, e02);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10);
	glEnable(GL_LIGHT5);



}

void dynamicLights(){
	if(dia == 1){
		GLfloat a00[] = { 3,3,3 };
		GLfloat d00[] = { 1,1,1};
		glLightfv(GL_LIGHT0, GL_AMBIENT, a00);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, d00);
	}else{
		GLfloat a00[] = { 0.3,0.3,0.3 };
		GLfloat d00[] = { 0.1,0.1,0.1};
		glLightfv(GL_LIGHT0, GL_AMBIENT, a00);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, d00);
	}
	
	if(vista ==1){
		GLfloat posl1[] = { 0, 0, -9.3, 1.0 };
		GLfloat dir_centrall1[] = {0, 2, -1.0 };
		glLightfv(GL_LIGHT1, GL_POSITION, posl1);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_centrall1);
		GLfloat p02[] = { 0.0,0.0,-1.0 };
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION,p02 );
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION,p02 );
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION,p02 );
		glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION,p02 );
	}else{
		GLfloat p02[] = { 0.0,-1.0,0.0 };
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION,p02 );
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION,p02 );
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION,p02 );
		glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION,p02 );
		GLfloat posl1[] = { 0, -0.3, -2, 1.0 };
		GLfloat dir_centrall1[] = { 0, -1.5, -5 };
		glLightfv(GL_LIGHT1, GL_POSITION, posl1);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_centrall1);
	}

}

void loadTextures(){
    //glEnable( GL_TEXTURE_2D );
    glGenTextures( 1, &texArrow );
    glBindTexture( GL_TEXTURE_2D, texArrow );
    loadImageFile("images/arrow.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	
    glGenTextures( 1, &texBar );
    glBindTexture( GL_TEXTURE_2D, texBar );
    loadImageFile("images/bar.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &texFondo );
    glBindTexture( GL_TEXTURE_2D, texFondo );
    loadImageFile("images/fondo.jpeg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &texCarretera );
    glBindTexture( GL_TEXTURE_2D, texCarretera );
    loadImageFile("images/carretera.jpeg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &texSuelo );
    glBindTexture( GL_TEXTURE_2D, texSuelo );
    loadImageFile("images/suelo2.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &texCar );
    glBindTexture( GL_TEXTURE_2D, texCar );
    loadImageFile("images/coche1.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glGenTextures( 1, &texCarDark );
    glBindTexture( GL_TEXTURE_2D, texCarDark );
    loadImageFile("images/coche1_dark.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glGenTextures( 1, &texCarTop );
    glBindTexture( GL_TEXTURE_2D, texCarTop );
    loadImageFile("images/coche2.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	

	glGenTextures( 1, &texCarTopDark );
    glBindTexture( GL_TEXTURE_2D, texCarTopDark );
    loadImageFile("images/coche2_dark.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glGenTextures( 1, &texPublicidad );
    glBindTexture( GL_TEXTURE_2D, texPublicidad );
    loadImageFile("images/publicidad.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	
	glGenTextures( 1, &texPublicidad2 );
    glBindTexture( GL_TEXTURE_2D, texPublicidad2 );
    loadImageFile("images/publicidad2.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	/*glGenTextures( 1, &texCarretaraLight );
    glBindTexture( GL_TEXTURE_2D, texCarretaraLight );
    loadImageFile("images/carretera_light.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );*/

	glGenTextures( 1, &texPole );
    glBindTexture( GL_TEXTURE_2D, texPole );
    loadImageFile("images/pole.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}

void init(){
	cout << "Flechas izquierda / derecha: giran el coche" << endl;
	cout << "Flechas arriba / abajo: aceleran y desaceleran el coche" << endl;
	cout << "'l': activa el dia/noche" << endl;
	cout << "'s': actia/desactiva el modo alambrico" << endl;
	cout << "'n': activa la niebla, la tormenta de arena o la desactiva" << endl;
	cout << "'c': activa/desactiva los elementos solidarios a la camara" << endl;
	cout << "'v': cambia la camara de posición" << endl;

	loadTextures();


	loadLights();

	glClearColor(COLOR_FONDO 1.0);
	


	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_FOG);

}

void drawFloor(){
	int max = 200;
	int tamano_quad = 50;
	int numero_quad = 8;
	float altura = -0.1;
	int xflor = int(x) - int(x)%50; 
	int zflor = int(z) - int(z)%50;
	for(int i = 0; i < numero_quad;i++){
		for(int j = 0; j < numero_quad;j++){
			float v0[] = {xflor+i*tamano_quad-max,altura,zflor+j*tamano_quad-max};
			float v1[] = {xflor+i*tamano_quad-max,altura,zflor+j*tamano_quad-max + tamano_quad};
			float v2[] = {xflor+i*tamano_quad-max+tamano_quad,altura,zflor+j*tamano_quad-max + tamano_quad};
			float v3[] = {xflor+i*tamano_quad-max+tamano_quad,altura,zflor+j*tamano_quad-max};
			glBindTexture(GL_TEXTURE_2D, texSuelo);
			if(i <= 5 && i >= 3 and j <= 5 and j >= 3){
				quadtex(v0, v1, v2, v3, 0, 4, 0, 4,100,100);
			}else{
				quadtex(v0, v1, v2, v3, 0, 4, 0, 4,5,5);
			}
		}
	}

}

void drawComercial()
{	int separacion = 30;
	float inicio = int(x)-int(x)%(separacion*2);
	float vfsin = fsin(inicio);
	float derivada = dfsin(inicio);
	
	for (int i = 0; i <= 6; i ++) {
		float aux = inicio + i*separacion;
		float vfsin = fsin(aux);
		float derivada = dfsin(aux);
		GLfloat tz[3] = { -derivada,0,1 };
		GLfloat normales2[3] = { (1 / sqrt(1 + derivada * derivada)) * tz[0] , 0 ,(1 / sqrt(1 + derivada * derivada)) * tz[2] };

		float xLightDer = aux + (normales2[0] *2 );
		float zLightDer = vfsin + (normales2[2] * 2);
		float xLightIz = aux + (normales2[0] *-2 );
		float zLightIz = vfsin + (normales2[2] * -2);
		float xLightCent = aux;
		float zLightCent = vfsin;
		
		glBindTexture(GL_TEXTURE_2D, texPole);



		glPushMatrix();
		if(i %2 == 1){
			glTranslatef(xLightDer,0,zLightDer);
			glRotatef(-90,1,0,0);
			glutSolidCylinder(0.1,3,10,4);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(xLightIz,0,zLightIz);
			glRotatef(-90,1,0,0);
			glutSolidCylinder(0.1,3,10,4);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(xLightCent,3,zLightCent);
			glRotatef(-90,1,0,0);
			glutSolidCylinder(0.2,0.2,10,4);
			glPopMatrix();

			float v0[] = {xLightDer,2,zLightDer};
			float v1[] = {xLightDer,3,zLightDer};
			float v2[] = {xLightIz,3,zLightIz};
			float v3[] = {xLightIz,2,zLightIz};
			glBindTexture(GL_TEXTURE_2D, texPublicidad);
			quadtex(v0,v1,v2,v3);
		}else{
			glTranslatef(xLightDer,0,zLightDer);
			glRotatef(-90,1,0,0);
			glutSolidCylinder(0.1,3.5,10,4);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(xLightIz,0,zLightIz);
			glRotatef(-90,1,0,0);
			glutSolidCylinder(0.1,3.5,10,4);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(xLightCent,3.5,zLightCent);
			glRotatef(-90,1,0,0);
			glutSolidCylinder(0.2,0.2,10,4);
			glPopMatrix();

			float v0[] = {xLightDer,2,zLightDer};
			float v1[] = {xLightDer,3.5,zLightDer};
			float v2[] = {xLightIz,3.5,zLightIz};
			float v3[] = {xLightIz,2,zLightIz};
			glBindTexture(GL_TEXTURE_2D, texPublicidad2);
			quadtex(v0,v1,v2,v3);
		}
		glPopMatrix();

	}
	
}

void drawStreetlights()
{	int separacion = 30;
	float inicio = int(x)-int(x)%separacion;
	float vfsin = fsin(inicio);
	float derivada = dfsin(inicio);
	
	for (int i = 0; i < 4; i ++) {
		float aux = inicio + i*separacion;
		cout << aux << " : " << i << endl;
		float vfsin = fsin(aux);
		drawStreetlight(aux,vfsin,i);
	}
	
}

void drawRoad()
{
	float inicio = int(x) - 100;
	float vfsin = fsin(inicio);
	float derivada = dfsin(inicio);
	GLfloat inicial[3] = { inicio,0,vfsin };
	GLfloat tz[3] = { -derivada,0,1 };
	GLfloat normales[3] = { (1 / sqrt(1 + derivada * derivada)) * tz[0] , 0 ,(1 / sqrt(1 + derivada * derivada)) * tz[2] };
	for (int i = 0; i < 3; i++) {
		v0[i] = inicial[i] - (normales[i] * anchuraCarretera);
		v3[i] = inicial[i] + (normales[i] * anchuraCarretera);
	}
	for (int i = 1; i <= 200; i++) {
		vfsin = fsin(inicio + i);
		 derivada = dfsin(inicio + i);
		GLfloat calculo[3] = { inicio + i,0,vfsin };
		GLfloat tz[3] = { -derivada,0,1 };
		GLfloat normales[3] = { (1 / sqrt(1 + derivada * derivada)) * tz[0] , 0 ,(1 / sqrt(1 + derivada * derivada)) * tz[2] };
		for (int i = 0; i < 3; i++) {
			v1[i] = calculo[i] - (normales[i] * anchuraCarretera);
			v2[i] = calculo[i] + (normales[i] * anchuraCarretera);
		}

		glBindTexture(GL_TEXTURE_2D, texCarretera);
		quadtex(v2, v1, v0, v3,0,1,0,1, 10, 10);
		for (int i = 0; i < 3; i++) {
			v0[i] = v1[i];
			v3[i] = v2[i];
		}
	}
}

void display()
{



	stringstream titulo;
	titulo << speed * 40;
	glutSetWindowTitle(titulo.str().c_str());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Borra la pantalla
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	dynamicLights();

	rotation();	



	drawBackground();
	drawStreetlights();
	drawComercial();
	drawFloor();
	drawRoad();
	drawParticle();

	if(texturas == 1){
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(solidarios == 1){
			drawGUI();
		}
	}else{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}


	glutSwapBuffers(); // Intercambia los buffers
}

void reshape(GLint w, GLint h){
	//Calcula FOV
	float modulo_camara = sqrt(POS_X * POS_X + POS_Y * POS_Y + POS_Z * POS_Z);
	float fov = 2 * asin(1.0 / modulo_camara) * 180 / PI;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float razon = (float)w / h;
	gluPerspective(FOV, razon, CERCA, LEJOS);
}

void onTimer(int valor){

	//Calculamos el tiempo transcurrido desde la �ltima vez
	static int antes = 0;
	int ahora, tiempo_transcurrido;
	ahora = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurrido = ahora - antes; //Tiempo transcurrido desde antes msg
	angulo += velocidad * tiempo_transcurrido / 1000.0;
	if (angulo > 2 * PI) {
		angulo -= 2 * PI;
	}
	antes = ahora;
	glutTimerFunc(1000 / tasaFPS, onTimer, tasaFPS); // Se encola un nuevo timer
	glutPostRedisplay(); // Se manda el dibujo
}

void onClick(int boton, int estado, int x, int y) {
	//callback atencion a los botones del raton
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
	}
}

void onDrag(int x, int y) {
	//callback atencion al movimiento dell raton con el boton pulsado
	//al mover el mouse hacia la derecha, x aumenta y el giro al rededor del eje y es positivo
	static const float pixel2grados = 1;
	giroy += (x - xanterior) * pixel2grados;
	//al mover el raton hacia abajo la y aumenta y el giro es alredor del eje x
	girox += (y - yanterior) * pixel2grados;

	xanterior = x;
	yanterior = y;
	glutPostRedisplay();
}


void onKey(unsigned char key, int x, int y)
{
	key = tolower( key );
	switch (key)
	{
	case 'v':
		if(vista == 1){
			vista = 0;
		}else{
			vista = 1;
		}
		break;

	case 'n':
		particle_mode += 1;
		if(particle_mode > 2){
			particle_mode = 0;
		}
		break;

	case 'l':
		if(dia == 1){dia = 0;}else{dia = 1;}
		break;

	case 's':
		if(texturas == 1){texturas = 0;}else{texturas = 1;}
		break;

	case 'c':
		if(solidarios == 1){solidarios = 0;}else{solidarios = 1;}
		break;
	}

}

void onSpecialKey( int cursor, int x, int y ) {
    if( cursor == GLUT_KEY_UP ) {
		speed += 0.1/40;
    }
    if( cursor == GLUT_KEY_DOWN ) {
        speed -= 0.1/40;
		if (speed <= 0) {
			speed = 0;
		}
    }
    if( cursor == GLUT_KEY_LEFT ) {
       giroy += 1;
    }
    if( cursor == GLUT_KEY_RIGHT ) {
        giroy -= 1;
    }
}

int main(int argc, char** argv)
{
	FreeImage_Initialise();
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(600, 600); // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutMouseFunc(onClick);
	glutSpecialFunc(onSpecialKey);
	glutKeyboardFunc(onKey);
	glutMotionFunc(onDrag);
	glutTimerFunc(1000 / tasaFPS, onTimer, tasaFPS); // Alta de la funcion de atencion al timer
	init(); // Inicializacion propia
	//char patata[] = "salida.png";
	//saveScreenshot(patata, 100, 100);
	glutMainLoop(); // Puesta en marcha del programa
	FreeImage_DeInitialise();
}

/*

void test(float xini, float yini, float xfin, float yfin){
	float window_width = glutGet(GLUT_WINDOW_WIDTH);
	float window_height = glutGet(GLUT_WINDOW_HEIGHT);
	float xini_abs = absolute(xini,window_width);
	float yini_abs = absolute(yini,window_height);
	float xfin_abs = absolute(xfin,window_width);
	float yfin_abs = absolute(yfin,window_height);
	GLfloat v0[] = { xini_abs,yini_abs,0 };
	GLfloat v1[] = { xfin_abs,yini_abs,0 };
	GLfloat v2[] = { xfin_abs,yfin_abs,0 };
	GLfloat v3[] = { xini_abs,yfin_abs,0 };
	glBindTexture(GL_TEXTURE_2D, texCar);
	quadtex(v0, v1, v2, v3, 0, 1, 0, 1);
}

*/