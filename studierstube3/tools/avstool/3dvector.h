// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       3Dvektor.h
//  TYPE:       c code
//  PROJECT:    Studierstube
//  CONTENT:    common functions for the work with 3D-Vectors
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    Georg Fischel, 880/8625233
// ===========================================================================
//  HISTORY:
//
//  23-nov-93 08:00:00  Georg Fischel, 880/8625233
// ===========================================================================  */

#define NULLVEC 1E-8  /* Laenge des Nullvektors */

struct vector { float x, y, z; };


/****************************************************/
/* Prozeduren fuer die Vektorberechnung             */
/****************************************************/


float VecLen( struct vector v )
/* Berechnen der Vektorlaenge von v */
 {
  return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
 }


void Norm( struct vector *v ) /* we changed float-> void
/* Normieren des Vektors v */
 {
  float VLen;

  VLen  = VecLen( *v );
  if ( fabsf( VLen ) > 0 )
    {
     v->x /= VLen;
     v->y /= VLen;
     v->z /= VLen;
    }
 }


void Skalar( struct vector *v, float a ) /* changed float->void
/* Multiplikation des Vektors v mit einen Skalar */
 {

  v->x *= a;
  v->y *= a;
  v->z *= a;
 }


void VecMul( struct vector a, struct vector b,struct vector *v )
/* Berechnnen des Vektorproduktes */
 {
  v->x = a.y * b.z - a.z * b.y;
  v->y = a.z * b.x - a.x * b.z;
  v->z = a.x * b.y - a.y * b.x;
 }
  

void VecMove( struct vector a, struct vector *v )
/* Vektorzuweisungsoperation */
 {
  v->x = a.x;
  v->y = a.y;
  v->z = a.z;
 }

void VecAdd( struct vector a, struct vector b, struct vector *v )
/* Vektoraddition */
 {
  v->x = a.x + b.x;
  v->y = a.y + b.y;
  v->z = a.z + b.z;
 }

void VecSub( struct vector a, struct vector b, struct vector *v )
/* Vektorsubtraktion */
 {
  v->x = a.x - b.x;
  v->y = a.y - b.y;
  v->z = a.z - b.z;
 }
 
  
float det( struct vector a, struct vector b, struct vector c )
/* Berechnen der Determinante von 3 Vektoren */
 {
  return ( a.x*b.y*c.z + a.y*b.z*c.x + a.z*b.x*c.y - 
         ( c.x*b.y*a.z + c.y*b.z*a.x + c.z*b.x*a.y ) );
 }

float dist( struct vector a, struct vector b )
/* Abstand zwischen zwei Punkten */
 {
  struct vector help;

  VecSub( a, b, &help );
  return VecLen( help );
 }


/*I dont use the optimized sin and cos lookup routines */
/*

float sinarr[720], cosarr[720];

void initSinCos( void ) {
  int i;

  for( i=0; i<360; i++ ) {
    sinarr[i] = sinf( i*3.141592654/180 );
    cosarr[i] = cosf( i*3.141592654/180 );
    sinarr[360+i] = sinf( i*3.141592654/180 );
    cosarr[360+i] = cosf( i*3.141592654/180 );
  }
}


float mysin( float x ) {
  float helpsin  = sinarr[(int )x];
  float helpsin2 = sinarr[(int )x+1];

  return helpsin + (helpsin2 - helpsin) * ( x - (float )((int )x));
}



float mycos( float x ) {
  float helpcos  = cosarr[(int )x];
  float helpcos2 = cosarr[(int )x+1];

  return helpcos + (helpcos2 - helpcos) * ( x - (float )((int )x));
}

*/
