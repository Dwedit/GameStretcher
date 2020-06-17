#include "D3D9Override.h"
#include "TinyMap.h"
#include <type_traits>

using std::remove_pointer;

CachedVectorMap<IDirect3D9ExVtbl*, IDirect3D9ExVtbl*> d3d9VTableMap;
CachedVectorMap<IDirect3DDevice9ExVtbl*, IDirect3DDevice9ExVtbl*> d3d9DeviceVTableMap;
CachedVectorMap<IDirect3DSwapChain9ExVtbl*, IDirect3DSwapChain9ExVtbl*> d3d9SwapChainVTableMap;

IDirect3D9ExVtbl* GetOriginalVTable(IDirect3D9* d3d9)
{
	//Given a D3D9 object that has a modified VTable, return the original VTable
	auto& map = d3d9VTableMap;
	IDirect3D9Ex_* obj = (IDirect3D9Ex_*)d3d9;
	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Find the key that was associated with that VTable
	auto key = map.GetKey(inputVTable);
	//If nothing was found, return the original object's VTable
	if (key == NULL) return inputVTable;
	//Return the VTable that was found
	return key;
}
IDirect3DDevice9ExVtbl* GetOriginalVTable(IDirect3DDevice9* device)
{
	//Given a D3D9 Device object that has a modified VTable, return the original VTable
	auto& map = d3d9DeviceVTableMap;
	IDirect3DDevice9Ex_* obj = (IDirect3DDevice9Ex_*)device;
	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Find the key that was associated with that VTable
	auto key = map.GetKey(inputVTable);
	//If nothing was found, return the original object's VTable
	if (key == NULL) return inputVTable;
	//Return the VTable that was found
	return key;
}
IDirect3DSwapChain9ExVtbl* GetOriginalVTable(IDirect3DSwapChain9* swapChain)
{
	//Given a D3D9 Swap chain object that has a modified VTable, return the original VTable
	auto& map = d3d9SwapChainVTableMap;
	IDirect3DSwapChain9Ex_* obj = (IDirect3DSwapChain9Ex_*)swapChain;
	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Find the key that was associated with that VTable
	auto key = map.GetKey(inputVTable);
	//If nothing was found, return the original object's VTable
	if (key == NULL) return inputVTable;
	//Return the VTable that was found
	return key;
}

IDirect3D9ExVtbl* GetNewVTable(IDirect3D9* d3d9)
{
	//Given a D3D9 object which has the original VTable, allocate and return a new VTable, or return a previously found VTable
	auto& map = d3d9VTableMap;
	IDirect3D9Ex_* obj = (IDirect3D9Ex_*)d3d9;

	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Is this VTable already hooked, with a reference back to the original?
	auto key = map.GetKey(inputVTable);
	if (key != NULL)
	{
		//return the original object
		return inputVTable;
	}
	//Is this VTable already hooked, but we provided the original VTable?
	auto value = map.Get(inputVTable);
	if (value != NULL)
	{
		return value;
	}
	//allocate a new object
	typedef remove_pointer<decltype(inputVTable)>::type TValue;
	auto newVTable = new TValue(*inputVTable);
	map.Set(inputVTable, newVTable);
	return newVTable;
}
IDirect3DDevice9ExVtbl* GetNewVTable(IDirect3DDevice9* device)
{
	//Given a D3D9 Device object which has the original VTable, allocate and return a new VTable, or return a previously found VTable
	auto& map = d3d9DeviceVTableMap;
	IDirect3DDevice9Ex_* obj = (IDirect3DDevice9Ex_*)device;

	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Is this VTable already hooked, with a reference back to the original?
	auto key = map.GetKey(inputVTable);
	if (key != NULL)
	{
		//return the original object
		return inputVTable;
	}
	//Is this VTable already hooked, but we provided the original VTable?
	auto value = map.Get(inputVTable);
	if (value != NULL)
	{
		return value;
	}
	//allocate a new object
	typedef remove_pointer<decltype(inputVTable)>::type TValue;
	auto newVTable = new TValue(*inputVTable);
	map.Set(inputVTable, newVTable);
	return newVTable;
}
IDirect3DSwapChain9ExVtbl* GetNewVTable(IDirect3DSwapChain9* swapChain)
{
	//Given a D3D9 Swapchain object which has the original VTable, allocate and return a new VTable, or return a previously found VTable
	auto& map = d3d9SwapChainVTableMap;
	IDirect3DSwapChain9Ex_* obj = (IDirect3DSwapChain9Ex_*)swapChain;

	//Get input VTable
	auto inputVTable = obj->lpVtbl;
	//Is this VTable already hooked, with a reference back to the original?
	auto key = map.GetKey(inputVTable);
	if (key != NULL)
	{
		//return the original object
		return inputVTable;
	}
	//Is this VTable already hooked, but we provided the original VTable?
	auto value = map.Get(inputVTable);
	if (value != NULL)
	{
		return value;
	}
	//allocate a new object
	typedef remove_pointer<decltype(inputVTable)>::type TValue;
	auto newVTable = new TValue(*inputVTable);
	map.Set(inputVTable, newVTable);
	return newVTable;
}
