#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <d3d9.h>
#include "Region.h"

#include "CRenderPass.h"


class CUpscaler2Impl
{
private:
	IDirect3DTexture9* sourceTexture;
	IDirect3DSurface9* sourceTextureSurface;
	IDirect3DDevice9* device;
	IDirect3DSwapChain9* defaultSwapChain;
	IDirect3DSwapChain9* selectedSwapChain;
	


public:
	CUpscaler2Impl();
	~CUpscaler2Impl();
	void Dispose();
	HRESULT SetSourceTexture(IDirect3DTexture9* texture);
	HRESULT SetSwapChain(IDirect3DSwapChain9* swapChain);
	HRESULT DrawFrame(const Region& updateRegion);
	bool SetDoBeginScene(bool doBeginScene);
	bool GetDoBeginScene() const;
	void SetInputRectangle(LPCRECT rect);
	void SetOutputRectangle(LPCRECT rect);
	void GetInputRectangle(LPRECT rect) const;
	void GetOutputRectangle(LPRECT rect) const;
	bool GetCurrentInputRectangle(LPRECT rect) const;
	bool GetCurrentOutputRectangle(LPRECT rect) const;
	void SetBorderDirty();
	void SetUpscaleFilterType(int upscaleFilterType);
	int GetUpscaleFilterType() const;
	int GetCurrentUpscaleFilterType() const;
	void SetHWND(HWND hwnd);
	HWND GetHWND() const;
	HWND GetCurrentHWND() const;
private:
	HRESULT DrawFrame();

};
