﻿#ifndef GAMEAPP_H
#define GAMEAPP_H
#include "d3dApp.h"
#include "TextureRender.h"
#include <memory>
class GameApp : public D3DApp
{
public:

	struct VertexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	};

	struct CB
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};


	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
	bool InitEffect();
	bool InitResource();

private:
	std::unique_ptr<TextureRender> m_pGameContent;
	std::unique_ptr<TextureRender> m_pRayTracingContent;

	ComPtr<ID3D11InputLayout> m_pVertexLayout;	    // 顶点输入布局
	VertexBuffer<VertexPosColor>		m_pVertexBuffer;
	IndexBuffer<DWORD>				m_pIndexBuffer;
	ConstantBuffer<CB>		m_pConstantBuffer;
// 	ComPtr<ID3D11Buffer> m_pVertexBuffer;			// 顶点缓冲区
// 	ComPtr<ID3D11Buffer> m_pIndexBuffer;			// 索引缓冲区
// 	ComPtr<ID3D11Buffer> m_pConstantBuffer;		    // 常量缓冲区

	ComPtr<ID3D11VertexShader> m_pVertexShader;	    // 顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader;		// 像素着色器
	CB m_CBuffer;	                    // 用于修改GPU常量缓冲区的变量

};


#endif