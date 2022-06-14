#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "VectorChange.h"

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet 
{
public:
	PlayerBullet();
	~PlayerBullet();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initalize(Model* model, const Vector3& position);

	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルのポインタ(借りてくるやつ)
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	VectorChange* vectorChange_ = nullptr;
};