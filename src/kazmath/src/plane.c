/*
Copyright 2007 Luke Benstead

This file is part of KazMath.

KazMath is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

KazMath is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser Public License for more details.

You should have received a copy of the GNU Lesser Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <assert.h>

#include "vec3.h"
#include "vec4.h"
#include "plane.h"

kmScalar kmPlaneDot(const kmPlane* pP, const kmVec4* pV)
{
	//a*x + b*y + c*z + d*w

	return (pP->m_N.x * pV->x +
			pP->m_N.y * pV->y +
			pP->m_N.z * pV->z +
			pP->m_D * pV->w);
}

kmScalar kmPlaneDotCoord(const kmPlane* pP, const kmVec3* pV)
{
	return (pP->m_N.x * pV->x +
			pP->m_N.y * pV->y +
			pP->m_N.z * pV->z + pP->m_D);
}

kmScalar kmPlaneDotNormal(const kmPlane* pP, const kmVec3* pV)
{
	return (pP->m_N.x * pV->x +
			pP->m_N.y * pV->y +
			pP->m_N.z * pV->z);
}

kmPlane* kmPlaneFromPointNormal(kmPlane* pOut, const kmVec3* pPoint, const kmVec3* pNormal)
{
	/*
		Planea = Nx
		Planeb = Ny
		Planec = Nz
		Planed = −N⋅P
	*/


	pOut->m_N.x = pNormal->x;
	pOut->m_N.y = pNormal->y;
	pOut->m_N.z = pNormal->z;
	pOut->m_D = -kmVec3Dot(pNormal, pPoint);

	return pOut;
}

kmPlane* kmPlaneFromPoints(kmPlane* pOut, const kmVec3* p1, const kmVec3* p2, const kmVec3* p3)
{
	/*
	v = (B − A) × (C − A)
	n = 1⁄|v| v
	Outa = nx
	Outb = ny
	Outc = nz
	Outd = −n⋅A
	*/

	kmVec3 n, v1, v2;
	kmVec3Subtract(&v1, p2, p1); //Create the vectors for the 2 sides of the triangle
	kmVec3Subtract(&v2, p3, p1);
	kmVec3Cross(&n, &v1, &v2); //Use the cross product to get the normal

	kmVec3Normalize(&pOut->m_N, &n); //Normalize it and assign to pOut->m_N
	pOut->m_D = kmVec3Dot(kmVec3Scale(&n, &n, -1.0), p1);

	return pOut;
}

kmVec3*  kmPlaneIntersectLine(kmVec3* pOut, const kmPlane* pP, const kmVec3* pV1, const kmVec3* pV2)
{
	/*
		n = (Planea, Planeb, Planec)
		d = V − U
		Out = U − d⋅(Pd + n⋅U)⁄(d⋅n) [iff d⋅n ≠ 0]
	*/

	assert(0);

	kmVec3 d;
	kmVec3Subtract(&d, pV2, pV1); //Get the direction vector

	if (fabs(kmVec3Dot(&pP->m_N, &d)) > kmEpsilon)
	{
		//If we get here then the plane and line are parallel (i.e. no intersection)
		pOut = nullptr; //Set to nullptr

		return pOut;
	}

	//TODO: Continue here!

}

kmPlane* kmPlaneNormalize(kmPlane* pOut, const kmPlane* pP)
{
	kmScalar l = 1.0 / kmVec3Length(&pP->m_N); //Get 1/length
	kmVec3Normalize(&pOut->m_N, &pP->m_N); //Normalize the vector and assign to pOut
	pOut->m_D = pP->m_D * l; //Scale the D value and assign to pOut

	return pOut;
}

kmPlane* kmPlaneScale(kmPlane* pOut, const kmPlane* pP, kmScalar s)
{
	assert(0);
}
