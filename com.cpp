#include <windows.h>
#include <iostream>
#include <Objbase.h>
#include <Shobjidl.h>
#include <Shldisp.h>
int i = 0;
BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam) {
	i++;
	const int maxClassNameLength = 256;
	char className[maxClassNameLength];
	int classNameLength = GetClassNameA(hwndChild, className, maxClassNameLength);

	if (classNameLength > 0) {
		// 分配内存来存储类名   
		char* className = new char[classNameLength + 1];
		GetClassNameA(hwndChild, className, classNameLength + 1);
		//std::cout << "控件类名: " << className << std::endl;
		if (i == 5) {
			const wchar_t* textToSend = L"mstsc";
			//const wchar_t* textToSend = L" 1.lnk";
			SendMessage(hwndChild, WM_SETTEXT, 0, (LPARAM)textToSend);
		}

		delete[] className;
	}
	return TRUE;
}
int main() {

	// 初始化COM库 
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr)) {
		// 创建Shell对象  
		IShellDispatch* pShell = NULL;
		hr = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_IShellDispatch, (void**)&pShell);
		if (SUCCEEDED(hr)) {
			VARIANT vtEmpty;
			VariantInit(&vtEmpty);
			pShell->FileRun();
		}

		while (true)
		{
			HWND hwnd = FindWindowA(NULL, "运行");
			if (hwnd != NULL) {

				for (int i = 0; i < 50000; i++) {
					ShowWindow(hwnd, SW_HIDE);
				}
				//Sleep(1000);
				EnumChildWindows(hwnd, EnumChildProc, 0);
				HWND hwndButton = GetDlgItem(hwnd, 00000001);
				if (hwndButton != NULL) {
					// 模拟点击按钮
					SendMessage(hwndButton, BM_CLICK, 0, 0);
				}
				else {
					// 找不到按钮控件
					MessageBox(NULL, L"找不到按钮控件！", L"错误", MB_ICONERROR);
				}
				break;
			}
		}
		pShell->Release();

		// 反初始化COM库
		CoUninitialize();
	}
	else
	{
		std::cerr << "COM initialization failed: " << hr << std::endl;
	}
	system("pause");
	return 0;
}

