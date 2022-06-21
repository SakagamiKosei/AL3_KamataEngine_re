#pragma once
#include <Model.h>
#include "VectorChange.h"
#include "DebugText.h"
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

	void Initialize(Model* model, uint32_t textureHandle);
	void ApproachMove();
	void LeaveMove();
	void Update();
	void Draw(ViewProjection& viewProjection_);


private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// デバックテキスト
	DebugText* debugText_ = nullptr;

	VectorChange* vectorChange_ = nullptr;

	// 初期フェーズ
	Phase phase_ = Phase::Approach;
};