#pragma once
#include <memory>
using std::unique_ptr;

typedef long LONG;
typedef LONG HRESULT;
struct IDirect3DTexture9;
struct IDirect3DSwapChain9;
typedef struct tagRECT RECT, *LPRECT;
typedef const RECT *LPCRECT;

class Region;

class CUpscaler2Impl;
class CUpscaler2
{
	unique_ptr<CUpscaler2Impl> impl;
public:
	CUpscaler2();
	~CUpscaler2();
	void Dispose();
	HRESULT SetSourceTexture(IDirect3DTexture9* texture);
	HRESULT SetSwapChain(IDirect3DSwapChain9* swapChain);
	HRESULT DrawFrame();
	HRESULT DrawFrame(LPCRECT updateBox);
	HRESULT DrawFrame(const Region& updateRegion);
	void SetDoBeginScene(bool doBeginScene);
	bool GetDoBeginScene() const;
	void SetInputRectangle(int x, int y, int width, int height);
	void SetInputRectangle(LPCRECT rect);
	void SetOutputRectangle(int x, int y, int width, int height);
	void SetOutputRectangle(LPCRECT rect);
	bool GetInputRectangle(LPRECT rect) const;
	bool GetOutputRectangle(LPRECT rect) const;
	bool GetCurrentInputRectangle(LPRECT rect) const;
	bool GetCurrentOutputRectangle(LPRECT rect) const;
	void SetBorderDirty();
	void SetUpscaleFilterType(int upscaleFilterType);
	int GetUpscaleFilterType() const;
	int GetCurrentUpscaleFilterType() const;
	void SetHWND(void* hwnd);
	void* GetHWND() const;
	void* GetCurrentHWND() const;
};
