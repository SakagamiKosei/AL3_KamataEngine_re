#include "Skydome.h"
#include <cassert>
Skydome::Skydome()
{

}

Skydome::~Skydome()
{
	delete vectorChange_;
}
void Skydome::Initialize(Model* model)
{
	worldTransform_.Initialize();
	assert(model);

	model_ = model;


	vectorChange_ = new VectorChange();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.scale_ = { 50,50,50 };
}


void Skydome::Update()
{
	vectorChange_->MyUpdate(worldTransform_);


	debugText_->SetPos(70, 200);
	debugText_->Printf("Skydome Pos:(% f, % f, % f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Skydome::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_);
}