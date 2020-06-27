#include "D3D9Override.h"
#include "TinyMap.h"
#include <type_traits>
#include <memory>

using std::unique_ptr;
using std::make_unique;
using std::remove_reference;

class D3D9ObjectStore
{
public:
	~D3D9ObjectStore()
	{
		Dispose();
	}

	void RemoveDisposedObjects()
	{
		auto d3d9s = this->d3d9ContextMap.GetKeys();
		auto d3d9Devices = this->d3d9DeviceContextMap.GetKeys();
		auto d3d9SwapChains = this->d3d9SwapChainContextMap.GetKeys();
		
		auto d3d9Contexts = this->d3d9ContextMap.GetValues();
		auto d3d9DeviceContexts = this->d3d9DeviceContextMap.GetValues();
		auto d3d9SwapChainContexts = this->d3d9SwapChainContextMap.GetValues();

		vector<IDirect3D9*> d3d9sToRemove;
		vector<IDirect3DDevice9*> devicesToRemove;
		vector<IDirect3DSwapChain9*> swapChainsToRemove;

		for (int i = 0; i < d3d9Contexts.size(); i++)
		{
			auto context = d3d9Contexts[i]->get();
			if (context->d3d9 == NULL)
			{
				d3d9sToRemove.push_back(d3d9s[i]);
			}
		}
		for (int i = 0; i < d3d9DeviceContexts.size(); i++)
		{
			auto context = d3d9SwapChainContexts[i]->get();
			if (context->device == NULL)
			{
				devicesToRemove.push_back(d3d9Devices[i]);
			}
		}
		for (int i = 0; i < d3d9SwapChainContexts.size(); i++)
		{
			auto context = d3d9SwapChainContexts[i]->get();
			if (context->realSwapChain == NULL)
			{
				swapChainsToRemove.push_back(d3d9SwapChains[i]);
			}
		}

		for (int i = 0; i < d3d9sToRemove.size(); i++)
		{
			this->RemoveD3D9Context(d3d9sToRemove[i]);
		}
		for (int i = 0; i < devicesToRemove.size(); i++)
		{
			this->RemoveD3D9DeviceContext(devicesToRemove[i]);
		}
		for (int i = 0; i < swapChainsToRemove.size(); i++)
		{
			this->RemoveD3D9SwapChainContext(swapChainsToRemove[i]);
		}
	}

	void Dispose()
	{
		DisposeValues();
		DisposeKeys();
		this->d3d9ContextMap.Clear();
		this->d3d9DeviceContextMap.Clear();
		this->d3d9SwapChainContextMap.Clear();
	}

	void DisposeKeys()
	{
		auto d3d9s = this->d3d9ContextMap.GetKeys();
		auto d3d9Devices = this->d3d9DeviceContextMap.GetKeys();
		auto d3d9SwapChains = this->d3d9SwapChainContextMap.GetKeys();

		for (int i = 0; i < d3d9SwapChains.size(); i++)
		{
			SafeRelease(d3d9SwapChains[i]);
		}
		for (int i = 0; i < d3d9Devices.size(); i++)
		{
			SafeRelease(d3d9Devices[i]);
		}
		for (int i = 0; i < d3d9s.size(); i++)
		{
			SafeRelease(d3d9s[i]);
		}
	}
	void DisposeValues()
	{
		auto d3d9Contexts = this->d3d9ContextMap.GetValues();
		auto d3d9DeviceContexts = this->d3d9DeviceContextMap.GetValues();
		auto d3d9SwapChainContexts = this->d3d9SwapChainContextMap.GetValues();

		for (int i = 0; i < d3d9SwapChainContexts.size(); i++)
		{
			auto context = d3d9SwapChainContexts[i]->get();
			context->Dispose();
		}
		for (int i = 0; i < d3d9DeviceContexts.size(); i++)
		{
			auto context = d3d9DeviceContexts[i]->get();
			context->Dispose();
		}
		for (int i = 0; i < d3d9Contexts.size(); i++)
		{
			auto context = d3d9Contexts[i]->get();
			context->Dispose();
		}
	}

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
			key->AddRef();
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
		if (d3d9ContextMap.Remove(key))
		{
			key->Release();
		}
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
			key->AddRef();
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
		if (d3d9DeviceContextMap.Remove(key))
		{
			key->Release();
		}
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
			key->AddRef();
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
		if (d3d9SwapChainContextMap.Remove(key))
		{
			key->Release();
		}
	}

private:
	CachedVectorMap<IDirect3D9*, unique_ptr<D3D9Context2>> d3d9ContextMap;
	CachedVectorMap<IDirect3DDevice9*, unique_ptr<D3D9DeviceContext>> d3d9DeviceContextMap;
	CachedVectorMap<IDirect3DSwapChain9*, unique_ptr<D3D9SwapChainContext>> d3d9SwapChainContextMap;
};

D3D9ObjectStore objectStore;




D3D9Context2* GetD3D9Context(IDirect3D9* key, bool allocate)
{
	return objectStore.GetD3D9Context(key, allocate);
}
D3D9Context2* GetD3D9Context()
{
	return objectStore.GetD3D9Context();
}
void RemoveD3D9Context(IDirect3D9* key)
{
	objectStore.RemoveD3D9Context(key);
}
D3D9DeviceContext* GetD3D9DeviceContext(IDirect3DDevice9* key, bool allocate)
{
	return objectStore.GetD3D9DeviceContext(key, allocate);
}
D3D9DeviceContext* GetD3D9DeviceContext()
{
	return objectStore.GetD3D9DeviceContext();
}
void RemoveD3D9DeviceContext(IDirect3DDevice9* key)
{
	objectStore.RemoveD3D9DeviceContext(key);
}
D3D9SwapChainContext* GetD3D9SwapChainContext(IDirect3DSwapChain9* key, bool allocate)
{
	return objectStore.GetD3D9SwapChainContext(key, allocate);
}
D3D9SwapChainContext* GetD3D9SwapChainContext()
{
	return objectStore.GetD3D9SwapChainContext();
}
bool SwapChainContextExists(IDirect3DSwapChain9* key)
{
	return objectStore.SwapChainContextExists(key);
}
void RemoveD3D9SwapChainContext(IDirect3DSwapChain9* key)
{
	objectStore.RemoveD3D9SwapChainContext(key);
}
void RemoveD3D9DisposedObjects()
{
	objectStore.RemoveDisposedObjects();
}
