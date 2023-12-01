#include <iostream>
#include <atlstr.h>
#include <string>
#include <iphlpapi.h>
#include <windows.h>
#include <tchar.h>

#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "iphlpapi.lib")

using namespace std;

void GetTime() {
	struct tm t;
	char str_time[32];
	time_t now = time(nullptr);
	localtime_s(&t, &now);
	strftime(str_time, 100, "%F %X", &t);
	cout << "Current Time : " << str_time;
}
void GetUUID() {
	UUID uuid;
	UuidCreate(&uuid);
	char *str_uuid;
	UuidToStringA(&uuid, (RPC_CSTR*)&str_uuid);
	cout << "UUID : " << str_uuid;
	RpcStringFreeA((RPC_CSTR*)&str_uuid);
}
void GetUser() {
	char str_username[1024];
	DWORD dwUserNameLen = 1024;
	GetUserNameA(str_username, &dwUserNameLen);
	cout << "Username : " << string(str_username);
}
void GetHostname() {
	char str_hostname[1024];
	DWORD dwCompNameLen = 1024;
	GetComputerNameA(str_hostname, &dwCompNameLen);
	cout << "Hostname : " << string(str_hostname);
}
void GetIP() {
	char IPv4[16];
	char buffer[4];
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long adapter_size = sizeof(IP_ADAPTER_INFO);
	int ret = GetAdaptersInfo(pIpAdapterInfo, &adapter_size);
	if (ret == ERROR_BUFFER_OVERFLOW) {
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[adapter_size];
		ret = GetAdaptersInfo(pIpAdapterInfo, &adapter_size);
	}
	if (ret == ERROR_SUCCESS) {
		while (pIpAdapterInfo) {
			PIP_ADDR_STRING pIpAddr = &(pIpAdapterInfo->IpAddressList);
			while (pIpAddr) {
				strcpy_s(IPv4, pIpAddr->IpAddress.String);
				pIpAddr = pIpAddr->Next;
			}
			strncpy_s(buffer, IPv4, 3);
			if (strcmp(buffer, "169") == 0 || strcmp(buffer, "0.0") == 0) {
				pIpAdapterInfo = pIpAdapterInfo->Next;
			}
			else {
				break;
			}
		}
	}
	if (pIpAdapterInfo) {
		pIpAdapterInfo = nullptr;
		delete pIpAdapterInfo;
	}
	cout << "IPv4 : " << IPv4;
}
void GetOS() {
	HKEY hkey;
	LPCTSTR subkey = TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion");
	TCHAR str_value[100] = { 0 };
	DWORD size = sizeof(str_value);
	DWORD type = REG_SZ;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, NULL, KEY_READ, &hkey);
	RegQueryValueEx(hkey, TEXT("ProductName"), NULL, &type, (LPBYTE)&str_value, &size);
	wcout << "OS : " << str_value << "\n ";
	RegQueryValueEx(hkey, TEXT("CurrentBuildNumber"), NULL, &type, (LPBYTE)&str_value, &size);
	wcout << "BuildNumber : " << str_value << "\n ";
	RegQueryValueEx(hkey, TEXT("ReleaseId"), NULL, &type, (LPBYTE)&str_value, &size);
	wcout << "ReleaseId : " << str_value << "\n";
	RegCloseKey(hkey);
}

int main()
{
	cout << "-------------------Get Info--------------------\n";
	cout << "-----------------------------------------------\n ";
	GetTime();
	cout << "\n ";
	GetUUID();
	cout << "\n ";
	GetUser();
	cout << "\n ";
	GetHostname();
	cout << "\n ";
	GetIP();
	cout << "\n ";
	GetOS();
	cout << "-----------------------------------------------\n";
	system("pause");
}
