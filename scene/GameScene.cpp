#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <DirectXMath.h>
#include "Matrix4.h"

Matrix4 MyScale(WorldTransform& worldTransform_)
{
	//// X,Y,Z方向のスケーリングを設定(どれくらい引き伸ばすの？)
	//worldTransform_.scale_ = { 5,5,5 };
	// スケーリング行列を宣言(住所)
	Matrix4 matScale;

	// 単位行列に変換
	matScale.IdentityMatrix();

	// スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 MyRotationX(WorldTransform& worldTransform_)
{
	// 各軸用回転行列を宣言
	Matrix4 matRotX;

	// 単位行列に変換
	matRotX.IdentityMatrix();
	// X軸回転行列の各要素を設定する
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sinf(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sinf(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cosf(worldTransform_.rotation_.x);
	matRotX.m[3][3] = 1;

	return matRotX;
}

Matrix4 MyRotationY(WorldTransform& worldTransform_)
{
	// 各軸用回転行列を宣言
	Matrix4 matRotY;

	matRotY.IdentityMatrix();
	// Y軸回転行列の各要素を設定する
	matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sinf(worldTransform_.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sinf(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);
	matRotY.m[3][3] = 1;

	return matRotY;
}

Matrix4 MyRotationZ(WorldTransform& worldTransform_)
{
	// 各軸用回転行列を宣言
	Matrix4 matRotZ;

	matRotZ.IdentityMatrix();
	// Z軸回転行列の各要素を設定する
	matRotZ.m[0][0] = cosf(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sinf(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sinf(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cosf(worldTransform_.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	return matRotZ;
}

Matrix4 MyTranslation(WorldTransform& worldTransform_)
{

	//// 平行移動行列を宣言
	//Matrix4 matTrans = MathUtility::Matrix4Identity();

	Matrix4 matTrans;
	matTrans.IdentityMatrix();

	// 移動量を行列に設定する
	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	matTrans.m[3][3] = 1;

	return matTrans;

}

namespace My
{
	Matrix4 MyUpdate(WorldTransform& worldTransform_)
	{
		Matrix4 mat;
		mat.IdentityMatrix();
		worldTransform_.matWorld_ = mat;

		// スケーリング
		Matrix4 matScale = MyScale(worldTransform_);
		Matrix4 matRotationZ = MyRotationZ(worldTransform_);
		Matrix4 matRotationX = MyRotationX(worldTransform_);;
		Matrix4 matRotationY = MyRotationY(worldTransform_);
		Matrix4 matTrans = MyTranslation(worldTransform_);

		worldTransform_.matWorld_ *= matScale;

		worldTransform_.matWorld_ *= matRotationZ;


		// 回転角Z
		worldTransform_.matWorld_ *= matRotationZ;
		// 回転角X
		worldTransform_.matWorld_ *= matRotationX;
		// 回転角Y
		worldTransform_.matWorld_ *= matRotationY;
		// 平行移動
		worldTransform_.matWorld_ *= matTrans;

		// 行列の転送
		worldTransform_.TransferMatrix();

		return worldTransform_.matWorld_;
	}
}

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete debugCamera_;
}


void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 数学を生成
	myMath_ = new MyMath();

	// 各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;
#pragma endregion

#pragma region 初期化
	// 範囲forを使用した初期化
	// 範囲forで全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		// ワールドトランスフォームの初期化
		worldTransform.Initialize();

		// 行列の転送
		worldTransform.TransferMatrix();
	}

	viewProjection_.Initialize();

#pragma endregion

	//// 親(0番)
	//worldTransforms_[0].Initialize();
	//// 子(1番)
	//worldTransforms_[1].Initialize();
	//worldTransforms_[1].translation_ = { 0,4.5f,0 };
	//worldTransforms_[1].parent_ = &worldTransforms_[0];

	// キャラクターの大元
	worldTransforms_[PartId::kRoot].Initialize();
	// 脊椎
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].translation_ = { 0,4.5f,0 };

	// ---------------------------------------------------------------------
	// 上半身
	// ---------------------------------------------------------------------
	// 胸
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kChest].translation_ = { 0.0f, 5.0f, 0.0f };
	// 頭
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kHead].translation_ = { 0.0f, 3.0f, 0.0f };
	// 左腕
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmL].translation_ = { -3.0f,0.0f, 0.0f };
	// 右腕
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmR].translation_ = { 3.0f, 0.0f, 0 };

	// ----------------------------------------------------------------------
	// 下半身
	// ----------------------------------------------------------------------
	// 尻
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kHip].translation_ = { 0.0f, 2.0f, 0 };
	// 左足
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegL].translation_ = { -3.0f, -3.0f, 0 };
	// 右足
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegR].translation_ = { 3.0f, -3.0f, 0 };

	for (int i = 0; i < kNumPartId; i++)
	{
		My::MyUpdate(worldTransforms_[i]);
	}
	

}

void GameScene::Update()
{
	// デバックカメラの更新
	debugCamera_->Update();

#pragma region 視点移動

	// デバック用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"eye:(%f,%f,%f)", viewProjection_.eye.x,
		viewProjection_.eye.y,
		viewProjection_.eye.z
	);
#pragma endregion 

#pragma region 連続変更

#pragma endregion

	// キャラクター移動処理
	{
		// キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };

		// キャラクターの移動速さ
		const float kCharacterSpeed = 0.2f;

		// 押した方向で移動ベクトルを変更
		// 左か右キーを押していたらmove(移動量)を変化させる
		if (input_->PushKey(DIK_LEFT)) {
			move = { -kCharacterSpeed,0,0 };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move = { kCharacterSpeed,0,0 };
		}

		// 親の更新
		worldTransforms_[PartId::kRoot].translation_ += move;

		worldTransforms_[PartId::kRoot].translation_.x += move.x;
		worldTransforms_[PartId::kRoot].translation_.y += move.y;
		worldTransforms_[PartId::kRoot].translation_.z += move.z;

		My::MyUpdate(worldTransforms_[PartId::kRoot]);

		// 子の更新
		{
			for (int i = 0; i < kNumPartId; i++)
			{
				worldTransforms_[i].matWorld_ = My::MyUpdate(worldTransforms_[i]);
				if (worldTransforms_[i].parent_ != nullptr) {
					worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
				}
				worldTransforms_[i].TransferMatrix();
			}
		}

		// 上半身回転処理
		{
			const float kChestRotSpeed = 0.05f;
			// 押した方向で移動ベクトルを変更
			if (input_->PushKey(DIK_U)) {
				worldTransforms_[kChest].rotation_.y -= kChestRotSpeed;
			}
			else if (input_->PushKey(DIK_I)) {
				worldTransforms_[kChest].rotation_.y += kChestRotSpeed;
			}
		}

		// 下方向回転処理
		{
			const float kChestRotSpeed = 0.05f;
			// 押した方向で移動ベクトルを変更
			if (input_->PushKey(DIK_J)) {
				worldTransforms_[kHip].rotation_.y -= kChestRotSpeed;
			}
			else if (input_->PushKey(DIK_K)) {
				worldTransforms_[kHip].rotation_.y += kChestRotSpeed;
			}
		}




		// デバック用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
			"kRoot:(%f,%f,%f)", worldTransforms_[kRoot].translation_.x,
			worldTransforms_[kRoot].translation_.y,
			worldTransforms_[kRoot].translation_.z);
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// // 範囲forで全てのワールドトランスフォームを順に処理する
	//for (WorldTransform& worldTransform : worldTransforms_)
	//{
	//	// 3Dモデル描画
	//	model_->Draw(worldTransform, viewProjection_, textureHandle_);
	//}
	// 
	// for分で100個描画していた処理を置き換える
	//model_->Draw(worldTransforms_[PartId::kRoot], viewProjection_, textureHandle_);
	//model_->Draw(worldTransforms_[PartId::kSpine], viewProjection_, textureHandle_);

	for (int i = 2; i < kNumPartId; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}
	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
