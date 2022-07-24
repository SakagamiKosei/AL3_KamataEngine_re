#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "VectorChange.h"

/// <summary>
/// 天球
/// </summary>
class Skydome
{
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	VectorChange* vectorChange_ = nullptr;

public:
	Skydome();
	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);



	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);
};