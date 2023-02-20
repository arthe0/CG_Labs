#pragma once
#include "GameComponent.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

class TriangleComponent : public GameComponent
{

public:
	TriangleComponent(Game* g);
	~TriangleComponent();
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void Reload() override;

private:
	ID3D11InputLayout* layout;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode;
	DirectX::XMFLOAT4 points[8];
	ID3D11RasterizerState* rastState;
	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode;
	int indices[3];
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	UINT strides[1];
	UINT offsets[1];
};
