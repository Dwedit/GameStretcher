#include "D3D9Override.h"
#include "TinyMap.h"
#include <type_traits>
#include <memory>

using std::unique_ptr;
using std::make_unique;
using std::remove_reference;

CachedVectorMap<IDirect3D9*, unique_ptr<D3D9Context2>> d3d9ContextMap;
CachedVectorMap<IDirect3DDevice9*, unique_ptr<D3D9DeviceContext>> d3d9DeviceContextMap;
CachedVectorMap<IDirect3DSwapChain9*, unique_ptr<D3D9SwapChainContext>> d3d9SwapChainContextMap;

D3D9Context2* GetD3D9Context(IDirect3D9* key, bool allocate)
{
	//Given a D3D9 object, create or find an existing D3D9 Context
	auto& map = d3d9ContextMap;
	typedef remove_reference<decltype(map)>::type::ValueType::element_type TValue;

	auto* ref = map.GetReference(key);
	if (ref != NULL) { return ref->get(); }
	else
	{
		if (!allocate) return NULL;
		map.Set(key, make_unique<TValue>());
		ref = map.GetReference(key);
		//ref->get()->Init(key);
		return ref->get();
	}
}
D3D9Context2* GetD3D9Context()
{
	//Gets the last used D3D9 Context
	auto* ref = d3d9ContextMap.GetMostRecentValue();
	if (ref == NULL) return NULL;
	return ref->get();
}
void RemoveD3D9Context(IDirect3D9* key)
{
	//Deletes a D3D9 Context
	d3d9ContextMap.Remove(key);
}
D3D9DeviceContext* GetD3D9DeviceContext(IDirect3DDevice9* key, bool allocate)
{
	auto& map = d3d9DeviceContextMap;
	typedef remove_reference<decltype(map)>::type::ValueType::element_type TValue;

	//Given a D3D9 Device object, create or find an existing D3D9 Device Context
	auto* ref = map.GetReference(key);
	if (ref != NULL) { return ref->get(); }
	else
	{
		if (!allocate) return NULL;
		map.Set(key, make_unique<TValue>());
		ref = map.GetReference(key);
		//ref->get()->Init(key);
		return ref->get();
	}
}
D3D9DeviceContext* GetD3D9DeviceContext()
{
	//Gets the last used D3D9 Device Context
	auto* ref = d3d9DeviceContextMap.GetMostRecentValue();
	if (ref == NULL) return NULL;
	return ref->get();
}
void RemoveD3D9DeviceContext(IDirect3DDevice9* key)
{
	//Deletes a D3D9 Device Context
	d3d9DeviceContextMap.Remove(key);
}
D3D9SwapChainContext* GetD3D9SwapChainContext(IDirect3DSwapChain9* key, bool allocate)
{
	auto& map = d3d9SwapChainContextMap;
	typedef remove_reference<decltype(map)>::type::ValueType::element_type TValue;

	//Given a D3D9 Swapchain object, create or find an existing D3D9 Swapchain Context
	auto* ref = map.GetReference(key);
	if (ref != NULL) { return ref->get(); }
	else
	{
		if (!allocate) return NULL;
		map.Set(key, make_unique<TValue>());
		ref = map.GetReference(key);
		//ref->get()->Init(key);
		return ref->get();
	}
}
D3D9SwapChainContext* GetD3D9SwapChainContext()
{
	//Gets the last used D3D9 Swapchain Context
	auto* ref = d3d9SwapChainContextMap.GetMostRecentValue();
	if (ref == NULL) return NULL;
	return ref->get();
}
bool SwapChainContextExists(IDirect3DSwapChain9* key)
{
	auto& map = d3d9SwapChainContextMap;
	return map.ContainsKey(key);
}
void RemoveD3D9SwapChainContext(IDirect3DSwapChain9* key)
{
	//Deletes a D3D9 Swapchain Context
	d3d9SwapChainContextMap.Remove(key);
}
