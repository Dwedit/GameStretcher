#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <d3d9.h>
#include "Region.h"

class CRenderPass
{
protected:
	IDirect3DDevice9* device;
	IDirect3DSurface9* depthStencilSurface;
	IDirect3DSurface9* renderTarget;
	IDirect3DTexture9* renderTargetTexture;
	IDirect3DTexture9* sourceTexture;
	IDirect3DTexture9* sourceTexture2;
	IDirect3DPixelShader9* pixelShader;
	IDirect3DVertexShader9* vertexShader;

	int scaleFactor;

	SIZE sourceTextureSize;
	SIZE renderTargetSize;
	RECT selectedInputRect;
	RECT selectedOutputRect;

	bool doBeginScene;

	void SetDeviceIfUnassigned(IDirect3DResource9* resource);
public:
	CRenderPass();
	~CRenderPass();
	void Dispose();

	void SetDevice(IDirect3DDevice9* device);
	void SetDepthStencilSurface(IDirect3DSurface9* depthStencilSurface);
	void SetRenderTarget(IDirect3DSurface9* renderTarget);
	void SetRenderTargetTexture(IDirect3DTexture9* renderTargetTexture);
	void SetSourceTexture(IDirect3DTexture9* sourceTexture);
	void SetSourceTexture2(IDirect3DTexture9* sourceTexture2);
	void SetPixelShader(IDirect3DPixelShader9* pixelShader);
	void SetVertexShader(IDirect3DVertexShader9* vertexShader);
	void SetInputRect(LPCRECT rect);
	void SetOutputRect(LPCRECT rect);
	void SetDoBeginScene(bool doBeginScene);
	void SetScaleFactor(int scaleFactor);

	HRESULT CreateRenderTargetTexture(int scaleFactor = -1);
	HRESULT CreateDepthStencilSurface(int scaleFactor = -1);

	IDirect3DDevice9* GetDevice();
	IDirect3DSurface9* GetDepthStencilSurface();
	IDirect3DSurface9* GetRenderTarget();
	IDirect3DTexture9* GetRenderTargetTexture();
	IDirect3DTexture9* GetSourceTexture();
	IDirect3DTexture9* GetSourceTexture2();
	IDirect3DPixelShader9* GetPixelShader();
	IDirect3DVertexShader9* GetVertexShader();
	bool GetInputRect(LPRECT rect) const;
	bool GetOutputRect(LPRECT rect) const;
	bool GetCurrentInputRect(LPRECT rect) const;
	bool GetCurrentOutputRect(LPRECT rect) const;
	bool GetDoBeginScene() const;
	int GetScaleFactor() const;

	HRESULT Render();
	HRESULT SetDefaultSettings();
	HRESULT SetDefaultSettings2();
	HRESULT SetUpdateRegionUnscaled(Region& region);
};
