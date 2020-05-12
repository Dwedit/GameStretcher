#pragma once

struct IUnknown;
#define NOMINMAX
#include <Windows.h>

#include <vector>
using std::vector;
typedef unsigned char byte;

class Region
{
	HRGN hRegion;
	HRGN hRegionTemp;
	bool isEmpty;
public:
	Region();
	~Region();
	void AddRectangle(LPCRECT rect);
	void AddRectangle(const RECT& rect);
	void Clear();
	void Release();
	bool IsEmpty() const;
	RECT GetBoundingBox() const;
	vector<RECT> GetRegionRectangles() const;
	vector<byte> GetRegionData() const;

	Region(const Region& region);
	Region(Region&& region) noexcept;
	void operator=(const Region& region);
	void operator=(Region&& region) noexcept;
	static void swap(Region& region1, Region&& region2) noexcept;
	static void swap(Region& region1, Region& region2) noexcept;

	void UnionWith(const Region& otherRegion);
	Region Union(const Region& otherRegion) const;
	Region Union(Region&& otherRegion) const;

	Region ZoomAndDialate(float zoomFactor, float dialate, float xOffset, float yOffset, int minX, int minY, int maxX, int maxY) const;

};