#include "Enemy.h"
#include "VectorChange.h"
#include "Player.h"
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


	// 引数として受け取ったデータをメンバ変数に記録する
	// 3Dモデルの生成
	this->model_ = model;


	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("enemy_sentai.png");

	debugText_ = DebugText::GetInstance();
	// 敵の初期座標
	worldTransform_.translation_ = { 0,2,30 };
	// 敵のスケール
	worldTransform_.scale_ = { 1,1,1 };
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
		ApproachUpdate();
		// 敵の弾の座標を画面表示する処理
		debugText_->SetPos(50, 110);
		debugText_->Printf("Phase:Approach");
		break;
	case Phase::Leave:
		LeaveMove();
		// 敵の弾の座標を画面表示する処理
		debugText_->SetPos(50, 110);
		debugText_->Printf("Phase:Leave");
		break;
	}


	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}


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

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::Fire()
{
	assert(player_);

	// 弾の速度
	float kBulletSpeed = 0.02f;

	// 自キャラのワールド座標を取得する
	Vector3 playerVec = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	Vector3 enemyVec = GetWorldPosition();
	// 敵キャラ→自キャラの差分ベクトルを求める
	Vector3 Difference = playerVec;
	Difference -= enemyVec;
	//// ベクトルの正規化
	Vector3 normalize(Difference);

	//// ベクトルの長さを速さに合わせる
	Difference *= kBulletSpeed;

	Vector3 velocity(Difference);


	//// 速度ベクトルを自機の向きに合わせて回転させる
	//velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

	//// 自キャラの座標をコピー
	//Vector3 position = worldTransform_.translation_;

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initalize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(std::move(newBullet));



}
