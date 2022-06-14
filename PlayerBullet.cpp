#include "PlayerBullet.h"
#include <cassert>


PlayerBullet::PlayerBullet()
{

}

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Initalize(Model* model, const Vector3& position)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	// ワールドトランスフォームの初期化
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	vectorChange_ = new VectorChange();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::Update()
{
	vectorChange_->MyUpdate(worldTransform_);
}