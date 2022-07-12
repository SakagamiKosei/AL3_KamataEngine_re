#include "Collider.h"


Collider::Collider()
{

}

Collider::~Collider()
{

}

bool Collider::OnBallCollision(int posA_x, int posA_y, int posA_z,int radiusA,
	int posB_x, int posB_y, int posB_z,int radiusB)
{
	if((posB_x - posA_x) * (posB_x - posA_x) 
		+ (posB_y - posA_y) * (posB_y - posA_y)
		+ (posB_z - posA_z) * (posB_z - posA_z)
		<= (radiusA + radiusB) * (radiusA + radiusB))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}