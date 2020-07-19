#include "CRenderPass.h"
#include "RectUtil.h"
#include "Win32Ex.h"

bool GetSizeFromResource(IDirect3DTexture9* texture, LPSIZE size)
{
	if (texture == NULL || size == NULL) return false;
	D3DSURFACE_DESC desc;
	HRESULT hr = texture->GetLevelDesc(0, &desc);
	if (SUCCEEDED(hr))
	{
		*size = { desc.Width, desc.Height };
		return true;
	}
	return false;
}

bool GetSizeFromResource(IDirect3DSurface9* surface, LPSIZE size)
{
	if (surface == NULL || size == NULL) return false;
	D3DSURFACE_DESC desc;
	HRESULT hr = surface->GetDesc(&desc);
	if (SUCCEEDED(hr))
	{
		*size = { desc.Width, desc.Height };
		return true;
	}
	return false;
}

template <class TDirect3DResource9>
IDirect3DDevice9* GetDeviceFromResource(TDirect3DResource9* resource)
{
	if (resource == NULL) return NULL;
	IDirect3DDevice9* device = NULL;
	HRESULT hr = resource->GetDevice(&device);
	if (SUCCEEDED(hr) && device != NULL)
	{
		device->Release();
		return device;
	}
	return NULL;
}

IDirect3DSurface9* GetSurfaceFromTexture(IDirect3DTexture9* texture)
{
	if (texture == NULL) return NULL;
	HRESULT hr;
	IDirect3DSurface9* surface = NULL;
	hr = texture->GetSurfaceLevel(0, &surface);
	if (SUCCEEDED(hr) && surface != NULL)
	{
		surface->Release();
		return surface;
	}
	return NULL;
}

CRenderPass::CRenderPass()
{
	this->device = NULL;
	this->depthStencilSurface = NULL;
	this->renderTarget = NULL;
	this->renderTargetTexture = NULL;
	this->sourceTexture = NULL;
	this->sourceTexture2 = NULL;
	this->pixelShader = NULL;
	this->vertexShader = NULL;

	this->scaleFactor = 0;

	this->sourceTextureSize = {};
	this->renderTargetSize = {};
	this->selectedInputRect = {};
	this->selectedOutputRect = {};

	bool doBeginScene = true;
}
CRenderPass::~CRenderPass()
{
	Dispose();
}
void CRenderPass::Dispose()
{
	SafeRelease(this->device);
	SafeRelease(this->depthStencilSurface);
	SafeRelease(this->renderTarget);
	SafeRelease(this->renderTargetTexture);
	SafeRelease(this->sourceTexture);
	SafeRelease(this->sourceTexture2);
	SafeRelease(this->pixelShader);
	SafeRelease(this->vertexShader);
	this->renderTargetSize = {};
	this->sourceTextureSize = {};
}

void CRenderPass::SetDeviceIfUnassigned(IDirect3DResource9* resource)
{
	if (this->device == NULL)
	{
		SetDevice(GetDeviceFromResource(resource));
	}
}

void CRenderPass::SetDevice(IDirect3DDevice9* device)
{
	if (this->device == device) return;
	SafeRelease(this->device);
	this->device = device;
	SafeAddRef(this->device);
}
void CRenderPass::SetDepthStencilSurface(IDirect3DSurface9* depthStencilSurface)
{
	if (this->depthStencilSurface == depthStencilSurface) return;
	SafeRelease(this->depthStencilSurface);
	this->depthStencilSurface = depthStencilSurface;
	SafeAddRef(this->depthStencilSurface);
	SetDeviceIfUnassigned(this->depthStencilSurface);
}
void CRenderPass::SetRenderTarget(IDirect3DSurface9* renderTarget)
{
	if (this->renderTarget == renderTarget) return;
	this->renderTargetSize = {};
	SafeRelease(this->renderTarget);
	this->renderTarget = renderTarget;
	SafeAddRef(this->renderTarget);
	SetDeviceIfUnassigned(this->renderTarget);
	GetSizeFromResource(this->renderTarget, &this->renderTargetSize);
}
void CRenderPass::SetRenderTargetTexture(IDirect3DTexture9* renderTargetTexture)
{
	if (this->renderTargetTexture == renderTargetTexture) return;
	SafeRelease(this->renderTarget);
	SafeRelease(this->renderTargetTexture);
	this->renderTargetTexture = renderTargetTexture;
	SafeAddRef(this->renderTargetTexture);
	SetDeviceIfUnassigned(this->renderTargetTexture);
	SetRenderTarget(GetSurfaceFromTexture(this->renderTargetTexture));
}
void CRenderPass::SetSourceTexture(IDirect3DTexture9* sourceTexture)
{
	if (this->sourceTexture == sourceTexture) return;
	this->sourceTextureSize = {};
	SafeRelease(this->sourceTexture);
	this->sourceTexture = sourceTexture;
	SafeAddRef(this->sourceTexture);
	SetDeviceIfUnassigned(this->sourceTexture);
	GetSizeFromResource(this->sourceTexture, &this->sourceTextureSize);
}
void CRenderPass::SetSourceTexture2(IDirect3DTexture9* sourceTexture2)
{
	if (this->sourceTexture2 == sourceTexture2) return;
	SafeRelease(this->sourceTexture2);
	this->sourceTexture2 = sourceTexture2;
	SafeAddRef(this->sourceTexture2);
	SetDeviceIfUnassigned(this->sourceTexture2);
}
void CRenderPass::SetPixelShader(IDirect3DPixelShader9* pixelShader)
{
	if (this->pixelShader == pixelShader) return;
	SafeRelease(this->pixelShader);
	this->pixelShader = pixelShader;
	SafeAddRef(this->pixelShader);
	if (this->device == NULL) SetDevice(GetDeviceFromResource(this->pixelShader));
}
void CRenderPass::SetVertexShader(IDirect3DVertexShader9* vertexShader)
{
	if (this->vertexShader == vertexShader) return;
	SafeRelease(this->vertexShader);
	this->vertexShader = vertexShader;
	SafeAddRef(this->vertexShader);
	if (this->device == NULL) SetDevice(GetDeviceFromResource(this->vertexShader));
}
void CRenderPass::SetInputRect(LPCRECT rect)
{
	if (rect != NULL && RectHasArea(rect))
	{
		this->selectedInputRect = *rect;
	}
	else
	{
		this->selectedInputRect = {};
	}
}
void CRenderPass::SetOutputRect(LPCRECT rect)
{
	if (rect != NULL && RectHasArea(rect))
	{
		this->selectedOutputRect = *rect;
	}
	else
	{
		this->selectedOutputRect = {};
	}
}
void CRenderPass::SetDoBeginScene(bool doBeginScene)
{
	this->doBeginScene = doBeginScene;
}
void CRenderPass::SetScaleFactor(int scaleFactor)
{
	this->scaleFactor = scaleFactor;
}

HRESULT CRenderPass::CreateRenderTargetTexture(int scaleFactor)
{
	if (this->device == NULL) return E_POINTER;
	if (this->sourceTexture == NULL) return E_POINTER;

	if (scaleFactor == -1)
	{
		scaleFactor = GetScaleFactor();
	}
	else
	{
		SetScaleFactor(scaleFactor);
	}

	SafeRelease(this->renderTargetTexture);
	SafeRelease(this->renderTarget);
	this->renderTargetSize = {};

	if (scaleFactor <= 0)
	{
		return E_INVALIDARG;
	}

	D3DSURFACE_DESC desc;
	HRESULT hr;
	hr = this->sourceTexture->GetLevelDesc(0, &desc);
	if (SUCCEEDED(hr))
	{
		IDirect3DTexture9* newTexture = NULL;
		hr = this->device->CreateTexture(desc.Width * scaleFactor, desc.Height * scaleFactor, 1, D3DUSAGE_RENDERTARGET, desc.Format, D3DPOOL_DEFAULT, &newTexture, NULL);
		if (SUCCEEDED(hr) && newTexture != NULL)
		{
			SetRenderTargetTexture(newTexture);
			hr = 0;
			IDirect3DSurface9* oldRenderTarget;
			hr |= device->GetRenderTarget(0, &oldRenderTarget);
			hr |= device->SetRenderTarget(0, this->renderTarget);
			hr |= device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 255), 0, 0);
			hr |= device->SetRenderTarget(0, oldRenderTarget);
			SafeRelease(oldRenderTarget);
		}
		else
		{
			return hr;
		}
		SafeRelease(newTexture);
		return hr;
	}
	return hr;
}
HRESULT CRenderPass::CreateDepthStencilSurface(int scaleFactor)
{
	if (this->device == NULL) return E_POINTER;
	if (this->sourceTexture == NULL) return E_POINTER;

	if (scaleFactor == -1)
	{
		scaleFactor = GetScaleFactor();
	}
	else
	{
		SetScaleFactor(scaleFactor);
	}
	SafeRelease(this->depthStencilSurface);
	if (scaleFactor <= 0)
	{
		return E_INVALIDARG;
	}

	D3DSURFACE_DESC desc;
	HRESULT hr;
	hr = this->sourceTexture->GetLevelDesc(0, &desc);
	if (SUCCEEDED(hr))
	{
		IDirect3DSurface9* newSurface = NULL;
		hr = this->device->CreateDepthStencilSurface(desc.Width * scaleFactor, desc.Height * scaleFactor, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, false, &newSurface, NULL);
		if (SUCCEEDED(hr) && newSurface != NULL)
		{
			SetDepthStencilSurface(newSurface);
			hr = 0;
			IDirect3DSurface9* oldDepthStencil = NULL;
			hr |= this->device->GetDepthStencilSurface(&oldDepthStencil);
			hr |= this->device->SetDepthStencilSurface(this->depthStencilSurface);
			hr |= this->device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			hr |= this->device->SetDepthStencilSurface(oldDepthStencil);
			SafeRelease(oldDepthStencil);
		}
		else
		{
			return hr;
		}
		SafeRelease(newSurface);
		return hr;
	}
	return hr;
}

IDirect3DDevice9* CRenderPass::GetDevice()
{
	SafeAddRef(this->device);
	return this->device;
}
IDirect3DSurface9* CRenderPass::GetDepthStencilSurface()
{
	SafeAddRef(this->depthStencilSurface);
	return this->depthStencilSurface;
}
IDirect3DSurface9* CRenderPass::GetRenderTarget()
{
	SafeAddRef(this->renderTarget);
	return this->renderTarget;
}
IDirect3DTexture9* CRenderPass::GetRenderTargetTexture()
{
	SafeAddRef(this->renderTargetTexture);
	return this->renderTargetTexture;
}
IDirect3DTexture9* CRenderPass::GetSourceTexture()
{
	SafeAddRef(this->sourceTexture);
	return this->sourceTexture;
}
IDirect3DTexture9* CRenderPass::GetSourceTexture2()
{
	SafeAddRef(this->sourceTexture2);
	return this->sourceTexture2;
}
IDirect3DPixelShader9* CRenderPass::GetPixelShader()
{
	SafeAddRef(this->pixelShader);
	return this->pixelShader;
}
IDirect3DVertexShader9* CRenderPass::GetVertexShader()
{
	SafeAddRef(this->vertexShader);
	return this->vertexShader;
}
bool CRenderPass::GetInputRect(LPRECT rect) const
{
	if (rect != NULL)
	{
		*rect = this->selectedInputRect;
		return RectHasArea(rect);
	}
	return false;
}
bool CRenderPass::GetOutputRect(LPRECT rect) const
{
	if (rect != NULL)
	{
		*rect = this->selectedOutputRect;
		return RectHasArea(rect);
	}
	return false;
}
bool CRenderPass::GetCurrentInputRect(LPRECT rect) const
{
	if (rect != NULL)
	{
		*rect = this->selectedInputRect;
		return RectHasArea(rect);
	}
	return false;
}
bool CRenderPass::GetCurrentOutputRect(LPRECT rect) const
{
	if (rect != NULL)
	{
		*rect = this->selectedOutputRect;
		return RectHasArea(rect);
	}
	return false;
}
bool CRenderPass::GetDoBeginScene() const
{
	return this->doBeginScene;
}
int CRenderPass::GetScaleFactor() const
{
	return this->scaleFactor;
}

HRESULT CRenderPass::Render()
{

}
HRESULT CRenderPass::SetDefaultSettings()
{
	HRESULT hr = 0;
	hr |= device->SetRenderState(D3DRS_LIGHTING, false);
	hr |= device->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);
	hr |= device->SetRenderState(D3DRS_STENCILENABLE, false);
	hr |= device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	hr |= device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	hr |= device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	hr |= device->SetRenderState(D3DRS_STENCILENABLE, false);
	return hr;
}
HRESULT CRenderPass::SetDefaultSettings2()
{
	HRESULT hr = 0;
	hr |= device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, true);
	hr |= device->SetSamplerState(1, D3DSAMP_SRGBTEXTURE, true);
	hr |= device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	hr |= device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	hr |= device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	hr |= device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	hr |= device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	if (this->scaleFactor != 0)
	{
		hr |= device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		hr |= device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		hr |= device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		hr |= device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}
	else
	{
		hr |= device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		hr |= device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		hr |= device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		hr |= device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	}
	return hr;
}
HRESULT CRenderPass::SetUpdateRegionUnscaled(Region& region)
{

}
