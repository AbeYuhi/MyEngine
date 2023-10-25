#include "Triangle.h"

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

/// <summary>
/// 静的メンバ変数の実体化
/// </summary>
ID3D12GraphicsCommandList* Triangle::sCommandList_ = nullptr;
Vector3 Triangle::sDefaultPos[3] = {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}};

void Triangle::PreDraw(ID3D12GraphicsCommandList* commandList) {
	//1フレーム前にPostDrawが呼ばれていなかったらエラー
	assert(Triangle::sCommandList_ == nullptr);
	//コマンドリストのセット
	sCommandList_ = commandList;

	//PSOManagerの取得
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//ViewPortの設定
	sCommandList_->RSSetViewports(1, psoManager->GetViewPort());
	//Scirssorの設定
	sCommandList_->RSSetScissorRects(1, psoManager->GetScissorRect());
	//パイプラインステートの設定
	sCommandList_->SetPipelineState(psoManager->GetPSO());
	//ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(psoManager->GetRootSignature());
	//プリミティブ形状を設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Triangle::PostDraw() {
	//コマンドリストを解除
	sCommandList_ = nullptr;
}

std::unique_ptr<Triangle> Triangle::Create(Vector3 pos[3]) {
	std::unique_ptr<Triangle> object = std::make_unique<Triangle>();
	object->Initialize(pos);

	return object;
}

void Triangle::Initialize(Vector3 pos[3]) {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//VertexResourceの生成
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * kVertexNumber);
	//MaterialResourceの生成
	materialResource_ = CreateBufferResource(sizeof(Material));
	//wvpResourceの生成
	transformMatrixResource_ = CreateBufferResource(sizeof(TransformMatrix));

	//リソースの先頭のアドレスを使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * kVertexNumber;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//左下
	vertexData_[0].position = { pos[0].x, pos[0].y, pos[0].z, 1.0f };
	vertexData_[0].texcoord = {0.0f, 1.0f};
	vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };
	//上
	vertexData_[1].position = { pos[1].x, pos[1].y, pos[1].z, 1.0f};
	vertexData_[1].texcoord = {0.5f, 0.0f};
	vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };
	//右下
	vertexData_[2].position = { pos[2].x, pos[2].y, pos[2].z, 1.0f};
	vertexData_[2].texcoord = {1.0f, 1.0f};
	vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };

	//Materialデータの記入
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//色の書き込み
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLightint = false;

	//wvpデータの記入
	transformMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transform_.matrix_));
	transform_.matrix_->World_ = MakeIdentity4x4();

	//トランスフォームの初期化
	transform_.data_.scale_ = { 1.0f, 1.0f, 1.0f };
	transform_.data_.rotate_ = { 0.0f, 0.0f, 0.0f };
	transform_.data_.translate_ = { 0.0f, 0.0f, 0.0f };
}

void Triangle::Update() {

	ImGui::Begin("Triangle");
	ImGui::SliderFloat3("translate", &transform_.data_.translate_.x, -10, 10);
	ImGui::SliderFloat3("rotate", &transform_.data_.rotate_.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::SliderFloat3("scale", &transform_.data_.scale_.x, -10, 10);
	ImGui::ColorEdit3("Color", &materialData_->color.x);
	ImGui::End();
}

void Triangle::Draw(Matrix4x4 viewProjectionMatrix, UINT textureName) {
	TextureManager* textureManager = TextureManager::GetInstance();

	//ワールドMatrixの更新
	transform_.UpdateWorld();
	//カメラ移動によるwvpの変化
	transform_.UpdateWVP(viewProjectionMatrix);

	//VBVの設定
	sCommandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//マテリアルCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(1, transformMatrixResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	sCommandList_->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureName));
	//描画
	sCommandList_->DrawInstanced(kVertexNumber, 1, 0, 0);
}