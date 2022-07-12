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

	worldTransform_.scale_ = { 1,1,1 };

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

void EnemyBullet::OnCollision()
{
	isDead_ = false;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

Vector3 EnemyBullet::GetRadius()
{
	Vector3 enemyBulletRadius;
	// 横の大きさを半分にして半径にする
	enemyBulletRadius.x = worldTransform_.scale_.x / 2;
	// 縦の大きさを半分にして半径にする
	enemyBulletRadius.y = worldTransform_.scale_.y / 2;
	// 奥行きの大きさを半分にして半径にする
	enemyBulletRadius.z = worldTransform_.scale_.z / 2;

	return enemyBulletRadius;
}
