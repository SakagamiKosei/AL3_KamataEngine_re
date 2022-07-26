#include "RailCamera.h"


RailCamera::RailCamera() {

}


RailCamera::~RailCamera()
{

}


void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//ワールドトランスフォームの初期設定
	// 引数でワールド座標を受け取ってワールドトランスフォームに設定
	worldTransform_.translation_ = position;
	// 引数で回転角[ラジアン]を受けとってワールドトランスフォームに設定
	/*worldTransform_.rotation_ = rotation*/

	//ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	worldTransform_.Initialize();

}

void RailCamera::Update()
{
	//ワールドトランスフォームの座標の数値を加算したりする(移動)
	worldTransform_.translation_ += Vector3(0, 0, 0.1f);
	//ワールドトランスフォームの角度の数値を加算したりする(回転)
   /* worldTransform_.rotation_+=Vector3(0,0.1f,0);*/

	vectorChange_->MyUpdate(worldTransform_);
	//ワールドトランスフォームのワールド行列の再計算
	//viewProjection_.UpdateMatrix();
	//ワールド行列の平行移動成分を取得
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	// ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	// レールカメラの回転を反映
	//forward = worldTransform_.matWorld_.transform(forward, worldTransform_.matWorld_);
	forward = MathUtility::Vector3TransformNormal(forward, worldTransform_.matWorld_);
	// 視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;
	// ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	// レールカメラの回転を反映(レールカメラの上方ベクトル)
	//viewProjection_.up = worldTransform_.matWorld_.transform(up, worldTransform_.matWorld_);
	viewProjection_.up = MathUtility::Vector3TransformNormal(up, worldTransform_.matWorld_);
	//ビュープロジェクションの更新と定数バッファへの転送
	viewProjection_.UpdateMatrix();


	DebugText* debugText_ = DebugText::GetInstance();

	// デバック用表示
	debugText_->SetPos(70, 300);
	debugText_->Printf("RailCamera Pos:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(70, 320);
	debugText_->Printf("Target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

}

//Vector3 RailCamera::GetWorldPosition()
//{
//	Vector3 worldPos;
//
//	worldPos.x = worldTransform_.translation_.x;
//	worldPos.y = worldTransform_.translation_.y;
//	worldPos.z = worldTransform_.translation_.z;
//
//	return worldPos;
//
//}
//
