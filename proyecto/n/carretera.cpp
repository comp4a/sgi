#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <Utilidades.h>

#define NUM_QUADS    25
#define QUAD_WIDTH   10
#define QUAD_LENGTH  5

#define NUM_QUADS_WP 50
#define DISTANCE_WP  100
#define HEIGHT_WP    50

#define NUM_QUADS_FLOOR_LENGTH 3
#define NUM_QUADS_FLOOR_WIDTH  7
#define FLOOR_WIDTH            40
#define FLOOR_LENGTH           50

#define NUM_STREETLIGHTS 4
#define NUM_DECORATION 7


/***************************** FUNCIONES ****************************/
/* Muestra por pantalla la ayuda (botones y sus funciones) del videojuego */
void printUsage();

/* Inicializa constantes requeridas por el programa en la función init() */
void initConstants();

/* Activa la iluminación en la función init() */
void enableIlum();

/* Carga las texturas en la función init() */
void loadTextures();

/* Genera las listas de dibujo necesarias */
void generateLists();

/* Activa las luces del coche (luz de minero, acompaña a la cámara) */
void enableCarLights();

/* Activa las farolas (luz estacionaria) */
void enableStreetLights( float pos );

/* Activa la luz de la luna (luz estacionaria) */
void enableMoonLight();

/* Dibuja la interfaz de usuario */
void drawUI();

/* Dibuja el cilindro de fondo */
void drawBackground();

/* Dibuja el suelo */
void drawFloor( float posX, float posZ );

/* Dibuja la carretera */
void drawRoad( float pos );

/* Dibuja la decoración que envuelve a la carretera */
void drawDecoration( float pos );

/* Dibuja las nubes (elementos translúcidos) */
void drawClouds( float posX, float posZ );

/* Dibuja el sol o la luna dependiendo de si el modo noche
 * está activado o no
 */
void drawCelestialBodies();

/* Actualiza la velocidad y el ángulo de la cámara a cada frame
 * según las teclas pulsadas (en la función update())
 */
void updateSpeedAndAngle();

/* Comprueba si ha cambiado algún modo de los que ofrece el juego
 * y reconfigura la escena si eso ocurre
 */
void updateSceneModes();

/*  Modifica el color de la carretera (visible con el modo arcoiris) */
void modifyColors( float *color, float colorIncrement );

/* Modifica los parámetros de elementos como la cámara o las luces
 * para pasar de primera a tercera persona
 */
void thirdPerson();

/* Modifica los parámetros de elementos como la cámara o las luces
 * para pasar de tercera a primera persona
 */
void firstPerson();

/* Obtiene el punto a representar según la función usada.
 * Se utiliza al dibujar los elementos afectados por esta función.
 */
float function_x( float x );

/* Obtener la derivada del punto a representar según la función usada.
 * Se utiliza al dibujar los elementos afectados por esta función.
 */
float function_x_prime( float x );


/****************************** VENTANA *************************/
/* Ratio de aspecto */
static float aspectRatio;


/***************************** CÁMARA *****************************/
// Posición, punto de interés y vector UP de la cámara
static float EYE_X;
static float EYE_Y;
static float EYE_Z;
static float LOOK_X;
static float LOOK_Y;
static float LOOK_Z;
static float UP_X;
static float UP_Y;
static float UP_Z;
/* Incremento del giro de la cámara en grados */
static float deltaRotateCamera;


/***************************** VELOCIDAD ***************************/
// Magnitud de la velocidad
static float velocity;
// Incremento de la velocidad
static float deltaVelocity;
// Dirección de la velocidad
static float vel[ 2 ];

/*************************** CARRETERA *********************************/
// Vértices de cada quad
static float v0[ 3 ];
static float v1[ 3 ];
static float v2[ 3 ];
static float v3[ 3 ];
// Coordenadas de los vectores que determinan el vector normal de cada punto de la recta
float vectorTangente[ 2 ];
// Módulo del vector que determina la normal de cada punto de la recta
float norm;


/***************************** ILUMINACIÓN ****************************/
// Iluminacion de la luna (iluminación de ambiente)
static float lunaPosicion[ 4 ];
static float lunaAmbiental[ 3 ];
static float lunaDifusa[ 3 ];
static float lunaEspecular[ 3 ];

// Iluminación de los faros
static float farosPosicion[ 4 ];
static float farosDireccion[ 4 ];
static float farosAmbiental[ 3 ];
static float farosDifusa[ 3 ];
static float farosEspecular[ 3 ];
static float farosCutoff;
static float farosExponente;

// 4 farolas focales distribuidas a lo largo del circuito
static float farolaPosicion[ 4 ];
static float farolaDireccion[ 4 ];
static float farolaAmbiental[ 3 ];
static float farolaDifusa[ 3 ];
static float farolaEspecular[ 3 ];
static float farolaCutoff;
static float farolaExponente;

// Carretera
static float carreteraAmbiental[ 3 ];
static float carreteraDifusa[ 3 ];
static float carreteraEspecular[ 3 ];
static float carreteraEmisivo[ 3 ];
static float carreteraExponente;


/************************* TEXTURAS ***************************/
/* Textura del fondo (día) */
static GLuint wpTexture;
/* Textura del fondo (noche) */
static GLuint wpNightTexture;
/* Textura de la carretera */
static GLuint roadTexture;
/* Textura del suelo */
static GLuint floorTexture;
/* Textura del arcoiris (día) */
static GLuint rainbowTexture;
/* Textura del arcoiris (noche) */
static GLuint rainbowNightTexture;
/* Texturas de los carteles */
static GLuint panelTexture;
/* Textura del personaje mirando hacia la izquierda */
static GLuint thirdPersonLeft;
/* Textura del personaje mirando hacia la derecha */
static GLuint thirdPersonRight;
/* Textura correspondiente al botón que ha pulsado
 * el usuario en tercera persona. La textura cambia
 * según si se pulsa la izquierda o la derecha
 */
static GLuint thirdPersonDirection;
/* Textura en primera persona del personaje */
static GLuint firstPersonView;
/* Textura de la brújula */
static GLuint compassTexture;
/* Textura de la luna */
static GLuint moonTexture;


/******************** LISTAS DE DIBUJO ************************/
/* Corola de la flor (todos los pétalos) */
static GLuint corola;
/* Pistilo de la flor (donde está el polen, el centro) */
static GLuint pistilo;
/* Tallo de la flor */
static GLuint tallo;
/* Estrella de David */
static GLuint davidStar;
/* Poste de la farola */
static GLuint streetLight;
/* Nubes */
static GLuint cloud;


/************************ COLORES *****************************/
static float MARRON[ 4 ] = { 0.3, 0, 0.1, 1 };
static float VERDERARO[ 4 ] = { 0.125, 0.75, 0.125, 1 };
static float GRANATE[ 4 ] = { 0.5, 0.125, 0, 1 };
static float PLATEADO[ 4 ] = { 0.8, 0.8, 0.6, 1 };
static float AMARILLO_PALIDO[ 4 ] = { 1, 1, 0.5, 1 };
static float TRANSPARENTE[ 4 ] = { 0, 0, 0, 0 };
static float TRANSLUCIDO[ 4 ] = { 1, 1, 1, 0.4 };
static float TRANSLUCIDO_BRUJULA[ 4 ] = { 1, 1, 1, 0.5 };


/******************** MODOS DEL PROGRAMA **********************/
// Modo alámbrico. Activado = true. Desactivado = false
static bool alambricMode;
// Modo noche. Activado = true. Desactivado = false
static bool nightMode;
// Modo niebla. Activado = true. Desactivado = false
static bool fogMode;
// Modo UI. Activado = true. Desactivado = false
static bool uiMode;
// Modo arcoiris. Activado = true. Desactivado = false
static bool rainbowMode;
// Modo de la cámara. Tercera persona o primera persona.
static bool thirdPersonMode;
// Modo de faros. Largos o cortos.
static bool farosLargos;



/****************************** OTROS *****************************/

/* Array de cursores pulsados.
 * 0 = UP
 * 1 = DOWN
 * 2 = LEFT
 * 3 = RIGHT
 */
static bool keyArray[ 4 ];
/* Color de cada quad */
static float color[ 4 ];
/* Color con el que se empieza a dibujar la carretera */
static float colorStart[ 3 ];
/* Incremento del color por cada quad pasado */
static float deltaColor;
/* Vueltas por segundo que da el fondo en el modo arcoiris */
static float omega;
/* "Ángulo" en el que el fondo gira en el modo arcoiris,
 * calculado en base al tiempo transcurrido
 */
float rainbowAngle;
/* Distancia a la que se tiene que dibujar cada elemento decorativo */
static float deltaDecoration;
/* Distancia pequeña que tiene que recorrer el coche para que,
 * cuando se recoloque la decoración, no se vea que la decoración
 * que acaba de pasar ha desaparecido
 */
static float umbralDecoration;
/* Distancia a la que se dibuja la decoración a partir del borde
 * de la carretera (para que quede más alejada de la carretera)
 */
static float umbralDecoCarretera;
/* Distancia a la que se dibuja cada farola con respecto a la siguiente */
static float deltaFarola;
/* Distancia pequeña que tiene que recorrer el coche para que,
 * cuando se recoloquen las farolas, no se vea que la farola
 * que acaba de pasar ha desaparecido
 */
static float umbralFarola;
/* Distancia a la que se dibujan las farolas a partir del borde
 * de la carretera (para que queden más alejadas de la carretera)
 */
static float umbralFarolasCarretera;


float function_x( float x ) {
    return 8 * sin( x / 32.0 );
    // return 0;
}

float function_x_prime( float x ) {
    // El coseno es la derivada del seno
    return cos( x / 32.0 ) / 4.0;
    // return 0;
}

void init() {
    // Activar la visibilidad
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glClearColor( 1, 1, 1, 1 );

    loadTextures();
    initConstants();
    enableIlum();

    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    generateLists();
}

void initConstants() {
    // Inicializar velocidad, dirección de velocidad y cámara
    velocity = 0;
    vel[ 0 ] = 1;
    vel[ 1 ] = 0;
    EYE_X  = 0;
    EYE_Y  = 1;
    EYE_Z  = 0;
    LOOK_X = 1;
    LOOK_Y = 1;
    LOOK_Z = 0;
    UP_X   = 0;
    UP_Y   = 1;
    UP_Z   = 0;

    keyArray[ 0 ] = false;
    keyArray[ 1 ] = false;
    keyArray[ 2 ] = false;
    keyArray[ 3 ] = false;

    deltaVelocity = 0.1;

    // La rotación de la cámara se define como un cuarto de grado
    deltaRotateCamera = 0.25;

    // La cámara empieza estando en primera persona.
    thirdPersonMode = false;

    // La carretera empieza en el color rojo
    colorStart[ 0 ] = 1;
    colorStart[ 1 ] = 0;
    colorStart[ 2 ] = 0;

    // Debido al añadir el canal alfa, el color tiene que tener opacidad 1.
    // La buena hora y media encontrando este error XD
    color[ 3 ] = 1;

    // Inicializar incremento de color por cada quad dibujado
    deltaColor = 0.05;

    // En el modo arcoiris se da una vuelta al fondo cada dos segundos
    omega = 0.5;

    // La textura de inicio de la tercera persona es Lakitu hacia la izquierda
    thirdPersonDirection = thirdPersonLeft;

    deltaFarola = 25;
    umbralFarola = 3.5;
    deltaDecoration = 15;
    umbralDecoration = 5;
    umbralFarolasCarretera = 0.5;
    umbralDecoCarretera = 2;

    // Inicializar niebla (densidad)
    glFogf( GL_FOG_DENSITY, 0.035 );
    glFogi( GL_FOG_MODE, GL_EXP2 );

    // Inicializar colores del programa
    alambricMode = false;
    nightMode = false;
    fogMode = false;
    uiMode = true;
    rainbowMode = false;
    farosLargos = false;
}

void enableIlum() {
    // Activar la iluminación
    glEnable( GL_LIGHTING );
    glEnable( GL_NORMALIZE );   // Todos los vectores serán de módulo 1

    // Iluminación de la luna
    lunaPosicion[ 0 ] = 0;
    lunaPosicion[ 1 ] = 10;
    lunaPosicion[ 2 ] = 0;
    lunaPosicion[ 3 ] = 0;
    lunaAmbiental[ 0 ] = 0.02;
    lunaAmbiental[ 1 ] = 0.02;
    lunaAmbiental[ 2 ] = 0.02;
    lunaDifusa[ 0 ] = 0.05;
    lunaDifusa[ 1 ] = 0.05;
    lunaDifusa[ 2 ] = 0.05;
    lunaEspecular[ 0 ] = 0;
    lunaEspecular[ 1 ] = 0;
    lunaEspecular[ 2 ] = 0;

    // Iluminación de los faros
    farosPosicion[ 0 ] = 0;
    farosPosicion[ 1 ] = EYE_Y - 0.7;
    farosPosicion[ 2 ] = 0;
    farosPosicion[ 3 ] = 1;
    farosDireccion[ 0 ] = 0;
    farosDireccion[ 1 ] = -0.083;
    farosDireccion[ 2 ] = -0.2;
    farosDireccion[ 3 ] = 0;
    farosAmbiental[ 0 ] = 0.2;
    farosAmbiental[ 1 ] = 0.2;
    farosAmbiental[ 2 ] = 0.2;
    farosDifusa[ 0 ] = 1;
    farosDifusa[ 1 ] = 1;
    farosDifusa[ 2 ] = 1;
    farosEspecular[ 0 ] = 0.3;
    farosEspecular[ 1 ] = 0.3;
    farosEspecular[ 2 ] = 0.3;
    farosCutoff = 20;       // Grados
    farosExponente = 2;    // Exponente

    // Farolas focales distribuidas a lo largo del circuito
    farolaDireccion[ 0 ] = 0;
    farolaDireccion[ 1 ] = -1;
    farolaDireccion[ 2 ] = -0.4;
    farolaDireccion[ 3 ] = 0;
    // farolaPosicion[ 0 ] y farolaPosicion[ 2 ] se asignan
    // dinámicamente en la función enableStreetLights()
    farolaPosicion[ 1 ] = 4;
    farolaPosicion[ 3 ] = 1;
    farolaAmbiental[ 0 ] = 0;
    farolaAmbiental[ 1 ] = 0;
    farolaAmbiental[ 2 ] = 0;
    farolaDifusa[ 0 ] = 0.7;
    farolaDifusa[ 1 ] = 0.7;
    farolaDifusa[ 2 ] = 0.5;
    farolaEspecular[ 0 ] = 0;
    farolaEspecular[ 1 ] = 0;
    farolaEspecular[ 2 ] = 0;
    farolaCutoff = 60;
    farolaExponente = 10;

    // Configurar luz de la luna
    glEnable( GL_LIGHT0 );
    glLightfv( GL_LIGHT0, GL_AMBIENT, lunaAmbiental );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lunaDifusa );
    glLightfv( GL_LIGHT0, GL_SPECULAR, lunaEspecular );
    
    // Configurar luz de los faros
    glEnable( GL_LIGHT1 );
    glLightfv( GL_LIGHT1, GL_AMBIENT, farosAmbiental );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, farosDifusa );
    glLightfv( GL_LIGHT1, GL_SPECULAR, farosEspecular );
    glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, farosCutoff );
    glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, farosExponente );
    glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, farosDireccion );

    // Configurar luz de las farolas
    for( int i = 0; i < NUM_STREETLIGHTS; i++ ) {
        glEnable( GL_LIGHT2 + i );
        glLightfv( GL_LIGHT2 + i, GL_AMBIENT, farolaAmbiental );
        glLightfv( GL_LIGHT2 + i, GL_DIFFUSE, farolaDifusa );
        glLightfv( GL_LIGHT2 + i, GL_SPECULAR, farolaEspecular );
        glLightf( GL_LIGHT2 + i, GL_SPOT_CUTOFF, farolaCutoff );
        glLightf( GL_LIGHT2 + i, GL_SPOT_EXPONENT, farolaExponente );
        glLightfv( GL_LIGHT2 + i, GL_SPOT_DIRECTION, farolaDireccion );
    }
}

void loadTextures() {
    // Activar las texturas
    glEnable( GL_TEXTURE_2D );
    glGenTextures( 1, &roadTexture );
    glBindTexture( GL_TEXTURE_2D, roadTexture );
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    loadImageFile( ( char * ) "road.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &wpTexture );
    glBindTexture( GL_TEXTURE_2D, wpTexture );
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    // loadImageFile( ( char * ) "fondopanorama.png" );
    loadImageFile( ( char * ) "wp.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &wpNightTexture );
    glBindTexture( GL_TEXTURE_2D, wpNightTexture );
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    loadImageFile( ( char * ) "wpnight.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &floorTexture );
    glBindTexture( GL_TEXTURE_2D, floorTexture );
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    loadImageFile( ( char * ) "grass.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &rainbowTexture );
    glBindTexture( GL_TEXTURE_2D, rainbowTexture );
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    loadImageFile( ( char * ) "rainbow.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &rainbowNightTexture );
    glBindTexture( GL_TEXTURE_2D, rainbowNightTexture );
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    loadImageFile( ( char * ) "rainbownight.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &panelTexture );
    glBindTexture( GL_TEXTURE_2D, panelTexture );
    loadImageFile( ( char * ) "panel.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &thirdPersonLeft );
    glBindTexture( GL_TEXTURE_2D, thirdPersonLeft );
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    // Cambiar para windows el path de carga de las imágenes
    loadImageFile( ( char * ) "lakituL.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &thirdPersonRight );
    glBindTexture( GL_TEXTURE_2D, thirdPersonRight );
    loadImageFile( ( char * ) "lakituR.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &firstPersonView );
    glBindTexture( GL_TEXTURE_2D, firstPersonView );
    loadImageFile( ( char * ) "glasses.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &compassTexture );
    glBindTexture( GL_TEXTURE_2D, compassTexture );
    loadImageFile( ( char * ) "compass.png" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glGenTextures( 1, &moonTexture );
    glBindTexture( GL_TEXTURE_2D, moonTexture );
    loadImageFile( ( char * ) "white.jpg" );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}

void generateLists() {
    corola = glGenLists( 1 );
    glNewList( corola, GL_COMPILE );
        // Corola (todos los pétalos)
        for( int i = 0; i < 6; i++ ) {
            glPushMatrix();
                glRotatef( i * 360.0 / 6.0, 0, 0, 1 );
                glScalef( 0.2, 0.5, 0.2 );
                glutSolidSphere( 1, 20, 20 );
            glPopMatrix();
        }
    glEndList();
    
    pistilo = glGenLists( 1 );
    glNewList( pistilo, GL_COMPILE );
        // Pistilo (donde está el polen)
        glPushMatrix();
            glutSolidCylinder( 0.2, 0.2, 20, 20 );
        glPopMatrix();
    glEndList();

    tallo = glGenLists( 1 );
    glNewList( tallo, GL_COMPILE );
        // Tallo
        glPushMatrix();
            glRotatef( 90, 1, 0, 0 );
            glutSolidCylinder( 0.05, 1.5, 10, 10 );
        glPopMatrix();
    glEndList();

    davidStar = glGenLists( 1 );
    glNewList( davidStar, GL_COMPILE );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glBegin( GL_TRIANGLE_STRIP );
            int numVertices = 3;
            float alpha0 = M_PI_2;
            float alpha0inv = 3 * M_PI_2;
            float alphaIncr = 2 * M_PI / float( numVertices );
            for( int i = 0; i < numVertices; i++ ) {
                glVertex3d(
                    0.7 * cos( i * alphaIncr + alpha0 ),
                    0.7 * sin( i * alphaIncr + alpha0 ),
                    0 );
                glVertex3d(
                    cos( i * alphaIncr + alpha0 ),
                    sin( i * alphaIncr + alpha0 ),
                    0 );
            }
            glVertex3d(
                0.7 * cos( alpha0 ),
                0.7 * sin( alpha0 ),
                0 );
            glVertex3d(
                cos( alpha0 ),
                sin( alpha0 ),
                0 );
            
            for( int i = 0; i < numVertices; i++ ) {
                glVertex3d(
                    0.7 * cos( i * alphaIncr + alpha0inv ),
                    0.7 * sin( i * alphaIncr + alpha0inv ),
                    0 );
                glVertex3d(
                    cos( i * alphaIncr + alpha0inv ),
                    sin( i * alphaIncr + alpha0inv ),
                    0 );
            }
            glVertex3d(
                0.7 * cos( alpha0inv ),
                0.7 * sin( alpha0inv ),
                0 );
            glVertex3d(
                cos( alpha0inv ),
                sin( alpha0inv ),
                0 );
        glEnd();
    glEndList();

    streetLight = glGenLists( 1 );
    glNewList( streetLight, GL_COMPILE );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glPushMatrix();
            glRotatef( -90, 1, 0, 0 );
            glutSolidCylinder( 0.125, 5, 10, 10 );
        glPopMatrix();
        glPushMatrix();
            glTranslatef( 0, 5, 0 );
            glRotatef( -170, 1, 0, 0 );
            glutSolidCylinder( 0.125, 1, 10, 10 );
        glPopMatrix();
    glEndList();

    cloud = glGenLists( 1 );
    glNewList( cloud, GL_COMPILE );
        glPushMatrix();
            glTranslatef( 5, 1, 0 );
            glScalef( 2, 2, 2 );
            glPushMatrix();
                glScalef( 1, 0.4, 1 );
                glutSolidSphere( 1, 20, 20 );
            glPopMatrix();
            glPushMatrix();
                glTranslatef( 0, -0.4, 1.25 );
                glScalef( 1, 0.3, 1 );
                glutSolidSphere( 1, 20, 20 );
            glPopMatrix();
        glPopMatrix();
    glEndList();
}

void modifyColors( float *color, float colorIncrement ) {
    if( color[ 2 ] > 0 && color[ 0 ] == 1 ) {           // Decrementar color inicial
        color[ 2 ] -= colorIncrement;
    } else if( color[ 0 ] == 1 && color[ 1 ] < 1 ) {    // 1
        color[ 1 ] += colorIncrement;
    } else if( color[ 0 ] > 0 && color[ 1 ] == 1 ) {    // 2
        color[ 0 ] -= colorIncrement;
    } else if( color[ 1 ] == 1 && color[ 2 ] < 1 ) {    // 3
        color[ 2 ] += colorIncrement;
    } else if( color[ 1 ] > 0 && color[ 2 ] == 1 ) {    // 4
        color[ 1 ] -= colorIncrement;
    } else if( color[ 2 ] == 1 && color[ 0 ] < 1 ) {    // 5
        color[ 0 ] += colorIncrement;
    }
    
    // Comprobar que ningún color se pasa del rango [ 0, 1 ]
    if( color[ 0 ] < 0 ) {
        color[ 0 ] = 0;
    } else if( color[ 0 ] > 1 ) {
        color[ 0 ] = 1;
    }
    if( color[ 1 ] < 0 ) {
        color[ 1 ] = 0;
    } else if( color[ 1 ] > 1 ) {
        color[ 1 ] = 1;
    }
    if( color[ 2 ] < 0 ) {
        color[ 2 ] = 0;
    } else if( color[ 2 ] > 1 ) {
        color[ 2 ] = 1;
    }
}

void enableCarLights() {
    glLightfv( GL_LIGHT1, GL_POSITION, farosPosicion );
    glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, farosDireccion );
}

void enableStreetLights( float pos ) {
    int l = 0;
        //glBindTexture( GL_TEXTURE_2D, 0 );
        for( float i = pos + deltaFarola;
            i < pos + (NUM_STREETLIGHTS + 1) * deltaFarola;
            i += deltaFarola ) {
                farolaPosicion[ 0 ] = i;
                farolaPosicion[ 2 ] = function_x( i ) + 1;
                // Situar la luz de la farola
                glLightfv( GL_LIGHT2 + l++, GL_POSITION, farolaPosicion );

                // Añadir la decoración (poste de la farola)
                glPushAttrib( GL_ALL_ATTRIB_BITS );
                    glBindTexture( GL_TEXTURE_2D, 0 );
                    if( rainbowMode ) {
                        if( nightMode ) {
                            glMaterialfv( GL_FRONT, GL_DIFFUSE, color );
                            glMaterialfv( GL_FRONT, GL_SPECULAR, color );
                            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                        }
                        else {
                            glColor3fv( color );
                        }
                    }
                    else if( nightMode ) {
                        glMaterialfv( GL_FRONT, GL_DIFFUSE, GRANATE );
                        glMaterialfv( GL_FRONT, GL_SPECULAR, GRANATE );
                        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    }
                    else {
                        glColor3fv( GRANATE );
                    }
                    static float v0[ 3 ] = { -0.125, 1.2, -1 };
                    static float v1[ 3 ] = { -0.125, 2.8, -1 };
                    static float v2[ 3 ] = { -0.125, 2.8, 1 };
                    static float v3[ 3 ] = { -0.125, 1.2, 1 };
                    
                    glPushMatrix();
                        glTranslatef( i, 0, function_x( i ) + QUAD_WIDTH / 2.0 + umbralFarolasCarretera );
                        glPushMatrix();
                            glCallList( streetLight );
                        glPopMatrix();
                        if( rainbowMode && alambricMode ) {
                            glColor3fv( color );
                        }
                        else if( alambricMode ) {
                            glColor3fv( GRANATE );
                        }
                        else if( nightMode ) {
                            glMaterialfv( GL_FRONT, GL_DIFFUSE, BLANCO );
                            glMaterialfv( GL_FRONT, GL_SPECULAR, BLANCO );
                            glMaterialf( GL_FRONT, GL_SHININESS, 200 );
                        }
                        else {
                            glColor3fv( BLANCO );
                        }
            
                        glBindTexture( GL_TEXTURE_2D, panelTexture );
                        glPushMatrix();
                            quadtex( v0, v3, v2, v1 );
                        glPopMatrix();
                    glPopMatrix();
                glPopAttrib();
        }
}

void enableMoonLight() {
    glLightfv( GL_LIGHT0, GL_POSITION, lunaPosicion );
}

void drawBackground() {
    GLUquadric *quad = gluNewQuadric();

    glPushAttrib( GL_ALL_ATTRIB_BITS );
        if( alambricMode ) {
            if( nightMode ) {
                glColor3fv( MARINO );
            }
            else if( rainbowMode ) {
                glColor3fv( color );
            }
            else {
                glColor3fv( AZUL );
            }
        }
        else {
            if( rainbowMode ) {
                if( nightMode ) {
                    glBindTexture( GL_TEXTURE_2D, rainbowNightTexture );
                }
                else {
                    glBindTexture( GL_TEXTURE_2D, rainbowTexture );
                }
            }
            else {
                if( nightMode ) {
                    glBindTexture( GL_TEXTURE_2D, wpNightTexture );
                }
                else {
                    glBindTexture( GL_TEXTURE_2D, wpTexture );
                }
            }
            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
        }
        
        gluQuadricTexture( quad, true );
        // Actualizar giro
        rainbowAngle = glutGet( GLUT_ELAPSED_TIME ) / 20.0;
        glPushMatrix();
            glTranslatef( EYE_X, EYE_Y - 5, EYE_Z );
            if( rainbowMode && !alambricMode ) {
                glRotatef( rainbowAngle, 0, 1, 0 );
            }
            glRotatef( -90, 1, 0, 0 );
            gluCylinder( quad, DISTANCE_WP, DISTANCE_WP, HEIGHT_WP, 20, 10 );
        glPopMatrix();
    glPopAttrib();
}

void drawFloor( float posX, float posZ ) {
    glPushAttrib( GL_ALL_ATTRIB_BITS );
        if( rainbowMode ) {
            glBindTexture( GL_TEXTURE_2D, rainbowTexture );
            glMaterialfv( GL_FRONT, GL_DIFFUSE, color );
            glMaterialfv( GL_FRONT, GL_SPECULAR, color );
        }
        else if( !alambricMode ) {
            glBindTexture( GL_TEXTURE_2D, floorTexture );
        }
        else {
            if( nightMode ) {
                glMaterialfv( GL_FRONT, GL_DIFFUSE, VERDERARO );
                glMaterialfv( GL_FRONT, GL_SPECULAR, VERDERARO );
            }
            else {
                glColor3fv( VERDE );
            }
        }
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

        float translate = ( rainbowMode ) ? -0.5 : -0.01;
        glPushMatrix();
            glTranslatef( 0, translate, 0 );
            for( float i = posX - FLOOR_LENGTH;
                i < posX + NUM_QUADS_FLOOR_LENGTH * FLOOR_LENGTH;
                i += FLOOR_LENGTH ) {
                    for( float j = posZ - FLOOR_WIDTH * NUM_QUADS_FLOOR_WIDTH / 2.0;
                        j <= posZ + FLOOR_WIDTH * NUM_QUADS_FLOOR_WIDTH / 2.0;
                        j += FLOOR_WIDTH ) {
                            float v0[ 3 ] = { i, 0, j };
                            float v1[ 3 ] = { i, 0, j + FLOOR_WIDTH };
                            float v2[ 3 ] = { i + FLOOR_LENGTH, 0, j + FLOOR_WIDTH };
                            float v3[ 3 ] = { i + FLOOR_LENGTH, 0, j };
                            quadtex( v2, v3, v0, v1, 0, 1, 0, 1, 40, 40 );
                }
            }
        glPopMatrix();
    glPopAttrib();
}

void drawRoad( float posX ) {
    glPushAttrib( GL_ALL_ATTRIB_BITS );
        // glBindTexture( GL_TEXTURE_2D, 0 );
        
        if( rainbowMode ) {
            if( nightMode ) {
                glMaterialfv( GL_FRONT, GL_DIFFUSE, color );
                glMaterialfv( GL_FRONT, GL_SPECULAR, color );
                glMaterialf( GL_FRONT, GL_SHININESS, carreteraExponente );
                glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
            }
            else {
                glColor3fv( color );
            }
        }
        else if( !alambricMode ) {
            glMaterialfv( GL_FRONT, GL_DIFFUSE, BLANCO );
            glMaterialfv( GL_FRONT, GL_SPECULAR, BLANCO );
            glMaterialf( GL_FRONT, GL_SHININESS, carreteraExponente );
            glBindTexture( GL_TEXTURE_2D, roadTexture );
            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        }
        else {
            glColor3fv( GRISOSCURO );
        }
        // Vértices de cada quad de la carretera
        static float v0[ 3 ], v1[ 3 ], v2[ 3 ], v3[ 3 ];
        // Coordenadas de la X y la Z del vector tangente a la carretera
        static float vectorTangente[ 2 ];
        // Módulo del vector tangente (para sacar el vector perpendicular)
        static float norm;

        // Se empieza a dibujar 8 quads antes por si al usuario se le ocurre
        // dar un rodeo por el terreno para ver la carretera de lado.
        // 8 es arbitrario; queda bien así (da sensación de continuidad si pasa eso).
        for( float i = posX - QUAD_LENGTH * 8; i < posX + NUM_QUADS * QUAD_LENGTH; i += QUAD_LENGTH ) {
            if( rainbowMode ) {
                glColor3fv( color );
            }

            v0[ 0 ] = i;
            v1[ 0 ] = i + QUAD_LENGTH;
            v2[ 0 ] = v1[ 0 ];
            v3[ 0 ] = v0[ 0 ];

            v0[ 2 ] = function_x( i );
            v1[ 2 ] = function_x( i + QUAD_LENGTH );
            v2[ 2 ] = v1[ 2 ];
            v3[ 2 ] = v0[ 2 ];

            // Trabajar con los vértices más próximos a la cámara (v0, v3)
            // Sacar el vector tangente
            vectorTangente[ 0 ] = 1;
            vectorTangente[ 1 ] = function_x_prime( i );
            
            norm = sqrt( 1 + vectorTangente[ 1 ] * vectorTangente[ 1 ] );
            // Sacar el vector perpendicular (normal)
            vectorTangente[ 0 ] = -vectorTangente[ 1 ] / norm;
            vectorTangente[ 1 ] = 1 / norm;
            
            v0[ 0 ] = v0[ 0 ] - vectorTangente[ 0 ] * QUAD_WIDTH / 2.0;
            v3[ 0 ] = v3[ 0 ] + vectorTangente[ 0 ] * QUAD_WIDTH / 2.0;
            v0[ 2 ] = v0[ 2 ] - vectorTangente[ 1 ] * QUAD_WIDTH / 2.0;
            v3[ 2 ] = v3[ 2 ] + vectorTangente[ 1 ] * QUAD_WIDTH / 2.0;

            // Trabajar con los vértices más lejanos a la cámara (v1, v2)
            // Sacar el vector tangente
            vectorTangente[ 0 ] = 1;
            vectorTangente[ 1 ] = function_x_prime( i + QUAD_LENGTH );
            norm = sqrt( 1 + vectorTangente[ 1 ] * vectorTangente[ 1 ] );
            // Sacar el vector perpendicular (normal)
            vectorTangente[ 0 ] = -vectorTangente[ 1 ] / norm;
            vectorTangente[ 1 ] = 1 / norm;
            v1[ 0 ] = v1[ 0 ] - vectorTangente[ 0 ] * QUAD_WIDTH / 2.0;
            v2[ 0 ] = v2[ 0 ] + vectorTangente[ 0 ] * QUAD_WIDTH / 2.0;
            v1[ 2 ] = v1[ 2 ] - vectorTangente[ 1 ] * QUAD_WIDTH / 2.0;
            v2[ 2 ] = v2[ 2 ] + vectorTangente[ 1 ] * QUAD_WIDTH / 2.0;

            quadtex( v0, v3, v2, v1, 0, 1, 0, 1, 20, 10 );
            
            if( rainbowMode ) {
                modifyColors( color, deltaColor );
            }
        }
    glPopAttrib();
}

void drawDecoration( float posX ) {
    int position = 0;
    if( rainbowMode ) {
        glPushAttrib( GL_ALL_ATTRIB_BITS );
            for( float i = posX + deltaDecoration;
            i < posX + deltaDecoration * (1 + NUM_DECORATION);
            i += deltaDecoration ) {
                glColor3fv( color );
                if( rainbowMode ) {
                    glMaterialfv( GL_FRONT, GL_DIFFUSE, color );
                    glMaterialfv( GL_FRONT, GL_SPECULAR, color );
                    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                }
                glPushMatrix();
                    glTranslatef( i, 1, function_x( i ) - QUAD_WIDTH / 2.0 - umbralDecoCarretera );
                    glRotatef( rainbowAngle, 0, 1, 0 );
                    glCallList( davidStar );
                glPopMatrix();
            }
        glPopAttrib();
    }
    else {
        glPushAttrib( GL_ALL_ATTRIB_BITS );
            // glBindTexture( GL_TEXTURE_2D, 0 );
            for( float i = posX + deltaDecoration;
            i < posX + deltaDecoration * (1 + NUM_DECORATION);
            i += deltaDecoration ) {
                glPushMatrix();
                    glTranslatef( i, 1, function_x( i ) - QUAD_WIDTH / 2.0 - umbralDecoCarretera );
                    glRotatef( -90, 0, 1, 0 );
                    if( nightMode ) {
                        glMaterialfv( GL_FRONT, GL_DIFFUSE, AMARILLO );
                        glMaterialfv( GL_FRONT, GL_SPECULAR, AMARILLO );
                    }
                    else {
                        glColor3fv( AMARILLO );
                    }
                    glCallList( corola );
                    
                    if( nightMode ) {
                        glMaterialfv( GL_FRONT, GL_DIFFUSE, MARRON );
                        glMaterialfv( GL_FRONT, GL_SPECULAR, MARRON );
                    }
                    else {
                        glColor3fv( MARRON );
                    }
                    glCallList( pistilo );

                    if( nightMode ) {
                        glMaterialfv( GL_FRONT, GL_DIFFUSE, VERDE );
                        glMaterialfv( GL_FRONT, GL_SPECULAR, VERDE );
                    }
                    else {
                        glColor3fv( VERDE );
                    }
                    glCallList( tallo );
                glPopMatrix();
            }
        glPopAttrib();
    }
}

void drawClouds( float posX, float posZ ) {
    glPushAttrib( GL_ALL_ATTRIB_BITS );
        glDepthMask( GL_FALSE );
        if( nightMode ) {
            glMaterialfv( GL_FRONT, GL_DIFFUSE, TRANSLUCIDO );
            glMaterialfv( GL_FRONT, GL_SPECULAR, TRANSLUCIDO );
            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        }
        else {
            glColor4fv( TRANSLUCIDO );
        }
        for( float i = posX - FLOOR_LENGTH;
            i < posX + NUM_QUADS_FLOOR_LENGTH * FLOOR_LENGTH;
            i += FLOOR_LENGTH ) {
                for( float j = posZ - FLOOR_WIDTH * NUM_QUADS_FLOOR_WIDTH / 2.0;
                    j <= posZ + FLOOR_WIDTH * NUM_QUADS_FLOOR_WIDTH / 2.0;
                    j += FLOOR_WIDTH ) {
                        glPushMatrix();
                            glTranslatef( i, 10, j );
                            glCallList( cloud );
                        glPopMatrix();
                }
        }
        glDepthMask( GL_TRUE );
    glPopAttrib();
}

void drawCelestialBodies() {
    glPushAttrib( GL_ALL_ATTRIB_BITS );
        glPushMatrix();
            if( nightMode ) {
                glPushMatrix();
                    glMaterialfv( GL_FRONT, GL_DIFFUSE, TRANSPARENTE );
                    glMaterialfv( GL_FRONT, GL_SPECULAR, TRANSPARENTE );
                    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
                    glTranslatef( EYE_X + 79.5, 10, EYE_Z - 14.4 );
                    glutSolidSphere( 0.75, 20, 20 );
                glPopMatrix();
            }

            glBindTexture( GL_TEXTURE_2D, moonTexture );
            if( nightMode ) {
                glTranslatef( EYE_X + 80, 10, EYE_Z - 15 );
                glMaterialfv( GL_FRONT, GL_DIFFUSE, PLATEADO );
                glMaterialfv( GL_FRONT, GL_SPECULAR, PLATEADO );
                glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
            }
            else {
                glTranslatef( EYE_X + 80, 10, EYE_Z + 15 );
                glColor3fv( AMARILLO_PALIDO );
            }
            glutSolidSphere( 1, 10, 10 );
        glPopMatrix();
    glPopAttrib();
}

void drawUI() {
    glPushMatrix();
        glLoadIdentity();
        glMatrixMode( GL_PROJECTION );
        glPushMatrix();
            glLoadIdentity();
            glOrtho( -1, 1, -1, 1, -1, 1 );
            glMatrixMode( GL_MODELVIEW );
            gluLookAt( 0, 0, 0, 0, 0, -1, 0, 1, 0 );
            if( !thirdPersonMode ) {
                glBindTexture( GL_TEXTURE_2D, firstPersonView );
                glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                float v0[ 3 ] = { -0.95, -0.8, 0 };
                float v1[ 3 ] = { 0.95, -0.8, 0 };
                float v2[ 3 ] = { 0.95, 0.8, 0 };
                float v3[ 3 ] = { -0.95, 0.8, 0 };
                glPushMatrix();
                    // Escalar según el ratio de aspecto para evitar
                    // que las texturas se vean más alargadas o altas.
                    if( aspectRatio < 1 ) {
                        glScalef( 1, aspectRatio, 1 );
                    }
                    else {
                        glScalef( 1 / aspectRatio, 1, 1 );
                    }
                    quadtex( v0, v1, v2, v3 );
                glPopMatrix();
            }
            else {
                glBindTexture( GL_TEXTURE_2D, thirdPersonDirection );
                glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                float v0[ 3 ] = { -0.2, -0.9, 0 };
                float v1[ 3 ] = { 0.2, -0.9, 0 };
                float v2[ 3 ] = { 0.2, -0.3, 0 };
                float v3[ 3 ] = { -0.2, -0.3, 0 };
                glPushMatrix();
                    if( aspectRatio < 1 ) {
                        glScalef( 1, aspectRatio, 1 );
                    }
                    else {
                        glScalef( 1 / aspectRatio, 1, 1 );
                    }
                    quadtex( v0, v1, v2, v3 );
                glPopMatrix();
            }
            glBindTexture( GL_TEXTURE_2D, compassTexture );
            
            glPushAttrib( GL_ALL_ATTRIB_BITS );
                if( nightMode ) {
                    glMaterialfv( GL_FRONT, GL_DIFFUSE, TRANSLUCIDO_BRUJULA );
                    glMaterialfv( GL_FRONT, GL_SPECULAR, TRANSLUCIDO_BRUJULA );
                    // GL_ADD pone la transparencia del material pero
                    // a la vez no hace que la brújula se oscurezca por
                    // la noche, que es lo que se pretende conseguir.
                    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                }
                else {
                    glColor4fv( TRANSLUCIDO_BRUJULA );
                }
                float v0[ 3 ] = { -0.145, -0.15, 0 };
                float v1[ 3 ] = { 0.145, -0.15, 0 };
                float v2[ 3 ] = { 0.145, 0.15, 0 };
                float v3[ 3 ] = { -0.145, 0.15, 0 };
                glPushMatrix();
                        glTranslatef( 0, 0.8, 0.1 );
                        if( aspectRatio < 1 ) {
                            glScalef( 1, aspectRatio, 1 );
                        }
                        else {
                            glScalef( 1 / aspectRatio, 1, 1 );
                        }
                        glRotatef( deg( asin( vel[ 1 ] ) ), 0, 0, 1 );
                        quadtex( v0, v1, v2, v3 );
                    glPopMatrix();
            glPopAttrib();
            glMatrixMode( GL_PROJECTION );
        glPopMatrix();
        glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glBindTexture( GL_TEXTURE_2D, 0 );
}

void thirdPerson() {
    EYE_X -= 1;
    EYE_Y += 1.5;
    LOOK_Y += 1.3;
    farosPosicion[ 1 ] += 1.75;
}

void firstPerson() {
    EYE_X += 1;
    EYE_Y -= 1.5;
    LOOK_Y -= 1.3;
    farosPosicion[ 1 ] -= 1.75;
}

void display() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Las luces aquí son dependientes de la posición de la cámara
    enableCarLights();

    // Situar la cámara
    gluLookAt(
                EYE_X, EYE_Y, EYE_Z,
                LOOK_X, LOOK_Y, LOOK_Z,
                UP_X, UP_Y, UP_Z
             );
    
    float currentPosX = EYE_X;
    // Modificar posición de inicio de la carretera
    static float previousPosRoad = EYE_X;
    if( currentPosX > QUAD_LENGTH + previousPosRoad ) {
        previousPosRoad = floor( currentPosX );
        if( rainbowMode ) {
            modifyColors( colorStart, deltaColor );
        }
    }
    // Modificar posición de inicio del suelo
    static float previousPosFloorX = EYE_X;
    static float previousPosFloorZ = EYE_Z;
    float currentPosZ = EYE_Z;
    // Comprobar si la cámara ha cambiado mucho hacia adelante (no se puede ir hacia atrás)
    if( currentPosX > FLOOR_LENGTH + previousPosFloorX ) {
        previousPosFloorX = floor( currentPosX );
    }
    // Comprobar si la cámara ha cambiado mucho hacia los lados
    if( currentPosZ > previousPosFloorZ + FLOOR_WIDTH || currentPosZ < previousPosFloorZ - FLOOR_WIDTH ) {
        previousPosFloorZ = floor( currentPosZ );
    }
    // Modificar posición de las farolas
    static float previousPosFarola = EYE_X;
    if( currentPosX > deltaFarola + previousPosFarola + umbralFarola ) {
        previousPosFarola = floor( currentPosX - umbralFarola );
    }
    // Modificar posición de la decoración
    static float previousPosDeco = EYE_X;
    if( currentPosX > previousPosDeco + deltaDecoration + umbralDecoration ) {
        previousPosDeco = floor( currentPosX - umbralDecoration );
    }

    // Modificar los colores de inicio de la carretera para el modo arcoiris
    color[ 0 ] = colorStart[ 0 ];
    color[ 1 ] = colorStart[ 1 ];
    color[ 2 ] = colorStart[ 2 ];

    drawFloor( previousPosFloorX, previousPosFloorZ );
    drawBackground();
    drawRoad( previousPosRoad );
    drawDecoration( previousPosDeco );

    // Las luces aquí son independientes de la posición de la cámara
    enableMoonLight();
    enableStreetLights( previousPosFarola );
    
    drawCelestialBodies();

    drawClouds( previousPosFloorX, previousPosFloorZ );
    
    if( uiMode && !alambricMode ) {
        drawUI();
    }
    
    glutSwapBuffers();
}

void reshape( GLint w, GLint h ) {
    aspectRatio = w/(float)h;

    // Definir marco de dibujo
    glViewport( 0, 0, w, h );

    // Construir cámara propia
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    // Cámara perspectiva
    gluPerspective( 45, aspectRatio, 1, 100 );
}

void update() {
    static int prevTime = glutGet( GLUT_ELAPSED_TIME );
    int currTime = glutGet( GLUT_ELAPSED_TIME );
    float deltaTime = (currTime - prevTime) / 1000.0;
    EYE_X = EYE_X + vel[ 0 ] * velocity * deltaTime;
    EYE_Z = EYE_Z + vel[ 1 ] * velocity * deltaTime;
    LOOK_X = EYE_X + vel[ 0 ];
    LOOK_Z = EYE_Z + vel[ 1 ];
    prevTime = currTime;

    updateSpeedAndAngle();
    updateSceneModes();

    glutPostRedisplay();
}

void updateSpeedAndAngle() {
    float angle, norm;
    
    if( keyArray[ 0 ] ) {
        velocity += deltaVelocity;
    }
    
    if( keyArray[ 1 ] ) {
        velocity -= deltaVelocity;
        if( velocity < 0 ) {
            velocity = 0;
        }
    }

    if( keyArray[ 2 ] ) {
        angle = asin( vel[ 1 ] );
        vel[ 0 ] = cos( angle - rad( deltaRotateCamera ) );
        vel[ 1 ] = sin( angle - rad( deltaRotateCamera ) );
    }
    
    if( keyArray[ 3 ] ) {
        angle = asin( vel[ 1 ] );
        vel[ 0 ] = cos( angle + rad( deltaRotateCamera ) );
        vel[ 1 ] = sin( angle + rad( deltaRotateCamera ) );
    }
}

void updateSceneModes() {
    if( alambricMode ) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glDisable( GL_TEXTURE_2D );
    }
    else {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable( GL_TEXTURE_2D );
    }

    if( nightMode ) {
        glEnable( GL_LIGHTING );
    }
    else {
        glDisable( GL_LIGHTING );
    }

    if( fogMode ) {
        glEnable( GL_FOG );
        if( rainbowMode ) {
            glFogfv( GL_FOG_COLOR, color );
        }
        else {
            glFogfv( GL_FOG_COLOR, GRISCLARO );
        }
    }
    else {
        glDisable( GL_FOG );
    }
}

void onKey( unsigned char key, int x, int y ) {
    key = tolower( key );
    switch( key ) {
        case 's':
            alambricMode = !alambricMode;
            break;
        case 'l':
            nightMode = !nightMode;
            // Se llama a update() aquí para activar la iluminación
            // antes de volver a llamar al display().
            // 
            // Hago esto porque había una frame en la que los elementos se
            // ponían en blanco; seguramente esto pase porque el programa
            // aún no estaba "idle" y por tanto redibujaba las cosas
            // con materiales pero todavía sin iluminación (nightMode ya había cambiado).
            // 
            // Esta llamada aquí funciona y quita el frame malo.
            update();
            break;
        case 'n':
            fogMode = !fogMode;
            break;
        case 'c':
            uiMode = !uiMode;
            break;
        case 'r':
            rainbowMode = !rainbowMode;
            rainbowAngle = 0;
            break;
        case 'p':
            thirdPersonMode = !thirdPersonMode;
            if( thirdPersonMode ) {
                thirdPerson();
            }
            else {
                firstPerson();
            }
            break;
        case 'm':
            farosLargos = !farosLargos;
            if( farosLargos ) {
                farosDireccion[ 1 ] = -0.02;
            }
            else {
                farosDireccion[ 1 ] = -0.083;
            }
            break;
    }
}

void onSpecialKey( int cursor, int x, int y ) {
    if( cursor == GLUT_KEY_UP ) {
        keyArray[ 0 ] = true;
    }
    if( cursor == GLUT_KEY_DOWN ) {
        keyArray[ 1 ] = true;
    }
    if( cursor == GLUT_KEY_LEFT ) {
        keyArray[ 2 ] = true;
        thirdPersonDirection = thirdPersonLeft;
    }
    if( cursor == GLUT_KEY_RIGHT ) {
        keyArray[ 3 ] = true;
        thirdPersonDirection = thirdPersonRight;
    }
}

void onSpecialUpKey( int cursor, int x, int y ) {
    if( cursor == GLUT_KEY_UP ) {
        keyArray[ 0 ] = false;
    }
    if( cursor == GLUT_KEY_DOWN ) {
        keyArray[ 1 ] = false;
    }
    if( cursor == GLUT_KEY_LEFT ) {
        keyArray[ 2 ] = false;
    }
    if( cursor == GLUT_KEY_RIGHT ) {
        keyArray[ 3 ] = false;
    }
}

void printUsage() {
    std::cout << "Cursores:\tcontrolar el vehículo" << std::endl;
    std::cout << "'s':\t\tactivar/desactivar modo alámbrico" << std::endl;
    std::cout << "'l':\t\tactivar/desactivar modo noche (iluminación)" << std::endl;
    std::cout << "'n':\t\tactivar/desactivar niebla" << std::endl;
    std::cout << "'c':\t\tactivar/desactivar elementos del personaje (interfaz)" << std::endl;
    std::cout << "'r':\t\tactivar/desactivar modo arcoiris" << std::endl;
    std::cout << "'p':\t\tactivar/desactivar modo tercera persona" << std::endl;
    std::cout << "'m':\t\tcambiar modo de faros (largos o cortos)" << std::endl;
}

int main( int argc, char **argv ) {
    // Inicializar la librería freeimage
    FreeImage_Initialise();

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( 600, 600 );
    glutCreateWindow( "SGI proyecto carretera" );
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutIdleFunc( update );
    glutSpecialFunc( onSpecialKey );
    glutSpecialUpFunc( onSpecialUpKey );
    glutKeyboardFunc( onKey );

    init();
    printUsage();

    glutMainLoop();

    // Desinicializar la librería freeimage
    FreeImage_DeInitialise();
    
    return 0;
}