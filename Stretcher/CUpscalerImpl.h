#pragma once

#define USE_SHADER_MODEL_2 0
#define NO_CHECKERBOARD 1
#define NO_DIRTY_RECT 0
#define DIRTY_RECT_NO_Z_BUFFER 0
#define USE_STATE_SAVER 0

template <class T>
static inline void SafeRelease(T &ptr) { if (ptr != NULL) { ptr->Release(); ptr = NULL; } }
template <class T>
static inline void SafeAddRef(T &ptr) { if (ptr != NULL) { ptr->AddRef(); } }

#define NOMINMAX
struct IUnknown;
#include <Windows.h>
#include <d3d9.h>
#include <vector>
#include <algorithm>
using std::min;
using std::max;

#include "Region.h"
#include "RectUtil.h"

#define ARRSIZE(xxxx) sizeof((xxxx)) / sizeof((xxxx)[0])

namespace shaderPrograms
{
#include "shaderPrograms.h"
}

#if USE_SHADER_MODEL_2
namespace shaderPrograms2
{
#include "shaderPrograms2.h"
}
#endif

static const D3DMATRIX identityMatrix =
{
	{ 1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 }
};

#if !USE_STATE_SAVER
//dummy version
class CStateSaver
{
public:
	CStateSaver(IDirect3DDevice9* device)
	{

	}
};
#else

class CStateSaver
{
	IDirect3DStateBlock9 *stateBlock;
	IDirect3DSurface9 *renderTarget;
	IDirect3DSurface9 *depthStencilSurface;
	IDirect3DDevice9 *device;
public:
	CStateSaver(IDirect3DDevice9* device)
	{
		this->device = device;
		SafeAddRef(device);
		stateBlock = NULL;
		if (device == NULL) return;
		HRESULT hr;
		hr = device->CreateStateBlock(D3DSBT_ALL, &stateBlock);
		hr = device->GetDepthStencilSurface(&depthStencilSurface);
		hr = device->GetRenderTarget(0, &renderTarget);
	}
	~CStateSaver()
	{
		HRESULT hr;
		hr = device->SetRenderTarget(0, renderTarget);
		hr = device->SetDepthStencilSurface(depthStencilSurface);
		SafeRelease(depthStencilSurface);
		SafeRelease(renderTarget);
		if (stateBlock != NULL)
		{
			hr = stateBlock->Apply();
		}
		SafeRelease(stateBlock);
		SafeRelease(device);
	}
};
#endif

struct Vertex
{
public:
	static const int FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	float x, y, z;
	//D3DCOLOR color;
	float u, v;
	Vertex() : x(0), y(0), z(0.5), u(0), v(0)//, color(D3DCOLOR_ARGB(255, 255, 255, 255))
	{

	}
	Vertex(float x, float y, float z, float u, float v) :
		x(x), y(y), z(z), u(u), v(v)//, color(D3DCOLOR_ARGB(255, 255, 255, 255))
	{

	}
};

struct ShaderInput
{
	float VideoWidth, VideoHeight;
	float TextureWidth, TextureHeight;
	float OutputWidth, OutputHeight;
	float FrameCount;
	float FrameDirection;
	float FrameRotation;

	void SetDimensions(int inputWidth, int inputHeight, int outputWidth, int outputHeight)
	{
		VideoWidth = (float)outputWidth;
		VideoHeight = (float)outputHeight;
		OutputWidth = (float)outputWidth;
		OutputHeight = (float)outputHeight;
		TextureWidth = (float)inputWidth;
		TextureHeight = (float)inputHeight;
	}
	void SetDimensions(int width, int height)
	{
		SetDimensions(width, height, width, height);
	}
};

static const D3DVERTEXELEMENT9 VertexFormat[] =
{
	{ 0, offsetof(Vertex,x), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, offsetof(Vertex,u), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END(),
};

class CUpscalerImpl
{
private:
	bool SetViewMatrix(int renderTargetWidth, int renderTargetHeight, float xPos, float yPos, float scaleX, float scaleY, float modelWidth, float modelHeight, float zOverride = -1, bool addPointFive = true)
	{
		//without half pixel correction:
		// pixel to normalized:
		//         pixelX = xPos + modelX * scaleX
		// pixelX_flipped = xPos + (modelW - modelX) * scaleX
		//          normX = pixelX / surfaceW
		// normalized to NDC
		//           ndcX = normX * 2 - 1
		//
		//        ndcX = modelX * 2 * scaleX / surfaceW + 2 * xPos / surfaceW - 1
		//ndcX_flipped = modelX * -2 * scaleX / surfaceW + modelW * 2 * scaleX / surfaceW + 2 * xPos / surfaceW - 1
		//
		//        ndcY = modelY * -2 * scaleY / surfaceH - 2 * yPos / surfaceH + 1
		//ndcY_flipped = modelY * 2 * scaleY / surfaceH - 2 * modelH * scaleY / surfaceH - 2 * yPos + 1

		D3DMATRIX viewMatrix = identityMatrix;

		float halfPixelX = addPointFive ? 0.5f * 2.0f / (float)renderTargetWidth : 0.0f;
		float halfPixelY = addPointFive ? 0.5f * 2.0f / (float)renderTargetHeight : 0.0f;
		float m00, m30, m11, m31;
		if (scaleX >= 0)
		{
			m00 = 2.0f * scaleX / (float)renderTargetWidth;
			m30 = 2.0f * (float)xPos / (float)renderTargetWidth - 1.0f - halfPixelX;
		}
		else
		{
			m00 = -2.0f * (float)fabs(scaleX) / (float)renderTargetWidth;
			m30 = 2.0f * (float)modelWidth * (float)fabs(scaleX) / (float)renderTargetWidth + 2.0f * (float)xPos / (float)renderTargetWidth - 1.0f - halfPixelX;
		}
		if (scaleY >= 0)
		{
			m11 = -2.0f * scaleY / (float)renderTargetHeight;
			m31 = -2.0f * (float)yPos / (float)renderTargetHeight + 1.0f + halfPixelY;
		}
		else
		{
			m11 = 2.0f * (float)fabs(scaleY) / (float)renderTargetHeight;
			m31 = -2.0f * (float)modelHeight * (float)fabs(scaleY) / (float)renderTargetHeight - 2.0f * (float)yPos / (float)renderTargetHeight + 1.0f + halfPixelY;
		}

		if (zOverride != -1)
		{
			viewMatrix.m[2][2] = 0;
			viewMatrix.m[3][2] = zOverride;
		}

		viewMatrix.m[0][0] = m00;
		viewMatrix.m[1][1] = m11;
		viewMatrix.m[3][0] = m30;
		viewMatrix.m[3][1] = m31;

		HRESULT hr = 0;
		hr |= device->SetTransform(D3DTS_VIEW, &viewMatrix);
		hr |= device->SetTransform(D3DTS_PROJECTION, &identityMatrix);
		hr |= device->SetTransform(D3DTS_WORLD, &identityMatrix);
		hr |= device->SetVertexShaderConstantF(0, &(viewMatrix.m[0][0]), 4);
		return SUCCEEDED(hr);
	}

	bool SetViewMatrix(int renderTargetWidth, int renderTargetHeight, float xPos, float yPos, float scaleX, float scaleY, float zOverride = -1)
	{
		return SetViewMatrix(renderTargetWidth, renderTargetHeight, xPos, yPos, scaleX, scaleY, 1, 1, zOverride, true);
	}

	bool SetViewMatrix(int renderTargetWidth, int renderTargetHeight, int xPos, int yPos, int scaleX, int scaleY, float zOverride = -1)
	{
		return SetViewMatrix(renderTargetWidth, renderTargetHeight, (float)xPos, (float)yPos, (float)scaleX, (float)scaleY, 1.0f, 1.0f, zOverride, true);
	}

	bool SetViewMatrix(float scaleX, float scaleY, float zOverride = -1)
	{
		return SetViewMatrix((int)scaleX, (int)scaleY, 0, 0, scaleX, scaleY, 1, 1, zOverride, true);
	}
	bool SetViewMatrix(int scaleX, int scaleY, float zOverride = -1)
	{
		return SetViewMatrix(scaleX, scaleY, 0, 0, (float)scaleX, (float)scaleY, 1, 1, zOverride, true);
	}

private:
	IDirect3DDevice9 *device;
	IDirect3DSwapChain9 *swapChain;
	IDirect3DTexture9 *sourceTexture;
	IDirect3DSurface9 *sourceTextureSurface;

	IDirect3DTexture9 *pass0RenderTargetTexture;
	IDirect3DSurface9 *pass0RenderTargetSurface;
	IDirect3DTexture9 *pass1RenderTargetTexture;
	IDirect3DSurface9 *pass1RenderTargetSurface;

	IDirect3DSurface9 *depthStencilSurface;
	IDirect3DSurface9* backBuffer;

	IDirect3DVertexDeclaration9 *vertexDeclaration;
	IDirect3DVertexBuffer9 *vertexBuffer;

	IDirect3DVertexShader9 *stockVertexShader;
	IDirect3DPixelShader9 *stockPixelShader;
	IDirect3DVertexShader9 *superXbrPass0VertexShader;
	IDirect3DPixelShader9 *superXbrPass0PixelShader;
#if !NO_CHECKERBOARD
	IDirect3DPixelShader9 *checkerboardPixelShader;
	IDirect3DPixelShader9 *superXbrPass1OddPixelShader;
	IDirect3DPixelShader9 *superXbrPass1EvenPixelShader;
#else
	IDirect3DPixelShader9 *superXbrPass1PixelShader;
#endif
	IDirect3DSurface9* depthSurface2x;
	IDirect3DSurface9* depthSurface1x;
	bool depthSurfaceReady;
	int inputLeft, inputTop, inputWidth, inputHeight;
	int textureWidth;
	int textureHeight;
	int backBufferWidth;
	int backBufferHeight;

	int updateLeft, updateTop, updateWidth, updateHeight;
	HWND hwnd;
	D3DSWAPEFFECT swapEffect;
	int bufferCount;
	//int windowWidth, windowHeight;
	bool doBeginScene;
	int upscaleFilter;
	bool borderDirty;
	bool upscaledTextureDirty;
	bool isWine;

	Region updateRegionOriginal;
	Region updateRegion1X;
	Region updateRegion2X;
	Region updateRegionScreen;
	vector<Region> oldUpdateRegions;

public:
	CUpscalerImpl() :
		device(NULL),
		sourceTexture(NULL),
		sourceTextureSurface(NULL),
		pass0RenderTargetTexture(NULL),
		pass0RenderTargetSurface(NULL),
		pass1RenderTargetTexture(NULL),
		pass1RenderTargetSurface(NULL),
		depthStencilSurface(NULL),

		vertexDeclaration(NULL),
		vertexBuffer(NULL),

		stockVertexShader(NULL),
		stockPixelShader(NULL),
		superXbrPass0VertexShader(NULL),
		superXbrPass0PixelShader(NULL),
#if !NO_CHECKERBOARD
		superXbrPass1OddPixelShader(NULL),
		superXbrPass1EvenPixelShader(NULL),
		checkerboardPixelShader(NULL),
#else
		superXbrPass1PixelShader(NULL),
#endif
		depthSurface2x(NULL),
		depthSurface1x(NULL),
		depthSurfaceReady(false),
		doBeginScene(true),
		textureWidth(0), textureHeight(0), backBufferWidth(0), backBufferHeight(0),
		updateLeft(0), updateTop(0), updateWidth(0), updateHeight(0),
		inputLeft(0), inputTop(0), inputWidth(0), inputHeight(0),
		borderDirty(), upscaledTextureDirty(), isWine(),
		upscaleFilter(1),
		swapEffect(D3DSWAPEFFECT_COPY), bufferCount(0), hwnd(0)
	{
		Destroy();
	}
	~CUpscalerImpl()
	{
		Destroy();
	}
private:
	bool CreateVertexDeclaration()
	{
		if (vertexDeclaration != NULL) return true;
		if (device == NULL) return false;

		HRESULT hr;
		hr = device->CreateVertexDeclaration(VertexFormat, &vertexDeclaration);
		return SUCCEEDED(hr);
	}

	bool CreateVertexBuffer()
	{
		if (vertexBuffer != NULL) return true;
		if (device == NULL) return false;
		const int VertexCount = 8;


		HRESULT hr;
		void *vData = NULL;
		Vertex* vertices = NULL;

		hr = device->CreateVertexBuffer(VertexCount * sizeof(Vertex), D3DUSAGE_DYNAMIC, Vertex::FVF, D3DPOOL_DEFAULT, &vertexBuffer, NULL);
		if (FAILED(hr)) goto failed;

		hr = vertexBuffer->Lock(0, VertexCount * sizeof(Vertex), &vData, 0);
		if (FAILED(hr)) goto failed;

		vertices = (Vertex*)vData;
		if (vertices != NULL)
		{
			const float x = 0.0;
			const float y = 0.0;
			const float x2 = 1.0;
			const float y2 = 1.0;
			const float z = 0.5;

			vertices[0] = Vertex(x, y, z, 0, 0);
			vertices[1] = Vertex(x2, y, z, 1, 0);
			vertices[2] = Vertex(x, y2, z, 0, 1);
			vertices[3] = Vertex(x2, y2, z, 1, 1);

			vertices[4] = Vertex(x, y, z, 0, 0);
			vertices[5] = Vertex(x2, y, z, 1, 0);
			vertices[6] = Vertex(x, y2, z, 0, 1);
			vertices[7] = Vertex(x2, y2, z, 1, 1);
		}
		hr = vertexBuffer->Unlock();
		if (FAILED(hr)) goto failed;
		return true;
	failed:
		SafeRelease(vertexBuffer);
		return false;
	}

	bool UpdateVertexBuffer()
	{
		if (device == NULL) return false;
		bool okay = true;
		if (vertexBuffer == NULL) okay &= CreateVertexBuffer();

		void* vData = NULL;
		Vertex* vertices = NULL;
		const int VertexCount = 8;
		HRESULT hr;
		hr = vertexBuffer->Lock(0, VertexCount * sizeof(Vertex), &vData, 0);
		if (FAILED(hr)) goto failed;

		vertices = (Vertex*)vData;
		if (vertices != NULL)
		{
			const float x = 0.0;
			const float y = 0.0;
			const float x2 = 1.0;
			const float y2 = 1.0;
			const float z = 0.5;

			float u0 = (float)this->inputLeft / (float)this->textureWidth;
			float u1 = (float)(this->inputLeft + this->inputWidth) / (float)this->textureWidth;
			float v0 = (float)this->inputTop / (float)this->textureHeight;
			float v1 = (float)(this->inputTop + this->inputHeight) / (float)this->textureHeight;

			vertices[4] = Vertex(x, y, z, u0, v0);
			vertices[5] = Vertex(x2, y, z, u1, v0);
			vertices[6] = Vertex(x, y2, z, u0, v1);
			vertices[7] = Vertex(x2, y2, z, u1, v1);
		}
		hr = vertexBuffer->Unlock();
		if (FAILED(hr)) goto failed;
		return true;
	failed:
		return false;
	}


public:
	bool ClearZBufferAndCarveRects(const vector<RECT>& rects)
	{
		HRESULT hr = 0;
#if NO_DIRTY_RECT | DIRTY_RECT_NO_Z_BUFFER
		hr |= device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		return true;
#endif

		if (rects.size() <= 1)
		{
			hr |= device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
			hr |= device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			return SUCCEEDED(hr);
		}

		hr |= device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		hr |= device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 0.0f, 0);
		hr |= device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		//carve out rects
		for (size_t i = 0; i < rects.size(); i++)
		{
			hr |= device->SetScissorRect(&rects[i]);
			hr |= device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		}
		hr |= device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		return SUCCEEDED(hr);
	}
	bool ClearZBufferAndCarveRects(const vector<RECT>& rects, IDirect3DSurface9* pixels, IDirect3DSurface9* depth)
	{
		bool okay = true;
		HRESULT hr = 0;
		hr |= device->SetRenderTarget(0, pixels);
		hr |= device->SetDepthStencilSurface(depth);
		okay = ClearZBufferAndCarveRects(rects);
		return SUCCEEDED(hr) & okay;
	}
	bool Update()
	{
		if (device == NULL) return false;
		bool okay = true;
		
		if (depthSurface2x == NULL) okay &= CreateDepthSurface2x();
		if (depthSurface1x == NULL) okay &= CreateDepthSurface1x();

		RECT clientRect;
		GetClientRect(hwnd, &clientRect);

		if (isWine)
		{
			this->updateRegionScreen.AddRectangle(clientRect);
		}

		if (this->bufferCount > 1)
		{
			this->oldUpdateRegions.push_back(this->updateRegionScreen);
			if (this->oldUpdateRegions.size() > this->bufferCount)
			{
				this->oldUpdateRegions.erase(this->oldUpdateRegions.begin() + 0, this->oldUpdateRegions.begin() + 1);
			}
			for (int i = 0; i < this->oldUpdateRegions.size() - 1; i++)
			{
				this->updateRegionScreen.UnionWith(this->oldUpdateRegions[i]);
			}
		}

		RECT boundingBoxOriginal = this->updateRegionOriginal.GetBoundingBox();
		RECT boundingBox1x = this->updateRegion1X.GetBoundingBox();
		RECT boundingBox2x = this->updateRegion2X.GetBoundingBox();
		RECT boundingBoxScreen = this->updateRegionScreen.GetBoundingBox();

		bool borderDirty = false;
		if (boundingBoxScreen.left < updateLeft || boundingBoxScreen.right > updateLeft + updateWidth ||
			boundingBoxScreen.top < updateTop || boundingBoxScreen.bottom > updateTop + updateHeight)
		{
			borderDirty = true;
		}

		vector<RECT> rectsOriginal = this->updateRegionOriginal.GetRegionRectangles();
		vector<RECT> rects1x = this->updateRegion1X.GetRegionRectangles();
		vector<RECT> rects2x = this->updateRegion2X.GetRegionRectangles();
		vector<RECT> rectsScreen = this->updateRegionScreen.GetRegionRectangles();

		//clear Z buffers to 0.0 (Near), carve out rectangles as 1.0 (far)
		okay &= ClearZBufferAndCarveRects(rects2x, pass1RenderTargetSurface, depthSurface2x);
		okay &= ClearZBufferAndCarveRects(rects1x, pass0RenderTargetSurface, depthSurface1x);
		okay &= ClearZBufferAndCarveRects(rectsScreen, backBuffer, depthStencilSurface);

#if NO_DIRTY_RECT
		boundingBoxOriginal = { inputLeft,inputTop,inputLeft + inputWidth,inputTop + inputHeight };
#endif

#if !NO_CHECKERBOARD
		RenderCheckerboard();
#endif
		
		int x = boundingBoxOriginal.left;
		int y = boundingBoxOriginal.top;
		int width = boundingBoxOriginal.right - boundingBoxOriginal.left;
		int height = boundingBoxOriginal.bottom - boundingBoxOriginal.top;
		HRESULT hr = 0;

		if (borderDirty)
		{
			int windowWidth = clientRect.right - clientRect.left;
			int windowHeight = clientRect.bottom - clientRect.top;

			hr |= device->SetRenderTarget(0, backBuffer);
			hr |= device->SetDepthStencilSurface(depthStencilSurface);
			
			int leftEdge = this->updateLeft;
			int topEdge = this->updateTop;
			int rightEdge = windowWidth - leftEdge - this->updateWidth;
			int bottomEdge = windowHeight - topEdge - this->updateHeight;

			RECT rect;
			if (leftEdge > 0)
			{
				rect.top = 0; rect.bottom = windowHeight;
				rect.left = 0; rect.right = leftEdge;
				hr |= device->ColorFill(backBuffer, &rect, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
			if (rightEdge > 0)
			{
				rect.top = 0; rect.bottom = windowHeight;
				rect.left = leftEdge + updateWidth; rect.right = windowWidth;
				hr |= device->ColorFill(backBuffer, &rect, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
			if (topEdge > 0)
			{
				rect.left = 0; rect.right = windowWidth;
				rect.top = 0; rect.bottom = topEdge;
				hr |= device->ColorFill(backBuffer, &rect, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
			if (bottomEdge > 0)
			{
				rect.left = 0; rect.right = windowWidth;
				rect.top = topEdge + updateHeight; rect.bottom = windowHeight;
				hr |= device->ColorFill(backBuffer, &rect, D3DCOLOR_RGBA(0, 0, 0, 0));
			}
		}
		okay &= UpdateToTexture(x, y, width, height);
		okay &= UpdateToBackBuffer(x, y, width, height);
		
#if !NO_DIRTY_RECT
		vector<byte> regionDataBytes = this->updateRegionScreen.GetRegionData();
		if (regionDataBytes.size() == 0)
		{
			return true;
		}
		const RGNDATA* rgnData = (const RGNDATA*)&regionDataBytes[0];

#else
		boundingBoxScreen = { 0,0,updateLeft * 2 + updateWidth,updateTop * 2 + updateHeight };
		const RGNDATA* rgnData = NULL;
#endif
		//temporary
		if (rectsScreen.size() <= 1)
		{
			rgnData = NULL;
		}
		hr |= device->Present(&boundingBoxScreen, &boundingBoxScreen, NULL, rgnData);
		return okay && SUCCEEDED(hr);
	}

	bool UpdateToTexture(int x, int y, int width, int height)
	{
		if (device == NULL) return false;
		//if we are not in Upscaling mode, do not run this function
		if (GetUpscaleFilter() == 0)
		{
			upscaledTextureDirty = true;
			return true;
		}

		//coordinates are non-scaled, and not pre-expanded
		CStateSaver stateSaver(device);	//save device state, restore state upon stateSaver leaving scope

		int leftSmall = max(x - 2, 0);
		int rightSmall = min(x + width + 2, inputWidth);
		int topSmall = max(y - 2, 0);
		int bottomSmall = min(y + height + 2, inputHeight);

		int leftBig = max(x * 2 - 3, 0);
		int rightBig = min((x + width) * 2 + 3, inputWidth * 2);
		int topBig = max(y * 2 - 3, 0);
		int bottomBig = min((y + height) * 2 + 3, inputHeight * 2);

		RECT smallRect = { leftSmall, topSmall, rightSmall, bottomSmall };
		RECT bigRect = { leftBig, topBig, rightBig, bottomBig };
		
		bool useScissorRect = true;
		if (leftBig == 0 && topBig == 0 && rightBig == textureWidth * 2 && bottomBig == textureHeight * 2)
		{
			useScissorRect = false;
		}

		bool okay = true;
		okay &= CreateVertexDeclaration();
		okay &= CreateVertexBuffer();
		if (stockPixelShader == NULL) LoadShaders();
		if (pass1RenderTargetSurface == NULL) okay &= EnsureRenderTexturesExist();
		if (depthSurface1x == NULL) okay &= CreateDepthSurface1x();
		if (depthSurface2x == NULL) okay &= CreateDepthSurface2x();
#if !NO_CHECKERBOARD
		if (!depthSurfaceReady) okay &= CreateCheckerboardSurface();
		//okay &= RenderCheckerboard();
#endif
		HRESULT hr = 0;
		HRESULT hr2 = 0;

		if (doBeginScene) hr |= device->BeginScene();
		hr |= device->SetFVF(Vertex::FVF);
		hr |= device->SetVertexDeclaration(vertexDeclaration);
		hr |= device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
		hr |= device->SetRenderState(D3DRS_LIGHTING, false);
		hr |= device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		hr |= device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		hr |= device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		hr |= device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		hr |= device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		hr |= device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		hr |= device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		hr |= device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		hr2 |= device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, true);
		hr2 |= device->SetSamplerState(1, D3DSAMP_SRGBTEXTURE, true);
		hr2 |= device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
		//set default settings
		hr |= device->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);
		hr |= device->SetRenderState(D3DRS_STENCILENABLE, false);
		hr |= device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		hr |= device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		hr |= device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		//Pass 1
		if (useScissorRect)
		{
			hr |= device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		}
		else
		{
			hr |= device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		}
		hr |= device->SetRenderTarget(0, pass0RenderTargetSurface);
		hr |= device->SetDepthStencilSurface(depthSurface1x);
		hr |= device->SetScissorRect(&smallRect);
		hr |= device->SetRenderState(D3DRS_ZENABLE, true);
		hr |= device->SetRenderState(D3DRS_ZWRITEENABLE, false);
		hr |= device->SetTexture(0, sourceTexture);
		okay &= SetViewMatrix(textureWidth, textureHeight);
		okay &= SetShaderParameters(textureWidth, textureHeight);
		hr |= device->SetVertexShader(superXbrPass0VertexShader);
		hr |= device->SetPixelShader(superXbrPass0PixelShader);
		hr |= device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

#if !NO_CHECKERBOARD
		//Pass 2a - Just copying pixels (no complicated math), use the "ODD" shader, use Greater Than Z test
		hr |= device->SetRenderTarget(0, pass1RenderTargetSurface);
		hr |= device->SetDepthStencilSurface(depthSurface2x);
		hr |= device->SetScissorRect(&bigRect);
		hr |= device->SetRenderState(D3DRS_ZENABLE, true);
		hr |= device->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
		hr |= device->SetTexture(0, pass0RenderTargetTexture);
		hr |= device->SetTexture(1, sourceTexture);
		hr |= device->SetVertexShader(stockVertexShader);
		hr |= device->SetPixelShader(superXbrPass1OddPixelShader);
		okay &= SetShaderParameters(textureWidth, textureHeight);
		okay &= SetViewMatrix(textureWidth * 2, textureHeight * 2, 0.25);
		hr |= device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

		//Pass 2b - Processing pixels, use the "EVEN" shader, and LESS Z testing
		hr |= device->SetRenderState(D3DRS_ZENABLE, true);
		hr |= device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		hr |= device->SetPixelShader(superXbrPass1EvenPixelShader);
		okay &= SetShaderParameters(textureWidth, textureHeight);
		okay &= SetViewMatrix(textureWidth * 2, textureHeight * 2, 0.125);
		hr |= device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
#else
		//TODO
		//Pass 2 - Processing pixels
		hr |= device->SetRenderTarget(0, pass1RenderTargetSurface);
		hr |= device->SetDepthStencilSurface(depthSurface2x);
		hr |= device->SetRenderState(D3DRS_ZENABLE, true);
		hr |= device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		hr |= device->SetRenderTarget(0, pass1RenderTargetSurface);
		hr |= device->SetScissorRect(&bigRect);
		hr |= device->SetTexture(0, pass0RenderTargetTexture);
		hr |= device->SetTexture(1, sourceTexture);
		hr |= device->SetVertexShader(stockVertexShader);
		hr |= device->SetPixelShader(superXbrPass1PixelShader);
		okay &= SetShaderParameters(textureWidth, textureHeight);
		okay &= SetViewMatrix(textureWidth * 2, textureHeight * 2);
		hr |= device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
#endif

		//IDirect3DSurface9 *backBuffer = NULL;
		//hr |= device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
		hr |= device->SetRenderTarget(0, backBuffer);
		//SafeRelease(backBuffer);
		hr |= device->SetDepthStencilSurface(depthStencilSurface);

		if (doBeginScene) hr |= device->EndScene();

		return okay && SUCCEEDED(hr);
	}

	bool UpdateToBackBuffer(int x, int y, int width, int height)
	{
		if (device == NULL) return false;
		if (updateWidth == 0 || updateHeight == 0) return true;
		if (inputWidth == 0 || inputHeight == 0) return true;
		if (textureWidth == 0 || textureHeight == 0) return true;

		bool useSourceTexture = GetUpscaleFilter() == 0;

		HRESULT hr = 0, hr2 = 0;

		//coordinates are non-scaled, and not pre-expanded
		CStateSaver stateSaver(device);	//save device state, restore state upon stateSaver leaving scope

		bool okay = true;
		okay &= CreateVertexDeclaration();
		okay &= UpdateVertexBuffer();

		if (stockPixelShader == NULL) LoadShaders();
		if (!useSourceTexture)
		{
			if (pass1RenderTargetSurface == NULL) okay &= EnsureRenderTexturesExist();
		}

		int leftBig, topBig, rightBig, bottomBig, leftScaled, topScaled, rightScaled, bottomScaled;
		if (!useSourceTexture)
		{
			leftBig = max(x * 2 - 3, 0);
			topBig = max(y * 2 - 3, 0);
			rightBig = min((x + width) * 2 + 3, inputWidth * 2);
			bottomBig = min((y + height) * 2 + 3, inputHeight * 2);

			leftScaled = updateLeft + max(0, (int)floorf((float)leftBig * (float)updateWidth / (float)(inputWidth * 2)) - 1);
			topScaled = updateTop + max(0, (int)floorf((float)topBig * (float)updateHeight / (float)(inputHeight * 2)) - 1);
			rightScaled = updateLeft + min(updateWidth, (int)ceilf((float)rightBig * (float)updateWidth / (float)(inputWidth * 2)) + 1);
			bottomScaled = updateTop + min(updateHeight, (int)ceilf((float)bottomBig * (float)updateHeight / (float)(inputHeight * 2)) + 1);
		}
		else
		{
			leftBig = max(x * 2, 0);
			topBig = max(y * 2, 0);
			rightBig = min((x + width) * 2, inputWidth * 2);
			bottomBig = min((y + height) * 2, inputHeight * 2);

			leftScaled = updateLeft + max(0, (int)floorf((float)leftBig * (float)updateWidth / (float)(inputWidth * 2)) - 1);
			topScaled = updateTop + max(0, (int)floorf((float)topBig * (float)updateHeight / (float)(inputHeight * 2)) - 1);
			rightScaled = updateLeft + min(updateWidth, (int)ceilf((float)rightBig * (float)updateWidth / (float)(inputWidth * 2)) + 1);
			bottomScaled = updateTop + min(updateHeight, (int)ceilf((float)bottomBig * (float)updateHeight / (float)(inputHeight * 2)) + 1);
		}

		RECT scissorRect = { leftScaled, topScaled, rightScaled, bottomScaled };

		bool useScissorRect = true;
		if (leftBig == 0 && topBig == 0 && rightBig == inputWidth * 2 && bottomBig == inputHeight * 2)
		{
			useScissorRect = false;
		}

		if (doBeginScene) hr |= device->BeginScene();
		hr |= device->SetFVF(Vertex::FVF);
		hr |= device->SetVertexDeclaration(vertexDeclaration);
		hr |= device->SetStreamSource(0, vertexBuffer, 4 * sizeof(Vertex), sizeof(Vertex));
		hr |= device->SetRenderState(D3DRS_LIGHTING, false);
		hr |= device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		hr |= device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		hr |= device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		hr |= device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		hr |= device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		hr |= device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		hr |= device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		hr |= device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		hr2 |= device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, true);
		hr2 |= device->SetSamplerState(1, D3DSAMP_SRGBTEXTURE, true);
		hr2 |= device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
		//set default settings
		hr |= device->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);
		hr |= device->SetRenderState(D3DRS_STENCILENABLE, false);
		hr |= device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		hr |= device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		hr |= device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		hr |= device->SetRenderState(D3DRS_ZENABLE, true);
		hr |= device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		//IDirect3DSurface9 *backBuffer = NULL;
		//hr |= device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
		hr |= device->SetRenderTarget(0, backBuffer);
		//D3DSURFACE_DESC desc = {};
		//if (backBuffer != NULL)
		//{
		//	hr |= backBuffer->GetDesc(&desc);
		//}
		//backBufferWidth = desc.Width;
		//backBufferHeight = desc.Height;
		//SafeRelease(backBuffer);
		hr |= device->SetDepthStencilSurface(depthStencilSurface);
		if (useScissorRect)
		{
			hr |= device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
			hr |= device->SetScissorRect(&scissorRect);
		}
		else
		{
			hr |= device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		}
		if (!useSourceTexture)
		{
			hr |= device->SetTexture(0, pass1RenderTargetTexture);
		}
		else
		{
			hr |= device->SetTexture(0, sourceTexture);
		}
		hr |= device->SetPixelShader(stockPixelShader);
		hr |= device->SetVertexShader(stockVertexShader);
		okay &= SetViewMatrix(backBufferWidth, backBufferHeight, updateLeft, updateTop, updateWidth, updateHeight);
		hr |= device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

		if (doBeginScene) hr |= device->EndScene();

		return okay && SUCCEEDED(hr);
	}

	HRESULT UpdateToScreen(int x, int y, int width, int height)
	{
		if (device == NULL) return -1;
		if (updateWidth == 0 || updateHeight == 0) return 0;
		if (textureWidth == 0 || textureHeight == 0) return 0;

		//coordinates are non-scaled, and not pre-expanded

		bool useSourceTexture = GetUpscaleFilter() == 0;

		HRESULT hr = 0;

		//coordinates are non-scaled, and not pre-expanded
		//CStateSaver stateSaver(device);	//save device state, restore state upon stateSaver leaving scope

		int leftBig, topBig, rightBig, bottomBig, leftScaled, topScaled, rightScaled, bottomScaled;
		if (!useSourceTexture)
		{
			leftBig = max(x * 2 - 3, 0);
			topBig = max(y * 2 - 3, 0);
			rightBig = min((x + width) * 2 + 3, inputWidth * 2);
			bottomBig = min((y + height) * 2 + 3, inputHeight * 2);

			leftScaled = updateLeft + max(0, (int)floorf((float)leftBig * (float)updateWidth / (float)(inputWidth * 2)) - 1);
			topScaled = updateTop + max(0, (int)floorf((float)topBig * (float)updateHeight / (float)(inputHeight * 2)) - 1);
			rightScaled = updateLeft + min(updateWidth, (int)ceilf((float)rightBig * (float)updateWidth / (float)(inputWidth * 2)) + 1);
			bottomScaled = updateTop + min(updateHeight, (int)ceilf((float)bottomBig * (float)updateHeight / (float)(inputHeight * 2)) + 1);
		}
		else
		{
			leftBig = max(x * 2, 0);
			topBig = max(y * 2, 0);
			rightBig = min((x + width) * 2, inputWidth * 2);
			bottomBig = min((y + height) * 2, inputHeight * 2);

			leftScaled = updateLeft + max(0, (int)floorf((float)leftBig * (float)updateWidth / (float)(inputWidth * 2)) - 1);
			topScaled = updateTop + max(0, (int)floorf((float)topBig * (float)updateHeight / (float)(inputHeight * 2)) - 1);
			rightScaled = updateLeft + min(updateWidth, (int)ceilf((float)rightBig * (float)updateWidth / (float)(inputWidth * 2)) + 1);
			bottomScaled = updateTop + min(updateHeight, (int)ceilf((float)bottomBig * (float)updateHeight / (float)(inputHeight * 2)) + 1);
		}

		RECT updateRect = { leftScaled, topScaled, rightScaled, bottomScaled };
		hr = device->Present(&updateRect, &updateRect, NULL, NULL);
		return hr;
	}
public:
	void Destroy()
	{
		SafeRelease(device);
		SafeRelease(swapChain);
		SafeRelease(sourceTexture);
		SafeRelease(sourceTextureSurface);
		SafeRelease(pass0RenderTargetTexture);
		SafeRelease(pass0RenderTargetSurface);
		SafeRelease(pass1RenderTargetTexture);
		SafeRelease(pass1RenderTargetSurface);
		SafeRelease(depthStencilSurface);

		SafeRelease(vertexDeclaration);
		SafeRelease(vertexBuffer);

		SafeRelease(stockVertexShader);
		SafeRelease(stockPixelShader);
		SafeRelease(superXbrPass0VertexShader);
		SafeRelease(superXbrPass0PixelShader);
#if !NO_CHECKERBOARD
		SafeRelease(superXbrPass1OddPixelShader);
		SafeRelease(superXbrPass1EvenPixelShader);
		SafeRelease(checkerboardPixelShader);
		depthSurfaceReady = false;
#else
		SafeRelease(superXbrPass1PixelShader);
#endif
		SafeRelease(depthSurface2x);
		SafeRelease(depthSurface1x);
		SafeRelease(backBuffer);
		textureWidth = 0;
		textureHeight = 0;
		backBufferWidth = 0;
		backBufferHeight = 0;
		swapEffect = D3DSWAPEFFECT_COPY;
		bufferCount = 0;
		this->oldUpdateRegions.clear();
	}
private:
	bool SetDevice(IDirect3DDevice9 *device)
	{
		if (this->device == device)
		{
			return true;
		}
		else
		{
			Destroy();
		}
		SafeAddRef(device);
		this->device = device;

		HRESULT hr = 0;
		if (backBuffer != NULL) SafeRelease(backBuffer);
		//IDirect3DSurface9 *backBuffer = NULL;
		hr |= device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
		D3DSURFACE_DESC desc = {};
		if (backBuffer != NULL) hr = backBuffer->GetDesc(&desc);
		//SafeRelease(backBuffer);
		backBufferWidth = desc.Width;
		backBufferHeight = desc.Height;

		if (depthStencilSurface != NULL) SafeRelease(depthStencilSurface);
		hr = device->GetDepthStencilSurface(&depthStencilSurface);

		D3DDEVICE_CREATION_PARAMETERS creationParameters = {};
		hr = device->GetCreationParameters(&creationParameters);
		if (SUCCEEDED(hr))
		{
			this->hwnd = creationParameters.hFocusWindow;
		}
		SafeRelease(this->swapChain);
		hr = device->GetSwapChain(0, &swapChain);
		if (swapChain != NULL)
		{
			D3DPRESENT_PARAMETERS presentParameters = {};
			hr = swapChain->GetPresentParameters(&presentParameters);
			if (SUCCEEDED(hr))
			{
				this->swapEffect = presentParameters.SwapEffect;
				this->bufferCount = presentParameters.BackBufferCount;
				extern bool IsWine();
				if (IsWine())
				{
					this->isWine = true;
					//this->bufferCount = 4;
				}
			}
		}
		
		//D3DCAPS9 caps = {};
		//hr = device->GetDeviceCaps(&caps);
		//IDirect3D9 *d3d9 = NULL;
		//hr = device->GetDirect3D(&d3d9);
		//if (d3d9 != NULL)
		//{
		//	//TESTME
		//	hr = d3d9->CheckDeviceFormat(caps.AdapterOrdinal, caps.DeviceType, desc.Format, D3DUSAGE_QUERY_SRGBREAD, D3DRTYPE_TEXTURE, desc.Format);
		//	hr |= d3d9->CheckDeviceFormat(caps.AdapterOrdinal, caps.DeviceType, desc.Format, D3DUSAGE_QUERY_SRGBWRITE, D3DRTYPE_TEXTURE, desc.Format);
		//	this->useLinearRGB = SUCCEEDED(hr);
		//}
		//SafeRelease(d3d9);

		return device != NULL;
	}
	bool SetDevice(IDirect3DResource9 *resource)
	{
		HRESULT hr;
		bool okay = true;
		IDirect3DDevice9 *device = NULL;
		hr = resource->GetDevice(&device);
		if (!SUCCEEDED(hr)) return false;
		okay &= SetDevice(device);
		SafeRelease(device);
		return SUCCEEDED(hr) && this->device != NULL && okay;
	}
public:
	bool SetSourceTexture(IDirect3DTexture9 *sourceTexture)
	{
		if (sourceTexture == this->sourceTexture) return true;
		HRESULT hr = 0;
		bool okay = true;
		okay &= SetDevice(sourceTexture);
		if (device == NULL) return false;
		SafeRelease(this->sourceTexture);
		SafeRelease(this->sourceTextureSurface);
		if (sourceTexture == NULL) return false;
		sourceTexture->AddRef();
		this->sourceTexture = sourceTexture;
		hr = sourceTexture->GetSurfaceLevel(0, &sourceTextureSurface);
		if (FAILED(hr)) return false;
		
		D3DSURFACE_DESC desc;
		hr = sourceTextureSurface->GetDesc(&desc);
		if (FAILED(hr)) return false;

		return SetTextureSize(desc.Width, desc.Height);
	}

#if USE_SHADER_MODEL_2
	bool LoadShaders2()
	{
		HRESULT hr = 0;
		hr |= device->CreatePixelShader(shaderPrograms2::old_stock_fragment, &this->stockPixelShader);
		hr |= device->CreatePixelShader(shaderPrograms2::super_xbr_pass0_fragment, &this->superXbrPass0PixelShader);
#if !NO_CHECKERBOARD
		hr |= device->CreatePixelShader(shaderPrograms2::checkerboard_fragment, &this->checkerboardPixelShader);
		hr |= device->CreatePixelShader(shaderPrograms2::super_xbr_pass1_EVEN_fragment, &this->superXbrPass1EvenPixelShader);
		hr |= device->CreatePixelShader(shaderPrograms2::super_xbr_pass1_ODD_fragment, &this->superXbrPass1OddPixelShader);
#else
		hr |= device->CreatePixelShader(shaderPrograms2::super_xbr_pass1_fragment, &this->superXbrPass1PixelShader);
#endif
		hr |= device->CreateVertexShader(shaderPrograms2::old_stock_vertex, &this->stockVertexShader);
		hr |= device->CreateVertexShader(shaderPrograms2::super_xbr_pass0_vertex, &this->superXbrPass0VertexShader);

		return SUCCEEDED(hr);
	}
#endif

	bool LoadShaders3()
	{
		HRESULT hr = 0;
		hr |= device->CreatePixelShader(shaderPrograms::old_stock_fragment, &this->stockPixelShader);
		hr |= device->CreatePixelShader(shaderPrograms::super_xbr_pass0_fragment, &this->superXbrPass0PixelShader);
#if !NO_CHECKERBOARD
		hr |= device->CreatePixelShader((const DWORD*)shaderPrograms::checkerboard_fragment, &this->checkerboardPixelShader);
		hr |= device->CreatePixelShader(shaderPrograms::super_xbr_pass1_EVEN_fragment, &this->superXbrPass1EvenPixelShader);
		hr |= device->CreatePixelShader(shaderPrograms::super_xbr_pass1_ODD_fragment, &this->superXbrPass1OddPixelShader);
#else
		hr |= device->CreatePixelShader(shaderPrograms::super_xbr_pass1_fragment, &this->superXbrPass1PixelShader);
#endif
		hr |= device->CreateVertexShader(shaderPrograms::old_stock_vertex, &this->stockVertexShader);
		hr |= device->CreateVertexShader(shaderPrograms::super_xbr_pass0_vertex, &this->superXbrPass0VertexShader);

		return SUCCEEDED(hr);
	}
	
	bool LoadShaders()
	{
#if !USE_SHADER_MODEL_2
		return LoadShaders3();
#else
		if (device == NULL) return false;
		HRESULT hr = 0;
		D3DCAPS9 caps;
		hr |= device->GetDeviceCaps(&caps);
		if (FAILED(hr)) return false;
		int vertexShaderVersionMajor = D3DSHADER_VERSION_MAJOR(caps.VertexShaderVersion);
		int pixelShaderVersionMajor = D3DSHADER_VERSION_MAJOR(caps.PixelShaderVersion);
		bool enoughInstructionSlots = caps.PS20Caps.NumInstructionSlots >= 210;
		bool canUseShader3 = pixelShaderVersionMajor >= 3 && vertexShaderVersionMajor >= 3;
		bool canUseShader2 = pixelShaderVersionMajor >= 2 && vertexShaderVersionMajor >= 2 && enoughInstructionSlots;

		if (canUseShader3)
		{
			if (canUseShader2)
			{
				bool loadedShaders = false;
				extern bool IsAtiRadeonX1(IDirect3DDevice9 *device);
				extern bool IsIntelGen9GPU(IDirect3DDevice9 *device);

				if (IsIntelGen9GPU(device) || IsAtiRadeonX1(device))
				{
					loadedShaders = LoadShaders2();
				}
				if (loadedShaders) return true;
			}
			return LoadShaders3();
		}
		if (canUseShader3) return LoadShaders3();
		if (canUseShader2) return LoadShaders2();
		return false;
#endif
	}
private:
	bool CreateRenderTarget(IDirect3DTexture9 *&texture, IDirect3DSurface9 *&surface, int width, int height)
	{
		if (device == NULL) return false;
		HRESULT hr;
		hr = device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
		if (FAILED(hr))
		{
			return false;
		}
		hr = texture->GetSurfaceLevel(0, &surface);
		if (FAILED(hr))
		{
			return false;
		}
		return true;
	}

	bool SetShaderParameters(int width, int height)
	{
		ShaderInput shaderInput = {};
		shaderInput.SetDimensions(width, height, width, height);
		HRESULT hr = 0;
		hr |= device->SetPixelShaderConstantF(0, (const float*)&shaderInput, sizeof(shaderInput));
		hr |= device->SetVertexShaderConstantF(4, (const float*)&shaderInput, sizeof(shaderInput));
		return SUCCEEDED(hr);
	}

	bool SetShaderParameters(int inputWidth, int inputHeight, int outputWidth, int outputHeight)
	{
		ShaderInput shaderInput = {};
		shaderInput.SetDimensions(inputWidth, inputHeight, outputWidth, outputHeight);
		HRESULT hr = 0;
		hr |= device->SetPixelShaderConstantF(0, (const float*)&shaderInput, sizeof(shaderInput));
		hr |= device->SetVertexShaderConstantF(4, (const float*)&shaderInput, sizeof(shaderInput));
		return SUCCEEDED(hr);
	}
	bool CreateDepthSurface2x()
	{
		if (depthSurface2x != NULL) return true;
		if (device == NULL) return false;
		if (vertexDeclaration == NULL) if (!CreateVertexDeclaration()) return false;
		if (vertexBuffer == NULL) if (!CreateVertexBuffer()) return false;

		CStateSaver stateSaver(device);

		HRESULT hr = 0;
		if (depthSurface2x == NULL)
		{
			hr = device->CreateDepthStencilSurface(textureWidth * 2, textureHeight * 2, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, false, &depthSurface2x, NULL);
			if (FAILED(hr)) return false;

			hr |= device->SetDepthStencilSurface(depthSurface2x);
			hr |= device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0, 0);
		}
		if (FAILED(hr)) return false;
		return true;
	}
	bool CreateDepthSurface1x()
	{
		if (depthSurface1x != NULL) return true;
		if (device == NULL) return false;
		if (vertexDeclaration == NULL) if (!CreateVertexDeclaration()) return false;
		if (vertexBuffer == NULL) if (!CreateVertexBuffer()) return false;

		CStateSaver stateSaver(device);

		HRESULT hr = 0;
		if (depthSurface1x == NULL)
		{
			hr = device->CreateDepthStencilSurface(textureWidth, textureHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, false, &depthSurface1x, NULL);
			if (FAILED(hr)) return false;

			hr |= device->SetDepthStencilSurface(depthSurface1x);
			hr |= device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0, 0);
		}
		if (FAILED(hr)) return false;
		return true;
	}
#if !NO_CHECKERBOARD
	bool RenderCheckerboard()
	{
		if (device == NULL) return false;
		if (pass1RenderTargetSurface == NULL) if (!EnsureRenderTexturesExist()) return false;
		if (checkerboardPixelShader == NULL) if (!LoadShaders()) return false;
		if (vertexDeclaration == NULL) if (!CreateVertexDeclaration()) return false;
		if (vertexBuffer == NULL) if (!CreateVertexBuffer()) return false;

		bool okay = true;
		HRESULT hr = 0;

		CStateSaver stateSaver(device);

		if (doBeginScene) hr |= device->BeginScene();
		hr |= device->SetFVF(Vertex::FVF);
		hr |= device->SetVertexDeclaration(vertexDeclaration);
		hr |= device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
		hr |= device->SetDepthStencilSurface(depthSurface2x);
		hr |= device->SetRenderTarget(0, pass1RenderTargetSurface);
		hr |= device->SetVertexShader(stockVertexShader);
		hr |= device->SetPixelShader(checkerboardPixelShader);
		hr |= device->SetRenderState(D3DRS_ZENABLE, true);
		hr |= device->SetRenderState(D3DRS_ZWRITEENABLE, true);
		hr |= device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		hr |= device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
		okay &= SetViewMatrix(textureWidth * 2, textureHeight * 2, 0.5f);
		okay &= SetShaderParameters(textureWidth * 2, textureHeight * 2);
		hr |= device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
		if (doBeginScene) hr |= device->EndScene();
		return SUCCEEDED(hr) & okay;
	}
	bool CreateCheckerboardSurface()
	{
		if (!CreateDepthSurface2x()) return false;
		if (!RenderCheckerboard()) return false;
		depthSurfaceReady = true;
		return true;
	}
#endif
	bool SetTextureSize(int width, int height)
	{
		if (device == NULL) return false;
		if (textureWidth != width || textureHeight != height)
		{
			textureWidth = width;
			textureHeight = height;

			SafeRelease(pass0RenderTargetTexture);
			SafeRelease(pass0RenderTargetSurface);
			SafeRelease(pass1RenderTargetTexture);
			SafeRelease(pass1RenderTargetSurface);
#if !NO_CHECKERBOARD
			depthSurfaceReady = false;
#endif
			SafeRelease(depthSurface2x);
			SafeRelease(depthSurface1x);
		}
		return true;
	}

	bool EnsureRenderTexturesExist()
	{
		if (pass0RenderTargetTexture != NULL && pass0RenderTargetSurface != NULL && pass1RenderTargetTexture != NULL && pass1RenderTargetSurface != NULL)
		{
			return true;
		}
		SafeRelease(pass0RenderTargetTexture);
		SafeRelease(pass0RenderTargetSurface);
		SafeRelease(pass1RenderTargetTexture);
		SafeRelease(pass1RenderTargetSurface);
		bool okay;
		okay = CreateRenderTarget(pass0RenderTargetTexture, pass0RenderTargetSurface, textureWidth, textureHeight);
		if (!okay) return false;
		okay = CreateRenderTarget(pass1RenderTargetTexture, pass1RenderTargetSurface, textureWidth * 2, textureHeight * 2);
		if (!okay) return false;
#if !NO_CHECKERBOARD
		//return CreateCheckerboardSurface();
#else
		return true;
#endif
		return true;
	}

public:
	void SetViewRectangle(int x, int y, int width, int height)
	{
		this->updateLeft = x;
		this->updateTop = y;
		this->updateWidth = width;
		this->updateHeight = height;
	}
	void SetInputRectangle(int x, int y, int width, int height)
	{
		this->inputLeft = x;
		this->inputTop = y;
		this->inputWidth = width;
		this->inputHeight = height;
	}
	void SetDoBeginScene(bool doBeginScene)
	{
		this->doBeginScene = doBeginScene;
	}
	void SetUpscaleFilter(int upscaleFilter)
	{
		this->upscaleFilter = upscaleFilter;
	}
	int GetUpscaleFilter()
	{
		if (this->updateWidth <= inputWidth || this->updateHeight <= inputHeight)
		{
			return 0;
		}
		return this->upscaleFilter;
	}


	static inline int Round(float f)
	{
		return ((int)(f + 32768.5)) - 32768;
	}

	void SetUpdateRegion(const Region& region)
	{
		float scaleX = (float)this->updateWidth / (float)this->inputWidth;
		float scaleY = (float)this->updateHeight / (float)this->inputHeight;
		float scale = std::min(scaleX, scaleY);

		//int scaledWidth = Round(scale * inputWidth);
		//int scaledHeight = Round(scale * inputHeight);
		//int xOffset = (this->updateWidth - ScaledWidth) / 2;
		//RightPadding = RealWidth - ScaledWidth - LeftPadding;
		//TopPadding = (RealHeight - ScaledHeight) / 2;
		//BottomPadding = RealHeight - ScaledHeight - TopPadding;
		//XOffset = LeftPadding;
		//YOffset = TopPadding;

		this->updateRegionOriginal = region;
		this->updateRegion1X = region.ZoomAndDialate(1.0f, 2.0f, 0.0f, 0.0f, 0, 0, this->inputWidth, this->inputHeight);
		this->updateRegion2X = region.ZoomAndDialate(2.0f, 3.0f, 0.0f, 0.0f, 0, 0, this->inputWidth * 2, this->inputHeight * 2);
		this->updateRegionScreen = region.ZoomAndDialate(scale, scale * 2.0f, (float)this->updateLeft, (float)this->updateTop, this->updateLeft, this->updateTop, this->updateLeft + this->updateWidth, this->updateTop + this->updateHeight);

		if (this->upscaledTextureDirty && this->GetUpscaleFilter() != 0)
		{
			this->upscaledTextureDirty = false;
			//this->borderDirty = true;
			this->updateRegion1X.AddRectangle(inputLeft, inputTop, inputWidth, inputHeight);
			this->updateRegion2X.AddRectangle(inputLeft * 2, inputTop * 2, inputWidth * 2, inputHeight * 2);
			this->updateRegionScreen.AddRectangle(updateLeft, updateTop, updateWidth, updateHeight);
		}

		if (this->borderDirty)
		{
			this->borderDirty = false;
			RECT entireWindow;
			GetClientRect(hwnd, &entireWindow);
			this->updateRegionScreen.AddRectangle(entireWindow);
			this->oldUpdateRegions.clear();
		}

	}

	void SetBorderDirty()
	{
		this->borderDirty = true;
	}
};
