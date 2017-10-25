#include "point.h"
#include <math.h>
#include <random>

Point * Point::copy()
{
	return new Point(this);
}

void Point::set(double v)
{
	this->x = v;
	this->y = v;
	this->z = v;
	this->w = v;
}

void Point::set(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point::set(double x, double y, double z, double w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void Point::set(Point * point)
{
	this->x = point->x;
	this->y = point->y;
	this->z = point->z;
	this->w = point->w;
}

void Point::setRandom(int n)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-n, n);

	this->x = dis(gen);
	this->y = dis(gen);
	this->z = dis(gen);
	this->w = dis(gen);
}

double Point::dot(Point * P)
{
	return (this->x * P->x) + (this->y * P->y) + (this->z * P->z);
}

double Point::magnitude()
{
	return sqrt(this->dot(this));
}

void Point::normalize()
{
	double mag = this->magnitude();

	if (mag > 0)
	{
		this->x /= mag;
		this->y /= mag;
		this->z /= mag;
	}
}

double Point::distance(Point * p)
{
	double d = ((p->x - this->x) * (p->x - this->x)) + ((p->y - this->y) * (p->y - this->y)) + ((p->z - this->z) * (p->z - this->z));
	return sqrt(d);
}

Point * Point::direction(Point * p)
{
	return new Point(p->x - this->x, p->y - this->y, p->z - this->z);;
}

void Point::multiply(double s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;
}

void Point::add(Point * p)
{
	this->x += p->x;
	this->y += p->y;
	this->z += p->z;
	this->w += p->w;
}

double Point::angle(Point * p)
{
	Point * p1 = this->copy();
	p1->normalize();

	Point * p2 = p->copy();
	p2->normalize();

	double d = p1->dot(p2);

	delete p1;
	delete p2;

	return acos(d);
}
