/*
 * primitives.cpp
 *
 *  Created on: Nov 24, 2010
 *      Author: dlb
 */

#include <cmath>
#include <ctime>
#include <vector>

#include "primitives.h"

Color::Color()
{
    this->r = 0.0;
    this->g = 0.0;
    this->b = 0.0;
}

Vec3::Vec3()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vec3::normalize()
{
    float len = sqrt(x * x + y * y + z * z);
    if (len != 0.0) {
        x /= len;
        y /= len;
        z /= len;
    } else {
        x = 0;
        y = 0;
        z = 0;
    }
}

Vec3 Vec3::cross(Vec3 &v)
{
    return Vec3((this->y * v.z) - (this->z * v.y), (this->z * v.x) - (this->x * v.z), (this->x
            * v.y) - (this->y * v.x));
}

Vec3 Vec3::midpoint(Vec3 &v)
{
    return Vec3((this->x + v.x) / 2.0, (this->y + v.y) / 2.0, (this->z + v.z) / 2.0);
}

Vec3 Vec3::sub(Vec3 &v)
{
    return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
}

float Vec3::dot(Vec3 &v)
{
    return (this->x * v.x + this->y * v.y + this->z * v.z);
}

float Vec3::len()
{
    return this->dot(*this);
}

void Vec3::randomizeInUnitSpace()
{
    Vec3 result(randgen.rand(2.0) - 1.0, randgen.rand(2.0) - 1.0, randgen.rand(2.0) - 1.0);
    while (result.len() >= 1.0) {
        result = Vec3(randgen.rand(2.0) - 1.0, randgen.rand(2.0) - 1.0, randgen.rand(2.0) - 1.0);
    }

    *this = result;
}

Poly3::Poly3()
{
    this->p0 = Vec3();
    this->p1 = Vec3();
    this->p2 = Vec3();

    this->c0 = Color();
    this->c1 = Color();
    this->c2 = Color();
}

Vec3 Poly3::getNormal()
{
    Vec3 result, dif1, dif2;

    // Calculate vectors
    dif1.x = p0.x - p1.x;
    dif1.y = p0.y - p1.y;
    dif1.z = p0.z - p1.z;

    dif2.x = p1.x - p2.x;
    dif2.y = p1.y - p2.y;
    dif2.z = p1.z - p2.z;

    result = dif1.cross(dif2);
    result.normalize();

    return result;
}

Sphere::Sphere(int t, int no_of_cuts, float shift)
{
    randgen.seed(time(NULL));
    this->constructWithOctahedrons(t);
    this->generateHeights(no_of_cuts, shift);
//    this->setWaterLine();
    this->setColors();
}

int Sphere::constructWithOctahedrons(int iterations)
{
    printf("Generating Sphere...");
    //number of triangles is (4^iterations) * 8
    this->polies = std::vector<Poly3>(static_cast<int> (pow(4, iterations) * 8), Poly3());
    Vec3 p[6];
    Vec3 pa, pb, pc;

    //create vertices of inital unit-sized octahedron
    p[0] = Vec3(0.0, 0.0, 1.0);
    p[1] = Vec3(0.0, 0.0, -1.0);
    p[2] = Vec3(-1.0, -1.0, 0.0);
    p[2].normalize();
    p[3] = Vec3(1.0, -1.0, 0.0);
    p[3].normalize();
    p[4] = Vec3(1.0, 1.0, 0.0);
    p[4].normalize();
    p[5] = Vec3(-1.0, 1.0, 0.0);
    p[5].normalize();

    //create triangles of inital unit-sized octahedron
    this->polies[0].p0 = p[0];
    this->polies[0].p1 = p[3];
    this->polies[0].p2 = p[4];
    this->polies[1].p0 = p[0];
    this->polies[1].p1 = p[4];
    this->polies[1].p2 = p[5];
    this->polies[2].p0 = p[0];
    this->polies[2].p1 = p[5];
    this->polies[2].p2 = p[2];
    this->polies[3].p0 = p[0];
    this->polies[3].p1 = p[2];
    this->polies[3].p2 = p[3];
    this->polies[4].p0 = p[1];
    this->polies[4].p1 = p[4];
    this->polies[4].p2 = p[3];
    this->polies[5].p0 = p[1];
    this->polies[5].p1 = p[5];
    this->polies[5].p2 = p[4];
    this->polies[6].p0 = p[1];
    this->polies[6].p1 = p[2];
    this->polies[6].p2 = p[5];
    this->polies[7].p0 = p[1];
    this->polies[7].p1 = p[3];
    this->polies[7].p2 = p[2];
    int ct = 8;
    int np = ct;

    if (iterations < 1)
        return np;

    for (int it = 0; it < iterations; it++) {
        np = ct;
        for (int i = 0; i < np; i++) {
            //calc normalized midpoints of sides of triangle
            pa = this->polies[i].p0.midpoint(this->polies[i].p1);
            pa.normalize();
            pb = this->polies[i].p1.midpoint(this->polies[i].p2);
            pb.normalize();
            pc = this->polies[i].p2.midpoint(this->polies[i].p0);
            pc.normalize();

            this->polies[ct].p0 = this->polies[i].p0;
            this->polies[ct].p1 = pa;
            this->polies[ct].p2 = pc;
            ct++;
            this->polies[ct].p0 = pa;
            this->polies[ct].p1 = this->polies[i].p1;
            this->polies[ct].p2 = pb;
            ct++;
            this->polies[ct].p0 = pb;
            this->polies[ct].p1 = this->polies[i].p2;
            this->polies[ct].p2 = pc;
            ct++;
            this->polies[i].p0 = pa;
            this->polies[i].p1 = pb;
            this->polies[i].p2 = pc;
        }
    }
    printf("done.\n");
    return np;
}

void Sphere::generateHeights(int no_of_cuts, float shift)
{
    std::vector<Plane> cuts = std::vector<Plane>(no_of_cuts, Plane());
    Plane aPlane;
    Vec3 to_plane;
    Vec3 normal;
    Vec3 tvec;
    long int noOfUps0 = 0;
    long int noOfUps1 = 0;
    long int noOfUps2 = 0;
    float finalshift = 0;

    float dist;

    //generate all cutting planes
    //two random vectors describe a plane
    for (unsigned int i = 0; i < cuts.size(); i++) {
        tvec.randomizeInUnitSpace();
        to_plane = tvec;
        tvec.randomizeInUnitSpace();
        tvec.normalize();
        normal = tvec;
        aPlane.normal = normal;
        aPlane.to_origin = to_plane;
        cuts[i] = aPlane;
    }

    //now elevate and lower terrain

    //for every poly
    for (unsigned int p = 0; p < this->polies.size(); p++) {
        noOfUps0 = noOfUps1 = noOfUps2 = 0;
        //for every cut
        //count number of raises for current vertices
        for (unsigned int c = 0; c < cuts.size(); c++) {
            to_plane = cuts[c].to_origin;
            normal = cuts[c].normal;

            //for every vertex of triangle
            tvec = this->polies[p].p0.sub(to_plane);
            dist = tvec.dot(normal);
            //if point is above plain > elevate it
            if (this->pointIsAbovePlane(dist)) {
                noOfUps0++;
            } else { //else lower it
                noOfUps0--;
            }

            tvec = this->polies[p].p1.sub(to_plane);
            dist = tvec.dot(normal);
            //if point is above plain > elevate it
            if (this->pointIsAbovePlane(dist)) {
                noOfUps1++;
            } else { //else lower it
                noOfUps1--;
            }

            tvec = this->polies[p].p2.sub(to_plane);
            dist = tvec.dot(normal);
            //if point is above plain > elevate it
            if (this->pointIsAbovePlane(dist)) {
                noOfUps2++;
            } else { //else lower it
                noOfUps2--;
            }
        }

        //raise terrain if ups > 0
        //with shift*ups + 1.0
        if(noOfUps0 > 0) {
            finalshift = noOfUps0 * shift + 1.0;
            this ->polies[p].p0.x *= finalshift;
            this ->polies[p].p0.y *= finalshift;
            this ->polies[p].p0.z *= finalshift;
        }
        if(noOfUps1 > 0) {
            finalshift = noOfUps1 * shift + 1.0;
            this ->polies[p].p1.x *= finalshift;
            this ->polies[p].p1.y *= finalshift;
            this ->polies[p].p1.z *= finalshift;
        }
        if(noOfUps2 > 0) {
            finalshift = noOfUps2 * shift + 1.0;
            this ->polies[p].p2.x *= finalshift;
            this ->polies[p].p2.y *= finalshift;
            this ->polies[p].p2.z *= finalshift;
        }
    }
}

void Sphere::setWaterLine()
{
    for (unsigned int i = 0; i < this->polies.size(); i++) {
        if (this->polies[i].p0.len() < 1.0)
            this->polies[i].p0.normalize();
        if (this->polies[i].p1.len() < 1.0)
            this->polies[i].p1.normalize();
        if (this->polies[i].p2.len() < 1.0)
            this->polies[i].p2.normalize();
    }
}

bool Sphere::pointIsAbovePlane(float dist)
{
    if (dist >= 0.0) {
        return true;
    } else {
        return false;
    }
}

Color Sphere::getMarsColor(float height, float hmin, float dv)
{
    Color c;
    Color c1, c2, c3;
    c1.r =  55 / 255.0; c1.g =  55 / 255.0; c1.b =  45 / 255.0;
    /* c2.r = 200 / 255.0; c2.g =  60 / 255.0; c2.b =   0 / 255.0; */
    c2.r = 235 / 255.0; c2.g =  90 / 255.0; c2.b =  30 / 255.0;
    c3.r = 250 / 255.0; c3.g = 160 / 255.0; c3.b = 110 / 255.0;
    float ratio = 0.4;
    float hmid = hmin + ratio * dv;
    if (height < hmid) {
       c.r = (c2.r - c1.r) * (height - hmin) / (ratio*dv) + c1.r;
       c.g = (c2.g - c1.g) * (height - hmin) / (ratio*dv) + c1.g;
       c.b = (c2.b - c1.b) * (height - hmin) / (ratio*dv) + c1.b;
    } else {
       c.r = (c3.r - c2.r) * (height - hmid) / ((1-ratio)*dv) + c2.r;
       c.g = (c3.g - c2.g) * (height - hmid) / ((1-ratio)*dv) + c2.g;
       c.b = (c3.b - c2.b) * (height - hmid) / ((1-ratio)*dv) + c2.b;
    }

    return c;
}

void Sphere::setColors()
{
    float maxheight = 0.0;
    float minheight = 1.0;
    float temp = 0.0;
    float dv = 0.0;


    //find max height
    for (unsigned int i = 0; i < this->polies.size(); i++) {
        temp = this->polies[i].p0.len();
        if (temp > maxheight) {
            maxheight = temp;
        }
        if (temp < minheight) {
            minheight = temp;
        }

        temp = this->polies[i].p1.len();
        if (temp > maxheight) {
            maxheight = temp;
        }
        if (temp < minheight) {
            minheight = temp;
        }

        temp = this->polies[i].p2.len();
        if (temp > maxheight) {
            maxheight = temp;
        }
        if (temp < minheight) {
            minheight = temp;
        }
    }

    dv = maxheight - minheight;

    //set color for every vertex
    for (unsigned int i = 0; i < this->polies.size(); i++) {
        this->polies[i].c0 = this->getMarsColor(this->polies[i].p0.len(), minheight, dv);
        this->polies[i].c1 = this->getMarsColor(this->polies[i].p1.len(), minheight, dv);
        this->polies[i].c2 = this->getMarsColor(this->polies[i].p2.len(), minheight, dv);
    }

}

