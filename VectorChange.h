#pragma once
#include "Matrix4.h"
#include "WorldTransform.h"
class VectorChange
{
public:
	VectorChange();
	~VectorChange();

	Matrix4 MyScale(WorldTransform& worldTransform_);
	Matrix4 MyRotationX(WorldTransform& worldTransform_);
	Matrix4 MyRotationY(WorldTransform& worldTransform_);
	Matrix4 MyRotationZ(WorldTransform& worldTransform_);
	Matrix4 MyTranslation(WorldTransform& worldTransform_);
	Matrix4 MyUpdate(WorldTransform& worldTransform_);
};