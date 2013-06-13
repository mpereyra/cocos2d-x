/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "CCAffineTransform.h"
#include <algorithm>
#include <math.h>

using namespace std;


NS_CC_BEGIN

CCAffineTransform __CCAffineTransformMake(float a, float b, float c, float d, float tx, float ty)
{
  CCAffineTransform t;
  t.a = a; t.b = b; t.c = c; t.d = d; t.tx = tx; t.ty = ty;
  return t;
}

CCPoint __CCPointApplyAffineTransform(const CCPoint& point, const CCAffineTransform& t)
{
  CCPoint p;
  p.x = (float)((double)t.a * point.x + (double)t.c * point.y + t.tx);
  p.y = (float)((double)t.b * point.x + (double)t.d * point.y + t.ty);
  return p;
}

CCSize __CCSizeApplyAffineTransform(const CCSize& size, const CCAffineTransform& t)
{
  CCSize s;
  s.width = (float)((double)t.a * size.width + (double)t.c * size.height);
  s.height = (float)((double)t.b * size.width + (double)t.d * size.height);
  return s;
}


CCAffineTransform CCAffineTransformMakeIdentity()
{
    return __CCAffineTransformMake(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

extern const CCAffineTransform CCAffineTransformIdentity = CCAffineTransformMakeIdentity();


CCRect CCRectApplyAffineTransform(const CCRect& rect, const CCAffineTransform& anAffineTransform)
{
    float top    = rect.getMinY();
    float left   = rect.getMinX();
    float right  = rect.getMaxX();
    float bottom = rect.getMaxY();
    
    CCPoint topLeft = CCPointApplyAffineTransform(CCPointMake(left, top), anAffineTransform);
    CCPoint topRight = CCPointApplyAffineTransform(CCPointMake(right, top), anAffineTransform);
    CCPoint bottomLeft = CCPointApplyAffineTransform(CCPointMake(left, bottom), anAffineTransform);
    CCPoint bottomRight = CCPointApplyAffineTransform(CCPointMake(right, bottom), anAffineTransform);

    float minX = min(min(topLeft.x, topRight.x), min(bottomLeft.x, bottomRight.x));
    float maxX = max(max(topLeft.x, topRight.x), max(bottomLeft.x, bottomRight.x));
    float minY = min(min(topLeft.y, topRight.y), min(bottomLeft.y, bottomRight.y));
    float maxY = max(max(topLeft.y, topRight.y), max(bottomLeft.y, bottomRight.y));
        
    return CCRectMake(minX, minY, (maxX - minX), (maxY - minY));
}

CCAffineTransform CCAffineTransformTranslate(const CCAffineTransform& t, float tx, float ty)
{
    return __CCAffineTransformMake(t.a, t.b, t.c, t.d, t.tx + t.a * tx + t.c * ty, t.ty + t.b * tx + t.d * ty);
}

CCAffineTransform CCAffineTransformScale(const CCAffineTransform& t, float sx, float sy)
{
    return __CCAffineTransformMake(t.a * sx, t.b * sx, t.c * sy, t.d * sy, t.tx, t.ty);
}

CCAffineTransform CCAffineTransformRotate(const CCAffineTransform& t, float anAngle)
{
    float fSin = sin(anAngle);
    float fCos = cos(anAngle);

    return __CCAffineTransformMake(    t.a * fCos + t.c * fSin,
                                    t.b * fCos + t.d * fSin,
                                    t.c * fCos - t.a * fSin,
                                    t.d * fCos - t.b * fSin,
                                    t.tx,
                                    t.ty);
}

/* Concatenate `t2' to `t1' and return the result:
     t' = t1 * t2 */
CCAffineTransform CCAffineTransformConcat(const CCAffineTransform& t1, const CCAffineTransform& t2)
{
    return __CCAffineTransformMake(    t1.a * t2.a + t1.b * t2.c, t1.a * t2.b + t1.b * t2.d, //a,b
                                    t1.c * t2.a + t1.d * t2.c, t1.c * t2.b + t1.d * t2.d, //c,d
                                    t1.tx * t2.a + t1.ty * t2.c + t2.tx,                  //tx
                                    t1.tx * t2.b + t1.ty * t2.d + t2.ty);                  //ty
}

/* Return true if `t1' and `t2' are equal, false otherwise. */
bool CCAffineTransformEqualToTransform(const CCAffineTransform& t1, const CCAffineTransform& t2)
{
    return (t1.a == t2.a && t1.b == t2.b && t1.c == t2.c && t1.d == t2.d && t1.tx == t2.tx && t1.ty == t2.ty);
}

CCAffineTransform CCAffineTransformInvert(const CCAffineTransform& t)
{
    float determinant = 1 / (t.a * t.d - t.b * t.c);

    return __CCAffineTransformMake(determinant * t.d, -determinant * t.b, -determinant * t.c, determinant * t.a,
                            determinant * (t.c * t.ty - t.d * t.tx), determinant * (t.b * t.tx - t.a * t.ty) );
}


//START: BPC PATCH
/* implementation de gluProject et gluUnproject */
/* M. Buffat 17/2/95 */



/*
 * Transform a point (column vector) by a 4x4 matrix.  I.e.  out = m * in
 * Input:  m - the 4x4 matrix
 *         in - the 4x1 vector
 * Output:  out - the resulting 4x1 vector.
 */
void
transform_point(GLfloat out[4], const GLfloat m[16], const GLfloat in[4])
{
#define M(row,col)  m[col*4+row]
	out[0] = M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
	out[1] = M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
	out[2] = M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
	out[3] = M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
#undef M
}




/*
 * Perform a 4x4 matrix multiplication  (product = a x b).
 * Input:  a, b - matrices to multiply
 * Output:  product - product of a and b
 */
void
matmul(GLfloat * product, const GLfloat * a, const GLfloat * b)
{
	/* This matmul was contributed by Thomas Malik */
	GLfloat temp[16];
	GLint i;
	
#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define T(row,col)  temp[(col<<2)+row]
	
	/* i-te Zeile */
	for (i = 0; i < 4; i++) {
		T(i, 0) = A(i, 0) * B(0, 0) + A(i, 1) * B(1, 0) + A(i, 2) * B(2, 0) + A(i, 3) * B(3, 0);
		T(i, 1) = A(i, 0) * B(0, 1) + A(i, 1) * B(1, 1) + A(i, 2) * B(2, 1) + A(i, 3) * B(3, 1);
		T(i, 2) = A(i, 0) * B(0, 2) + A(i, 1) * B(1, 2) + A(i, 2) * B(2, 2) + A(i, 3) * B(3, 2);
		T(i, 3) = A(i, 0) * B(0, 3) + A(i, 1) * B(1, 3) + A(i, 2) * B(2, 3) + A(i, 3) * B(3, 3);
	}
	
#undef A
#undef B
#undef T
	memcpy(product, temp, 16 * sizeof(GLfloat));
}

/*
 * Compute inverse of 4x4 transformation matrix.
 * Code contributed by Jacques Leroy jle@star.be
 * Return GL_TRUE for success, GL_FALSE for failure (singular matrix)
 */
GLboolean
invert_matrix(const GLfloat * m, GLfloat * out)
{
	/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { GLfloat *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]
	
	GLfloat wtmp[4][8];
	GLfloat m0, m1, m2, m3, s;
	GLfloat *r0, *r1, *r2, *r3;
	
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	
	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
	r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
	r0[4] = 1.0f, r0[5] = r0[6] = r0[7] = 0.0f,
	r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
	r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
	r1[5] = 1.0f, r1[4] = r1[6] = r1[7] = 0.0f,
	r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
	r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
	r2[6] = 1.0f, r2[4] = r2[5] = r2[7] = 0.0f,
	r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
	r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
	r3[7] = 1.0f, r3[4] = r3[5] = r3[6] = 0.0f;
	
	/* choose pivot - or die */
	if (fabsf(r3[0]) > fabsf(r2[0]))
		SWAP_ROWS(r3, r2);
	if (fabsf(r2[0]) > fabsf(r1[0]))
		SWAP_ROWS(r2, r1);
	if (fabsf(r1[0]) > fabsf(r0[0]))
		SWAP_ROWS(r1, r0);
	if (0.0f == r0[0])
		return GL_FALSE;
	
	/* eliminate first variable     */
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0f) {
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0f) {
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0f) {
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0f) {
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	
	/* choose pivot - or die */
	if (fabsf(r3[1]) > fabsf(r2[1]))
		SWAP_ROWS(r3, r2);
	if (fabsf(r2[1]) > fabsf(r1[1]))
		SWAP_ROWS(r2, r1);
	if (0.0f == r1[1])
		return GL_FALSE;
	
	/* eliminate second variable */
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0f != s) {
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0f != s) {
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0f != s) {
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0f != s) {
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	
	/* choose pivot - or die */
	if (fabsf(r3[2]) > fabsf(r2[2]))
		SWAP_ROWS(r3, r2);
	if (0.0f == r2[2])
		return GL_FALSE;
	
	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
	r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
	
	/* last check */
	if (0.0f == r3[3])
		return GL_FALSE;
	
	s = 1.0f / r3[3];		/* now back substitute row 3 */
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;
	
	m2 = r2[3];			/* now back substitute row 2 */
	s = 1.0f / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
	r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
	r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
	r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	
	m1 = r1[2];			/* now back substitute row 1 */
	s = 1.0f / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
	r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
	r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	
	m0 = r0[1];			/* now back substitute row 0 */
	s = 1.0f / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
	r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
	
	MAT(out, 0, 0) = r0[4];
	MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
	MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
	MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
	MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
	MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
	MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
	MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
	MAT(out, 3, 3) = r3[7];
	
	return GL_TRUE;
	
#undef MAT
#undef SWAP_ROWS
}



/* projection du point (objx,objy,obz) sur l'ecran (winx,winy,winz) */
//GLint GLAPIENTRY;
GLboolean gluProject(GLfloat objx, GLfloat objy, GLfloat objz,
					 const GLfloat model[16], const GLfloat proj[16],
					 const GLint viewport[4],
					 GLfloat * winx, GLfloat * winy, GLfloat * winz)
{
	/* matrice de transformation */
	GLfloat in[4], out[4];
	
	/* initilise la matrice et le vecteur a transformer */
	in[0] = objx;
	in[1] = objy;
	in[2] = objz;
	in[3] = 1.0f;
	transform_point(out, model, in);
	transform_point(in, proj, out);
	
	/* d'ou le resultat normalise entre -1 et 1 */
	if (in[3] == 0.0f)
		return GL_FALSE;
	
	in[0] /= in[3];
	in[1] /= in[3];
	in[2] /= in[3];
	
	/* en coordonnees ecran */
	*winx = viewport[0] + (1.0f + in[0]) * viewport[2] / 2.0f;
	*winy = viewport[1] + (1.0f + in[1]) * viewport[3] / 2.0f;
	/* entre 0 et 1 suivant z */
	*winz = (1.0f + in[2]) / 2.0f;
	return GL_TRUE;
}



/* transformation du point ecran (winx,winy,winz) en point objet */
//GLint GLAPIENTRY
GLboolean gluUnProject(GLfloat winx, GLfloat winy, GLfloat winz,
					   const GLfloat model[16], const GLfloat proj[16],
					   const GLint viewport[4],
					   GLfloat * objx, GLfloat * objy, GLfloat * objz)
{
	/* matrice de transformation */
	GLfloat m[16], A[16];
	GLfloat in[4], out[4];
	
	/* transformation coordonnees normalisees entre -1 et 1 */
	in[0] = (winx - viewport[0]) * 2.0f / viewport[2] - 1.0f;
	in[1] = (winy - viewport[1]) * 2.0f / viewport[3] - 1.0f;
	in[2] = 2.0f * winz - 1.0f;
	in[3] = 1.0f;
	
	/* calcul transformation inverse */
	matmul(A, proj, model);
	invert_matrix(A, m);
	
	/* d'ou les coordonnees objets */
	transform_point(out, m, in);
	if (out[3] == 0.0f)
		return GL_FALSE;
	*objx = out[0] / out[3];
	*objy = out[1] / out[3];
	*objz = out[2] / out[3];
	return GL_TRUE;
}
//END: BPC PATCH

NS_CC_END
