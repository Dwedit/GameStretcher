#include "RegistryWrappers.h"
#include "ImportReplacer.h"

//Replace the Functions
void BuildImportMap_Registry()
{
	extern ImportMap importMap;
#define ReplaceImport(dllName, functionName, replacementFunction, pOldFunction) \
    importMap.AddImport((dllName),(functionName),(replacementFunction),(pOldFunction))

	ReplaceImport("Advapi32.dll", "RegCloseKey", (FARPROC)RegCloseKey_Replacement, (FARPROC*)&RegCloseKey_OLD);
	ReplaceImport("Advapi32.dll", "RegOverridePredefKey", (FARPROC)RegOverridePredefKey_Replacement, (FARPROC*)&RegOverridePredefKey_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenUserClassesRoot", (FARPROC)RegOpenUserClassesRoot_Replacement, (FARPROC*)&RegOpenUserClassesRoot_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenCurrentUser", (FARPROC)RegOpenCurrentUser_Replacement, (FARPROC*)&RegOpenCurrentUser_OLD);
	ReplaceImport("Advapi32.dll", "RegDisablePredefinedCache", (FARPROC)RegDisablePredefinedCache_Replacement, (FARPROC*)&RegDisablePredefinedCache_OLD);
	ReplaceImport("Advapi32.dll", "RegDisablePredefinedCacheEx", (FARPROC)RegDisablePredefinedCacheEx_Replacement, (FARPROC*)&RegDisablePredefinedCacheEx_OLD);
	ReplaceImport("Advapi32.dll", "RegConnectRegistryA", (FARPROC)RegConnectRegistryA_Replacement, (FARPROC*)&RegConnectRegistryA_OLD);
	ReplaceImport("Advapi32.dll", "RegConnectRegistryW", (FARPROC)RegConnectRegistryW_Replacement, (FARPROC*)&RegConnectRegistryW_OLD);
	ReplaceImport("Advapi32.dll", "RegConnectRegistryExA", (FARPROC)RegConnectRegistryExA_Replacement, (FARPROC*)&RegConnectRegistryExA_OLD);
	ReplaceImport("Advapi32.dll", "RegConnectRegistryExW", (FARPROC)RegConnectRegistryExW_Replacement, (FARPROC*)&RegConnectRegistryExW_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyA", (FARPROC)RegCreateKeyA_Replacement, (FARPROC*)&RegCreateKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyW", (FARPROC)RegCreateKeyW_Replacement, (FARPROC*)&RegCreateKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyExA", (FARPROC)RegCreateKeyExA_Replacement, (FARPROC*)&RegCreateKeyExA_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyExW", (FARPROC)RegCreateKeyExW_Replacement, (FARPROC*)&RegCreateKeyExW_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyTransactedA", (FARPROC)RegCreateKeyTransactedA_Replacement, (FARPROC*)&RegCreateKeyTransactedA_OLD);
	ReplaceImport("Advapi32.dll", "RegCreateKeyTransactedW", (FARPROC)RegCreateKeyTransactedW_Replacement, (FARPROC*)&RegCreateKeyTransactedW_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyA", (FARPROC)RegDeleteKeyA_Replacement, (FARPROC*)&RegDeleteKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyW", (FARPROC)RegDeleteKeyW_Replacement, (FARPROC*)&RegDeleteKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyExA", (FARPROC)RegDeleteKeyExA_Replacement, (FARPROC*)&RegDeleteKeyExA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyExW", (FARPROC)RegDeleteKeyExW_Replacement, (FARPROC*)&RegDeleteKeyExW_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyTransactedA", (FARPROC)RegDeleteKeyTransactedA_Replacement, (FARPROC*)&RegDeleteKeyTransactedA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyTransactedW", (FARPROC)RegDeleteKeyTransactedW_Replacement, (FARPROC*)&RegDeleteKeyTransactedW_OLD);
	ReplaceImport("Advapi32.dll", "RegDisableReflectionKey", (FARPROC)RegDisableReflectionKey_Replacement, (FARPROC*)&RegDisableReflectionKey_OLD);
	ReplaceImport("Advapi32.dll", "RegEnableReflectionKey", (FARPROC)RegEnableReflectionKey_Replacement, (FARPROC*)&RegEnableReflectionKey_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryReflectionKey", (FARPROC)RegQueryReflectionKey_Replacement, (FARPROC*)&RegQueryReflectionKey_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteValueA", (FARPROC)RegDeleteValueA_Replacement, (FARPROC*)&RegDeleteValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteValueW", (FARPROC)RegDeleteValueW_Replacement, (FARPROC*)&RegDeleteValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumKeyA", (FARPROC)RegEnumKeyA_Replacement, (FARPROC*)&RegEnumKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumKeyW", (FARPROC)RegEnumKeyW_Replacement, (FARPROC*)&RegEnumKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumKeyExA", (FARPROC)RegEnumKeyExA_Replacement, (FARPROC*)&RegEnumKeyExA_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumKeyExW", (FARPROC)RegEnumKeyExW_Replacement, (FARPROC*)&RegEnumKeyExW_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumValueA", (FARPROC)RegEnumValueA_Replacement, (FARPROC*)&RegEnumValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegEnumValueW", (FARPROC)RegEnumValueW_Replacement, (FARPROC*)&RegEnumValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegFlushKey", (FARPROC)RegFlushKey_Replacement, (FARPROC*)&RegFlushKey_OLD);
	ReplaceImport("Advapi32.dll", "RegGetKeySecurity", (FARPROC)RegGetKeySecurity_Replacement, (FARPROC*)&RegGetKeySecurity_OLD);
	ReplaceImport("Advapi32.dll", "RegLoadKeyA", (FARPROC)RegLoadKeyA_Replacement, (FARPROC*)&RegLoadKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegLoadKeyW", (FARPROC)RegLoadKeyW_Replacement, (FARPROC*)&RegLoadKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegNotifyChangeKeyValue", (FARPROC)RegNotifyChangeKeyValue_Replacement, (FARPROC*)&RegNotifyChangeKeyValue_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyA", (FARPROC)RegOpenKeyA_Replacement, (FARPROC*)&RegOpenKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyW", (FARPROC)RegOpenKeyW_Replacement, (FARPROC*)&RegOpenKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyExA", (FARPROC)RegOpenKeyExA_Replacement, (FARPROC*)&RegOpenKeyExA_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyExW", (FARPROC)RegOpenKeyExW_Replacement, (FARPROC*)&RegOpenKeyExW_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyTransactedA", (FARPROC)RegOpenKeyTransactedA_Replacement, (FARPROC*)&RegOpenKeyTransactedA_OLD);
	ReplaceImport("Advapi32.dll", "RegOpenKeyTransactedW", (FARPROC)RegOpenKeyTransactedW_Replacement, (FARPROC*)&RegOpenKeyTransactedW_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryInfoKeyA", (FARPROC)RegQueryInfoKeyA_Replacement, (FARPROC*)&RegQueryInfoKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryInfoKeyW", (FARPROC)RegQueryInfoKeyW_Replacement, (FARPROC*)&RegQueryInfoKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryValueA", (FARPROC)RegQueryValueA_Replacement, (FARPROC*)&RegQueryValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryValueW", (FARPROC)RegQueryValueW_Replacement, (FARPROC*)&RegQueryValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryMultipleValuesA", (FARPROC)RegQueryMultipleValuesA_Replacement, (FARPROC*)&RegQueryMultipleValuesA_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryMultipleValuesW", (FARPROC)RegQueryMultipleValuesW_Replacement, (FARPROC*)&RegQueryMultipleValuesW_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryValueExA", (FARPROC)RegQueryValueExA_Replacement, (FARPROC*)&RegQueryValueExA_OLD);
	ReplaceImport("Advapi32.dll", "RegQueryValueExW", (FARPROC)RegQueryValueExW_Replacement, (FARPROC*)&RegQueryValueExW_OLD);
	ReplaceImport("Advapi32.dll", "RegReplaceKeyA", (FARPROC)RegReplaceKeyA_Replacement, (FARPROC*)&RegReplaceKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegReplaceKeyW", (FARPROC)RegReplaceKeyW_Replacement, (FARPROC*)&RegReplaceKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegRestoreKeyA", (FARPROC)RegRestoreKeyA_Replacement, (FARPROC*)&RegRestoreKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegRestoreKeyW", (FARPROC)RegRestoreKeyW_Replacement, (FARPROC*)&RegRestoreKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegRenameKey", (FARPROC)RegRenameKey_Replacement, (FARPROC*)&RegRenameKey_OLD);
	ReplaceImport("Advapi32.dll", "RegSaveKeyA", (FARPROC)RegSaveKeyA_Replacement, (FARPROC*)&RegSaveKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegSaveKeyW", (FARPROC)RegSaveKeyW_Replacement, (FARPROC*)&RegSaveKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegSetKeySecurity", (FARPROC)RegSetKeySecurity_Replacement, (FARPROC*)&RegSetKeySecurity_OLD);
	ReplaceImport("Advapi32.dll", "RegSetValueA", (FARPROC)RegSetValueA_Replacement, (FARPROC*)&RegSetValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegSetValueW", (FARPROC)RegSetValueW_Replacement, (FARPROC*)&RegSetValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegSetValueExA", (FARPROC)RegSetValueExA_Replacement, (FARPROC*)&RegSetValueExA_OLD);
	ReplaceImport("Advapi32.dll", "RegSetValueExW", (FARPROC)RegSetValueExW_Replacement, (FARPROC*)&RegSetValueExW_OLD);
	ReplaceImport("Advapi32.dll", "RegUnLoadKeyA", (FARPROC)RegUnLoadKeyA_Replacement, (FARPROC*)&RegUnLoadKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegUnLoadKeyW", (FARPROC)RegUnLoadKeyW_Replacement, (FARPROC*)&RegUnLoadKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyValueA", (FARPROC)RegDeleteKeyValueA_Replacement, (FARPROC*)&RegDeleteKeyValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteKeyValueW", (FARPROC)RegDeleteKeyValueW_Replacement, (FARPROC*)&RegDeleteKeyValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegSetKeyValueA", (FARPROC)RegSetKeyValueA_Replacement, (FARPROC*)&RegSetKeyValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegSetKeyValueW", (FARPROC)RegSetKeyValueW_Replacement, (FARPROC*)&RegSetKeyValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteTreeA", (FARPROC)RegDeleteTreeA_Replacement, (FARPROC*)&RegDeleteTreeA_OLD);
	ReplaceImport("Advapi32.dll", "RegDeleteTreeW", (FARPROC)RegDeleteTreeW_Replacement, (FARPROC*)&RegDeleteTreeW_OLD);
	ReplaceImport("Advapi32.dll", "RegCopyTreeA", (FARPROC)RegCopyTreeA_Replacement, (FARPROC*)&RegCopyTreeA_OLD);
	ReplaceImport("Advapi32.dll", "RegGetValueA", (FARPROC)RegGetValueA_Replacement, (FARPROC*)&RegGetValueA_OLD);
	ReplaceImport("Advapi32.dll", "RegGetValueW", (FARPROC)RegGetValueW_Replacement, (FARPROC*)&RegGetValueW_OLD);
	ReplaceImport("Advapi32.dll", "RegCopyTreeW", (FARPROC)RegCopyTreeW_Replacement, (FARPROC*)&RegCopyTreeW_OLD);
	ReplaceImport("Advapi32.dll", "RegLoadMUIStringA", (FARPROC)RegLoadMUIStringA_Replacement, (FARPROC*)&RegLoadMUIStringA_OLD);
	ReplaceImport("Advapi32.dll", "RegLoadMUIStringW", (FARPROC)RegLoadMUIStringW_Replacement, (FARPROC*)&RegLoadMUIStringW_OLD);
	ReplaceImport("Advapi32.dll", "RegLoadAppKeyA", (FARPROC)RegLoadAppKeyA_Replacement, (FARPROC*)&RegLoadAppKeyA_OLD);
	ReplaceImport("Advapi32.dll", "RegLoadAppKeyW", (FARPROC)RegLoadAppKeyW_Replacement, (FARPROC*)&RegLoadAppKeyW_OLD);
	ReplaceImport("Advapi32.dll", "RegSaveKeyExA", (FARPROC)RegSaveKeyExA_Replacement, (FARPROC*)&RegSaveKeyExA_OLD);
	ReplaceImport("Advapi32.dll", "RegSaveKeyExW", (FARPROC)RegSaveKeyExW_Replacement, (FARPROC*)&RegSaveKeyExW_OLD);
}

//Import Backups (Definitions)
RegCloseKey_FUNC RegCloseKey_OLD = NULL;
RegOverridePredefKey_FUNC RegOverridePredefKey_OLD = NULL;
RegOpenUserClassesRoot_FUNC RegOpenUserClassesRoot_OLD = NULL;
RegOpenCurrentUser_FUNC RegOpenCurrentUser_OLD = NULL;
RegDisablePredefinedCache_FUNC RegDisablePredefinedCache_OLD = NULL;
RegDisablePredefinedCacheEx_FUNC RegDisablePredefinedCacheEx_OLD = NULL;
RegConnectRegistryA_FUNC RegConnectRegistryA_OLD = NULL;
RegConnectRegistryW_FUNC RegConnectRegistryW_OLD = NULL;
RegConnectRegistryExA_FUNC RegConnectRegistryExA_OLD = NULL;
RegConnectRegistryExW_FUNC RegConnectRegistryExW_OLD = NULL;
RegCreateKeyA_FUNC RegCreateKeyA_OLD = NULL;
RegCreateKeyW_FUNC RegCreateKeyW_OLD = NULL;
RegCreateKeyExA_FUNC RegCreateKeyExA_OLD = NULL;
RegCreateKeyExW_FUNC RegCreateKeyExW_OLD = NULL;
RegCreateKeyTransactedA_FUNC RegCreateKeyTransactedA_OLD = NULL;
RegCreateKeyTransactedW_FUNC RegCreateKeyTransactedW_OLD = NULL;
RegDeleteKeyA_FUNC RegDeleteKeyA_OLD = NULL;
RegDeleteKeyW_FUNC RegDeleteKeyW_OLD = NULL;
RegDeleteKeyExA_FUNC RegDeleteKeyExA_OLD = NULL;
RegDeleteKeyExW_FUNC RegDeleteKeyExW_OLD = NULL;
RegDeleteKeyTransactedA_FUNC RegDeleteKeyTransactedA_OLD = NULL;
RegDeleteKeyTransactedW_FUNC RegDeleteKeyTransactedW_OLD = NULL;
RegDisableReflectionKey_FUNC RegDisableReflectionKey_OLD = NULL;
RegEnableReflectionKey_FUNC RegEnableReflectionKey_OLD = NULL;
RegQueryReflectionKey_FUNC RegQueryReflectionKey_OLD = NULL;
RegDeleteValueA_FUNC RegDeleteValueA_OLD = NULL;
RegDeleteValueW_FUNC RegDeleteValueW_OLD = NULL;
RegEnumKeyA_FUNC RegEnumKeyA_OLD = NULL;
RegEnumKeyW_FUNC RegEnumKeyW_OLD = NULL;
RegEnumKeyExA_FUNC RegEnumKeyExA_OLD = NULL;
RegEnumKeyExW_FUNC RegEnumKeyExW_OLD = NULL;
RegEnumValueA_FUNC RegEnumValueA_OLD = NULL;
RegEnumValueW_FUNC RegEnumValueW_OLD = NULL;
RegFlushKey_FUNC RegFlushKey_OLD = NULL;
RegGetKeySecurity_FUNC RegGetKeySecurity_OLD = NULL;
RegLoadKeyA_FUNC RegLoadKeyA_OLD = NULL;
RegLoadKeyW_FUNC RegLoadKeyW_OLD = NULL;
RegNotifyChangeKeyValue_FUNC RegNotifyChangeKeyValue_OLD = NULL;
RegOpenKeyA_FUNC RegOpenKeyA_OLD = NULL;
RegOpenKeyW_FUNC RegOpenKeyW_OLD = NULL;
RegOpenKeyExA_FUNC RegOpenKeyExA_OLD = NULL;
RegOpenKeyExW_FUNC RegOpenKeyExW_OLD = NULL;
RegOpenKeyTransactedA_FUNC RegOpenKeyTransactedA_OLD = NULL;
RegOpenKeyTransactedW_FUNC RegOpenKeyTransactedW_OLD = NULL;
RegQueryInfoKeyA_FUNC RegQueryInfoKeyA_OLD = NULL;
RegQueryInfoKeyW_FUNC RegQueryInfoKeyW_OLD = NULL;
RegQueryValueA_FUNC RegQueryValueA_OLD = NULL;
RegQueryValueW_FUNC RegQueryValueW_OLD = NULL;
RegQueryMultipleValuesA_FUNC RegQueryMultipleValuesA_OLD = NULL;
RegQueryMultipleValuesW_FUNC RegQueryMultipleValuesW_OLD = NULL;
RegQueryValueExA_FUNC RegQueryValueExA_OLD = NULL;
RegQueryValueExW_FUNC RegQueryValueExW_OLD = NULL;
RegReplaceKeyA_FUNC RegReplaceKeyA_OLD = NULL;
RegReplaceKeyW_FUNC RegReplaceKeyW_OLD = NULL;
RegRestoreKeyA_FUNC RegRestoreKeyA_OLD = NULL;
RegRestoreKeyW_FUNC RegRestoreKeyW_OLD = NULL;
RegRenameKey_FUNC RegRenameKey_OLD = NULL;
RegSaveKeyA_FUNC RegSaveKeyA_OLD = NULL;
RegSaveKeyW_FUNC RegSaveKeyW_OLD = NULL;
RegSetKeySecurity_FUNC RegSetKeySecurity_OLD = NULL;
RegSetValueA_FUNC RegSetValueA_OLD = NULL;
RegSetValueW_FUNC RegSetValueW_OLD = NULL;
RegSetValueExA_FUNC RegSetValueExA_OLD = NULL;
RegSetValueExW_FUNC RegSetValueExW_OLD = NULL;
RegUnLoadKeyA_FUNC RegUnLoadKeyA_OLD = NULL;
RegUnLoadKeyW_FUNC RegUnLoadKeyW_OLD = NULL;
RegDeleteKeyValueA_FUNC RegDeleteKeyValueA_OLD = NULL;
RegDeleteKeyValueW_FUNC RegDeleteKeyValueW_OLD = NULL;
RegSetKeyValueA_FUNC RegSetKeyValueA_OLD = NULL;
RegSetKeyValueW_FUNC RegSetKeyValueW_OLD = NULL;
RegDeleteTreeA_FUNC RegDeleteTreeA_OLD = NULL;
RegDeleteTreeW_FUNC RegDeleteTreeW_OLD = NULL;
RegCopyTreeA_FUNC RegCopyTreeA_OLD = NULL;
RegGetValueA_FUNC RegGetValueA_OLD = NULL;
RegGetValueW_FUNC RegGetValueW_OLD = NULL;
RegCopyTreeW_FUNC RegCopyTreeW_OLD = NULL;
RegLoadMUIStringA_FUNC RegLoadMUIStringA_OLD = NULL;
RegLoadMUIStringW_FUNC RegLoadMUIStringW_OLD = NULL;
RegLoadAppKeyA_FUNC RegLoadAppKeyA_OLD = NULL;
RegLoadAppKeyW_FUNC RegLoadAppKeyW_OLD = NULL;
RegSaveKeyExA_FUNC RegSaveKeyExA_OLD = NULL;
RegSaveKeyExW_FUNC RegSaveKeyExW_OLD = NULL;

int SubstituteHKey(HKEY& hKey)
{
	if (hKey == HKEY_LOCAL_MACHINE)
	{
		hKey = HKEY_CURRENT_USER;
	}
	return 0;
}

//Replacement Functions Code
LSTATUS APIENTRY RegCloseKey_Replacement(HKEY hKey)
{
	auto result = SubstituteHKey(hKey);
	return RegCloseKey_OLD(hKey);
}
LSTATUS APIENTRY RegOverridePredefKey_Replacement(HKEY hKey, HKEY hNewHKey)
{
	auto result = SubstituteHKey(hKey);
	return RegOverridePredefKey_OLD(hKey, hNewHKey);
}
LSTATUS APIENTRY RegOpenUserClassesRoot_Replacement(HANDLE hToken, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult)
{
	return RegOpenUserClassesRoot_OLD(hToken, dwOptions, samDesired, phkResult);
}
LSTATUS APIENTRY RegOpenCurrentUser_Replacement(REGSAM samDesired, PHKEY phkResult)
{
	return RegOpenCurrentUser_OLD(samDesired, phkResult);
}
LSTATUS APIENTRY RegDisablePredefinedCache_Replacement()
{
	return RegDisablePredefinedCache_OLD();
}
LSTATUS APIENTRY RegDisablePredefinedCacheEx_Replacement()
{
	return RegDisablePredefinedCacheEx_OLD();
}
LSTATUS APIENTRY RegConnectRegistryA_Replacement(LPCSTR lpMachineName, HKEY hKey, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegConnectRegistryA_OLD(lpMachineName, hKey, phkResult);
}
LSTATUS APIENTRY RegConnectRegistryW_Replacement(LPCWSTR lpMachineName, HKEY hKey, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegConnectRegistryW_OLD(lpMachineName, hKey, phkResult);
}
LSTATUS APIENTRY RegConnectRegistryExA_Replacement(LPCSTR lpMachineName, HKEY hKey, ULONG Flags, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegConnectRegistryExA_OLD(lpMachineName, hKey, Flags, phkResult);
}
LSTATUS APIENTRY RegConnectRegistryExW_Replacement(LPCWSTR lpMachineName, HKEY hKey, ULONG Flags, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegConnectRegistryExW_OLD(lpMachineName, hKey, Flags, phkResult);
}
LSTATUS APIENTRY RegCreateKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegCreateKeyA_OLD(hKey, lpSubKey, phkResult);
}
LSTATUS APIENTRY RegCreateKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegCreateKeyW_OLD(hKey, lpSubKey, phkResult);
}
LSTATUS APIENTRY RegCreateKeyExA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	auto result = SubstituteHKey(hKey);
	return RegCreateKeyExA_OLD(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}
LSTATUS APIENTRY RegCreateKeyExW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	auto result = SubstituteHKey(hKey);
	return RegCreateKeyExW_OLD(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}
LSTATUS APIENTRY RegCreateKeyTransactedA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID pExtendedParemeter)
{
	auto result = SubstituteHKey(hKey);
	return RegCreateKeyTransactedA_OLD(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition, hTransaction, pExtendedParemeter);
}
LSTATUS APIENTRY RegCreateKeyTransactedW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID pExtendedParemeter)
{
	auto result = SubstituteHKey(hKey);
	return RegCreateKeyTransactedW_OLD(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition, hTransaction, pExtendedParemeter);
}
LSTATUS APIENTRY RegDeleteKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteKeyA_OLD(hKey, lpSubKey);
}
LSTATUS APIENTRY RegDeleteKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteKeyW_OLD(hKey, lpSubKey);
}
LSTATUS APIENTRY RegDeleteKeyExA_Replacement(HKEY hKey, LPCSTR lpSubKey, REGSAM samDesired, DWORD Reserved)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteKeyExA_OLD(hKey, lpSubKey, samDesired, Reserved);
}
LSTATUS APIENTRY RegDeleteKeyExW_Replacement(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired, DWORD Reserved)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteKeyExW_OLD(hKey, lpSubKey, samDesired, Reserved);
}
LSTATUS APIENTRY RegDeleteKeyTransactedA_Replacement(HKEY hKey, LPCSTR lpSubKey, REGSAM samDesired, DWORD Reserved, HANDLE hTransaction, PVOID pExtendedParameter)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteKeyTransactedA_OLD(hKey, lpSubKey, samDesired, Reserved, hTransaction, pExtendedParameter);
}
LSTATUS APIENTRY RegDeleteKeyTransactedW_Replacement(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired, DWORD Reserved, HANDLE hTransaction, PVOID pExtendedParameter)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteKeyTransactedW_OLD(hKey, lpSubKey, samDesired, Reserved, hTransaction, pExtendedParameter);
}
LONG APIENTRY RegDisableReflectionKey_Replacement(HKEY hBase)
{
	return RegDisableReflectionKey_OLD(hBase);
}
LONG APIENTRY RegEnableReflectionKey_Replacement(HKEY hBase)
{
	return RegEnableReflectionKey_OLD(hBase);
}
LONG APIENTRY RegQueryReflectionKey_Replacement(HKEY hBase, BOOL* bIsReflectionDisabled)
{
	return RegQueryReflectionKey_OLD(hBase, bIsReflectionDisabled);
}
LSTATUS APIENTRY RegDeleteValueA_Replacement(HKEY hKey, LPCSTR lpValueName)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteValueA_OLD(hKey, lpValueName);
}
LSTATUS APIENTRY RegDeleteValueW_Replacement(HKEY hKey, LPCWSTR lpValueName)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteValueW_OLD(hKey, lpValueName);
}
LSTATUS APIENTRY RegEnumKeyA_Replacement(HKEY hKey, DWORD dwIndex, LPSTR lpName, DWORD cchName)
{
	auto result = SubstituteHKey(hKey);
	return RegEnumKeyA_OLD(hKey, dwIndex, lpName, cchName);
}
LSTATUS APIENTRY RegEnumKeyW_Replacement(HKEY hKey, DWORD dwIndex, LPWSTR lpName, DWORD cchName)
{
	auto result = SubstituteHKey(hKey);
	return RegEnumKeyW_OLD(hKey, dwIndex, lpName, cchName);
}
LSTATUS APIENTRY RegEnumKeyExA_Replacement(HKEY hKey, DWORD dwIndex, LPSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime)
{
	auto result = SubstituteHKey(hKey);
	return RegEnumKeyExA_OLD(hKey, dwIndex, lpName, lpcchName, lpReserved, lpClass, lpcchClass, lpftLastWriteTime);
}
LSTATUS APIENTRY RegEnumKeyExW_Replacement(HKEY hKey, DWORD dwIndex, LPWSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPWSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime)
{
	auto result = SubstituteHKey(hKey);
	return RegEnumKeyExW_OLD(hKey, dwIndex, lpName, lpcchName, lpReserved, lpClass, lpcchClass, lpftLastWriteTime);
}
LSTATUS APIENTRY RegEnumValueA_Replacement(HKEY hKey, DWORD dwIndex, LPSTR lpValueName, LPDWORD lpcchValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	auto result = SubstituteHKey(hKey);
	return RegEnumValueA_OLD(hKey, dwIndex, lpValueName, lpcchValueName, lpReserved, lpType, lpData, lpcbData);
}
LSTATUS APIENTRY RegEnumValueW_Replacement(HKEY hKey, DWORD dwIndex, LPWSTR lpValueName, LPDWORD lpcchValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	auto result = SubstituteHKey(hKey);
	return RegEnumValueW_OLD(hKey, dwIndex, lpValueName, lpcchValueName, lpReserved, lpType, lpData, lpcbData);
}
LSTATUS APIENTRY RegFlushKey_Replacement(HKEY hKey)
{
	auto result = SubstituteHKey(hKey);
	return RegFlushKey_OLD(hKey);
}
LSTATUS APIENTRY RegGetKeySecurity_Replacement(HKEY hKey, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor)
{
	auto result = SubstituteHKey(hKey);
	return RegGetKeySecurity_OLD(hKey, SecurityInformation, pSecurityDescriptor, lpcbSecurityDescriptor);
}
LSTATUS APIENTRY RegLoadKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpFile)
{
	auto result = SubstituteHKey(hKey);
	return RegLoadKeyA_OLD(hKey, lpSubKey, lpFile);
}
LSTATUS APIENTRY RegLoadKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpFile)
{
	auto result = SubstituteHKey(hKey);
	return RegLoadKeyW_OLD(hKey, lpSubKey, lpFile);
}
LSTATUS APIENTRY RegNotifyChangeKeyValue_Replacement(HKEY hKey, BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL fAsynchronous)
{
	auto result = SubstituteHKey(hKey);
	return RegNotifyChangeKeyValue_OLD(hKey, bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous);
}
LSTATUS APIENTRY RegOpenKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegOpenKeyA_OLD(hKey, lpSubKey, phkResult);
}
LSTATUS APIENTRY RegOpenKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegOpenKeyW_OLD(hKey, lpSubKey, phkResult);
}
LSTATUS APIENTRY RegOpenKeyExA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegOpenKeyExA_OLD(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}
LSTATUS APIENTRY RegOpenKeyExW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	auto result = SubstituteHKey(hKey);
	return RegOpenKeyExW_OLD(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}
LSTATUS APIENTRY RegOpenKeyTransactedA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID pExtendedParemeter)
{
	auto result = SubstituteHKey(hKey);
	return RegOpenKeyTransactedA_OLD(hKey, lpSubKey, ulOptions, samDesired, phkResult, hTransaction, pExtendedParemeter);
}
LSTATUS APIENTRY RegOpenKeyTransactedW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID pExtendedParemeter)
{
	auto result = SubstituteHKey(hKey);
	return RegOpenKeyTransactedW_OLD(hKey, lpSubKey, ulOptions, samDesired, phkResult, hTransaction, pExtendedParemeter);
}
LSTATUS APIENTRY RegQueryInfoKeyA_Replacement(HKEY hKey, LPSTR lpClass, LPDWORD lpcchClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime)
{
	auto result = SubstituteHKey(hKey);
	return RegQueryInfoKeyA_OLD(hKey, lpClass, lpcchClass, lpReserved, lpcSubKeys, lpcbMaxSubKeyLen, lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor, lpftLastWriteTime);
}
LSTATUS APIENTRY RegQueryInfoKeyW_Replacement(HKEY hKey, LPWSTR lpClass, LPDWORD lpcchClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime)
{
	auto result = SubstituteHKey(hKey);
	return RegQueryInfoKeyW_OLD(hKey, lpClass, lpcchClass, lpReserved, lpcSubKeys, lpcbMaxSubKeyLen, lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor, lpftLastWriteTime);
}
LSTATUS APIENTRY RegQueryValueA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPSTR lpData, PLONG lpcbData)
{
	auto result = SubstituteHKey(hKey);
	return RegQueryValueA_OLD(hKey, lpSubKey, lpData, lpcbData);
}
LSTATUS APIENTRY RegQueryValueW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPWSTR lpData, PLONG lpcbData)
{
	auto result = SubstituteHKey(hKey);
	return RegQueryValueW_OLD(hKey, lpSubKey, lpData, lpcbData);
}
LSTATUS APIENTRY RegQueryMultipleValuesA_Replacement(HKEY hKey, PVALENTA val_list, DWORD num_vals, LPSTR lpValueBuf, LPDWORD ldwTotsize)
{
	auto result = SubstituteHKey(hKey);
	return RegQueryMultipleValuesA_OLD(hKey, val_list, num_vals, lpValueBuf, ldwTotsize);
}
LSTATUS APIENTRY RegQueryMultipleValuesW_Replacement(HKEY hKey, PVALENTW val_list, DWORD num_vals, LPWSTR lpValueBuf, LPDWORD ldwTotsize)
{
	auto result = SubstituteHKey(hKey);
	return RegQueryMultipleValuesW_OLD(hKey, val_list, num_vals, lpValueBuf, ldwTotsize);
}
LSTATUS APIENTRY RegQueryValueExA_Replacement(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	auto result = SubstituteHKey(hKey);
	return RegQueryValueExA_OLD(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}
LSTATUS APIENTRY RegQueryValueExW_Replacement(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	auto result = SubstituteHKey(hKey);
	return RegQueryValueExW_OLD(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}
LSTATUS APIENTRY RegReplaceKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpNewFile, LPCSTR lpOldFile)
{
	auto result = SubstituteHKey(hKey);
	return RegReplaceKeyA_OLD(hKey, lpSubKey, lpNewFile, lpOldFile);
}
LSTATUS APIENTRY RegReplaceKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpNewFile, LPCWSTR lpOldFile)
{
	auto result = SubstituteHKey(hKey);
	return RegReplaceKeyW_OLD(hKey, lpSubKey, lpNewFile, lpOldFile);
}
LSTATUS APIENTRY RegRestoreKeyA_Replacement(HKEY hKey, LPCSTR lpFile, DWORD dwFlags)
{
	auto result = SubstituteHKey(hKey);
	return RegRestoreKeyA_OLD(hKey, lpFile, dwFlags);
}
LSTATUS APIENTRY RegRestoreKeyW_Replacement(HKEY hKey, LPCWSTR lpFile, DWORD dwFlags)
{
	auto result = SubstituteHKey(hKey);
	return RegRestoreKeyW_OLD(hKey, lpFile, dwFlags);
}
LSTATUS APIENTRY RegRenameKey_Replacement(HKEY hKey, LPCWSTR lpSubKeyName, LPCWSTR lpNewKeyName)
{
	auto result = SubstituteHKey(hKey);
	return RegRenameKey_OLD(hKey, lpSubKeyName, lpNewKeyName);
}
LSTATUS APIENTRY RegSaveKeyA_Replacement(HKEY hKey, LPCSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	auto result = SubstituteHKey(hKey);
	return RegSaveKeyA_OLD(hKey, lpFile, lpSecurityAttributes);
}
LSTATUS APIENTRY RegSaveKeyW_Replacement(HKEY hKey, LPCWSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	auto result = SubstituteHKey(hKey);
	return RegSaveKeyW_OLD(hKey, lpFile, lpSecurityAttributes);
}
LSTATUS APIENTRY RegSetKeySecurity_Replacement(HKEY hKey, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	auto result = SubstituteHKey(hKey);
	return RegSetKeySecurity_OLD(hKey, SecurityInformation, pSecurityDescriptor);
}
LSTATUS APIENTRY RegSetValueA_Replacement(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData)
{
	auto result = SubstituteHKey(hKey);
	return RegSetValueA_OLD(hKey, lpSubKey, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegSetValueW_Replacement(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData)
{
	auto result = SubstituteHKey(hKey);
	return RegSetValueW_OLD(hKey, lpSubKey, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegSetValueExA_Replacement(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	auto result = SubstituteHKey(hKey);
	return RegSetValueExA_OLD(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegSetValueExW_Replacement(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	auto result = SubstituteHKey(hKey);
	return RegSetValueExW_OLD(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegUnLoadKeyA_Replacement(HKEY hKey, LPCSTR lpSubKey)
{
	auto result = SubstituteHKey(hKey);
	return RegUnLoadKeyA_OLD(hKey, lpSubKey);
}
LSTATUS APIENTRY RegUnLoadKeyW_Replacement(HKEY hKey, LPCWSTR lpSubKey)
{
	auto result = SubstituteHKey(hKey);
	return RegUnLoadKeyW_OLD(hKey, lpSubKey);
}
LSTATUS APIENTRY RegDeleteKeyValueA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteKeyValueA_OLD(hKey, lpSubKey, lpValueName);
}
LSTATUS APIENTRY RegDeleteKeyValueW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteKeyValueW_OLD(hKey, lpSubKey, lpValueName);
}
LSTATUS APIENTRY RegSetKeyValueA_Replacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwType, LPCVOID lpData, DWORD cbData)
{
	auto result = SubstituteHKey(hKey);
	return RegSetKeyValueA_OLD(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegSetKeyValueW_Replacement(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPCVOID lpData, DWORD cbData)
{
	auto result = SubstituteHKey(hKey);
	return RegSetKeyValueW_OLD(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
}
LSTATUS APIENTRY RegDeleteTreeA_Replacement(HKEY hKey, LPCSTR lpSubKey)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteTreeA_OLD(hKey, lpSubKey);
}
LSTATUS APIENTRY RegDeleteTreeW_Replacement(HKEY hKey, LPCWSTR lpSubKey)
{
	auto result = SubstituteHKey(hKey);
	return RegDeleteTreeW_OLD(hKey, lpSubKey);
}
LSTATUS APIENTRY RegCopyTreeA_Replacement(HKEY hKeySrc, LPCSTR lpSubKey, HKEY hKeyDest)
{
	return RegCopyTreeA_OLD(hKeySrc, lpSubKey, hKeyDest);
}
LSTATUS APIENTRY RegGetValueA_Replacement(HKEY hkey, LPCSTR lpSubKey, LPCSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{
	auto result = SubstituteHKey(hkey);
	return RegGetValueA_OLD(hkey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
}
LSTATUS APIENTRY RegGetValueW_Replacement(HKEY hkey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{
	auto result = SubstituteHKey(hkey);
	return RegGetValueW_OLD(hkey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
}
LSTATUS APIENTRY RegCopyTreeW_Replacement(HKEY hKeySrc, LPCWSTR lpSubKey, HKEY hKeyDest)
{
	return RegCopyTreeW_OLD(hKeySrc, lpSubKey, hKeyDest);
}
LSTATUS APIENTRY RegLoadMUIStringA_Replacement(HKEY hKey, LPCSTR pszValue, LPSTR pszOutBuf, DWORD cbOutBuf, LPDWORD pcbData, DWORD Flags, LPCSTR pszDirectory)
{
	auto result = SubstituteHKey(hKey);
	return RegLoadMUIStringA_OLD(hKey, pszValue, pszOutBuf, cbOutBuf, pcbData, Flags, pszDirectory);
}
LSTATUS APIENTRY RegLoadMUIStringW_Replacement(HKEY hKey, LPCWSTR pszValue, LPWSTR pszOutBuf, DWORD cbOutBuf, LPDWORD pcbData, DWORD Flags, LPCWSTR pszDirectory)
{
	auto result = SubstituteHKey(hKey);
	return RegLoadMUIStringW_OLD(hKey, pszValue, pszOutBuf, cbOutBuf, pcbData, Flags, pszDirectory);
}
LSTATUS APIENTRY RegLoadAppKeyA_Replacement(LPCSTR lpFile, PHKEY phkResult, REGSAM samDesired, DWORD dwOptions, DWORD Reserved)
{
	return RegLoadAppKeyA_OLD(lpFile, phkResult, samDesired, dwOptions, Reserved);
}
LSTATUS APIENTRY RegLoadAppKeyW_Replacement(LPCWSTR lpFile, PHKEY phkResult, REGSAM samDesired, DWORD dwOptions, DWORD Reserved)
{
	return RegLoadAppKeyW_OLD(lpFile, phkResult, samDesired, dwOptions, Reserved);
}
LSTATUS APIENTRY RegSaveKeyExA_Replacement(HKEY hKey, LPCSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD Flags)
{
	auto result = SubstituteHKey(hKey);
	return RegSaveKeyExA_OLD(hKey, lpFile, lpSecurityAttributes, Flags);
}
LSTATUS APIENTRY RegSaveKeyExW_Replacement(HKEY hKey, LPCWSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD Flags)
{
	auto result = SubstituteHKey(hKey);
	return RegSaveKeyExW_OLD(hKey, lpFile, lpSecurityAttributes, Flags);
}
