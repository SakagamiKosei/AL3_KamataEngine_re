#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"
#include "VectorChange.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
	RailCamera();
	~RailCamera();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="rotation">初期角度</param>
	void Initialize(const Vector3& position, const Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//Vector3 GetWorldPosition();
	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//ワールド行列の取得
	const WorldTransform& GetWorldMatrix()const { return worldTransform_; }


private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	VectorChange* vectorChange_ = nullptr;
};