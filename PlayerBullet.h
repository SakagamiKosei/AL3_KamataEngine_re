#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "VectorChange.h"
#include "DebugText.h"

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
	void Initalize(Model*model,const Vector3& position,const Vector3& velocity);

	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	bool IsDead()const { return isDead_; }
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルのポインタ(借りてくるやつ)
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	VectorChange* vectorChange_ = nullptr;

	// 速度
	Vector3 velocity_;
	// デバックテキスト
	DebugText* debugText_ = nullptr;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

};