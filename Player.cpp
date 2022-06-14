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

void Player::Update()
{
	// 移動ベクトルを変更する処理

	// キャラクター移動処理
	{
		// キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };

		// キャラクターの移動速さ
		const float kCharacterSpeed = 0.2f;

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
		worldTransform_.translation_.x = myMath_->MinNum(worldTransform_.translation_.x, +kMoveLimitX);
		worldTransform_.translation_.x = myMath_->MaxNum(worldTransform_.translation_.x, -kMoveLimitX);
		worldTransform_.translation_.y = myMath_->MinNum(worldTransform_.translation_.y, +kMoveLimitY);
		worldTransform_.translation_.y = myMath_->MaxNum(worldTransform_.translation_.y, -kMoveLimitY);

		vectorChange_->MyUpdate(worldTransform_);
		
	}

	
	// 行列更新
	// 行列の転送
	worldTransform_.TransferMatrix();

	 //キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 130);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);

}

void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

