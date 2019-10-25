#include <windows.h>
#include <gl/glut.h>
#include <math.h>

//mendefinisikan sistem koordinat menggunakan int
typedef struct {
	int x;
	int y;
	int z;
	
} point2D_t;

//mendefinisikan warna R G B
typedef struct {
	float r;
	float g;
	float b;
} color_t;

typedef struct {
	float dx;
	float dy;
}sumbu;

typedef struct {
	float m[3][3];
} Matrix3D_t;

//typedef struct{ 
//	float v[2]; 
//}Vector2D_t;

typedef struct { 
	float v[3]; 
} Vector3D_t;


//fungsi untuk memberi wara pada obyek grafik
void setColor (color_t col){
	glColor3f(col.r, col.g, col.b);
}

void timer( int value ) {
	glutPostRedisplay();
	glutTimerFunc(100, timer, 0);
}

//fungsi untuk mewarnai sebuah polygon
void drawGaris(point2D_t shape[], int n, color_t color){
	int i;
	setColor(color);
	glBegin(GL_LINE_LOOP);
		for (i=0;i<n;i++){
			glVertex2f(shape[i].x, shape[i].y);
		}
	glEnd();
}

void drawPolygon( point2D_t shape[], int n, color_t color ) {
	int i;
	setColor( color );
	glBegin( GL_POLYGON );
		for( i = 0; i < n; i++ ) {
			glVertex2f( shape[i].x, shape[i].y );
		}
	glEnd();
}

//Matrix2D_t matrix2D; 
Matrix3D_t matrixTranslasi2D; 
Matrix3D_t matrix3DX,matrix3DY,matrix3DZ;

//Vector2D_t vec2D; 
//Vector3D_t vec3D;
//int counter = 0;

Vector3D_t point2vector( point2D_t pnt ) {
	Vector3D_t vector;
	vector.v[0] = pnt.x;
	vector.v[1] = pnt.y;
	vector.v[2] = 1;
	return vector;
}

//Vector3D_t Point2Vector2D(point2D_t pnt){     
//	Vector3D_t vec;     
//	vec.v[0]=pnt.x;     
//	vec.v[1]=pnt.y;     
//	vec.v[2]=1;     
//	return vec; 
//} 

point2D_t vector2point( Vector3D_t vector ) {
	point2D_t vec2pnt;
	vec2pnt.x = vector.v[0];
	vec2pnt.y = vector.v[1];
	return vec2pnt;
}

Vector3D_t operator * (Matrix3D_t a, Vector3D_t b)
{

    Vector3D_t c;
    int i,j;
    for (i=0; i<3; i++)
    {

        c.v[i]=0;
        for (j=0; j<3; j++)

            c.v[i]+=a.m[i][j]*b.v[j];

    }
    return c;

}

Matrix3D_t createIdentity(void){
	Matrix3D_t u;
	int i, j;
	
	for( i = 0; i < 3; i++ ){
		for( j = 0; j < 3; j++ ){
			u.m[i][j] = 0;
		}
		u.m[i][i] = 1;
	}
	return u;
}

Matrix3D_t translationMTX( float dx, float dy){ 
	Matrix3D_t rotate; 
	rotate.m[0][0]=1.0; 
	rotate.m[0][1]=0.0; 
	rotate.m[0][2]=dx; 
	rotate.m[1][0]=0.0; 
	rotate.m[1][1]=1.0; 
	rotate.m[1][2]=dy; 
	rotate.m[2][0]=0.0; 
	rotate.m[2][1]=0.0; 
	rotate.m[2][1]=0.0; 
	return rotate; 
}

Matrix3D_t scallingMTX(float mx, float my) {
	Matrix3D_t res = createIdentity();
	res.m[0][0] = mx;
	res.m[1][1] = my;
	return res;
}

Matrix3D_t rotationMTX( float theta ) {
	Matrix3D_t res = createIdentity();
	float cs = cos( theta );
	float sn = sin( theta );
	
	res.m[0][0] = cs;
	res.m[0][1] = -sn;
	res.m[1][0] = sn;
	res.m[1][1] = cs;
	return res;
}


Matrix3D_t translation2D(float dx, float dy){ 
	Matrix3D_t translation=createIdentity(); 
	translation.m[0][0]=1.0; translation.m[0][1]=0.0; translation.m[0][2]=dx; 
	translation.m[1][0]=0.0; translation.m[1][1]=1.0; translation.m[1][2]=dy; 
	translation.m[2][0]=0.0; translation.m[2][1]=0.0; translation.m[2][2]=1; 
	return translation; 
}

point2D_t center[5] = {{ -270., -190., 1. }, { 300., -220., 1. }, { 30., 20., 1. }, { 130., 40., 1. }, { 100., -20., 1. }};
point2D_t ball[5][360];
color_t color[5] = {{ 0., 0., 0. }, { 0., 0., 1. }, { 1., 0., 1. }, { 1., 1., 0. }, { 1., 0., 0. }};
Matrix3D_t matrixT3D;
Vector3D_t vec3D;
sumbu sumbu1[5] = {{10., 2.}, {2., 10.}, {5., 6.}, {10., 10.}, {12., 10.}};


void drawbola1(){
	for( int n = 0; n < 5; n++ ){
		for( int i = 0; i < 360; i++ ){
			int r = 20;
			
			ball[n][i].x = (float) ( center[n].x + ( r * cos( i ) ) );
			ball[n][i].y = (float) ( center[n].y + ( r * sin( i ) ) );
			
		}
		
		if( center[n].x > 300 || center[n].x < -300 ){
			sumbu1[n].dx = (float) (sumbu1[n].dx * (-1) );
		} else if( center[n].y > 220 || center[n].y < -220 ) {
			sumbu1[n].dy = (float) (sumbu1[n].dy * (-1) );
		}
			
		drawPolygon( ball[n], 360, color[n] );
		
		matrixT3D = translationMTX( sumbu1[n].dx, sumbu1[n].dy );
		vec3D = point2vector( center[n] );
		vec3D = operator * ( matrixT3D, vec3D );
		center[n].x = vec3D.v[0];
		center[n].y = vec3D.v[1];
	}
}
//	Garis(shape,360,col);

//Color_t col = {0.0,0.0,1.0};
//	Garis(shape,360,col); 
//
//	matrixTranslasi2D=translation2D(0.0, 2.0);//translasi
//        vec3D=Point2Vector2D(shape[i]);
//        vec3D=operator*(matrixTranslasi2D,vec3D);
//        shape[i].x=vec3D.v[0];
//        shape[i].y=vec3D.v[1];
//    
//}
//
//void userdraw(void)
//{ 
//	drawbola1();
//}


void display(void){
	glClear( GL_COLOR_BUFFER_BIT);
	drawbola1();
	glutSwapBuffers();
}

int main (int argc, char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Syahfiar Dhani A (2103171028)"); 
	glClearColor(1.0, 1.0, 1.0, 0.0); 
	gluOrtho2D(-320., 320., -240.0, 240.0); 
	glutIdleFunc(display); 
	glutDisplayFunc(display); 
	glutMainLoop(); 
	glutTimerFunc( 1, timer, 0 );
	return 0; 
}


