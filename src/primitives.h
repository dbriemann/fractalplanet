/*
 * primitives.h
 *
 *  Created on: Nov 24, 2010
 *      Author: dlb
 */

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <vector>

#include "MersenneTwister.h"

#define DTOR            0.0174532925
#define RTOD            57.2957795
#define TWOPI           6.283185307179586476925287
#define PI              3.141592653589793238462643
#define HALFPI          1.570796326794896619231322

static MTRand randgen;

struct Color
{
    Color();

    float r;
    float g;
    float b;
};

struct Vec3
{
    Vec3();
    Vec3(float x, float y, float z);

    float x;
    float y;
    float z;

    void normalize();
    Vec3 midpoint(Vec3 &v);
    Vec3 cross(Vec3 &v);
    Vec3 sub(Vec3 &v);
    float dot(Vec3 &v);
    float len();
    void randomizeInUnitSpace();
};

struct Plane
{
    Vec3 to_origin;
    Vec3 normal;
};

struct Poly3
{
    Poly3();

    Vec3 p0;
    Vec3 p1;
    Vec3 p2;

    //colors
    Color c0;
    Color c1;
    Color c2;

    Vec3 getNormal();
};

struct Sphere
{
    Sphere(int t, int no_of_cuts, float shift);

    std::vector<Poly3> polies;

    int constructWithOctahedrons(int iterations);
    void generateHeights(int no_of_cuts, float shift);
    bool pointIsAbovePlane(float dist);
    void setWaterLine();
    void setColors();
    Color getMarsColor(float height, float hmin, float dv);
};

#endif /* PRIMITIVES_H_ */
