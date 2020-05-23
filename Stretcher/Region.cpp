#include "Region.h"
#include <algorithm>
#include <utility>

Region::Region()
{
	hRegion = NULL;
	hRegionTemp = NULL;
	isEmpty = true;
}

Region::~Region()
{
	Release();
}

void Region::Release()
{
	if (hRegion != NULL)
	{
		DeleteObject(hRegion);
		hRegion = NULL;
	}
	if (hRegionTemp != NULL)
	{
		DeleteObject(hRegionTemp);
		hRegionTemp = NULL;
	}
	isEmpty = true;
}

bool Region::IsEmpty() const
{
	return hRegion == NULL || isEmpty;
}

void Region::AddRectangle(LPCRECT rect)
{
	if (rect == NULL) return;
	if (hRegion == NULL)
	{
		hRegion = CreateRectRgnIndirect(rect);
		isEmpty = false;
	}
	else
	{
		if (isEmpty)
		{
			isEmpty = false;
			SetRectRgn(hRegion, rect->left, rect->top, rect->right, rect->bottom);
			return;
		}
		if (hRegionTemp == NULL)
		{
			hRegionTemp = CreateRectRgnIndirect(rect);
		}
		else
		{
			SetRectRgn(hRegionTemp, rect->left, rect->top, rect->right, rect->bottom);
		}
		CombineRgn(hRegion, hRegion, hRegionTemp, RGN_OR);
	}
}

void Region::AddRectangle(const RECT& rect)
{
	AddRectangle(&rect);
}

void Region::AddRectangle(int x, int y, int width, int height)
{
	RECT rect{ x, y, x + width, y + height };
	AddRectangle(&rect);
}

void Region::Clear()
{
	//Release();
	isEmpty = true;
}

RECT Region::GetBoundingBox() const
{
	if (IsEmpty())
	{
		return RECT();
	}
	else
	{
		RECT result;
		int regionType = GetRgnBox(hRegion, &result);
		if (regionType == NULLREGION)
		{
			return RECT();
		}
		return result;
	}
}

vector<RECT> Region::GetRegionRectangles() const
{
	vector<RECT> result;
	vector<byte> regionData = GetRegionData();
	if (regionData.size() == 0) return result;
	LPRGNDATA rgnData = (LPRGNDATA)&regionData[0];
	int rectangleCount = rgnData->rdh.nCount;
	LPCRECT rects = (LPCRECT)&rgnData->Buffer[0];
	result.resize(rectangleCount);
	for (int i = 0; i < rectangleCount; i++)
	{
		result[i] = rects[i];
	}
	return result;
}
vector<byte> Region::GetRegionData() const
{
	vector<byte> result;
	if (IsEmpty()) return result;
	DWORD bytesNeeded = ::GetRegionData(hRegion, 0, NULL);
	result.resize(bytesNeeded);
	::GetRegionData(hRegion, result.size(), (LPRGNDATA)&result[0]);
	return result;
}

Region::Region(const Region& region)
{
	this->hRegion = NULL;
	this->hRegionTemp = NULL;
	this->isEmpty = true;
	*this = region;
}
Region::Region(Region&& region) noexcept
{
	this->hRegion = NULL;
	this->hRegionTemp = NULL;
	this->isEmpty = true;
	*this = std::forward<Region>(region);
	//swap(*this, region);
}

Region::Region(HRGN hRgn)
{
	this->hRegion = NULL;
	this->hRegionTemp = NULL;
	this->isEmpty = true;
	*this = hRgn;
}
void Region::operator=(HRGN hRgn)
{
	isEmpty = false;
	if (this->hRegion == NULL)
	{
		this->hRegion = CreateRectRgn(0, 0, 0, 0);
	}
	CombineRgn(this->hRegion, hRgn, NULL, RGN_COPY);
}

void Region::operator=(const Region& region)
{
	if (region.IsEmpty())
	{
		Clear();
		return;
	}
	isEmpty = false;
	if (this->hRegion == NULL)
	{
		this->hRegion = CreateRectRgn(0, 0, 0, 0);
	}
	CombineRgn(this->hRegion, region.hRegion, NULL, RGN_COPY);
}
void Region::operator=(Region&& region) noexcept
{
	if (this->hRegion == NULL)
	{
		swap(*this, region);
	}
	else
	{
		*this = region;
	}
}
void Region::swap(Region& region1, Region&& region2) noexcept
{
	std::swap(region1.hRegion, region2.hRegion);
	std::swap(region1.hRegionTemp, region2.hRegionTemp);
	std::swap(region1.isEmpty, region2.isEmpty);
}
void Region::swap(Region& region1, Region& region2) noexcept
{
	std::swap(region1.hRegion, region2.hRegion);
	std::swap(region1.hRegionTemp, region2.hRegionTemp);
	std::swap(region1.isEmpty, region2.isEmpty);
}

void Region::UnionWith(const Region& otherRegion)
{
	if (otherRegion.IsEmpty())
	{
		return;
	}
	if (IsEmpty())
	{
		*this = otherRegion;
		return;
	}
	CombineRgn(this->hRegion, this->hRegion, otherRegion.hRegion, RGN_OR);
}
Region Region::Union(const Region& otherRegion) const
{
	Region newRegion = *this;
	newRegion.UnionWith(otherRegion);
	return newRegion;
}
Region Region::Union(Region&& otherRegion) const
{
	Region newRegion = otherRegion;
	newRegion.UnionWith(*this);
	return newRegion;
}
HRGN Region::DetachHrgn()
{
	HRGN hRgn = this->hRegion;
	this->hRegion = NULL;
	return hRgn;
}
void Region::AttachHrgn(HRGN hrgn)
{
	Release();
	this->hRegion = hrgn;
	this->isEmpty = false;
}

#pragma float_control(precise, off)
#pragma fp_contract(on)

inline int Floor(float f)
{
	const float epsilon = 1.0f / 256.0f;
	return ((int)(f + 32768.0f + epsilon)) - 32768;
}
inline int Ceil(float f)
{
	const float epsilon = 1.0f / 256.0f;
	return ((int)(f + 32768.0f + (1.0f - epsilon))) - 32768;
}


Region Region::ZoomAndDialate(float zoomFactor, float dialate, float xOffset, float yOffset, int minX, int minY, int maxX, int maxY) const
{
	Region result;
	vector<RECT> rects = GetRegionRectangles();
	for (size_t i = 0; i < rects.size(); i++)
	{
		const RECT& rect = rects[i];
		RECT newRect;
		float left = (float)rect.left;
		float top = (float)rect.top;
		float right = (float)rect.right;
		float bottom = (float)rect.bottom;

		left = left * zoomFactor - dialate + xOffset;
		right = right * zoomFactor + dialate + xOffset;
		top = top * zoomFactor - dialate + yOffset;
		bottom = bottom * zoomFactor + dialate + yOffset;

		newRect.left = Floor(left);
		newRect.top = Floor(top);
		newRect.right = Ceil(right);
		newRect.bottom = Ceil(bottom);

		if (newRect.left < minX) newRect.left = minX;
		if (newRect.right < minX) newRect.right = minX;
		if (newRect.top < minY) newRect.top = minY;
		if (newRect.bottom < minY) newRect.bottom = minY;

		if (newRect.left > maxX) newRect.left = maxX;
		if (newRect.right > maxX) newRect.right = maxX;
		if (newRect.top > maxY) newRect.top = maxY;
		if (newRect.bottom > maxY) newRect.bottom = maxY;

		result.AddRectangle(&newRect);
	}
	return result;
}
