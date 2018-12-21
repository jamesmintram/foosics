#pragma comment(lib, "d3d9.lib")

#include <windows.h>
#include <assert.h>

#include "../common/dxmath/DirectXMath.h"

#include "DebugRenderer_DX9.h"

DebugRenderer *renderer = nullptr;

void checkLastError() {
	DWORD err = GetLastError();
	if (err == 0) return;

	LPWSTR errBuffer = NULL;
	size_t size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER 
		| FORMAT_MESSAGE_FROM_SYSTEM 
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		errBuffer,
		0,
		NULL);

	OutputDebugString(errBuffer);

	int t = 10;
	LocalFree(errBuffer);
}

//----------------------------------------------------------------

extern void demoDraw(IDebugRenderer *worldRenderer);
extern void demoInit();

float viewMat[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1,
};

float projmat[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1,
};



#define M_PI       3.14159265358979323846   // pi
const float deg2rad = (float)M_PI / 180.0f;

void matPerspective(float *result, float nearPlane, float farPlane, float fov, float aspect)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearPlane, farPlane);
	memcpy(result, &mat, sizeof(float) * 16);
}

void vec3Normalize(float *vec)
{
	float const sqrmag = 
		vec[0] * vec[0] +
		vec[1] * vec[1] +
		vec[2] * vec[2];

	assert(sqrmag > 0);

	float const mag = sqrtf(sqrmag);
	float const invFac = 1.0f / mag;

	vec[0] *= invFac;
	vec[1] *= invFac;
	vec[2] *= invFac;
}

void vec3Cross(float *result, float const *lhs, float const *rhs)
{
	result[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
	result[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
	result[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

void mat4Mul(float *result, float const *lhs, float const *rhs)
{
	const int matSize = 4;
	for (int i = 0; i < matSize; ++i)
	{
		for (int j = 0; j < matSize; j++)
		{
			float sum = 0;
			for (int k = 0; k < matSize; k++)
			{
				sum += lhs[i * 4 + k] * rhs[k * 4 + j];
			}
			result[i * 4 + j] = sum;
		}
	}
}

void matLookat(float *result, float *i_eye, float *i_target, float *i_up)
{
	float f[] = {
		i_target[0] - i_eye[0],
		i_target[1] - i_eye[1],
		i_target[2] - i_eye[2]
	};

	vec3Normalize(f);
	
	float s[3];
	vec3Cross(s, f, i_up);
	vec3Normalize(s);

	float u[3];
	vec3Cross(u, s, f);

	float rotationMatrix[16] = {
		 s[0],  s[1],  s[2], 0,
		 u[0],  u[1],  u[2], 0,
		-f[0], -f[1], -f[2], 0,
		 0,     0,     0,    1
	};

	/*rotationMatrix[12] = -i_eye[0];
	rotationMatrix[13] = -i_eye[1];
	rotationMatrix[14] = -i_eye[2];*/
	
	rotationMatrix[3] = -i_eye[0];
	rotationMatrix[7] = -i_eye[1];
	rotationMatrix[11] = -i_eye[2];

	memcpy(result, rotationMatrix, sizeof(rotationMatrix));
}

LRESULT drawDX(HWND hwnd)
{
	renderer->SetViewProj(viewMat, projmat);

	demoDraw(renderer);
	return 0;
}


// Add this

void init()
{
	//Setup the view/proj mat
	float eye[] = { 0, 5, -10 };
	float target[] = { 0, 0, 0 };
	float up[] = { 0, 1, 0 };

	//matLookat(viewMat, eye, target, up);
	matPerspective(projmat, 1.0f, 100.0f, 60.0f, 4.0f / 3.0f);

	demoInit();
}


//----------------------------------------------------------------

LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break;
	case WM_PAINT:
		return drawDX(hwnd);
	case WM_LBUTTONDOWN:
		return true;
	case WM_LBUTTONUP:
		return true;
	case WM_RBUTTONDOWN:
		return true;
	case WM_RBUTTONUP:
		return true;
	case WM_MBUTTONDOWN:
		return true;
	case WM_MBUTTONUP:
		return true;
	case WM_MOUSEWHEEL:
		return true;
	case WM_MOUSEMOVE:
		return true;
	case WM_KEYDOWN:
		return true;
	case WM_KEYUP:
		return true;
	case WM_CHAR:
		return true;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void printGLVersion() {
	OutputDebugString(L"DirectX Version: ");

	OutputDebugString(L"\n");
}


ATOM createWindowClass(HINSTANCE hinstance) {

	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(windowClass);
	windowClass.style = (UINT)CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;

	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	windowClass.hInstance = hinstance;
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	windowClass.lpszClassName = L"phoosicswindow";

	return RegisterClassEx(&windowClass);
}

HWND createWindow(HINSTANCE hinstance) {

	createWindowClass(hinstance);

	checkLastError();

	RECT wr = { 0, 0, 1024, 768}; 
	AdjustWindowRectEx(&wr, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		L"phoosicswindow",
		L"Physics Testbed",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hinstance,
		NULL);

	checkLastError();

	ShowWindow(hWnd, SW_SHOW);

	renderer = new DebugRenderer();
	renderer->initDX(hWnd);
	renderer->SetViewport(0, 0, 1027, 768);

	UpdateWindow(hWnd);

	return hWnd;
}


int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	MSG msg;
	BOOL ret = 0;

	HWND hWnd = createWindow(hInstance);

	init();

	INT64 g_Time = 0;
	INT64 g_TicksPerSecond = 0;

	for (;;) {
		checkLastError();
		ret = GetMessage(&msg, NULL, 0, 0);

		if (ret > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (ret < 0) {
			DWORD err = GetLastError();
			break;
		}
		else {
			break;
		}

		INT64 current_time;
		QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
		
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
	}

	return 0;
}