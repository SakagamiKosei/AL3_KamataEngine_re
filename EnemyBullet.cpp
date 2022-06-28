#include "EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet()
{

}

EnemyBullet::~EnemyBullet()
{

}

void EnemyBullet::Initalize(Model* model, const Vector3& position,
	const Vector3& velocity)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy_bullet.png");

	// ワールドトランスフォームの初期化
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	vectorChange_ = new VectorChange();

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::Update()
{
	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;
	vectorChange_->MyUpdate(worldTransform_);
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}