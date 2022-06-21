#pragma once
#include <Model.h>
#include "VectorChange.h"
#include "DebugText.h"
/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	Enemy();
	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);
	void Move();
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
};