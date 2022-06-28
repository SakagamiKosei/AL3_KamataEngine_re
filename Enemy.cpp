#include "Enemy.h"
#include <cassert>
Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{


	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	// 3Dモデルの生成
	this->model_ = model;


	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("enemy_sentai.png");

	debugText_ = DebugText::GetInstance();
	// 敵の初期座標
	worldTransform_.translation_ = { 0,2,30 };
	// 敵のスケール
	worldTransform_.scale_ = { 1,1,1};
	//// 敵が発生すると同時に弾を発射する
	//Fire();
	// 接近フェーズ初期化
	ApproachInitialize();
}

void Enemy::ApproachMove()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.1f;

	move = { 0,0,-kCharacterSpeed };

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	vectorChange_->MyUpdate(worldTransform_);

	// 行列更新
	// 行列の転送
	worldTransform_.TransferMatrix();

	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}

	//キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 90);
	debugText_->Printf("EnemyPos:(%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::LeaveMove()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.1f;

	move = { -kCharacterSpeed ,kCharacterSpeed,0 };

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	vectorChange_->MyUpdate(worldTransform_);

	// 行列更新
	// 行列の転送
	worldTransform_.TransferMatrix();

	//キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 90);
	debugText_->Printf("EnemyPos:(%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::ApproachInitialize()
{
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;
}

void Enemy::ApproachUpdate()
{
	// 発射タイマーカウントダウン
	fireTimer_--;
	// 指定時間に達した
	if (fireTimer_ <= 3)
	{
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}

}

void Enemy::Update()
{
	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachMove();
		break;
	case Phase::Leave:
		LeaveMove();
		break;
	}

	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	ApproachUpdate();

	vectorChange_->MyUpdate(worldTransform_);


	// 行列更新
	// 行列の転送
	worldTransform_.TransferMatrix();

}

void Enemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	// 弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);


	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

	// 自キャラの座標をコピー
	Vector3 position = worldTransform_.translation_;

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initalize(model_, position, velocity);

	// 弾を登録する
	bullets_.push_back(std::move(newBullet));
}
