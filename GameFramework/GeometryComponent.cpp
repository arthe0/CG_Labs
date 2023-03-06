#include "GeometryComponent.h"
#include "Engine.h"

GeometryComponent::GeometryComponent(Engine* g, GeometryShape Shape, DirectX::XMFLOAT4 position) 
	: GameComponent(g), Shape(Shape), position(position)
{
}

GeometryComponent::~GeometryComponent()
{
}

void GeometryComponent::DestroyResources()
{
	layout->Release();
	pixelShader->Release();
	pixelShaderByteCode->Release();
	rastState->Release();
	vertexShader->Release();
	vertexShaderByteCode->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
}

void GeometryComponent::Draw()
{
	engine->Context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(engine->Display->ClientWidth);
	viewport.Height = static_cast<float>(engine->Display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	engine->Context->RSSetViewports(1, &viewport);

	engine->Context->IASetInputLayout(layout);
	engine->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	engine->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	engine->Context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	engine->Context->VSSetShader(vertexShader, nullptr, 0);
	engine->Context->VSSetConstantBuffers(0, 1, &constBuffer);
	engine->Context->PSSetShader(pixelShader, nullptr, 0);

	engine->Context->DrawIndexed(Shape.GetIndecesSize(), 0, 0);
}

void GeometryComponent::Initialize()
{
	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(
		shaderPath,
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode
	);

	if (FAILED(res)) 
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) 
		{
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(engine->Display->hWnd, shaderName, L"Missing Shader File", MB_OK);
		}

		return;
	}

	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(
		shaderPath,
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorPixelCode
	);

	engine->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, 
		&vertexShader
	);

	engine->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, 
		&pixelShader
	);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = 
	{
		D3D11_INPUT_ELEMENT_DESC 
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	engine->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);


	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = Shape.PointsByteSize;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = Shape.Points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	engine->Device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = Shape.IndecesByteSize;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = Shape.Indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	engine->Device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	strides[0] = 32;
	offsets[0] = 0;

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = engine->Device->CreateRasterizerState(&rastDesc, &rastState);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = 0;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &position;

	engine->Device->CreateBuffer(&constBufDesc, &csd, &constBuffer);
}

void GeometryComponent::Update()
{
	engine->Context->UpdateSubresource(constBuffer, 0, 0, &position, 0, 0);
}

void GeometryComponent::Reload()
{
}

void GeometryComponent::SetPosX(float NewX) { position.x = NewX; }
void GeometryComponent::SetPosY(float NewY) { position.y = NewY; }
void GeometryComponent::SetPosition(DirectX::XMFLOAT4 NewPosition) { position = NewPosition; }

float GeometryComponent::GetPosX() { return position.x; }
float GeometryComponent::GetPosY() { return position.y; }
float GeometryComponent::GetPosZ() { return position.z; }

DirectX::XMFLOAT4 GeometryComponent::GetPosition() { return position; }

#pragma region GeometryShape
GeometryShape::GeometryShape(DirectX::XMFLOAT4* Points, int PointsSize, int* Indeces, int IndecesSize) : Points(Points), Indeces(Indeces)
{
	this->PointsByteSize = PointsSize * sizeof(DirectX::XMFLOAT4);
	this->IndecesByteSize = IndecesSize * sizeof(int);
}

int GeometryShape::GetPointsSize()
{
	return PointsByteSize / sizeof(DirectX::XMFLOAT4);
}

int GeometryShape::GetIndecesSize()
{
	return IndecesByteSize / sizeof(int);
}
#pragma endregion GeometryShape