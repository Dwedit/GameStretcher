#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <d3d9.h>

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

class CVertexBuffer
{
protected:
	IDirect3DVertexBuffer9* vertexBuffer;
public:


};