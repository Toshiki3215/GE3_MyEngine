#include "Player.h"

Player::Player() {

}

Player::~Player() {

	//FBXオブジェクト解放
	delete playerObj;
	delete playerMD;
	delete shootObj_;
	delete shootModel_;
	delete hitboxObj_;
	delete hitboxModel_;
	delete retObj_;
	delete retModel_;
}

void Player::Initialize(DirectXInitialize* dxInit, Input* input) {
	// nullptrチェック
	assert(dxInit);
	assert(input);

	this->dxInit = dxInit;
	input_ = input;
	camTransForm = new Transform();

	// デバイスをセット
	/*FBXObject3d::SetDevice(dxCommon->GetDevice());*/
	// グラフィックスパイプライン生成
	//FBXObject3d::CreateGraphicsPipeline();

	//待機
	playerMD = Model::LoadFromOBJ("obj");
	playerObj = Object3d::Create();
	playerObj->SetModel(playerMD);
	playerObj->wtf.position = { 0.0f,-0.3f,0.0f };

	//自機の弾
	shootModel_ = Model::LoadFromOBJ("obj");
	shootObj_ = Object3d::Create();
	shootObj_->SetModel(shootModel_);
	shootObj_->wtf.position = { playerObj->wtf.position.x,playerObj->wtf.position.y + 0.07f, playerObj->wtf.position.z };
	shootObj_->wtf.scale = { 0.5f,0.5f,0.5f };

	//ヒットボックス可視化
	/*hitboxModel_ = Model::LoadFromOBJ("slash");
	hitboxObj_ = Object3d::Create();
	hitboxObj_->SetModel(hitboxModel_);
	hitboxObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };
	hitboxObj_->wtf.scale = { 0.05f,0.05f,0.05f };
	hitboxObj_->wtf.rotation = { 0.0f,80.0f,0.0f };*/

	//レティクル
	retModel_ = Model::LoadFromOBJ("obj2");
	retObj_ = Object3d::Create();
	retObj_->SetModel(retModel_);
	retObj_->wtf.scale = { 0.5f,0.5f,0.5f };
	retObj_->wtf.position = { -1.5f,1.0f,10.0f };
}

void Player::Update() 
{
	playerObj->Update();

	shootObj_->Update();
	/*hitboxObj_->wtf.position = { playerObj->wtf.position.x,playerObj->wtf.position.y + 0.1f, playerObj->wtf.position.z + 0.1f };
	hitboxObj_->Update();*/
	retObj_->Update();
	enemylen = retObj_->wtf.position - shootObj_->wtf.position;
	enemylen.nomalize();

	//プレイヤーの行動一覧
	PlayerAction();

}

void Player::Draw() 
{
	playerObj->Draw();

	if (isShootFlag == true) 
	{
		shootObj_->Draw();
	}

	retObj_->Draw();
}

void Player::PlayerAction()
{
	//移動(自機)
	if (input_->PushKey(DIK_W)) {
		playerObj->wtf.position.y += 0.05f;
	}
	else if (input_->PushKey(DIK_S)) {
		playerObj->wtf.position.y -= 0.05f;
	}
	else if (input_->PushKey(DIK_A)) {
		playerObj->wtf.position.x -= 0.05f;
	}
	else if (input_->PushKey(DIK_D)) {
		playerObj->wtf.position.x += 0.05f;
	}
	//移動(レティクル)
	/*if (input_->PushKey(DIK_UP)) {
		retObj_->wtf.position.y += 0.08f;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		retObj_->wtf.position.y -= 0.08f;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		retObj_->wtf.position.x -= 0.08f;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		retObj_->wtf.position.x += 0.08f;
	}*/
	//移動制限(自機とレティクル)
	/*if (playerObj->wtf.position.x >= 0.6f) {
		playerObj->wtf.position.x = 0.6f;
	}
	else if (playerObj->wtf.position.x <= -0.6f) {
		playerObj->wtf.position.x = -0.6f;
	}
	else if (playerObj->wtf.position.y >= 0.19f) {
		playerObj->wtf.position.y = 0.19f;
	}
	else if (playerObj->wtf.position.y <= -0.35f) {
		playerObj->wtf.position.y = -0.35f;
	}*/

	//弾発射
	float ShortSpeed = 0.05f;

	if (input_->PushKey(DIK_SPACE)) 
	{
		isShootFlag = true;
	}
	if (isShootFlag == true) 
	{
		shootObj_->wtf.position += enemylen;
		len = enemylen;
		len *= ShortSpeed;

	}
	else 
	{
		shootObj_->wtf.position = { playerObj->wtf.position.x,playerObj->wtf.position.y + 0.07f, playerObj->wtf.position.z };
	}
	if (shootObj_->wtf.position.z >= retObj_->wtf.position.z) 
	{
		isShootFlag = false;
	}

}

Vector3 Player::bVelocity(Vector3& velocity, Transform& worldTransform)
{
	Vector3 result = { 0,0,0 };

	//内積
	result.z = velocity.x * worldTransform.matWorld.m[0][2] +
		velocity.y * worldTransform.matWorld.m[1][2] +
		velocity.z * worldTransform.matWorld.m[2][2];

	result.x = velocity.x * worldTransform.matWorld.m[0][0] +
		velocity.y * worldTransform.matWorld.m[1][0] +
		velocity.z * worldTransform.matWorld.m[2][0];

	result.y = velocity.x * worldTransform.matWorld.m[0][1] +
		velocity.y * worldTransform.matWorld.m[1][1] +
		velocity.z * worldTransform.matWorld.m[2][1];

	return result;
}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;

	playerObj->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = playerObj->wtf.matWorld.m[3][0];
	worldPos.y = playerObj->wtf.matWorld.m[3][1];
	worldPos.z = playerObj->wtf.matWorld.m[3][2];

	return worldPos;
}

Vector3 Player::GetBulletWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 BulletWorldPos;

	shootObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos.x = shootObj_->wtf.matWorld.m[3][0];
	BulletWorldPos.y = shootObj_->wtf.matWorld.m[3][1];
	BulletWorldPos.z = shootObj_->wtf.matWorld.m[3][2];

	return BulletWorldPos;
}

Vector3 Player::GetRetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 RetWorldPos;

	shootObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	RetWorldPos.x = retObj_->wtf.matWorld.m[3][0];
	RetWorldPos.y = retObj_->wtf.matWorld.m[3][1];
	RetWorldPos.z = retObj_->wtf.matWorld.m[3][2];

	return RetWorldPos;
}
