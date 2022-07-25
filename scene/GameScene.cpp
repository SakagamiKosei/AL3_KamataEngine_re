#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"



GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete modelSkydome_;
	delete skydome_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// カメラ視点座標を設定
	viewProjection_.eye = { 0,0,-50 };

	// 自キャラの生成
	player_ = std::make_unique<Player>();

	player_->Initialize(model_, textureHandle_);

	// 敵の生成
	enemy_ = std::make_unique<Enemy>();

	enemy_->Initialize(model_, textureHandle_);

	// 敵の弾の生成
	enemyBullet_ = std::make_unique<EnemyBullet>();

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_.get());

	collider_ = new Collider();

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
}

void GameScene::CheckAllCollisions()
{
	// 判定対象の座標
	Vector3 p_pos, e_pos,pb_pos,eb_pos;

	// 判定対象の半径
	Vector3 p_radius, e_radius,pb_radius,eb_radius;

	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>
		& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>
		& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	p_pos = player_->GetWorldPosition();

	// 自キャラの半径
	p_radius = player_->GetRadius();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		// 敵弾の座標
		e_pos = bullet->GetWorldPosition();
		// 敵弾の半径
		e_radius = bullet->GetRadius();

		// 球と球の交差判定
		if (collider_->OnBallCollision(p_pos.x, p_pos.y, p_pos.z,p_radius.x,
			e_pos.x, e_pos.y, e_pos.z,e_radius.x) == true)
		{
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	e_pos = enemy_->GetWorldPosition();
	// 敵キャラの半径
	e_radius = enemy_->GetRadius();

	// 自弾と敵キャラ全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		// 自弾の座標
		pb_pos = bullet->GetWorldPosition();
		// 自弾の半径
		pb_radius = bullet->GetRadius();

		// 球と球の交差判定
		if (collider_->OnBallCollision(pb_pos.x, pb_pos.y, pb_pos.z, p_radius.x,
			e_pos.x, e_pos.y, e_pos.z, e_radius.x) == true)
		{
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
		}

	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定


	// 自弾と敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
		{
			// 自弾の座標
			pb_pos = playerBullet->GetWorldPosition();
			// 自弾の半径
			pb_radius = playerBullet->GetRadius();

			// 敵弾の座標
			eb_pos = enemyBullet->GetWorldPosition();
			// 敵弾の半径
			eb_radius = enemyBullet->GetRadius();

			// 球と球の交差判定
			if (collider_->OnBallCollision(pb_pos.x, pb_pos.y, pb_pos.z, pb_radius.x,
				eb_pos.x, eb_pos.y, eb_pos.z, eb_radius.x) == true)
			{
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion
}


void GameScene::Update()
{
	// デバッグ切り替えの処理
	if (input_->TriggerKey(DIK_P))
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	
	// カメラの処理
	if (isDebugCameraActive_)
	{
		// 軸方向表示の表示を有効にする
		AxisIndicator::GetInstance()->SetVisible(true);
		// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
		AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else
	{
		// 軸方向表示の表示を無効にする
		AxisIndicator::GetInstance()->SetVisible(false);
		// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
		AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}
	player_->Update();

	enemy_->Update();

	skydome_->Update();
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
	// スカイドームの描画
	skydome_->Draw(viewProjection_);
	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	enemy_->Draw(viewProjection_);

	


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
