#include "CUpscaler2Impl.h"
#include "CUpscaler2.h"

using std::make_unique;

CUpscaler2::CUpscaler2()
{
	this->impl = make_unique<CUpscaler2Impl>();
}
CUpscaler2::~CUpscaler2()
{
	Dispose();
}
void CUpscaler2::Dispose()
{
	impl->Dispose();
}
HRESULT CUpscaler2::SetSourceTexture(IDirect3DTexture9* texture)
{
	impl->SetSourceTexture(texture);
}
HRESULT CUpscaler2::SetSwapChain(IDirect3DSwapChain9* swapChain)
{
	impl->SetSwapChain(swapChain);
}
HRESULT CUpscaler2::DrawFrame()
{
	return DrawFrame(NULL);
}
HRESULT CUpscaler2::DrawFrame(LPCRECT updateBox)
{
	Region updateRegion;
	if (updateBox == NULL)
	{
		RECT fullRect;
		GetCurrentInputRectangle(&fullRect);
		updateRegion.AddRectangle(fullRect);
	}
	else
	{
		updateRegion.AddRectangle(updateBox);
	}
	return DrawFrame(updateRegion);
}
HRESULT CUpscaler2::DrawFrame(const Region& updateRegion)
{
	return impl->DrawFrame(updateRegion);
}
void CUpscaler2::SetDoBeginScene(bool doBeginScene)
{
	impl->SetDoBeginScene(doBeginScene);
}
bool CUpscaler2::GetDoBeginScene() const
{
	return impl->GetDoBeginScene();
}
void CUpscaler2::SetInputRectangle(int x, int y, int width, int height)
{
	RECT rect{ x, y, x + width, y + height };
	SetInputRectangle(&rect);
}
void CUpscaler2::SetInputRectangle(LPCRECT rect)
{
	impl->SetInputRectangle(rect);
}
void CUpscaler2::SetOutputRectangle(int x, int y, int width, int height)
{
	RECT rect{ x, y, x + width, y + height };
	SetOutputRectangle(&rect);
}
void CUpscaler2::SetOutputRectangle(LPCRECT rect)
{
	impl->SetOutputRectangle(rect);
}
bool CUpscaler2::GetInputRectangle(LPRECT rect) const
{
	return impl->GetInputRectangle(rect);
}
bool CUpscaler2::GetOutputRectangle(LPRECT rect) const
{
	return impl->GetOutputRectangle(rect);
}
bool CUpscaler2::GetCurrentInputRectangle(LPRECT rect) const
{
	return impl->GetCurrentInputRectangle(rect);
}
bool CUpscaler2::GetCurrentOutputRectangle(LPRECT rect) const
{
	return impl->GetCurrentOutputRectangle(rect);
}
void CUpscaler2::SetBorderDirty()
{
	impl->SetBorderDirty();
}
void CUpscaler2::SetUpscaleFilterType(int upscaleFilterType)
{
	return impl->SetUpscaleFilterType(upscaleFilterType);
}
int CUpscaler2::GetUpscaleFilterType() const
{
	return impl->GetUpscaleFilterType();
}
int CUpscaler2::GetCurrentUpscaleFilterType() const
{
	return impl->GetCurrentUpscaleFilterType();
}
void CUpscaler2::SetHWND(void* hwnd)
{
	impl->SetHWND((HWND)hwnd);
}
void* CUpscaler2::GetHWND() const
{
	return (void*)impl->GetHWND();
}
void* CUpscaler2::GetCurrentHWND() const
{
	return (void*)impl->GetCurrentHWND();
}
