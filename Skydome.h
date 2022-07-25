#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "VectorChange.h"

class Skydome {

public:
	Skydome();
	~Skydome();
	void Initialize(Model* model);

	void Draw(ViewProjection& viewProjection);

	void Update();
private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	VectorChange* vectorChange_ = nullptr;
	
};