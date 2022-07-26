#include "Player.h"
#include <cassert>


Player::Player()
{

}
Player::~Player()
{
	delete vectorChange_;
	delete myMath_;
}

/// <summary>
/// 
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Player::Initialize(Model* model, uint32_t textureHandle)
{
    // NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	// 3Dモデルの生成
	this->model_ = model;
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("player_shade.jpg");

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ワールド変換の初期化
	worldTransform_.Initialize();

	myMath_ = new MyMath();

	worldTransform_.scale_ = { 1,1,1 };

}

void Player::Rotate()
{
	// 回転処理
	{
		// キャラクターの回転ベクトル
		Vector3 roteMove = { 0,0,0 };

		const float kChestRotSpeed = 0.02f;
		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_I)) 
		{
			worldTransform_.rotation_.y += kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_U)) 
		{
			worldTransform_.rotation_.y -= kChestRotSpeed;
		}

		// キャラクターの回転角度を画面表示する処理
		debugText_->SetPos(50, 70);
		debugText_->Printf("PlayerRot:(%f)", 
			worldTransform_.rotation_.y);

		// 回転限界座標
		const float kRoteLimitY = 45.0f;


		//// 座標移動(ベクトルの加算)
		

		// 範囲を超えない処理

	/*	worldTransform_.rotation_.y = myMath_->Clamp(myMath_->ConvertToRadian(-kRoteLimitY), myMath_->ConvertToRadian(kRoteLimitY), worldTransform_.rotation_.y);*/

	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);


		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

		// 自キャラの座標をコピー
		Vector3 position = worldTransform_.translation_;

		// 弾を生成し、初期化
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initalize(model_,position,velocity);

		// 弾を登録する
		playerBullets_.push_back(std::move(newBullet));

	}


}

void Player::Update()
{
	// デスフラグの立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
		return bullet->IsDead();
		});
	// キャラクター旋回処理
	// 旋回(回転)
	Rotate();

	// 移動ベクトルを変更する処理

	// キャラクター移動処理
	{
		// キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };

		// キャラクターの移動速さ
		const float kCharacterSpeed = 0.1f;
		const float kChestRotSpeed = 0.001f;

		// 押した方向で移動ベクトルを変更
		// 左か右キーを押していたらmove(移動量)を変化させる
		if (input_->PushKey(DIK_LEFT))
		{
			move = { -kCharacterSpeed,0,0 };
			worldTransform_.rotation_.y += kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT))
		{
			move = { kCharacterSpeed,0,0 };
			worldTransform_.rotation_.y -= kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_UP))
		{
			move = { 0,kCharacterSpeed,0 };
		}
		else if (input_->PushKey(DIK_DOWN))
		{
			move = { 0,-kCharacterSpeed,0 };
		}

		// 移動限界座標
		const float kMoveLimitX = 35;
		const float kMoveLimitY = 20;
		// 回転限界
		const float kRotLimitY = 0.5f;
		// 座標移動(ベクトルの加算)
		worldTransform_.translation_ += move;
	

		// 範囲を超えない処理
		worldTransform_.translation_.x = myMath_->Clamp(-kMoveLimitX, kMoveLimitX,worldTransform_.translation_.x);
		worldTransform_.rotation_.y = myMath_->Clamp(-kRotLimitY, kRotLimitY, worldTransform_.rotation_.y);
	}

	// キャラクター攻撃処理
	Attack();
	

	// 弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_)
	{
		bullet->Update();
	}

	vectorChange_->MyUpdate(worldTransform_);

	
	// 行列更新
	// 行列の転送
	worldTransform_.TransferMatrix();

	 //キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 50);
	debugText_->Printf("PlayerPos:(%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);

}


void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetRadius()
{
	Vector3 playerRadius;
	// 横の大きさを半分にして半径にする
	playerRadius.x = worldTransform_.scale_.x / 2;
	// 縦の大きさを半分にして半径にする
	playerRadius.y = worldTransform_.scale_.y / 2;
	// 奥行きの大きさを半分にして半径にする
	playerRadius.z = worldTransform_.scale_.z / 2;

	return playerRadius;
}

void Player::OnCollision()
{
	// 何もしない
}

Vector3 transform(const Vector3& velocity, const Matrix4& matWorld)
{
	Vector3 result;
	
		result.x = velocity.x * matWorld.m[0][0] + velocity.y * matWorld.m[1][0] + velocity.z * matWorld.m[2][0] + matWorld.m[3][0];
	    result.y = velocity.x * matWorld.m[0][1] + velocity.y * matWorld.m[1][1] + velocity.z * matWorld.m[2][1] + matWorld.m[3][1];
		result.z = velocity.x * matWorld.m[0][2] + velocity.y * matWorld.m[1][2] + velocity.z * matWorld.m[2][2] + matWorld.m[3][2];
	
	return result;
}