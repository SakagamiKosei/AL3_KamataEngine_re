#pragma once
#include "Model.h"
#include "WorldTransform.h"	
#include "DebugText.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "VectorChange.h"
#include "MyMath.h"
#include "PlayerBullet.h"
#include <memory.h>
#include <list>

class Player {

public:
	Player();
	~Player();
/// <summary>
///  初期化
/// </summary>
	void Initialize(Model* model,uint32_t textureHandle);
/// <summary>
///  更新
/// </summary>
    void Rotate();
	void Update();

/// <summary>
/// 攻撃
/// </summary>
	void Attack();

/// <summary>
///  描画
/// </summary>
	void Draw(ViewProjection &viewProjection_);

	/// <summary>
	/// 座標を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 半径を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetRadius();

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();
	// 弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets()
	{
		return playerBullets_;
	}

	void SetRailCamera(const WorldTransform& worldTransform) { worldTransform_.parent_ = &worldTransform; }


private:
	// ワールド変換行列データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 入力処理するため
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;
	// ベクトル変換
	VectorChange* vectorChange_ = nullptr;
	MyMath* myMath_ = nullptr;

	// 弾
	std::list<std::unique_ptr<PlayerBullet>>playerBullets_;
	
};

