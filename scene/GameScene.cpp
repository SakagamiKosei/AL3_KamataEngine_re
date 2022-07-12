#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"



GameScene::GameScene() {}

GameScene::~GameScene()
{

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	// 3Dモデルの生成
	model_ = Model::Create();
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
}

void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 判定対象AとBの半径
	Vector3 radiusA, radiusB;

	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>
		& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>
		& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラの半径
	radiusA = player_->GetRadius();

	

	//// 敵の弾の座標
	//posB = enemyBullet_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		float distance = (posB.x - posA.x) * (posB.x - posA.x)
			+ (posB.y - posA.y) * (posB.y - posA.y)
			+ (posB.z - posA.z) * (posB.z - posA.z);

		float radius = (radiusA.x + radiusB.x) * (radiusA.x + radiusB.x);
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 敵弾の半径
		radiusB = bullet->GetRadius();

		// 球と球の交差判定
		if (collider_->OnBallCollision(posA.x, posA.y, posA.z,radiusA.x,
			posB.x, posB.y, posB.z,radiusB.x) == true)
		{
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();

			// デバック用表示
			debugText_->SetPos(50, 170);
			debugText_->Printf(
				"当たり");
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
	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	enemy_->Draw(viewProjection_);

	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 判定対象AとBの半径
	Vector3 radiusA, radiusB;

	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>
		& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>
		& enemyBullets = enemy_->GetBullets();

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラの半径
	radiusA = player_->GetRadius();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 敵キャラの半径
		radiusB = bullet->GetRadius();
	
	}

	float distance = (posB.x - posA.x) * (posB.x - posA.x)
		+ (posB.y - posA.y) * (posB.y - posA.y)
		+ (posB.z - posA.z) * (posB.z - posA.z);

	float radius = (radiusA.x + radiusB.x) * (radiusA.x + radiusB.x);


	// デバック用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"distance:(%f)", distance);

	// デバック用表示
	debugText_->SetPos(50, 170);
	debugText_->Printf(
		"radius:(%f)", radius);

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
