#include <cassert>

#include "plane.h"

Plane::Plane():
        a(0.0),
        b(0.0),
        c(0.0),
        d(0.0) {

}

Plane::Plane(const kmVec3& v1, const kmVec3& v2, const kmVec3& v3) {
    // This function will create a plane out of a triangle.  First we normalize
    // the triangle.

    kmVec3 vec1, vec2, normal;
    kmVec3Subtract(&vec1, &v3, &v1);
    kmVec3Subtract(&vec2, &v2, &v1);
    kmVec3Cross(&normal, &vec1, &vec2);
    kmVec3Normalize(&normal, &normal);

    // Save the normal data to our plane data.  To calculate the d we take the
    // dot product of the triangle normal with the first triangle.
    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = - (a * v1.x + b * v1.y + c * v1.z);
}

Plane::Plane(const Plane& p) {
    assert(0);
}

PLANE_CLASSIFY Plane::classifyPoint(const kmVec3& point) {

}
