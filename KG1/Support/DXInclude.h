#pragma once

#include <windows.h>
#include <WinUser.h>
#include <wrl.h>

#include <d3d11.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef _DEBUG
#include <d3d11sdklayers.h>
#include <dxgidebug.h>
#endif