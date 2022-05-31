#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <DirectXMath.h>

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

#pragma region 乱数の準備
	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	// 乱数範囲の指定
	std::uniform_real_distribution<float>dist(-10.0f, 10.0f);
	// 乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float value = dist(engine);
	// 回転角用の乱数範囲を設定
	std::uniform_real_distribution<float>rotDist(0.0f, MathUtility::PI);
	// 座標用の乱数範囲を設定
	std::uniform_real_distribution<float>posDist(-10.0f, 10.0f);
#pragma endregion

#pragma region 初期化
	// 範囲forを使用した初期化
	// 範囲forで全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		// ワールドトランスフォームの初期化
		worldTransform.Initialize();
		// X,Y,Z 方向のスケーリングを設定
		worldTransform.scale_ = { 1,1,1 };
		// X,Y,Z 軸周りの回転角を乱数で設定
		worldTransform.rotation_ = { rotDist(engine),rotDist(engine), rotDist(engine) };
		// X,Y,Z 軸周りの平行移動(座標)を乱数で設定
		worldTransform.translation_ = { posDist(engine),posDist(engine),
			posDist(engine) };
		// スケーリング行列を宣言
		Matrix4 matScale;
		// 各軸用回転行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;
		// 平行移動行列を宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();
		// Z軸回転行列の各要素を設定する
		matRotZ.m[0][0] = cosf(worldTransform.rotation_.z);
		matRotZ.m[0][1] = sinf(worldTransform.rotation_.z);
		matRotZ.m[1][0] = -sinf(worldTransform.rotation_.z);
		matRotZ.m[1][1] = cosf(worldTransform.rotation_.z);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;
		// X軸回転行列の各要素を設定する
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(worldTransform.rotation_.x);
		matRotX.m[1][2] = sinf(worldTransform.rotation_.x);
		matRotX.m[2][1] = -sinf(worldTransform.rotation_.x);
		matRotX.m[2][2] = cosf(worldTransform.rotation_.x);
		matRotX.m[3][3] = 1;
		// Y軸回転行列の各要素を設定する
		matRotY.m[0][0] = cosf(worldTransform.rotation_.y);
		matRotY.m[0][2] = -sinf(worldTransform.rotation_.y);
		matRotY.m[1][1] = 1;
		matRotY.m[2][0] = sinf(worldTransform.rotation_.y);
		matRotY.m[2][2] = cosf(worldTransform.rotation_.y);
		matRotY.m[3][3] = 1;
		// スケーリング倍率を行列に設定する
		matScale.m[0][0] = worldTransform.scale_.x;
		matScale.m[1][1] = worldTransform.scale_.y;
		matScale.m[2][2] = worldTransform.scale_.z;
		matScale.m[3][3] = 1;
		// 移動量を行列に設定する
		matTrans.m[0][0] = 1;
		matTrans.m[1][1] = 1;
		matTrans.m[2][2] = 1;
		matTrans.m[3][0] = worldTransform.translation_.x;
		matTrans.m[3][1] = worldTransform.translation_.y;
		matTrans.m[3][2] = worldTransform.translation_.z;
		matTrans.m[3][3] = 1;
		//スケーリング・回転・平行移動を合成した行列を計算
		// ワールド行列に単位行列を代入する
		worldTransform.matWorld_ = Matrix4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		// ワールド行列にスケール行列を掛け算して代入する
		worldTransform.matWorld_ *= matScale;
		// ワールド行列に回転行列を掛け算して代入する
		worldTransform.matWorld_ *= matRotZ;
		worldTransform.matWorld_ *= matRotX;
		worldTransform.matWorld_ *= matRotY;
		// ワールド行列に平行移動を掛け算して代入する
		worldTransform.matWorld_ *= matTrans;
		// 行列の転送
		worldTransform.TransferMatrix();
	}
#pragma endregion

#pragma region 垂直方向視野角
	// カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = DirectX::XMConvertToRadians(10.0f);
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
#pragma endregion

#pragma region アスペクト比
	// アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
#pragma endregion

#pragma region ニアクリップ,ファークリップ
	// ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;
	// ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

#pragma endregion

	// 親(0番)
	worldTransforms_[0].Initialize();
	// 子(1番)
	worldTransforms_[1].Initialize();
	worldTransforms_[1].translation_ = { 0,4.5f,0 };
	worldTransforms_[1].parent_ = &worldTransforms_[0];


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
	// Fov変更処理
	{
		// 上キーで視野角が広がる
		if (input_->PushKey(DIK_UP))
		{
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, MathUtility::PI);
			// 下キーで視野角が狭まる
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}

		// 行列の再計算
		viewProjection_.UpdateMatrix();
		
		// デバック用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngleY(Degree):%f", DirectX::XMConvertToDegrees(viewProjection_.fovAngleY));

	}

	// クリップ距離変更処理
	{
		// 上下キーでニアクリップ距離を増減
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.nearZ += 0.1f;
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.1f;
		}

		// 行列の再計算
		viewProjection_.UpdateMatrix();

		// デバック用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	}
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

		// translationにmoveを加算する(移動)
		worldTransforms_[0].translation_ += move;
		// ワールド行列を計算する
		worldTransforms_[0].matWorld_ *= worldTransforms_[0].matWorld_;
		//// ワールド行列を転送
		//worldTransforms_[0].UpdateMatrix();

		// デバック用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
			"Root:(%f,%f,%f)", worldTransforms_[0].translation_.x,
			worldTransforms_[0].translation_.y,
			worldTransforms_[0].translation_.z
		);

		
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
	///// // 範囲forで全てのワールドトランスフォームを順に処理する
	//for (WorldTransform& worldTransform : worldTransforms_)
	//{
	//	// 3Dモデル描画
	//	model_->Draw(worldTransform,viewProjection_, textureHandle_);
	//}
	// 
	// for分で100個描画していた処理を置き換える
	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);


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
