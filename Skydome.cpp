#include "Skydome.h"
#include <cassert>

Skydome::Skydome()
{

}

Skydome::~Skydome()
{

}


void Skydome::Initialize(Model* model)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	// 3Dモデルの生成
	this->model_ = model;

	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.scale_ = { 500.0f,500.0f,500.0f };

	// ワールド変換の初期化
	worldTransform_.Initialize();
	vectorChange_ = new VectorChange();
}

void Skydome::Update()
{
	vectorChange_->MyUpdate(worldTransform_);
}

void Skydome::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_);
}