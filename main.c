/***********************
 *                     *
 *  L I B R E R I A S  * 
 *                     *
 ***********************/
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
//#include <mem.h>
#include <math.h>
#include <string.h>
#include <time.h>

/*************************
 *                       *
 *  C O N S T A N T E S  * 
 *                       *
 *************************/
#define NUM_TEXTURAS 10
#define MAX_TAM_ESFERA 10000

/***************************
 *                         *
 *  E S T R U C T U R A S  * 
 *                         *
 ***************************/
typedef struct config_tag {
    short int SCREEN_SIZE_X;
    short int SCREEN_SIZE_Y;
    char FULLSCREEN;
} configuracion;

typedef	struct {
    GLubyte	*imageData;
    GLuint	bpp;
    GLuint	width;
    GLuint	height;
    GLuint	texID;
} Imagen;

typedef struct Point {
  double x;
  double y;
  double z;
} point;

struct vertice{
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    float su;
    float tv;
}vertice;

/***********************
 *                     *
 *  V A R I A B L E S  * 
 *                     *
 ***********************/
GLfloat anos;
GLint dia;
GLint hora;
GLint min;
GLfloat z;
GLfloat der;
//GLfloat izq;
GLfloat arr;

int aux2=0;
float mov;
float mov2;
int mov3;
float distancia=0;
int explo;
float cant;
float aux=-1;
int d0=0,d1=0,d2=0,d10=0,d3=0,d4=0,d5=0,d6=0,d7=0,d8=0,d9=0,d11=0,d12=0,d13=0,d14=0,d15=0,d16=0,d17=0,d18=0,d19=0;
float control=0;
int control1=-1;
float a0=0,a1=0,a2=0,a3=0,a4=0,a5=0,a6=0,a7=0,a8=0,a9=0,a10=0,a11=0,a12=0,a13=0,a14=0,a15=0,a16=0,a17=0,a18=0,a19=0;

float a[]={-17,-15.5, -13.4,-7.0,-2.0,0,2,7.0,13.0,15.3,17,-7,-2,0.1,2.1,7,13.1,15.4};
float b[]={0, 2.0, 7.0, 13.4, 15.3, 17.0, 15.5, 13.4, 7.1, 2.1,0.1,-13,-15.3,-17,-15.5,-13.4,-7,-3};
int d[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

GLfloat mat_emision_explosion[] = {1, 0.0, 0.0, 1.0};
GLint i;

static int slices=16;
static int stacks= 16;

char *textura_datos[NUM_TEXTURAS];  /* Aquí almacenaremos las texturas */
GLuint textura[NUM_TEXTURAS];       /* Aquí llevaremos el número de texturas*/
configuracion config;               /* Variable para almacenar la configuración */
char resultado_carga_tga[100];

float posicionZnemesis;
float posicionXnemesis;
void setZnemesis(float z){posicionZnemesis=z;}
void setXnemesis(float x){posicionXnemesis=x;}
float getXnemesis(){return(posicionXnemesis);}
float getZnemesis(){return(posicionZnemesis);}
int sw=10;

float x3=-13.4;

static int window;
static int menu_id;
static int solarSytem_submenu_id;
static int planets_submenu_id;
static int value = 0;

GLfloat mercurio_matrix[16];
GLfloat venus_matrix[16];
GLfloat tierra_matrix[16];
GLfloat marte_matrix[16];
GLfloat jupiter_matrix[16];
GLfloat saturno_matrix[16];
GLfloat urano_matrix[16];
GLfloat neptuno_matrix[16];

/***********************
 *                     *
 *  F U N C I O N E S  * 
 *                     *
 ***********************/
void *CargaTGA(char *filename,int *tam) {
    GLubyte TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
    GLubyte TGAcompare[12];
    GLubyte header[6];
    GLuint  bytesPerPixel;
    GLuint  imageSize;
    GLuint  temp,i;
    GLuint  type=GL_RGBA;
    Imagen  texture;
    GLubyte *aux;

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        strcpy(resultado_carga_tga, "archivo");
        return NULL;
    }

    /* Esto abre y comprueba que es un TGA */
    fread(TGAcompare,1,sizeof(TGAcompare),file);
    if (memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0) {
        strcpy(resultado_carga_tga, "memcmp");
        return NULL;
    }
    
    /* Leemos la cabecera*/
    fread(header,1,sizeof(header),file);
    
    /* Determinamos el tamaño */
    texture.width  = header[1] * 256 + header[0];
    texture.height = header[3] * 256 + header[2];
    
    /* Vemos las características y comprobamos si son correctas */
    if(	texture.width	<=0	||
        texture.height	<=0	||
        texture.width >256      ||
        texture.height !=texture.width || ( header[4]!=32)) {
            fclose(file);
            strcpy(resultado_carga_tga, "tamagno");
            return NULL;
    }

    /* Calculamos la memoria que será necesaria */
    texture.bpp	  = header[4];
    bytesPerPixel = texture.bpp/8;
    imageSize	  = texture.width*texture.height*bytesPerPixel;

    /* Reservamos memoria */
    texture.imageData=(GLubyte *)malloc(imageSize);
    //sprintf(resultado_carga_tga, "%d", imageSize);

    /* Cargamos y hacemos alguna comprobaciones */
    if ( texture.imageData==NULL || fread(texture.imageData, 1, imageSize, file)!=imageSize) {
        if(texture.imageData!=NULL)
            free(texture.imageData);
        fclose(file);
        strcpy(resultado_carga_tga, "imagedata");
        return NULL;
    }

    /* El TGA viene en formato BGR, lo pasamos a RGB */
    for(i=0; i<(GLuint)(imageSize); i+=bytesPerPixel) {
        temp=texture.imageData[i];
        texture.imageData[i] = texture.imageData[i + 2];
        texture.imageData[i + 2] = temp;
    }
    fclose (file);
    
    /* Ahora cambiamos el orden de las líneas, como si hiciesemos un flip vertical. */
    aux=(GLubyte *)malloc(imageSize);
    for(i=0; i<texture.height; i++)
        memcpy(&aux[imageSize-((i+1)*texture.width*4)],&texture.imageData[i*texture.width*4],texture.width*4);
    
    /* tam devolverá el tamaño */
    *tam=texture.width;
    /* Liberamos memoria */
    free(texture.imageData);
    
    return aux;
}

int carga_texturas() {

    int tam[NUM_TEXTURAS];
    int i;
    char nombre_archivo[30];

    for(i = 0; i < NUM_TEXTURAS; i++) {
        sprintf(nombre_archivo,"img/textura%d.tga", i);
        textura_datos[i]=(char *)CargaTGA(nombre_archivo,&tam[i]);
        
        if (textura_datos[i] == NULL)
            //strcpy(resultado_carga_tga, "-1");
            return -1;
        
        /* Genera una textura, referenciada por el entero textura */
        glGenTextures (NUM_TEXTURAS, &textura[i]);
        /* Esta función indica que será una textura en 2D. Las siguientes
        funciones hará referencia a esta textura */
        glBindTexture (GL_TEXTURE_2D, textura[i]);
        /* Aquí ajustamos algunos parámetros de la textura, concretamente los filtros */
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        /* Con esta función de GLU, creamos un mipmap. Nótese que especificamos
        el tamaño con tam, que devolvia la función CargaTGA*/
        gluBuild2DMipmaps (GL_TEXTURE_2D, 4, tam[i], tam[i], GL_RGBA, GL_UNSIGNED_BYTE, textura_datos[i]);
        /* Liberamos la memoria que ya no utilizaremos */
        free(textura_datos[i]);
    }
    return 0;
}

int RenderVertices(int renderPrimitive, /*BYTE*/char  *pVertex, unsigned int vertexSize, unsigned int vStart, unsigned int numVertex) { //,unsigned int vertexFormat ) 
    GLenum primitive;
    unsigned int offset;
    if ( !pVertex )
        return -1;
    switch ( renderPrimitive ) {
        case 0:
            primitive= GL_POINTS;
            break;
        case 1:
            primitive= GL_LINE_STRIP;
            break;
        case 2:
            primitive= GL_LINES;
            break;
        case 3:
            primitive= GL_TRIANGLE_FAN;
            break;
        case 4:
            primitive= GL_TRIANGLE_STRIP;
            break;
        default: //eTriangleList
            primitive= GL_TRIANGLES;
            break;
    }
    
    offset= vStart*vertexSize;
    glEnableClientState(GL_VERTEX_ARRAY);
    //(3,El vértice se compone de 3 nº reales., Tamaño de la estructura que define el vértice., Dónde empieza el primer vrtice a renderizar.)
    glVertexPointer(3, GL_FLOAT, vertexSize, pVertex);	
    offset += 3*sizeof(float);
    
    // Especificamos las normales
    glEnableClientState	(GL_NORMAL_ARRAY);
    //( Tipo de coordenada de la normal., Tamaño de la estructura que define el vértice.,Dónde empieza la primera normal.)
    glNormalPointer(GL_FLOAT, vertexSize, pVertex+offset);
    offset += 3*sizeof(float);

    // Especificamos las coordenadas de textura
    glEnableClientState	( GL_TEXTURE_COORD_ARRAY );
    //(Nº de coordenadas de textura.,Tipo de coordenadas de textura.,Tamaño de la estructura que define el vértice.,Dónde empieza la primera coordenada.)
    glTexCoordPointer	( 2, GL_FLOAT, vertexSize, pVertex+offset );
    offset += 2*sizeof(float);
    
    // Renderizamos
    glDrawArrays( primitive, 0, numVertex );
    return 0;
}

void render_esfera(float m_Radius, int sl, int st , int id_textura) {
    struct vertice m_Vertex[sl*st*2];
    float x0, y0, z0;
    float x1, y1, z1;
    float sinxy;
    int i, j, v= 0;
    unsigned int numVertex  = 2*sl*st;
    float        xzAngle    = 2.0f*3.1415/(float)sl;
    float        xyAngle    = 2.0f*3.1415/(float)st;
    glEnable(GL_TEXTURE_2D);
    // Con esta función indicamos que vamos a utilizar la textura
    glBindTexture(GL_TEXTURE_2D, textura[id_textura]);
    glColor3d(1,1,1);
    if ( numVertex <= MAX_TAM_ESFERA ) {
    	for (i=0; i < sl; i++){// Meridianos
            for (j=0; j < st; j++){// Paralelos
                sinxy= sin(j*xyAngle);  // Para no calcularlo 4 veces
                x0= sinxy * cos(i*xzAngle);
                y0= cos(j*xyAngle);
                z0= sinxy * sin(i*xzAngle);
                // Vertice
                m_Vertex[v].x = m_Radius*x0;
                m_Vertex[v].y = m_Radius*y0;
                m_Vertex[v].z = m_Radius*z0;
               // Normal
                m_Vertex[v].nx= x0;
                m_Vertex[v].ny= y0;
                m_Vertex[v].nz= z0;
               //m_Vertex[v].su= 1.0f-i/(float)slices;               // SU= 1-i*xzAngle/2PI
                m_Vertex[v].su= i/(float)sl;                    // SU= i*xzAngle/2PI
                m_Vertex[v].tv= ((float)st-2.0f*j)/(float)st;   // TV= 1-j*xyAngle/PI
                v++;
                x1= sinxy * cos((i+1)*xzAngle);
                y1= y0;
                z1= sinxy * sin((i+1)*xzAngle);
                m_Vertex[v].x = m_Radius*x1;
                m_Vertex[v].y = m_Radius*y1;
                m_Vertex[v].z = m_Radius*z1;
                m_Vertex[v].nx= x1;
                m_Vertex[v].ny= y1;
                m_Vertex[v].nz= z1;
                //   m_Vertex[v].su= 1.0f-(i+1)/(float)slices;
                m_Vertex[v].su= (i+1)/(float)sl;
                m_Vertex[v].tv= m_Vertex[v-1].tv;   // Igual que el anterior.
                v++;
            }
        }
        RenderVertices( 4, (char *)m_Vertex, sizeof(vertice), 0, numVertex);//, eFVF_XYZ|eFVF_NORMAL|eFVF_TEX1 );
    }
    glDisable(GL_TEXTURE_2D);
}

//Función creadora de estrellas (NO USADA)
void estrella (float x, float y , float z){
    glPushMatrix ();
    GLfloat mat_emision_estrellas[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_diffuse_estrellas[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_specular_estrellas[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_ambient_estrellas[] = {1.0, 1.0, 1.0, 1.0f};
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_estrellas);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse_estrellas);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_estrellas);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_estrellas);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.0);
    glTranslatef (x, y,z); 
    glutSolidSphere (0.015, 10, 10);
    glPopMatrix ();
}

//Función creadora de meteoritos
void meteoro (float x, float y, float z){
    glPushMatrix ();
    
    GLfloat mat_emision[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0f};
    
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    anos = ((float) dia / 365)*360;

    /* Si aux2==1, quiere decir que se presionó F7*/
    if(aux2==1){
        float cuadrado1;
        float cuadrado2;
        float m;
        
        cuadrado1=getXnemesis()-x;
        cuadrado1=cuadrado1*cuadrado1;
        cuadrado2=getZnemesis()-z;
        cuadrado2=cuadrado2*cuadrado2;
        distancia=sqrt(cuadrado1+cuadrado2);
        m=(z-getZnemesis())/(x-getXnemesis());

        if(distancia<10){
            aux2=0;
            control=1;
            int i=0;
            for(i=0;i<20;i++){
                if(a[i]==x){
                    control1=i;	
                }
            }
        }	
    }
    glTranslatef (x, y, z);
    glutSolidSphere (0.06, 100, 100);
    glPopMatrix ();
}

//Funciones creadoras de los planetas
void sol(float x, float y, float z) {
    glPushMatrix ();
    
    GLfloat light_direction[] = { 1.0, 1.0, 0.0 };
    GLfloat light_position[] = { 1.0, 0.0, 1.0, 1.0 };
    GLfloat mat_emision[] = {1.0, 0.9, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.9, 0.0, 1.0f};
    GLfloat mat_specular[] = {1.0, 1.0, 0.0, 1.0f};
    GLfloat mat_ambient[] = {1.0, 1.0, 0.0, 1.0f};
    
    glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    glTranslatef (x, y, z); 
    render_esfera(4.0, 100, 32 , 0);
    glPopMatrix ();
}

void nemesis(float x, float y, float z){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.0, 0.0, 1.0f};
    GLfloat mat_specular[] = {1.0, 0.0, 0.0, 1.0f};
    GLfloat mat_ambient[] = {1.0, 0.0, 0.0, 1.0f};
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    anos = ((float) dia / 365)*360;
    z= z+mov;
    if(mov2>0)
        x=sqrt(1-z*z/900)*20;
    else
        x=sqrt(1-z*z/900)*(-20);
    //glRotatef(11, 0,0,0 );
    setZnemesis(z);
    setXnemesis(x);
    glTranslatef (x,y,z); 
    //render_esfera(0.8, 100, 32 , 5);    
    glutSolidSphere (1, 100, 100);
    glPopMatrix ();
}

void mercurio(float x,float y,float z){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.1, 0.1, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {1.0, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    /*glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_mercurio);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_mercurio);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.0);*/
    anos = ((float) dia / 365)*360;
    glRotatef( anos/0.24, 0,1,0 );
    glTranslatef (x, y, z); 
    render_esfera(0.03, 100, 32 , 3);    
    //glutSolidSphere (0.03, 100, 100);
    glPopMatrix ();
}

void venus(float x, float y, float z){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.1, 0.1, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {1.0, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    /*glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_venus);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_venus);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.0);*/
    anos = ((float) dia / 365)*360;
    glRotatef( anos/0.6, 0,1,0 );
    glTranslatef (x, y, z); 
    render_esfera(0.08, 100, 32 , 2);    
    //glutSolidSphere (0.08, 100, 100);
    glPopMatrix ();
}

void tierra(float x, float y, float z){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.0, 0, 0.2, 1.0f}; 
    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    anos = ((float) dia / 686)*360;
    glRotatef( anos, 0,1,0 );
    // glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef (x, y, z); 
    render_esfera(0.08, 100, 32 , 1);    
    //glutSolidSphere (0.08, 100, 100);
    glPopMatrix ();
}

void marte (float x, float y, float z){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.5, 0.0, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0f};
    GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    /*glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_marte);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_marte);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.0);*/
    anos = ((float) dia / 365)*360;
    glRotatef( anos/1.88, 0,1,0 );
    glTranslatef (x,y,z); 
    render_esfera(0.046, 100, 32 , 4);    
    //glutSolidSphere (0.046, 100, 100);
    glPopMatrix ();
}

void jupiter (float x, float y, float z){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.1, 0.1, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {1.0, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    /*glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_jupiter);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_jupiter);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.0);*/
    anos = ((float) dia / 365)*360;
    glRotatef( anos/11, 0,1,0 );
    glTranslatef (x, y, z); 
    render_esfera(0.959, 100, 32 , 5);    
    //glutSolidSphere (0.959, 100, 100);
    glPopMatrix ();
}

void saturno (float x, float y, float z){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.1, 0.1, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {1.0, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {1.0, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    anos = ((float) dia / 365)*360;
    //Rotación 90°: Los ejes pasan de estar x-y-z => x-z-y.
    //Se aplica la rotación a ambos objetos, cambiando el eje en torno al cual 
    //gira el planeta (Primero en torno al eje Y, ahora en torno al eje Z).
    glRotatef(90, 1, 0, 0);
    glRotatef(-anos/29, 0, 0, 1);   
    glTranslatef (x, y, z);
    render_esfera(0.805, 100, 32 ,6);
    glScalef(1, 1, 0.05);
    glutSolidTorus(0.3, 1.5, 32 , 32);
    glGetFloatv(GL_MODELVIEW_MATRIX, saturno_matrix);
    glPopMatrix ();
}

void urano (float x, float y, float z){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.0, 0.0, 0.3, 1.0};
    GLfloat mat_diffuse[] = {0.0, 0.5, 0.0, 1.0f};
    GLfloat mat_specular[] = {0.0, 0.5, 0.0, 1.0f};
    GLfloat mat_ambient[] = {0.0, 0.5, 0.0, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    /*glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_urano);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_urano);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.0);*/
    anos = ((float) dia / 365)*360;
    glRotatef(anos/84, 0,1,0 );
    
    glTranslatef (x, y, z); 
    render_esfera(0.342, 100, 32 , 7);    
    //glutSolidSphere (0.342, 100, 100);
    glPopMatrix ();
}

void neptuno (float x, float y, float z){
    glPushMatrix ();
    GLfloat mat_emision[] = {0.0, 0.0, 0.3, 1.0};
    GLfloat mat_diffuse[] = {0.5, 0.5, 0.5, 1.0f};
    GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0f};
    GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0f};
    if(explo==0)
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision);
    else
        glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, mat_emision_explosion);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    /*glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_neptuno);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_neptuno);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.0);*/
    anos = ((float) dia / 365)*360;
    glRotatef( anos/164, 0,1,0 );
    glTranslatef (x, y, z); 
    render_esfera(0.328, 100, 32 , 8);
    glPopMatrix ();
}
//Fin funciones planetas

void getXYZ (int planeta){
    
    switch (planeta){
        //Mercurio
        case 1:
            break;
        //Venus
        case 2:
            break;
        //Tierra
        case 3:
            break;
        //Marte
        case 4:
            break;
        //Jupiter
        case 5:
            break;
        //Saturno
        case 6:
            der = saturno_matrix[12];
            arr = saturno_matrix[13];
            z = saturno_matrix[14];
            printf ( "x >> %F , y >> %F , z >> %F \n", saturno_matrix[12], saturno_matrix[13], saturno_matrix[14]);
            break;
        //Urano
        case 7:
            break;
        //Neptuno
        case 8:
            break;
    }

}

void walkFromTO ( float fromX, float fromY, float fromZ, float toX, float toY, float toZ ) {

}

void menuChoise ( ){
    switch (value){
        case 0:
            //set camera position
            gluLookAt(der, arr, z, 0.0, 0.0, 0.0, 0, 1, 0);
            break;
        case 1:
            printf("Nemesis action\n");
            aux2= 1;
            break;
        case 3:
            printf("Iniciar Movimiento \n" );
            hora += 50.0;
            gluLookAt(der, arr, z, 0.0, 0.0, 0.0, 0, 1, 0);
            break;
        case 4:
            printf("Detener Movimiento\n");
            hora  = 0.0;
            break;
        case 6:
            printf("Sol\n");
            walkFromTO (0.0, 0.0, -7.6, 0, 0, 1);
            gluLookAt(der, arr, z, 0.0, 0.0, 0.0, 0, 1, 0);
            break;
        case 7:
            printf("Mercurio\n");
            break;
        case 8:
            printf("Venus\n");
            break;
        case 9:
            printf("Tierra\n");
            break;
        case 10:
            printf("Marte\n");
            break;
        case 11:
            printf("Jupiter\n");
            break;
        case 12:
            printf("Saturno\n");
            //getXYZ (6=='Saturno');
            getXYZ (6);
            gluLookAt(der, arr, z, 0.0, 0.0, 0.0, 0, 1, 0);{
                glRotatef( -(((float) dia / 365)*360)/29, 0, 1.0f, 0 );
            }
            break;
        case 13:
            printf("Urano\n");
            break;
        case 14:
            printf("Neptuno\n");
            break;
        case 15:
            printf("Acerca de\n");
            break;
        case 16:
            printf("Fullscreen\n");
            glutFullScreen();
            break;
        case 17:
            printf("Salir\n");
            exit(1);
            break;
    }
}

void displayevent(void) {
    // limpia la escena (ventana)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     // inicializa la Matriz de transformación de coordenadas (Matriz del Modelo)
    glLoadIdentity();
    // verfica superficies visibles
    glEnable( GL_DEPTH_TEST );
    
    menuChoise ( );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    //Dibujamos los planetas
    sol(0.0,0.0,1);
    nemesis(0,0,-30);
    mercurio(2,0.0,2);
    venus(3,0.0,2);
    tierra(5,0.0,0.0);
    marte(6,0.0,0);
    jupiter(8.4,0.0,-2);
    saturno(-11.6,0.0,0);
    urano(14,0.0,0);
    neptuno(-16,0.0,0);    

    meteoro(a[0]+a0,0,b[0]); //1
    meteoro (a[1]+a1, 0, b[1]);
    meteoro(a[2]+a2, 0, b[2]); 
    meteoro(a[3]+a3, 0, b[3]); 
    meteoro(a[4]+a4, 0, b[4]); 
    meteoro(a[5]+a5, 0, b[5]);  //2
    meteoro(a[6]+a6, 0, b[6]);
    meteoro(a[7]+a7, 0, b[7]);
    meteoro(a[8]+a8, 0, b[8]);
    meteoro(a[9]+a9, 0, b[9]);
    meteoro(a[10]+a10,0,b[10]); //1*/

    //meteoro(x3, 0, z3); 
    meteoro(a[11]+a11, 0, b[11]); 
    meteoro(a[12]+a12, 0, b[12]); 
    meteoro (a[13]+a13, 0, b[13]);  //2
    meteoro(a[14]+a14, 0, b[14]);
    meteoro(a[15]+a15, 0, b[15]);
    meteoro(a[16]+a16, 0, b[16]);
    meteoro(a[17]+a17, 0,b[17]); 

    glutSwapBuffers();
}

void animacion() {
    // cambia el angulo de rotación constantemente (animación)
    if(hora>23){
        dia++;
        if(dia>364)
            anos++;
    }
          
    // movimiento de la estrella nemesis
    if(hora>23)
        mov += mov2;
    if(mov>=59.99)
        mov2=(-1)*0.05;
    if(mov<0.01)
        mov2=0.05;
    if(control==1){
        if(control1==0||d0==1) { a0=a0+0.1; b[0]=b[0]+0.1;d0=1; } 
        if(control1==1||d1==1) { a1=a1+0.1; b[1]=b[1]+0.1;d1=1; }  
        if(control1==2||d2==1) { a2=a2+0.1;b[2]=b[2]+0.1; d2=1; }  
        if(control1==3||d3==1) { a3=a3-0.1; b[3]=b[3]+0.1;d3=1; }  
        if(control1==4||d4==1) { a4=a4-0.1; b[4]=b[4]+0.1;d4=1; }  
        if(control1==5||d5==1) { a5=a5-0.1; b[5]=b[5]+0.1;d5=1; }  
        if(control1==6||d6==1) { a6=a6-0.1; b[6]=b[6]+0.1;d6=1; } 
        if(control1==7||d7==1) { a7=a7-0.1; b[7]=b[7]+0.1;d7=1; }  
        if(control1==8||d8==1) { a8=a8-0.1; b[8]=b[8]+0.1;d8=1; }  
        if(control1==9||d9==1) { a9=a9-0.1; b[9]=b[9]+0.1;d9=1; }  
        if(control1==10||d10==1) { a10=a10-0.1; b[10]=b[10]+0.1;d10=1; }
        if(control1==11||d11==1) { a11=a11-0.1;b[11]=b[11]+0.1; d11=1; }
        if(control1==12||d12==1) { a12=a12-0.1; b[12]=b[12]+0.1;d12=1; }  
        if(control1==13||d13==1) { a13=a13-0.1; b[13]=b[13]+0.1;d13=1; }  
        if(control1==14||d14==1) { a14=a14-0.1; b[14]=b[14]+0.1;d14=1; } 
        if(control1==15||d15==1) { a15=a15-0.1; b[15]=b[15]+0.1;d15=1; }  
        if(control1==16||d16==1) { a16=a16-0.1; b[16]=b[16]+0.1;d16=1; }  
        if(control1==17||d17==1) { a17=a17-0.1; b[17]=b[17]+0.1;d17=1; }		
    }
    // redibuja la escena
    glutPostRedisplay();
}

void menu(int num){
    if(num == 0){
        glutDestroyWindow(window);
        exit(0);
    } else {
        value = num;
    }
    glutPostRedisplay();
}

void createMenu(void){
    //submenú del sistema solar
    solarSytem_submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Iniciar Movimiento", 3);
    glutAddMenuEntry("Detener Movimiento", 4);
    planets_submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Sol", 6);
    glutAddMenuEntry("Mercurio", 7);
    glutAddMenuEntry("Venus", 8);
    glutAddMenuEntry("Tierra", 9);
    glutAddMenuEntry("Marte", 10);
    glutAddMenuEntry("Jupiter", 11);
    glutAddMenuEntry("Saturno", 12);
    glutAddMenuEntry("Urano", 13);
    glutAddMenuEntry("Neptuno", 14);
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Go Nemesis!", 1);
    glutAddSubMenu("Sistema Solar", solarSytem_submenu_id);
    glutAddSubMenu("Ir a", planets_submenu_id);
    glutAddMenuEntry("Acerca de", 15);
    glutAddMenuEntry("FullScreen", 16);
    glutAddMenuEntry("Salir", 17);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void specialkeyevent( int key, int Xx, int Yy ) {
    // manejo de teclas especiales
    switch ( key ) {
        // cambia la z para la traslación de la esfera
        case GLUT_KEY_UP:    z += 1.1; break;
        case GLUT_KEY_DOWN:  z -= 1.1; break;
        case GLUT_KEY_LEFT:  der += 1.05; break;
        case GLUT_KEY_RIGHT: der -= 1.05; break;
        // cambia el delta del angulo de rotación
        case GLUT_KEY_F1:    hora  = 0.0; break;
        case GLUT_KEY_F2:    hora += 50.0; break;
        case GLUT_KEY_F3:    arr -= 0.05; break;
        case GLUT_KEY_F4:    arr += 0.05; break;
        case GLUT_KEY_F5:    arr = 0.00; break;
        case GLUT_KEY_F6:    der = 0.00; break;
        case GLUT_KEY_F8:    explo = 1; break;
        case GLUT_KEY_F7:    aux2= 1; break;
   }
   // redibuja la escena
   glutPostRedisplay();
}

void reshapeevent(GLsizei width, GLsizei height) {
    // establecer el área de visualizacion en la ventana
    glViewport(0,0,width,height);
    // seleccionar la matriz de Proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Vista en Perspectiva
    gluPerspective(60,(GLfloat)width/(GLfloat)height,  0.01, 500);
    // Restaurar a la matriz del Modelo (escena)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*************
 *           *
 *  M A I N  * 
 *           *
 *************/
int main(int argc, char** argv) {
    int aux;	
    // inicialización del GLUT
    glutInit( &argc, argv );
    // inicialiación de la ventana
    glutInitWindowSize( 1020, 850 );  //Tamaño de la Ventana Creada
    glutInitWindowPosition( 100, 100 );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow( "Nemesis v2.0" );
    glEnable(GL_SMOOTH);
    // inicialización de los datos del programa
    anos = 0;
    dia  = 0;
    hora = 0;
    min  = 0;
    z    = -35;
    der  = 0;
    explo= 0;
    mov  = 0;
    mov2 = 0.01;
    mov3 = 0;
    arr  = 5;
    value = 0;
    
    createMenu();
    
    // registro de los eventos
    glutReshapeFunc (reshapeevent);
    glutDisplayFunc( displayevent );
    //Cambio de teclas por menú.
    glutSpecialFunc( specialkeyevent );
    glutIdleFunc( animacion );
    aux = carga_texturas();
    // lazo de eventos
    glutMainLoop();
    return 0;
}
