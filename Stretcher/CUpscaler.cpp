#include "CUpscaler.h"
#include "CUpscalerImpl.h"

CUpscaler::CUpscaler() : impl(new CUpscalerImpl())
{

}
CUpscaler::~CUpscaler()
{
	Destroy();
}
void CUpscaler::Destroy()
{
	impl->Destroy();
}
bool CUpscaler::SetSourceTexture(void *texture) //IDirect3DTexture9
{
	return impl->SetSourceTexture((IDirect3DTexture9*)texture);
}
bool CUpscaler::SetSwapChain(void* swapChain) //IDirect3DSwapChain9
{
	return impl->SetSwapChain((IDirect3DSwapChain9*)swapChain);
}
void CUpscaler::SetDoBeginScene(bool doBeginScene)
{
	impl->SetDoBeginScene(doBeginScene);
}
void CUpscaler::SetViewRectangle(int x, int y, int width, int height)
{
	impl->SetViewRectangle(x, y, width, height);
}
void CUpscaler::SetInputRectangle(int x, int y, int width, int height)
{
	impl->SetInputRectangle(x, y, width, height);
}
bool CUpscaler::Update(const Region& region)
{
	impl->SetUpdateRegion(region);
	return impl->Update();
}
bool CUpscaler::Update()
{
	impl->SetBorderDirty();
	impl->SetUpdateRegion();
	return impl->Update();
}
void CUpscaler::SetBorderDirty()
{
	impl->SetBorderDirty();
}
bool CUpscaler::UpdateToTexture(int x, int y, int width, int height)
{
	return impl->UpdateToTexture(x, y, width, height);
}
bool CUpscaler::UpdateToBackBuffer(int x, int y, int width, int height)
{
	return impl->UpdateToBackBuffer(x, y, width, height);
}
bool CUpscaler::UpdateToScreen(int x, int y, int width, int height)
{
	HRESULT result = impl->UpdateToScreen(x, y, width, height);
	return SUCCEEDED(result);
}
void CUpscaler::SetUpscaleFilter(int upscaleFilter)
{
	impl->SetUpscaleFilter(upscaleFilter);
}
//void CUpscaler::SetUpdateRegion(const Region& region)
//{
//	impl->SetUpdateRegion(region);
//}

