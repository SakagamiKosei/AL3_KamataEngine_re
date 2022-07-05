#pragma once
#include "Model.h"
#include "DebugText.h"
#include "EnemyBullet.h"

// 自機クラスの前方宣言
class Player;


/// <summary>
/// 敵
/// </summary>
// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave, // 離脱する
};

class Enemy
{
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* model, uint32_t textureHandle);
	/// <summary>
	/// 接近
	/// </summary>
	void ApproachMove();
	/// <summary>
	/// 離れる
	/// </summary>
	void LeaveMove();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection_"></param>
	void Draw(ViewProjection& viewProjection_);
	/// <summary>
	///	弾発射
	/// </summary>
	void Fire();
/// <summary>
/// 接近フェーズ初期化
	void ApproachInitialize();
/// </summary>

/// <summary>
/// 接近フェーズの更新関数
/// </summary>
	void ApproachUpdate();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

public:
	// 発射間隔
	static const int kFireInterval = 60;

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// デバックテキスト
	DebugText* debugText_ = nullptr;
	// ベクトル変換
	VectorChange* vectorChange_ = nullptr;
	// 弾
	std::list<std::unique_ptr<EnemyBullet>>bullets_;
	// 初期フェーズ
	Phase phase_ = Phase::Approach;
	// 発射タイマー
	int32_t fireTimer_ = 0;
	// 自キャラ
	Player* player_ = nullptr;
};