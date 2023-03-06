#pragma once
#include "GameComponent.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

struct GeometryShape
{
	DirectX::XMFLOAT4* Points;
	int* Indeces;

	size_t PointsByteSize;
	size_t IndecesByteSize;

	GeometryShape(DirectX::XMFLOAT4* Points, int PointsSize, int* Indeces, int IndecesSize);

	int GetPointsSize() ;
	int GetIndecesSize();
};

class GeometryComponent : public GameComponent
{

public:
	GeometryComponent(Engine* g, GeometryShape Shape, DirectX::XMFLOAT4 position);
	~GeometryComponent();
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void Reload() override;

	virtual void SetPosX(float NewX);
	virtual void SetPosY(float NewY);
	virtual void SetPosition(DirectX::XMFLOAT4 NewPosition);

	float GetPosX ();
	float GetPosY ();
	float GetPosZ ();
	DirectX::XMFLOAT4 GetPosition ();

	DirectX::XMFLOAT4 position;
protected:
	LPCTSTR shaderPath = L"./Shaders/QuadShader.hlsl";
	LPCTSTR shaderName = L"QuadShader.hlsl";

	
	GeometryShape Shape;

private:
	ID3D11InputLayout* layout;

	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode;

	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode;

	ID3D11RasterizerState* rastState;
	
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constBuffer;

	UINT strides[1];
	UINT offsets[1];
};

namespace Shapes
{
	const GeometryShape S_Triangle
	(
		new DirectX::XMFLOAT4[]
		{
			DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		},
		6,
		new int[] { 0,1,2 },
		3
	);

	const GeometryShape S_Square
	(
		new DirectX::XMFLOAT4[]
		{
			DirectX::XMFLOAT4(0.01f, 0.01f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(-0.01f, -0.01f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(0.01f, -0.01f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(-0.01f, 0.01f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		},
		8,
		new int[] { 0,1,2, 1,0,3 },
		6
	);

	const GeometryShape S_Raket
	(
		new DirectX::XMFLOAT4[]
		{
			DirectX::XMFLOAT4(0.015f, 0.15f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(-0.015f, -0.15f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(0.015f, -0.15f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(-0.015f, 0.15f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		},
		8,
		new int[] { 0, 1, 2, 1, 0, 3 },
		6
	);
}