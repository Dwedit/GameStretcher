#include "RegistryWrappers.h"
#include "ImportReplacer.h"

#include "StringUtil.h"
#include <string>
#include <type_traits>
#include <utility>
#include "tinymap.h"
#include "Win32Ex.h"
using std::swap;
using std::forward;
using std::string;
using std::wstring;

//Replace the Functions
void BuildImportMap_Registry()
{
	extern ImportMap importMap;
#define ReplaceImport(dllName, functionName, replacementFunction, pOldFunction) \
    importMap.AddImport((dllName),(functionName),(replacementFunction),(pOldFunction))

	ReplaceImport("Advapi32.dll", "RegCloseKey", (FARPROC)RegCloseKey_Replacement, (FARPROC*)&RegCloseKey_OLD);
	//ReplaceImport("Advapi32.dll", "RegOverridePredefKey", (FARPROC)RegOverridePredefKey_Replacement, (FARPROC*)&RegOverridePredefKey_OLD);
	//ReplaceImport("Advapi32.dll", "RegOpenUserClassesRoot", (FARPROC)RegOpenUserClassesRoot_Replacement, (FARPROC*)&RegOpenUserClassesRoot_OLD);
	//ReplaceImport("Advapi32.dll", "RegOpenCurrentUser", (FARPROC)RegOpenCurrentUser_Replacement, (FARPROC*)&RegOpenCurrentUser_OLD);
	//ReplaceImport("Advapi32.dll", "RegDisablePredefinedCache", (FARPROC)RegDisablePredefinedCache_Replacement, (FARPROC*)&RegDisablePredefinedCache_OLD);
	//ReplaceImport("Advapi32.dll", "RegDisablePredefinedCacheEx", (FARPROC)RegDisablePredefinedCacheEx_Replacement, (FARPROC*)&RegDisablePredefinedCacheEx_OLD);
	//ReplaceImport("Advapi32.dll", "RegConnectRegistryA", (FARPROC)RegConnectRegistryA_Replacement, (FARPROC*)&RegConnectRegistryA_OLD);
	//ReplaceImport("Advapi32.dll", "RegConnectRegistryW", (FARPROC)RegConnectRegistryW_Replacement, (FARPROC*)&RegConnectRegistryW_OLD);
	//ReplaceImport("Advapi32.dll", "RegConnectRegistryExA", (FARPROC)RegConnectRegistryExA_Replacement, (FARPROC*)&RegConnectRegistryExA_OLD);
	//ReplaceImport("Advapi32.dll", "RegConnectRegistryExW", (FARPROC)RegConnectRegistryExW_Replacement, (FARPROC*)&RegConnectRegistryExW_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyA", (FARPROC)RegCreateKeyA_Replacement, (FARPROC*)&RegCreateKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyW", (FARPROC)RegCreateKeyW_Replacement, (FARPROC*)&RegCreateKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyExA", (FARPROC)RegCreateKeyExA_Replacement, (FARPROC*)&RegCreateKeyExA_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyExW", (FARPROC)RegCreateKeyExW_Replacement, (FARPROC*)&RegCreateKeyExW_OLD);
	//ReplaceImport("Advapi32.dll", "RegCreateKeyTransactedA", (FARPROC)RegCreateKeyTransactedA_Replacement, (FARPROC*)&RegCreateKeyTransactedA_OLD);
	//ReplaceImport("Advapi32.dll", "RegCreateKeyTransactedW", (FARPROC)RegCreateKeyTransactedW_Replacement, (FARPROC*)&RegCreateKeyTransactedW_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyA", (FARPROC)RegDeleteKeyA_Replacement, (FARPROC*)&RegDeleteKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyW", (FARPROC)RegDeleteKeyW_Replacement, (FARPROC*)&RegDeleteKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyExA", (FARPROC)RegDeleteKeyExA_Replacement, (FARPROC*)&RegDeleteKeyExA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyExW", (FARPROC)RegDeleteKeyExW_Replacement, (FARPROC*)&RegDeleteKeyExW_OLD);
	//ReplaceImport("Advapi32.dll", "RegDeleteKeyTransactedA", (FARPROC)RegDeleteKeyTransactedA_Replacement, (FARPROC*)&RegDeleteKeyTransactedA_OLD);
	//ReplaceImport("Advapi32.dll", "RegDeleteKeyTransactedW", (FARPROC)RegDeleteKeyTransactedW_Replacement, (FARPROC*)&RegDeleteKeyTransactedW_OLD);
	//ReplaceImport("Advapi32.dll", "RegDisableReflectionKey", (FARPROC)RegDisableReflectionKey_Replacement, (FARPROC*)&RegDisableReflectionKey_OLD);
	//ReplaceImport("Advapi32.dll", "RegEnableReflectionKey", (FARPROC)RegEnableReflectionKey_Replacement, (FARPROC*)&RegEnableReflectionKey_OLD);
	//ReplaceImport("Advapi32.dll", "RegQueryReflectionKey", (FARPROC)RegQueryReflectionKey_Replacement, (FARPROC*)&RegQueryReflectionKey_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteValueA", (FARPROC)RegDeleteValueA_Replacement, (FARPROC*)&RegDeleteValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteValueW", (FARPROC)RegDeleteValueW_Replacement, (FARPROC*)&RegDeleteValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumKeyA", (FARPROC)RegEnumKeyA_Replacement, (FARPROC*)&RegEnumKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumKeyW", (FARPROC)RegEnumKeyW_Replacement, (FARPROC*)&RegEnumKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumKeyExA", (FARPROC)RegEnumKeyExA_Replacement, (FARPROC*)&RegEnumKeyExA_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumKeyExW", (FARPROC)RegEnumKeyExW_Replacement, (FARPROC*)&RegEnumKeyExW_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumValueA", (FARPROC)RegEnumValueA_Replacement, (FARPROC*)&RegEnumValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumValueW", (FARPROC)RegEnumValueW_Replacement, (FARPROC*)&RegEnumValueW_OLD);
	//ReplaceImport("Advapi32.dll", "RegFlushKey", (FARPROC)RegFlushKey_Replacement, (FARPROC*)&RegFlushKey_OLD);
	//ReplaceImport("Advapi32.dll", "RegGetKeySecurity", (FARPROC)RegGetKeySecurity_Replacement, (FARPROC*)&RegGetKeySecurity_OLD);
	//ReplaceImport("Advapi32.dll", "RegLoadKeyA", (FARPROC)RegLoadKeyA_Replacement, (FARPROC*)&RegLoadKeyA_OLD);
	//ReplaceImport("Advapi32.dll", "RegLoadKeyW", (FARPROC)RegLoadKeyW_Replacement, (FARPROC*)&RegLoadKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegNotifyChangeKeyValue", (FARPROC)RegNotifyChangeKeyValue_Replacement, (FARPROC*)&RegNotifyChangeKeyValue_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyA", (FARPROC)RegOpenKeyA_Replacement, (FARPROC*)&RegOpenKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyW", (FARPROC)RegOpenKeyW_Replacement, (FARPROC*)&RegOpenKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyExA", (FARPROC)RegOpenKeyExA_Replacement, (FARPROC*)&RegOpenKeyExA_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyExW", (FARPROC)RegOpenKeyExW_Replacement, (FARPROC*)&RegOpenKeyExW_OLD);
	//ReplaceImport("Advapi32.dll", "RegOpenKeyTransactedA", (FARPROC)RegOpenKeyTransactedA_Replacement, (FARPROC*)&RegOpenKeyTransactedA_OLD);
	//ReplaceImport("Advapi32.dll", "RegOpenKeyTransactedW", (FARPROC)RegOpenKeyTransactedW_Replacement, (FARPROC*)&RegOpenKeyTransactedW_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryInfoKeyA", (FARPROC)RegQueryInfoKeyA_Replacement, (FARPROC*)&RegQueryInfoKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryInfoKeyW", (FARPROC)RegQueryInfoKeyW_Replacement, (FARPROC*)&RegQueryInfoKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryValueA", (FARPROC)RegQueryValueA_Replacement, (FARPROC*)&RegQueryValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryValueW", (FARPROC)RegQueryValueW_Replacement, (FARPROC*)&RegQueryValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryMultipleValuesA", (FARPROC)RegQueryMultipleValuesA_Replacement, (FARPROC*)&RegQueryMultipleValuesA_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryMultipleValuesW", (FARPROC)RegQueryMultipleValuesW_Replacement, (FARPROC*)&RegQueryMultipleValuesW_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryValueExA", (FARPROC)RegQueryValueExA_Replacement, (FARPROC*)&RegQueryValueExA_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryValueExW", (FARPROC)RegQueryValueExW_Replacement, (FARPROC*)&RegQueryValueExW_OLD);
	//ReplaceImport("Advapi32.dll", "RegReplaceKeyA", (FARPROC)RegReplaceKeyA_Replacement, (FARPROC*)&RegReplaceKeyA_OLD);
	//ReplaceImport("Advapi32.dll", "RegReplaceKeyW", (FARPROC)RegReplaceKeyW_Replacement, (FARPROC*)&RegReplaceKeyW_OLD);
	//ReplaceImport("Advapi32.dll", "RegRestoreKeyA", (FARPROC)RegRestoreKeyA_Replacement, (FARPROC*)&RegRestoreKeyA_OLD);
	//ReplaceImport("Advapi32.dll", "RegRestoreKeyW", (FARPROC)RegRestoreKeyW_Replacement, (FARPROC*)&RegRestoreKeyW_OLD);
	//ReplaceImport("Advapi32.dll", "RegRenameKey", (FARPROC)RegRenameKey_Replacement, (FARPROC*)&RegRenameKey_OLD);
	//ReplaceImport("Advapi32.dll", "RegSaveKeyA", (FARPROC)RegSaveKeyA_Replacement, (FARPROC*)&RegSaveKeyA_OLD);
	//ReplaceImport("Advapi32.dll", "RegSaveKeyW", (FARPROC)RegSaveKeyW_Replacement, (FARPROC*)&RegSaveKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegSetKeySecurity", (FARPROC)RegSetKeySecurity_Replacement, (FARPROC*)&RegSetKeySecurity_OLD);
	ReplaceImport("Advapi32.dll", "RegSetValueA", (FARPROC)RegSetValueA_Replacement, (FARPROC*)&RegSetValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegSetValueW", (FARPROC)RegSetValueW_Replacement, (FARPROC*)&RegSetValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegSetValueExA", (FARPROC)RegSetValueExA_Replacement, (FARPROC*)&RegSetValueExA_OLD);
	ReplaceImport("Advapi32.dll", "RegSetValueExW", (FARPROC)RegSetValueExW_Replacement, (FARPROC*)&RegSetValueExW_OLD);
	//ReplaceImport("Advapi32.dll", "RegUnLoadKeyA", (FARPROC)RegUnLoadKeyA_Replacement, (FARPROC*)&RegUnLoadKeyA_OLD);
	//ReplaceImport("Advapi32.dll", "RegUnLoadKeyW", (FARPROC)RegUnLoadKeyW_Replacement, (FARPROC*)&RegUnLoadKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyValueA", (FARPROC)RegDeleteKeyValueA_Replacement, (FARPROC*)&RegDeleteKeyValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyValueW", (FARPROC)RegDeleteKeyValueW_Replacement, (FARPROC*)&RegDeleteKeyValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegSetKeyValueA", (FARPROC)RegSetKeyValueA_Replacement, (FARPROC*)&RegSetKeyValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegSetKeyValueW", (FARPROC)RegSetKeyValueW_Replacement, (FARPROC*)&RegSetKeyValueW_OLD);
	//ReplaceImport("Advapi32.dll", "RegDeleteTreeA", (FARPROC)RegDeleteTreeA_Replacement, (FARPROC*)&RegDeleteTreeA_OLD);
	//ReplaceImport("Advapi32.dll", "RegDeleteTreeW", (FARPROC)RegDeleteTreeW_Replacement, (FARPROC*)&RegDeleteTreeW_OLD);
	//ReplaceImport("Advapi32.dll", "RegCopyTreeA", (FARPROC)RegCopyTreeA_Replacement, (FARPROC*)&RegCopyTreeA_OLD);
	//ReplaceImport("Advapi32.dll", "RegCopyTreeW", (FARPROC)RegCopyTreeW_Replacement, (FARPROC*)&RegCopyTreeW_OLD);
	ReplaceImport("Advapi32.dll", "RegGetValueA", (FARPROC)RegGetValueA_Replacement, (FARPROC*)&RegGetValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegGetValueW", (FARPROC)RegGetValueW_Replacement, (FARPROC*)&RegGetValueW_OLD);
	//ReplaceImport("Advapi32.dll", "RegLoadMUIStringA", (FARPROC)RegLoadMUIStringA_Replacement, (FARPROC*)&RegLoadMUIStringA_OLD);
	//ReplaceImport("Advapi32.dll", "RegLoadMUIStringW", (FARPROC)RegLoadMUIStringW_Replacement, (FARPROC*)&RegLoadMUIStringW_OLD);
	//ReplaceImport("Advapi32.dll", "RegLoadAppKeyA", (FARPROC)RegLoadAppKeyA_Replacement, (FARPROC*)&RegLoadAppKeyA_OLD);
	//ReplaceImport("Advapi32.dll", "RegLoadAppKeyW", (FARPROC)RegLoadAppKeyW_Replacement, (FARPROC*)&RegLoadAppKeyW_OLD);
	//ReplaceImport("Advapi32.dll", "RegSaveKeyExA", (FARPROC)RegSaveKeyExA_Replacement, (FARPROC*)&RegSaveKeyExA_OLD);
	//ReplaceImport("Advapi32.dll", "RegSaveKeyExW", (FARPROC)RegSaveKeyExW_Replacement, (FARPROC*)&RegSaveKeyExW_OLD);
}

//Import Backups (Definitions)
RegCloseKey_FUNC RegCloseKey_OLD = NULL;
//RegOverridePredefKey_FUNC RegOverridePredefKey_OLD = NULL;
//RegOpenUserClassesRoot_FUNC RegOpenUserClassesRoot_OLD = NULL;
//RegOpenCurrentUser_FUNC RegOpenCurrentUser_OLD = NULL;
//RegDisablePredefinedCache_FUNC RegDisablePredefinedCache_OLD = NULL;
//RegDisablePredefinedCacheEx_FUNC RegDisablePredefinedCacheEx_OLD = NULL;
//RegConnectRegistryA_FUNC RegConnectRegistryA_OLD = NULL;
//RegConnectRegistryW_FUNC RegConnectRegistryW_OLD = NULL;
//RegConnectRegistryExA_FUNC RegConnectRegistryExA_OLD = NULL;
//RegConnectRegistryExW_FUNC RegConnectRegistryExW_OLD = NULL;
RegCreateKeyA_FUNC RegCreateKeyA_OLD = NULL;
RegCreateKeyW_FUNC RegCreateKeyW_OLD = NULL;
RegCreateKeyExA_FUNC RegCreateKeyExA_OLD = NULL;
RegCreateKeyExW_FUNC RegCreateKeyExW_OLD = NULL;
//RegCreateKeyTransactedA_FUNC RegCreateKeyTransactedA_OLD = NULL;
//RegCreateKeyTransactedW_FUNC RegCreateKeyTransactedW_OLD = NULL;
RegDeleteKeyA_FUNC RegDeleteKeyA_OLD = NULL;
RegDeleteKeyW_FUNC RegDeleteKeyW_OLD = NULL;
RegDeleteKeyExA_FUNC RegDeleteKeyExA_OLD = NULL;
RegDeleteKeyExW_FUNC RegDeleteKeyExW_OLD = NULL;
//RegDeleteKeyTransactedA_FUNC RegDeleteKeyTransactedA_OLD = NULL;
//RegDeleteKeyTransactedW_FUNC RegDeleteKeyTransactedW_OLD = NULL;
//RegDisableReflectionKey_FUNC RegDisableReflectionKey_OLD = NULL;
//RegEnableReflectionKey_FUNC RegEnableReflectionKey_OLD = NULL;
//RegQueryReflectionKey_FUNC RegQueryReflectionKey_OLD = NULL;
RegDeleteValueA_FUNC RegDeleteValueA_OLD = NULL;
RegDeleteValueW_FUNC RegDeleteValueW_OLD = NULL;
RegEnumKeyA_FUNC RegEnumKeyA_OLD = NULL;
RegEnumKeyW_FUNC RegEnumKeyW_OLD = NULL;
RegEnumKeyExA_FUNC RegEnumKeyExA_OLD = NULL;
RegEnumKeyExW_FUNC RegEnumKeyExW_OLD = NULL;
RegEnumValueA_FUNC RegEnumValueA_OLD = NULL;
RegEnumValueW_FUNC RegEnumValueW_OLD = NULL;
//RegFlushKey_FUNC RegFlushKey_OLD = NULL;
RegGetKeySecurity_FUNC RegGetKeySecurity_OLD = NULL;
//RegLoadKeyA_FUNC RegLoadKeyA_OLD = NULL;
//RegLoadKeyW_FUNC RegLoadKeyW_OLD = NULL;
RegNotifyChangeKeyValue_FUNC RegNotifyChangeKeyValue_OLD = NULL;
RegOpenKeyA_FUNC RegOpenKeyA_OLD = NULL;
RegOpenKeyW_FUNC RegOpenKeyW_OLD = NULL;
RegOpenKeyExA_FUNC RegOpenKeyExA_OLD = NULL;
RegOpenKeyExW_FUNC RegOpenKeyExW_OLD = NULL;
//RegOpenKeyTransactedA_FUNC RegOpenKeyTransactedA_OLD = NULL;
//RegOpenKeyTransactedW_FUNC RegOpenKeyTransactedW_OLD = NULL;
RegQueryInfoKeyA_FUNC RegQueryInfoKeyA_OLD = NULL;
RegQueryInfoKeyW_FUNC RegQueryInfoKeyW_OLD = NULL;
RegQueryValueA_FUNC RegQueryValueA_OLD = NULL;
RegQueryValueW_FUNC RegQueryValueW_OLD = NULL;
RegQueryMultipleValuesA_FUNC RegQueryMultipleValuesA_OLD = NULL;
RegQueryMultipleValuesW_FUNC RegQueryMultipleValuesW_OLD = NULL;
RegQueryValueExA_FUNC RegQueryValueExA_OLD = NULL;
RegQueryValueExW_FUNC RegQueryValueExW_OLD = NULL;
//RegReplaceKeyA_FUNC RegReplaceKeyA_OLD = NULL;
//RegReplaceKeyW_FUNC RegReplaceKeyW_OLD = NULL;
//RegRestoreKeyA_FUNC RegRestoreKeyA_OLD = NULL;
//RegRestoreKeyW_FUNC RegRestoreKeyW_OLD = NULL;
//RegRenameKey_FUNC RegRenameKey_OLD = NULL;
//RegSaveKeyA_FUNC RegSaveKeyA_OLD = NULL;
//RegSaveKeyW_FUNC RegSaveKeyW_OLD = NULL;
RegSetKeySecurity_FUNC RegSetKeySecurity_OLD = NULL;
RegSetValueA_FUNC RegSetValueA_OLD = NULL;
RegSetValueW_FUNC RegSetValueW_OLD = NULL;
RegSetValueExA_FUNC RegSetValueExA_OLD = NULL;
RegSetValueExW_FUNC RegSetValueExW_OLD = NULL;
//RegUnLoadKeyA_FUNC RegUnLoadKeyA_OLD = NULL;
//RegUnLoadKeyW_FUNC RegUnLoadKeyW_OLD = NULL;
RegDeleteKeyValueA_FUNC RegDeleteKeyValueA_OLD = NULL;
RegDeleteKeyValueW_FUNC RegDeleteKeyValueW_OLD = NULL;
RegSetKeyValueA_FUNC RegSetKeyValueA_OLD = NULL;
RegSetKeyValueW_FUNC RegSetKeyValueW_OLD = NULL;
//RegDeleteTreeA_FUNC RegDeleteTreeA_OLD = NULL;
//RegDeleteTreeW_FUNC RegDeleteTreeW_OLD = NULL;
//RegCopyTreeA_FUNC RegCopyTreeA_OLD = NULL;
RegGetValueA_FUNC RegGetValueA_OLD = NULL;
RegGetValueW_FUNC RegGetValueW_OLD = NULL;
//RegCopyTreeW_FUNC RegCopyTreeW_OLD = NULL;
//RegLoadMUIStringA_FUNC RegLoadMUIStringA_OLD = NULL;
//RegLoadMUIStringW_FUNC RegLoadMUIStringW_OLD = NULL;
//RegLoadAppKeyA_FUNC RegLoadAppKeyA_OLD = NULL;
//RegLoadAppKeyW_FUNC RegLoadAppKeyW_OLD = NULL;
//RegSaveKeyExA_FUNC RegSaveKeyExA_OLD = NULL;
//RegSaveKeyExW_FUNC RegSaveKeyExW_OLD = NULL;

struct RegistryVirtualPath;

RegistryVirtualPath* GetOpenKey(HKEY inputHKey);
void SetOpenKey(HKEY openKey, const RegistryVirtualPath& virtualPath);
bool RemoveOpenKey(HKEY openKey);

class AutoString : public wstring
{
private:
	mutable string temp;
public:
	operator LPCSTR() const
	{
		temp = ToAnsi(*this);
		return temp.c_str();
	}
	operator LPCWSTR() const
	{
		return this->c_str();
	}
	void operator=(LPCWSTR str)
	{
		wstring::operator=(str);
	}
	void operator=(const wstring& str)
	{
		wstring::operator=(str);
	}
	void operator=(wstring &&str)
	{
		wstring::operator=(forward<wstring>(str));
	}
};

struct RegistryVirtualPath
{
public:
	HKEY realHKey = NULL;
	HKEY virtualHKey = NULL;
	AutoString realPath;
	AutoString virtualPath;

	static HKEY OpenRegKey(HKEY key, LPCWSTR path, REGSAM regSam = KEY_WRITE | KEY_READ)
	{
		if (key == NULL) return NULL;
		HKEY resultHKey = NULL;
		LONG result = RegOpenKeyExW(key, path, 0, regSam, &resultHKey);
		if (result != ERROR_SUCCESS)
		{
			if (resultHKey != NULL)
			{
				RegCloseKey(resultHKey);
				resultHKey = NULL;
			}
		}
		return resultHKey;
	}
	HKEY OpenVirtualPath(REGSAM regSam = KEY_WRITE | KEY_READ)
	{
		return OpenRegKey(virtualHKey, virtualPath, regSam);
	}
	HKEY OpenRealPath(REGSAM regSam = KEY_WRITE | KEY_READ)
	{
		return OpenRegKey(realHKey, realPath, regSam);
	}
	static HKEY EnsurePathExistsAndOpen(HKEY key, LPCWSTR path, bool *keyExisted = NULL)
	{
		HKEY resultKey = NULL;
		DWORD disposition = 0;
		LONG result = RegCreateKeyExW(key, path, 0, NULL, 0, KEY_WRITE | KEY_READ, NULL, &resultKey, &disposition);
		if (keyExisted != NULL)
		{
			*keyExisted = disposition == REG_OPENED_EXISTING_KEY;
		}
		if (result != ERROR_SUCCESS)
		{
			if (resultKey != NULL)
			{
				RegCloseKey(resultKey);
				resultKey = NULL;
			}
		}
		return resultKey;
	}
	HKEY EnsureVirtualPathExistsAndOpen()
	{
		return EnsurePathExistsAndOpen(this->virtualHKey, this->virtualPath);
	}
	HKEY EnsureRealPathExistsAndOpen()
	{
		return EnsurePathExistsAndOpen(this->realHKey, this->realPath);
	}
	RegistryVirtualPath()
	{

	}
	RegistryVirtualPath(HKEY inputHKey)
	{
		Init(inputHKey, (LPCWSTR)NULL);
	}
	RegistryVirtualPath(HKEY inputHKey, LPCSTR inputPathA)
	{
		Init(inputHKey, inputPathA);
	}
	RegistryVirtualPath(HKEY inputHKey, LPCWSTR inputPathW)
	{
		Init(inputHKey, inputPathW);
	}
	void Init(HKEY inputHKey, LPCSTR inputPathA)
	{
		wstring inputPathW = ToUnicode(inputPathA);
		Init(inputHKey, inputPathW.c_str());
	}
	void Init(HKEY inputHKey, LPCWSTR inputPathW)
	{
		RegistryVirtualPath* openKey = GetOpenKey(inputHKey);
		if (openKey != NULL)
		{
			int len = -1;
			if (inputPathW != NULL)
			{
				len = wcslen(inputPathW);
			}
			this->realPath.reserve(openKey->realPath.size() + 1 + len);
			this->realHKey = openKey->realHKey;
			this->realPath = openKey->realPath;
			if (len > 0)
			{
				AddSlash(this->realPath);
				this->realPath += inputPathW;
			}
		}
		else
		{
			this->realHKey = inputHKey;
			this->realPath = inputPathW;
		}
		bool virtualizationBlocked = false;
#if defined(_WIN64)
		virtualizationBlocked = true;
#endif
		if (this->realHKey == HKEY_LOCAL_MACHINE)
		{
			if (StringStartsWithCaseInsensitive(realPath, L"SOFTWARE\\Classes") ||
				StringStartsWithCaseInsensitive(realPath, L"SOFTWARE\\Microsoft\\Windows"))
			{
				virtualizationBlocked = true;
			}
		}
		if (!virtualizationBlocked)
		{
			if (this->realHKey == HKEY_LOCAL_MACHINE && StringStartsWithCaseInsensitive(this->realPath, L"SOFTWARE\\"))
			{
				if (IsWow64())
				{
					this->virtualHKey = HKEY_CURRENT_USER;
					this->virtualPath = L"Software\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\WOW6432Node\\" + realPath.substr(9);
				}
				else
				{
					this->virtualHKey = HKEY_CURRENT_USER;
					this->virtualPath = L"Software\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\" + realPath.substr(9);
				}
			}
			else
			{
				//TODO: other virtualization (should include other keys that only Admins can write to)
			}
		}
	}
};

CachedVectorMap<HKEY, RegistryVirtualPath> OpenKeysMap;

RegistryVirtualPath* GetOpenKey(HKEY inputHKey)
{
	return OpenKeysMap.GetReference(inputHKey);
}
void SetOpenKey(HKEY openKey, const RegistryVirtualPath& virtualPath)
{
	OpenKeysMap.Set(openKey, virtualPath);
}
void SetOpenKey(HKEY openKey, RegistryVirtualPath&& virtualPath)
{
	OpenKeysMap.Set(openKey, forward<RegistryVirtualPath>(virtualPath));
}
bool RemoveOpenKey(HKEY openKey)
{
	return OpenKeysMap.Remove(openKey);
}
LONG TrySetOpenKey(LONG result, PHKEY phkResult, const RegistryVirtualPath& virtualPath)
{
	HKEY resultKey = NULL;
	if (result == ERROR_SUCCESS && phkResult != NULL)
	{
		resultKey = *phkResult;
		SetOpenKey(resultKey, virtualPath);
	}
	return result;
}

class RegistryVirtualPathHandle
{
public:
	RegistryVirtualPath virtualPath;
	HKEY inputHKey = NULL;
	HKEY realHKey = NULL;
	HKEY virtualHKey = NULL;
	HKEY virtualHKey2 = NULL;
private:
	RegistryVirtualPathHandle(const RegistryVirtualPathHandle&) = delete;
	RegistryVirtualPathHandle() = delete;
	RegistryVirtualPathHandle& operator=(const RegistryVirtualPathHandle&) = delete;
private:
	void Init(bool readWrite = true, bool create = false)
	{
		if (!create)
		{
			virtualHKey = virtualPath.OpenVirtualPath(readWrite ? (KEY_READ | KEY_WRITE) : KEY_READ);
			realHKey = virtualPath.OpenRealPath(readWrite ? (KEY_READ | KEY_WRITE) : KEY_READ);
		}
		else
		{
			virtualHKey = virtualPath.EnsureVirtualPathExistsAndOpen();
			realHKey = virtualPath.EnsureRealPathExistsAndOpen();
		}
	}

public:
	RegistryVirtualPathHandle(RegistryVirtualPathHandle&& other) noexcept
	{
		*this = forward<RegistryVirtualPathHandle>(other);
	}
	RegistryVirtualPathHandle& operator=(RegistryVirtualPathHandle&& other) noexcept
	{
		swap(virtualPath, other.virtualPath);
		swap(realHKey, other.realHKey);
		swap(virtualHKey, other.virtualHKey);
		swap(virtualHKey2, other.virtualHKey2);
	}
	RegistryVirtualPathHandle(HKEY hKey, bool readWrite = true, bool create = false) : inputHKey(hKey), virtualPath(hKey)
	{
		Init(readWrite, create);
	}
	RegistryVirtualPathHandle(HKEY hKey, LPCSTR lpSubKey, bool readWrite = true, bool create = false) : inputHKey(hKey), virtualPath(hKey, lpSubKey)
	{
		Init(readWrite, create);
	}
	RegistryVirtualPathHandle(HKEY hKey, LPCWSTR lpSubKey, bool readWrite = true, bool create = false) : inputHKey(hKey), virtualPath(hKey, lpSubKey)
	{
		Init(readWrite, create);
	}

	~RegistryVirtualPathHandle()
	{
		if (virtualHKey != NULL)
		{
			RegCloseKey(virtualHKey);
		}
		virtualHKey = NULL;
		if (virtualHKey2 != NULL)
		{
			RegCloseKey(virtualHKey2);
		}
		virtualHKey2 = NULL;
		if (realHKey != NULL)
		{
			RegCloseKey(realHKey);
		}
		realHKey = NULL;
	}
};

class EmptyString_T
{
public:
	operator LPCSTR() const {
		return "";
	}
	operator LPCWSTR() const {
		return L"";
	}
} EmptyString;

#define _FORK_REGISTRY(_readwrite, _create, _func, _hKey, ...) \
	RegistryVirtualPathHandle handle = RegistryVirtualPathHandle(_hKey, _readwrite, _create);\
	if (handle.virtualHKey && _func(handle.virtualHKey, __VA_ARGS__) == ERROR_SUCCESS) return ERROR_SUCCESS;\
	if (handle.realHKey && _func(handle.realHKey, __VA_ARGS__) == ERROR_SUCCESS) return ERROR_SUCCESS;\
	return _func(_hKey, __VA_ARGS__); \

#define _FORK_REGISTRY_2(_readwrite, _create, _func, _hKey, _lpSubKey, ...) \
	RegistryVirtualPathHandle handle = RegistryVirtualPathHandle(_hKey, _lpSubKey, _create, false);\
	if (handle.virtualHKey && _func(handle.virtualHKey, EmptyString, __VA_ARGS__) == ERROR_SUCCESS) return ERROR_SUCCESS;\
	if (handle.realHKey && _func(handle.realHKey, EmptyString, __VA_ARGS__) == ERROR_SUCCESS) return ERROR_SUCCESS;\
	return _func(_hKey, _lpSubKey, __VA_ARGS__); \

#define FORK_REGISTRY_3(_func, _hKey, _lpSubKey, ...) \
	RegistryVirtualPath virtualPath = RegistryVirtualPath(_hKey, _lpSubKey);\
	if (virtualPath.virtualHKey && _func(virtualPath.virtualHKey, virtualPath.virtualPath, __VA_ARGS__) == ERROR_SUCCESS) return ERROR_SUCCESS;\
	if (virtualPath.realHKey && _func(virtualPath.realHKey, virtualPath.realPath, __VA_ARGS__) == ERROR_SUCCESS) return ERROR_SUCCESS;\
	return _func(_hKey, _lpSubKey, __VA_ARGS__); \

#define FORK_REGISTRY_READONLY(_func, _hKey, ...) _FORK_REGISTRY(false, false, _func, _hKey, __VA_ARGS__)
#define FORK_REGISTRY_READWRITE(_func, _hKey, ...) _FORK_REGISTRY(true, false, _func, _hKey, __VA_ARGS__)
#define FORK_REGISTRY_CREATE(_func, _hKey, ...) _FORK_REGISTRY(true, true, _func, _hKey, __VA_ARGS__)
//#define FORK_REGISTRY_READONLY_2(_func, _hKey, _lpSubKey, ...) _FORK_REGISTRY_2(false, false, _func, _hKey, _lpSubKey, __VA_ARGS__)
//#define FORK_REGISTRY_READWRITE_2(_func, _hKey, _lpSubKey, ...) _FORK_REGISTRY_2(true, false, _func, _hKey, _lpSubKey, __VA_ARGS__)
#define FORK_REGISTRY_CREATE_2(_func, _hKey, _lpSubKey, ...) _FORK_REGISTRY_2(true, true, _func, _hKey, _lpSubKey, __VA_ARGS__)

//Replacement Functions Code
LSTATUS APIENTRY RegCloseKey_Replacement(HKEY hKey)
{
	//Remove the key from the map, then close it
	RemoveOpenKey(hKey);
	return RegCloseKey_OLD(hKey);
}
//LSTATUS APIENTRY RegOverridePredefKey_Replacement(HKEY hKey, HKEY hNewHKey)
//{
//	return RegOverridePredefKey_OLD(hKey, hNewHKey);
//}
//LSTATUS APIENTRY RegOpenUserClassesRoot_Replacement(HANDLE hToken, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult)
//{
//	return RegOpenUserClassesRoot_OLD(hToken, dwOptions, samDesired, phkResult);
//}
//LSTATUS APIENTRY RegOpenCurrentUser_Replacement(REGSAM samDesired, PHKEY phkResult)
//{
//	return RegOpenCurrentUser_OLD(samDesired, phkResult);
//}
//LSTATUS APIENTRY RegDisablePredefinedCache_Replacement()
//{
//	return RegDisablePredefinedCache_OLD();
//}
//LSTATUS APIENTRY RegDisablePredefinedCacheEx_Replacement()
//{
//	return RegDisablePredefinedCacheEx_OLD();
//}
//LSTATUS APIENTRY RegConnectRegistryA_Replacement(LPCSTR lpMachineName, HKEY hKey, PHKEY phkResult)
//{
//	return RegConnectRegistryA_OLD(lpMachineName, hKey, phkResult);
//}
//LSTATUS APIENTRY RegConnectRegistryW_Replacement(LPCWSTR lpMachineName, HKEY hKey, PHKEY phkResult)
//{
//	return RegConnectRegistryW_OLD(lpMachineName, hKey, phkResult);
//}
//LSTATUS APIENTRY RegConnectRegistryExA_Replacement(LPCSTR lpMachineName, HKEY hKey, ULONG Flags, PHKEY phkResult)
//{
//	return RegConnectRegistryExA_OLD(lpMachineName, hKey, Flags, phkResult);
//}
//LSTATUS APIENTRY RegConnectRegistryExW_Replacement(LPCWSTR lpMachineName, HKEY hKey, ULONG Flags, PHKEY phkResult)
//{
//	return RegConnectRegistryExW_OLD(lpMachineName, hKey, Flags, phkResult);
//}

//small functions to create the key, then store it in the map
inline LSTATUS RegCreateKeyA_(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult, const RegistryVirtualPath& virtualPath)
{
	LONG result = result = RegCreateKeyA_OLD(hKey, lpSubKey, phkResult);
	return TrySetOpenKey(result, phkResult, virtualPath);
}
inline LSTATUS RegCreateKeyW_(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult, const RegistryVirtualPath& virtualPath)
{
	LONG result = result = RegCreateKeyW_OLD(hKey, lpSubKey, phkResult);
	return TrySetOpenKey(result, phkResult, virtualPath);
}
inline LSTATUS RegCreateKeyExA_(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition, const RegistryVirtualPath& virtualPath)
{
	LONG result = RegCreateKeyExA_OLD(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
	return TrySetOpenKey(result, phkResult, virtualPath);
}
inline LSTATUS RegCreateKeyExW_(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition, const RegistryVirtualPath& virtualPath)
{
	LONG result = RegCreateKeyExW_OLD(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
	return TrySetOpenKey(result, phkResult, virtualPath);
}

//"Create" operations for registry attempt to create the full tree of a registry tree, then open it.
//If the key already exists, just open it.
//For virtualized registry, just use Virtual Path and always do creation/open operation.
LSTATUS APIENTRY RegCreateKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
	FORK_REGISTRY_3(RegCreateKeyA_, hKey, lpSubKey, phkResult, virtualPath);
}
LSTATUS APIENTRY RegCreateKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
	FORK_REGISTRY_3(RegCreateKeyW_, hKey, lpSubKey, phkResult, virtualPath);
}
LSTATUS APIENTRY RegCreateKeyExA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	FORK_REGISTRY_3(RegCreateKeyExA_, hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition, virtualPath);
}
LSTATUS APIENTRY RegCreateKeyExW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	FORK_REGISTRY_3(RegCreateKeyExW_, hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition, virtualPath);
}
//These are not used by 32-bit applications and should be removed from this program
//LSTATUS APIENTRY RegCreateKeyTransactedA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID pExtendedParemeter)
//{
//	return RegCreateKeyTransactedA_OLD(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition, hTransaction, pExtendedParemeter);
//}
//LSTATUS APIENTRY RegCreateKeyTransactedW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID pExtendedParemeter)
//{
//	return RegCreateKeyTransactedW_OLD(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition, hTransaction, pExtendedParemeter);
//}
//Delete operations
LSTATUS APIENTRY RegDeleteKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey)
{
	FORK_REGISTRY_3(RegDeleteKeyA_OLD, hKey, lpSubKey);
}
LSTATUS APIENTRY RegDeleteKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey)
{
	FORK_REGISTRY_3(RegDeleteKeyW_OLD, hKey, lpSubKey);
}
LSTATUS APIENTRY RegDeleteKeyExA_Replacement(HKEY hKey, LPCSTR lpSubKey, REGSAM samDesired, DWORD Reserved)
{
	FORK_REGISTRY_3(RegDeleteKeyExA_OLD, hKey, lpSubKey, samDesired, Reserved);
}
LSTATUS APIENTRY RegDeleteKeyExW_Replacement(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired, DWORD Reserved)
{
	FORK_REGISTRY_3(RegDeleteKeyExW_OLD, hKey, lpSubKey, samDesired, Reserved);
}
//These are not used by 32-bit windows apps and should be removed
//LSTATUS APIENTRY RegDeleteKeyTransactedA_Replacement(HKEY hKey, LPCSTR lpSubKey, REGSAM samDesired, DWORD Reserved, HANDLE hTransaction, PVOID pExtendedParameter)
//{
//	return RegDeleteKeyTransactedA_OLD(hKey, lpSubKey, samDesired, Reserved, hTransaction, pExtendedParameter);
//}
//LSTATUS APIENTRY RegDeleteKeyTransactedW_Replacement(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired, DWORD Reserved, HANDLE hTransaction, PVOID pExtendedParameter)
//{
//	return RegDeleteKeyTransactedW_OLD(hKey, lpSubKey, samDesired, Reserved, hTransaction, pExtendedParameter);
//}
//LONG APIENTRY RegDisableReflectionKey_Replacement(HKEY hBase)
//{
//	return RegDisableReflectionKey_OLD(hBase);
//}
//LONG APIENTRY RegEnableReflectionKey_Replacement(HKEY hBase)
//{
//	return RegEnableReflectionKey_OLD(hBase);
//}
//LONG APIENTRY RegQueryReflectionKey_Replacement(HKEY hBase, BOOL* bIsReflectionDisabled)
//{
//	return RegQueryReflectionKey_OLD(hBase, bIsReflectionDisabled);
//}
//Delete Value Operations
LSTATUS APIENTRY RegDeleteValueA_Replacement(HKEY hKey, LPCSTR lpValueName)
{
	FORK_REGISTRY_READWRITE(RegDeleteValueA_OLD, hKey, lpValueName);
}
LSTATUS APIENTRY RegDeleteValueW_Replacement(HKEY hKey, LPCWSTR lpValueName)
{
	FORK_REGISTRY_READWRITE(RegDeleteValueW_OLD, hKey, lpValueName);
}
LSTATUS APIENTRY RegEnumKeyA_Replacement(HKEY hKey, DWORD dwIndex, LPSTR lpName, DWORD cchName)
{
	//TODO, selects either Virtual Store or Real key, but not both
	FORK_REGISTRY_READONLY(RegEnumKeyA_OLD, hKey, dwIndex, lpName, cchName);
}
LSTATUS APIENTRY RegEnumKeyW_Replacement(HKEY hKey, DWORD dwIndex, LPWSTR lpName, DWORD cchName)
{
	//TODO, selects either Virtual Store or Real key, but not both
	FORK_REGISTRY_READONLY(RegEnumKeyW_OLD, hKey, dwIndex, lpName, cchName);
}
LSTATUS APIENTRY RegEnumKeyExA_Replacement(HKEY hKey, DWORD dwIndex, LPSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime)
{
	//TODO, selects either Virtual Store or Real key, but not both
	FORK_REGISTRY_READONLY(RegEnumKeyExA_OLD, hKey, dwIndex, lpName, lpcchName, lpReserved, lpClass, lpcchClass, lpftLastWriteTime);
}
LSTATUS APIENTRY RegEnumKeyExW_Replacement(HKEY hKey, DWORD dwIndex, LPWSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPWSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime)
{
	//TODO, selects either Virtual Store or Real key, but not both
	FORK_REGISTRY_READONLY(RegEnumKeyExW_OLD, hKey, dwIndex, lpName, lpcchName, lpReserved, lpClass, lpcchClass, lpftLastWriteTime);
}
LSTATUS APIENTRY RegEnumValueA_Replacement(HKEY hKey, DWORD dwIndex, LPSTR lpValueName, LPDWORD lpcchValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	//TODO, selects either Virtual Store or Real key, but not both
	FORK_REGISTRY_READONLY(RegEnumValueA_OLD,hKey, dwIndex, lpValueName, lpcchValueName, lpReserved, lpType, lpData, lpcbData);
}
LSTATUS APIENTRY RegEnumValueW_Replacement(HKEY hKey, DWORD dwIndex, LPWSTR lpValueName, LPDWORD lpcchValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	//TODO, selects either Virtual Store or Real key, but not both
	FORK_REGISTRY_READONLY(RegEnumValueW_OLD, hKey, dwIndex, lpValueName, lpcchValueName, lpReserved, lpType, lpData, lpcbData);
}
//LSTATUS APIENTRY RegFlushKey_Replacement(HKEY hKey)
//{
//	return RegFlushKey_OLD(hKey);
//}
LSTATUS APIENTRY RegGetKeySecurity_Replacement(HKEY hKey, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor)
{
	FORK_REGISTRY_READONLY(RegGetKeySecurity_OLD, hKey, SecurityInformation, pSecurityDescriptor, lpcbSecurityDescriptor);
}
//LSTATUS APIENTRY RegLoadKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpFile)
//{
//	FORK_REGISTRY_3(RegLoadKeyA_OLD, hKey, lpSubKey, lpFile);
//}
//LSTATUS APIENTRY RegLoadKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpFile)
//{
//	FORK_REGISTRY_3(RegLoadKeyW_OLD, hKey, lpSubKey, lpFile);
//}
LSTATUS APIENTRY RegNotifyChangeKeyValue_Replacement(HKEY hKey, BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL fAsynchronous)
{
	FORK_REGISTRY_READONLY(RegNotifyChangeKeyValue_OLD,hKey, bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous);
}

//small functions to open the registry key, then store it in the map
inline LSTATUS RegOpenKeyA_(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult, const RegistryVirtualPath& virtualPath)
{
	return TrySetOpenKey(
		RegOpenKeyA_OLD(hKey, lpSubKey, phkResult),
		phkResult, virtualPath);
}
inline LSTATUS RegOpenKeyW_(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult, const RegistryVirtualPath& virtualPath)
{
	return TrySetOpenKey(
		RegOpenKeyW_OLD(hKey, lpSubKey, phkResult),
		phkResult, virtualPath);
}
inline LSTATUS RegOpenKeyExA_(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult, const RegistryVirtualPath& virtualPath)
{
	return TrySetOpenKey(
		RegOpenKeyExA_OLD(hKey, lpSubKey, ulOptions, samDesired, phkResult),
		phkResult, virtualPath);
}
inline LSTATUS RegOpenKeyExW_(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult, const RegistryVirtualPath& virtualPath)
{
	return TrySetOpenKey(
		RegOpenKeyExW_OLD(hKey, lpSubKey, ulOptions, samDesired, phkResult),
		phkResult, virtualPath);
}

LSTATUS APIENTRY RegOpenKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
	FORK_REGISTRY_3(RegOpenKeyA_, hKey, lpSubKey, phkResult, virtualPath);
}
LSTATUS APIENTRY RegOpenKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
	FORK_REGISTRY_3(RegOpenKeyW_, hKey, lpSubKey, phkResult, virtualPath);
}
LSTATUS APIENTRY RegOpenKeyExA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	FORK_REGISTRY_3(RegOpenKeyExA_, hKey, lpSubKey, ulOptions, samDesired, phkResult, virtualPath);
}
LSTATUS APIENTRY RegOpenKeyExW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	FORK_REGISTRY_3(RegOpenKeyExW_, hKey, lpSubKey, ulOptions, samDesired, phkResult, virtualPath);
}
//Not used by 32-bit applications
//LSTATUS APIENTRY RegOpenKeyTransactedA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID pExtendedParemeter)
//{
//	return RegOpenKeyTransactedA_OLD(hKey, lpSubKey, ulOptions, samDesired, phkResult, hTransaction, pExtendedParemeter);
//}
//LSTATUS APIENTRY RegOpenKeyTransactedW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID pExtendedParemeter)
//{
//	return RegOpenKeyTransactedW_OLD(hKey, lpSubKey, ulOptions, samDesired, phkResult, hTransaction, pExtendedParemeter);
//}
LSTATUS APIENTRY RegQueryInfoKeyA_Replacement(HKEY hKey, LPSTR lpClass, LPDWORD lpcchClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime)
{
	FORK_REGISTRY_READONLY(RegQueryInfoKeyA_OLD, hKey, lpClass, lpcchClass, lpReserved, lpcSubKeys, lpcbMaxSubKeyLen, lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor, lpftLastWriteTime);
}
LSTATUS APIENTRY RegQueryInfoKeyW_Replacement(HKEY hKey, LPWSTR lpClass, LPDWORD lpcchClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime)
{
	FORK_REGISTRY_READONLY(RegQueryInfoKeyW_OLD, hKey, lpClass, lpcchClass, lpReserved, lpcSubKeys, lpcbMaxSubKeyLen, lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor, lpftLastWriteTime);
}
LSTATUS APIENTRY RegQueryValueA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPSTR lpData, PLONG lpcbData)
{
	FORK_REGISTRY_3(RegQueryValueA_OLD, hKey, lpSubKey, lpData, lpcbData);
}
LSTATUS APIENTRY RegQueryValueW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPWSTR lpData, PLONG lpcbData)
{
	FORK_REGISTRY_3(RegQueryValueW_OLD, hKey, lpSubKey, lpData, lpcbData);
}
LSTATUS APIENTRY RegQueryMultipleValuesA_Replacement(HKEY hKey, PVALENTA val_list, DWORD num_vals, LPSTR lpValueBuf, LPDWORD ldwTotsize)
{
	FORK_REGISTRY_READONLY(RegQueryMultipleValuesA_OLD, hKey, val_list, num_vals, lpValueBuf, ldwTotsize);
}
LSTATUS APIENTRY RegQueryMultipleValuesW_Replacement(HKEY hKey, PVALENTW val_list, DWORD num_vals, LPWSTR lpValueBuf, LPDWORD ldwTotsize)
{
	FORK_REGISTRY_READONLY(RegQueryMultipleValuesW_OLD, hKey, val_list, num_vals, lpValueBuf, ldwTotsize);
}
LSTATUS APIENTRY RegQueryValueExA_Replacement(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	FORK_REGISTRY_READONLY(RegQueryValueExA_OLD, hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}
LSTATUS APIENTRY RegQueryValueExW_Replacement(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	FORK_REGISTRY_READONLY(RegQueryValueExW_OLD, hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}
//LSTATUS APIENTRY RegReplaceKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpNewFile, LPCSTR lpOldFile)
//{
//	return RegReplaceKeyA_OLD(hKey, lpSubKey, lpNewFile, lpOldFile);
//}
//LSTATUS APIENTRY RegReplaceKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpNewFile, LPCWSTR lpOldFile)
//{
//	return RegReplaceKeyW_OLD(hKey, lpSubKey, lpNewFile, lpOldFile);
//}
//LSTATUS APIENTRY RegRestoreKeyA_Replacement(HKEY hKey, LPCSTR lpFile, DWORD dwFlags)
//{
//	return RegRestoreKeyA_OLD(hKey, lpFile, dwFlags);
//}
//LSTATUS APIENTRY RegRestoreKeyW_Replacement(HKEY hKey, LPCWSTR lpFile, DWORD dwFlags)
//{
//	return RegRestoreKeyW_OLD(hKey, lpFile, dwFlags);
//}
//LSTATUS APIENTRY RegRenameKey_Replacement(HKEY hKey, LPCWSTR lpSubKeyName, LPCWSTR lpNewKeyName)
//{
//	return RegRenameKey_OLD(hKey, lpSubKeyName, lpNewKeyName);
//}
//LSTATUS APIENTRY RegSaveKeyA_Replacement(HKEY hKey, LPCSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes)
//{
//	return RegSaveKeyA_OLD(hKey, lpFile, lpSecurityAttributes);
//}
//LSTATUS APIENTRY RegSaveKeyW_Replacement(HKEY hKey, LPCWSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes)
//{
//	return RegSaveKeyW_OLD(hKey, lpFile, lpSecurityAttributes);
//}
LSTATUS APIENTRY RegSetKeySecurity_Replacement(HKEY hKey, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	FORK_REGISTRY_READWRITE(RegSetKeySecurity_OLD, hKey, SecurityInformation, pSecurityDescriptor);
}
LSTATUS APIENTRY RegSetValueA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData)
{
	FORK_REGISTRY_CREATE_2(RegSetValueA_OLD, hKey, lpSubKey, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegSetValueW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData)
{
	FORK_REGISTRY_CREATE_2(RegSetValueW_OLD, hKey, lpSubKey, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegSetValueExA_Replacement(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	FORK_REGISTRY_CREATE(RegSetValueExA_OLD, hKey, lpValueName, Reserved, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegSetValueExW_Replacement(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	FORK_REGISTRY_CREATE(RegSetValueExW_OLD, hKey, lpValueName, Reserved, dwType, lpData, cbData);
}
//LSTATUS APIENTRY RegUnLoadKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey)
//{
//	return RegUnLoadKeyA_OLD(hKey, lpSubKey);
//}
//LSTATUS APIENTRY RegUnLoadKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey)
//{
//	return RegUnLoadKeyW_OLD(hKey, lpSubKey);
//}
LSTATUS APIENTRY RegDeleteKeyValueA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName)
{
	FORK_REGISTRY_3(RegDeleteKeyValueA_OLD, hKey, lpSubKey, lpValueName);
}
LSTATUS APIENTRY RegDeleteKeyValueW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName)
{
	FORK_REGISTRY_3(RegDeleteKeyValueW_OLD, hKey, lpSubKey, lpValueName);
}
LSTATUS APIENTRY RegSetKeyValueA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwType, LPCVOID lpData, DWORD cbData)
{
	FORK_REGISTRY_CREATE_2(RegSetKeyValueA_OLD, hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegSetKeyValueW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPCVOID lpData, DWORD cbData)
{
	FORK_REGISTRY_CREATE_2(RegSetKeyValueW_OLD, hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
}
//LSTATUS APIENTRY RegDeleteTreeA_Replacement(HKEY hKey, LPCSTR lpSubKey)
//{
//	return RegDeleteTreeA_OLD(hKey, lpSubKey);
//}
//LSTATUS APIENTRY RegDeleteTreeW_Replacement(HKEY hKey, LPCWSTR lpSubKey)
//{
//	return RegDeleteTreeW_OLD(hKey, lpSubKey);
//}
//LSTATUS APIENTRY RegCopyTreeA_Replacement(HKEY hKeySrc, LPCSTR lpSubKey, HKEY hKeyDest)
//{
//	return RegCopyTreeA_OLD(hKeySrc, lpSubKey, hKeyDest);
//}
//LSTATUS APIENTRY RegCopyTreeW_Replacement(HKEY hKeySrc, LPCWSTR lpSubKey, HKEY hKeyDest)
//{
//	return RegCopyTreeW_OLD(hKeySrc, lpSubKey, hKeyDest);
//}
LSTATUS APIENTRY RegGetValueA_Replacement(HKEY hkey, LPCSTR lpSubKey, LPCSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{
	FORK_REGISTRY_3(RegGetValueA_OLD, hkey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
}
LSTATUS APIENTRY RegGetValueW_Replacement(HKEY hkey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{
	FORK_REGISTRY_3(RegGetValueW_OLD, hkey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
}
//LSTATUS APIENTRY RegLoadMUIStringA_Replacement(HKEY hKey, LPCSTR pszValue, LPSTR pszOutBuf, DWORD cbOutBuf, LPDWORD pcbData, DWORD Flags, LPCSTR pszDirectory)
//{
//	return RegLoadMUIStringA_OLD(hKey, pszValue, pszOutBuf, cbOutBuf, pcbData, Flags, pszDirectory);
//}
//LSTATUS APIENTRY RegLoadMUIStringW_Replacement(HKEY hKey, LPCWSTR pszValue, LPWSTR pszOutBuf, DWORD cbOutBuf, LPDWORD pcbData, DWORD Flags, LPCWSTR pszDirectory)
//{
//	return RegLoadMUIStringW_OLD(hKey, pszValue, pszOutBuf, cbOutBuf, pcbData, Flags, pszDirectory);
//}
//LSTATUS APIENTRY RegLoadAppKeyA_Replacement(LPCSTR lpFile, PHKEY phkResult, REGSAM samDesired, DWORD dwOptions, DWORD Reserved)
//{
//	return RegLoadAppKeyA_OLD(lpFile, phkResult, samDesired, dwOptions, Reserved);
//}
//LSTATUS APIENTRY RegLoadAppKeyW_Replacement(LPCWSTR lpFile, PHKEY phkResult, REGSAM samDesired, DWORD dwOptions, DWORD Reserved)
//{
//	return RegLoadAppKeyW_OLD(lpFile, phkResult, samDesired, dwOptions, Reserved);
//}
//LSTATUS APIENTRY RegSaveKeyExA_Replacement(HKEY hKey, LPCSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD Flags)
//{
//	return RegSaveKeyExA_OLD(hKey, lpFile, lpSecurityAttributes, Flags);
//}
//LSTATUS APIENTRY RegSaveKeyExW_Replacement(HKEY hKey, LPCWSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD Flags)
//{
//	return RegSaveKeyExW_OLD(hKey, lpFile, lpSecurityAttributes, Flags);
//}
