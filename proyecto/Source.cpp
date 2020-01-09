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
#define LEJOS 1000

#define PROYECTO 0

using namespace std;

/**********************************

**********************************/


/**********************************
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


void display();

void rotation();
	static float x = 0;
	static float z = 0;
	static float speed = 0;

void material();

void drawFloor();
void drawStreetlights();
void drawBackground();
void drawRoad();
	float A = 4, T = 100; // Amplitud & Periodo
	float anchuraCarretera = 1;
	GLfloat v0[3] = {0,0,0}, v1[3] = {0,0,0}, v2[3] = {0,0,0}, v3[3] = {0,0,0};
	GLfloat xLight, zLight;

void dynamicLights();



int main(int argc, char** argv);
void onTimer(int valor);
void onClick(int boton, int estado, int x, int y);
void onDrag(int x, int y);
void onKey(int key, int x, int y);
void onTimer(int valor);
/**********************************
**********************************/


float fsin(float u) {
	return A * sin((2 * PI * u) / T);
}
float dfsin(float u) {
	return ((2 * PI * A) / T) * cos((2 * PI * u) / T);
}

void rotation(){
	x += sin(giroy * PI / 180) * speed;
	z += cos(giroy * PI / 180) * speed;
	//std::cout << "\n" << x << " " << z;
	gluLookAt(x, 1, z, sin(giroy * PI / 180) + x, 1, cos(giroy * PI / 180) + z, 0, 1, 0);

}

void drawStreetlight(float x, float z, int index){
	GLfloat p02[] = { x, 4, z, 1 };
	glLightfv(streetLights[index], GL_POSITION, p02);
}

void drawBackground(){
	float d = 300;
	float bottomY = -300;
	float topY = 300;
	float sides = 50.0;
	GLfloat smin = 0;
	GLfloat smax = 0;
	float alpha = rad(360 / sides);
	for (int i = 0; i < sides; i++) {
		float x1 = d * cos((i+1) * alpha+ rad(90) )+ x;	// Se mueve con el coche
		float z1 = d * sin((i+1) * alpha + rad(90)) + z;
		float x2 = d * cos((i) * alpha + rad(90)) + x;
		float z2 = d * sin((i) * alpha +rad(90)) + z;
		GLfloat v0[3] = { x1, bottomY, z1 };
		GLfloat v1[3] = { x2, bottomY, z2 };
		GLfloat v2[3] = { x2, topY, z2 };
		GLfloat v3[3] = { x1, topY, z1 };
		GLfloat tmin = 0;
		GLfloat tmax = 1;
		smax += 1 / sides;

		glBindTexture(GL_TEXTURE_2D, texFondo);
		quadtex(v1, v0, v3, v2, smin, smax, tmin, tmax);
		smin += 1 / sides;
	}
	glPopAttrib();
}

void material(){
	/*GLfloat dm[] = { 0.8,0.8,0.8,1.0 };
	GLfloat sm[] = { 0.3,0.3,0.3,1.0 };
	GLfloat s = 3.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dm);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sm);
	glMaterialf(GL_FRONT, GL_SHININESS, s);*/
}




void loadLights(){

	GLfloat a00[] = { 0.3,0.3,0.3 };
	GLfloat d00[] = { 0.1,0.1,0.1};
	GLfloat e00[] = { 0.0,0.0,0.0 };
	//GLfloat p00[] = { 0,10,0,0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, p00);
	glLightfv(GL_LIGHT0, GL_AMBIENT, a00);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, d00);
	glLightfv(GL_LIGHT0, GL_SPECULAR, e00);
	glEnable(GL_LIGHT0);

	//luz faro
	// TODO
	glLightfv(GL_LIGHT1, GL_AMBIENT, BLANCO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, BLANCO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, BLANCO);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0);
	glEnable(GL_LIGHT1);

	//luz FAROLA 1
	GLfloat a02[] = { 1,1,1,1.0 };
	GLfloat d02[] = { 0.5,0.5,0.2,1.0 };
	GLfloat e02[] = { 0.0,0.0,0.0,1.0 };
	GLfloat p02[] = { 0.0,-1.0,0.0 };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION,p02 );
	glLightfv(GL_LIGHT2, GL_AMBIENT, a02);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, d02);
	glLightfv(GL_LIGHT2, GL_SPECULAR, e02);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10);
	glEnable(GL_LIGHT2);

		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION,p02 );
	glLightfv(GL_LIGHT3, GL_AMBIENT, a02);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, d02);
	glLightfv(GL_LIGHT3, GL_SPECULAR, e02);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10);
	glEnable(GL_LIGHT3);

		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION,p02 );
	glLightfv(GL_LIGHT4, GL_AMBIENT, a02);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, d02);
	glLightfv(GL_LIGHT4, GL_SPECULAR, e02);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10);
	glEnable(GL_LIGHT4);

	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION,p02 );
	glLightfv(GL_LIGHT5, GL_AMBIENT, a02);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, d02);
	glLightfv(GL_LIGHT5, GL_SPECULAR, e02);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10);
	glEnable(GL_LIGHT5);



}

void dynamicLights(){
	GLfloat posl1[] = { 0.5, 0.9, -5, 2.0 };
	GLfloat dir_centrall1[] = { 0.0, -1.0, -1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, posl1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_centrall1);
}

void loadTextures(){
    glEnable( GL_TEXTURE_2D );
    glGenTextures( 1, &texFondo );
    glBindTexture( GL_TEXTURE_2D, texFondo );
    loadImageFile("images/fondo.jpeg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glEnable( GL_TEXTURE_2D );
    glGenTextures( 1, &texCarretera );
    glBindTexture( GL_TEXTURE_2D, texCarretera );
    loadImageFile("images/carretera.jpeg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glEnable( GL_TEXTURE_2D );
    glGenTextures( 1, &texSuelo );
    glBindTexture( GL_TEXTURE_2D, texSuelo );
    loadImageFile("images/suelo.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}

void init(){

	loadLights();

	glClearColor(COLOR_FONDO 1.0);
	
	loadTextures();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
}

void drawFloor(){
	glBindTexture(GL_TEXTURE_2D, texSuelo);
	int xflor = int(x) - int(x)%150; 
	int zflor = int(z) - int(z)%150; 
	float v0[] = {xflor-300,-1.3,zflor-300};
	float v1[] = {xflor-300,-1.3,zflor+300};
	float v2[] = {xflor+300,-1.3,zflor+300};
	float v3[] = {xflor+300,-1.3,zflor-300};
	quadtex(v3, v2, v1, v0, 0, 1, 0, 1);
}

void drawStreetlights()
{	int separacion = 30;
	float comienzo_carretera = int(x)-int(x)%separacion, vfsin = fsin(comienzo_carretera);
	float derivada = dfsin(comienzo_carretera);
	
	for (int i = 1; i <= 4; i ++) {
		float aux = comienzo_carretera + i*separacion;
		float vfsin = fsin(aux);
		float derivada = dfsin(aux);
		GLfloat tz[3] = { -derivada,0,1 };
		GLfloat normales2[3] = { (1 / sqrt(1 + derivada * derivada)) * tz[0] , 0 ,(1 / sqrt(1 + derivada * derivada)) * tz[2] };
		
		cout << xLight << " " << zLight << PROYECTO << endl;

		xLight = aux + (normales2[0] * anchuraCarretera);
		zLight = vfsin + (normales2[2] * anchuraCarretera);

		drawStreetlight(xLight,zLight,i+1);
	}
}

void drawRoad()
{
	int len = log10(x);
	float div = pow(10, len);
	int rounded = ceil(x / div) * div;
	float comienzo_carretera = int(x) - 100, vfsin = fsin(comienzo_carretera);
	float derivada = dfsin(comienzo_carretera);
	GLfloat precalculo[3] = { comienzo_carretera,0,vfsin };
	GLfloat tz[3] = { -derivada,0,1 };
	GLfloat normales[3] = { (1 / sqrt(1 + derivada * derivada)) * tz[0] , 0 ,(1 / sqrt(1 + derivada * derivada)) * tz[2] };
	for (int i = 0; i < 3; i++) {
		v0[i] = precalculo[i] - (normales[i] * anchuraCarretera);
		v3[i] = precalculo[i] + (normales[i] * anchuraCarretera);
	}
	for (int i = 1; i <= 200; i++) {
		float aux = comienzo_carretera + i;
		vfsin = fsin(aux);
		float derivada = dfsin(aux);
		GLfloat precalculo2[3] = { aux,0,vfsin };
		GLfloat tz[3] = { -derivada,0,1 };
		GLfloat normales2[3] = { (1 / sqrt(1 + derivada * derivada)) * tz[0] , 0 ,(1 / sqrt(1 + derivada * derivada)) * tz[2] };
		for (int i = 0; i < 3; i++) {
			v1[i] = precalculo2[i] - (normales2[i] * anchuraCarretera);
			v2[i] = precalculo2[i] + (normales2[i] * anchuraCarretera);
		}

		glColor3f(0, 0, 1);
		glBindTexture(GL_TEXTURE_2D, texCarretera);
		quadtex(v3, v0, v1, v2,0,1,0,1, 10, 10);
		for (int i = 0; i < 3; i++) {
			v0[i] = v1[i];
			v3[i] = v2[i];
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Borra la pantalla
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	dynamicLights();

	rotation();	


	material();

	drawBackground();
	drawStreetlights();
	drawFloor();
	drawRoad();

	glTranslatef(10,0.5,2);
	glutSolidCube(0.1);

	//OTRO
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


void onKey(int key, int x, int y)
{
	stringstream titulo;
	titulo << speed * 40;
	glutSetWindowTitle(titulo.str().c_str());

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		giroy -= 1;
		break;
	case GLUT_KEY_LEFT:
		giroy += 1;
		break;

	case GLUT_KEY_UP:
		speed += 0.1/40;
		break;
	case GLUT_KEY_DOWN:
		speed -= 0.1/40;
		if (speed <= 0) {
			speed = 0;
		}
		break;
		exit(0);
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	FreeImage_Initialise();
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(400, 400); // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutMouseFunc(onClick);
	glutSpecialFunc(onKey);
	glutMotionFunc(onDrag);
	glutTimerFunc(1000 / tasaFPS, onTimer, tasaFPS); // Alta de la funcion de atencion al timer
	init(); // Inicializacion propia
	//char patata[] = "salida.png";
	//saveScreenshot(patata, 100, 100);
	glutMainLoop(); // Puesta en marcha del programa
	FreeImage_DeInitialise();
}



	/*
	glPolygonMode(GL_FRONT, GLU_FILL);
	for (int i = 0; i < elements; i++) {
		glPushMatrix();
		glRotatef(alpha*180/PI*i, 0, 1, 0);
		glTranslatef(0, -height / 2, distance);
		GLfloat v0[3] = { -width / 2,0,0 }, v1[3] = { width/2,0,0 }, v2[3] = { width/2,height,0 }, v3[3] = { -width / 2,height,0 };
		quadtex(v3, v2, v1, v0, 10, 5);
		glPopMatrix();
	}
	*/

	//Calcula tiempo
	/*
	
	static int hour = 0;
	static int minute = 0;
	static int second = 0;
	
	[...]

	struct tm newtime;
	time_t now = time(0);
	localtime_r(&newtime, &now);
	hour = newtime.tm_hour;
	minute = newtime.tm_min;
	second = newtime.tm_sec;
	*/
