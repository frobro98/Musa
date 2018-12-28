
#include "Math/MathUtilities.h"
#include "PrimitiveGeneration.h"

void MostSeparatedPointsOnAABB(int &min, int &max, Vector *pt, uint32 numPts)
{
	// First find most extreme points along principal axes
	int minx = 0;
	int maxx = 0;
	int miny = 0;
	int maxy = 0;
	int minz = 0;
	int maxz = 0;

	for (uint32 i = 1; i < numPts; i++)
	{
		if (pt[i].x < pt[minx].x)
			minx = i;
		if (pt[i].x > pt[maxx].x)
			maxx = i;
		if (pt[i].y < pt[miny].y)
			miny = i;
		if (pt[i].y > pt[maxy].y)
			maxy = i;
		if (pt[i].z < pt[minz].z)
			minz = i;
		if (pt[i].z > pt[maxz].z)
			maxz = i;
	}

	// Given 6pts minx,miny,minz,maxx,maxy,maxz find the greatest distance
	struct tmpNode
	{
		Vector v;
		int index;
		int pad[3];  // padding
	};

	tmpNode data[6];

	data[0].v = pt[minx];
	data[1].v = pt[miny];
	data[2].v = pt[minz];
	data[3].v = pt[maxx];
	data[4].v = pt[maxy];
	data[5].v = pt[maxz];

	data[0].index = minx;
	data[1].index = miny;
	data[2].index = minz;
	data[3].index = maxx;
	data[4].index = maxy;
	data[5].index = maxz;


	float maxDist = (data[0].v - data[1].v).Magnitude();
	int maxA = data[0].index;
	int maxB = data[1].index;
	float dist = maxDist;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			dist = (data[i].v - data[j].v).Magnitude();
			//out("dist(%d,%d) = %f\n",i,j,dist);
			if (dist > maxDist)
			{
				//out("--->maxdist(%d,%d) = %f\n",i,j,dist);
				// new max
				maxDist = dist;
				maxA = data[i].index;
				maxB = data[j].index;
			}
		}
	}

	min = maxA;
	max = maxB;
}


void SphereOfSphereAndPt(Sphere &s, Vector &p)
{
	// Compute squared distance between point and sphere center
	Vector d = p - s.cntr;

	float dist2 = d.Dot(d);
	// Only update s if point p is outside it
	if (dist2 > s.rad * s.rad)
	{
		float dist = Math::Sqrt(dist2);
		float newRadius = (s.rad + dist) * 0.5f;
		float k = (newRadius - s.rad) / dist;
		s.rad = newRadius;
		s.cntr += d * k;
	}
}

void SphereFromDistantPoints(Sphere &s, Vector *pt, uint32 numPts)
{
	// Find the most separated point pair defining the encompassing AABB
	int min, max;
	MostSeparatedPointsOnAABB(min, max, pt, numPts);

	// Set up sphere to just encompass these two points
	s.cntr = (pt[min] + pt[max]) * 0.5f;
	s.rad = (pt[max] - s.cntr).Dot(pt[max] - s.cntr);
	s.rad = Math::Sqrt(s.rad);
}


void RitterSphere(Sphere &s, Vector *pt, uint32 numPts)
{
	// Get sphere encompassing two approximately most distant points
	SphereFromDistantPoints(s, pt, numPts);

	// Grow sphere to include all points
	for (uint32 i = 0; i < numPts; i++)
	{
		SphereOfSphereAndPt(s, pt[i]);
	}
}