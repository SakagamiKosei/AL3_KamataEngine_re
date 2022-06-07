#include "WorldTransform.h"

void WorldTransform::UpdateMatrix()
{
	if (parent_)
	{
		matWorld_ *= parent_->matWorld_;
	}
}