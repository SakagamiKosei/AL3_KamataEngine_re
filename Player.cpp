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
	model_ = model;
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("player_shade.jpg");

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ワールド変換の初期化
	worldTransform_.Initialize();

	//vectorChange_ = new VectorChange();

	myMath_ = new MyMath();

}

void Player::Rotate()
{
	// 回転処理
	{
		// キャラクターの回転ベクトル
		Vector3 roteMove = { 0,0,0 };

		const float kChestRotSpeed = 0.02f;
		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) 
		{
			worldTransform_.rotation_.y += kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT)) 
		{
			worldTransform_.rotation_.y -= kChestRotSpeed;
		}

		debugText_->SetPos(50, 70);
		debugText_->Printf("PlayerRot:(%f,%f,%f)", worldTransform_.rotation_.x,
			worldTransform_.rotation_.y, worldTransform_.rotation_.z);

		// 回転限界座標
		/*const float kRoteLimitX = 45;*/
		const float kRoteLimitY = 45.0f;


		//// 座標移動(ベクトルの加算)
		//worldTransform_.rotation_ += roteMove;


		// 範囲を超えない処理
		/*worldTransform_.rotation_.x = myMath_->MinNum(worldTransform_.rotation_.x, +kRoteLimitX);
		worldTransform_.rotation_.x = myMath_->MaxNum(worldTransform_.rotation_.x, -kRoteLimitX);*/


		worldTransform_.rotation_.y = myMath_->Clamp(myMath_->ConvertToRadian(-kRoteLimitY), myMath_->ConvertToRadian(kRoteLimitY), worldTransform_.rotation_.y);

		//worldTransform_.translation_.y = myMath_->MinNum(worldTransform_.translation_.y, +kMoveLimitY);
		//worldTransform_.translation_.y = myMath_->MaxNum(worldTransform_.translation_.y, -kMoveLimitY);
	}
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE))
	{
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initalize(model_, worldTransform_.translation_);

		// 弾を登録する
		bullet_ = newBullet;
	}
}

void Player::Update()
{
	
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

		// 押した方向で移動ベクトルを変更
		// 左か右キーを押していたらmove(移動量)を変化させる
		if (input_->PushKey(DIK_LEFT))
		{
			move = { -kCharacterSpeed,0,0 };
		}
		else if (input_->PushKey(DIK_RIGHT))
		{
			move = { kCharacterSpeed,0,0 };
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


		// 座標移動(ベクトルの加算)
		worldTransform_.translation_ += move;


		// 範囲を超えない処理
	
		worldTransform_.translation_.x = myMath_->Clamp(-kMoveLimitX, kMoveLimitX,worldTransform_.translation_.x);

	}

	// キャラクター攻撃処理
	Attack();

	if (bullet_)
	{
		bullet_->Update();
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
	if (bullet_) {
		bullet_->Draw(viewProjection_);
	}
}

