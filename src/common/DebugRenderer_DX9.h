#pragma once

#include "../../foosics/debug/DebugRenderer.h"

#include <d3d9.h>
#include <vector>

struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD color;
};

struct RenderedCube
{
	float pos[3];
	float rot[3];

	uint32_t color;
};

struct RenderedLine
{
	float start[3];
	float end[3];

	uint32_t color;
};

struct RenderedPoint
{
	float pos[3];

	uint32_t color;
};

const uint32_t kBufferSize = 1024 * 65 * sizeof(CUSTOMVERTEX);
const uint32_t kTriCount = kBufferSize / 3;
const uint32_t kLineCount = kBufferSize / 2;
const uint32_t kPointCount = kBufferSize;

const float kIdentity[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1,
};

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)


class DebugRenderer : public IDebugRenderer
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;

	LPDIRECT3DVERTEXBUFFER9 triBuffer;
	LPDIRECT3DVERTEXBUFFER9 lineBuffer;
	LPDIRECT3DVERTEXBUFFER9 pointBuffer;

	D3DMATRIX projectionMatrix;
	D3DMATRIX viewMatrix;

	D3DVIEWPORT9 viewPort;

	float penPos[3];
	uint32_t penColor;

	std::vector<RenderedCube> m_cubes;
	std::vector<RenderedLine> m_lines;
	std::vector<RenderedPoint> m_points;
public:
	DebugRenderer()
		: d3d(nullptr)
		, d3ddev(nullptr)
	{
		penPos[0] = penPos[1] = penPos[2] = 0;
		penColor = 0xFFFFFFFF;

		SetViewProj(kIdentity, kIdentity);
	}

	void SetViewProj(float const *view, float const *projection)
	{
		memcpy(&viewMatrix, view, sizeof(viewMatrix));
		memcpy(&projectionMatrix, projection, sizeof(projectionMatrix));
	}

	void SetViewport(int x, int y, int w, int h)
	{
		viewPort.X = x;
		viewPort.Y = y;
		viewPort.Width = w;
		viewPort.Height = h;
	}

	void initDX(HWND hWnd)
	{
		d3d = Direct3DCreate9(D3D_SDK_VERSION);
		D3DPRESENT_PARAMETERS d3dpp;

		ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
		d3dpp.hDeviceWindow = hWnd;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.BackBufferWidth = 1024;
		d3dpp.BackBufferHeight = 768;

		d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&d3ddev);

		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);

		//Create buffers
		d3ddev->CreateVertexBuffer(
			kBufferSize,
			0,
			CUSTOMFVF,
			D3DPOOL_MANAGED,
			&triBuffer,
			NULL);
		d3ddev->CreateVertexBuffer(
			kBufferSize,
			0,
			CUSTOMFVF,
			D3DPOOL_MANAGED,
			&lineBuffer,
			NULL);
		d3ddev->CreateVertexBuffer(
			kBufferSize,
			0,
			CUSTOMFVF,
			D3DPOOL_MANAGED,
			&pointBuffer,
			NULL);
	}

	void Render()
	{
		Begin();
		Clear(0x6495ED); //ARGB

		d3ddev->SetViewport(&viewPort);

		d3ddev->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
		d3ddev->SetTransform(D3DTS_VIEW, &viewMatrix);

		d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

		RenderCubes();
		RenderLines();
		RenderPoints();

		End();
		Flip();
	}

	void SetPenColor(uint32_t newColor)
	{
		penColor = newColor;
	}
	void SetPen(float *newPenPos)
	{
		memcpy(penPos, newPenPos, sizeof(*newPenPos) * 3);
	}

	void DrawPoint()
	{

	}

	void DrawLine(float *endPoint)
	{
		RenderedLine line;
		memcpy(&line.start, penPos, sizeof(*penPos) * 3);
		memcpy(&line.end, endPoint, sizeof(*endPoint) * 3);
		line.color = penColor;

		m_lines.push_back(line);

		memcpy(penPos, endPoint, sizeof(*endPoint) * 3);
	}

	void DrawCube(float *pos, float * rot)
	{
		//Transform verts and append to buffer
		RenderedCube cube;
		memcpy(&cube.pos, pos, sizeof(*pos) * 3);
		memcpy(&cube.rot, rot, sizeof(*rot) * 3);
		cube.color = penColor;

		m_cubes.push_back(cube);
	}

private:
	void RenderCubes()
	{

	}
	void RenderLines()
	{
		const size_t lineCount = m_lines.size();

		CUSTOMVERTEX* lineBufferData = nullptr;
		lineBuffer->Lock(0, 0, (void**)&lineBufferData, 0);

		for (size_t idx = 0; idx < lineCount; idx++)
		{
			RenderedLine const& line = m_lines[idx];

			lineBufferData->x = line.start[0];
			lineBufferData->y = line.start[1];
			lineBufferData->z = line.start[2];
			lineBufferData->color = line.color;

			lineBufferData++;

			lineBufferData->x = line.end[0];
			lineBufferData->y = line.end[1];
			lineBufferData->z = line.end[2];
			lineBufferData->color = line.color;

			lineBufferData++;
		}

		lineBuffer->Unlock();
		m_lines.clear();

		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		d3ddev->SetStreamSource(0, lineBuffer, 0, sizeof(CUSTOMVERTEX));
		d3ddev->SetFVF(CUSTOMFVF);
		d3ddev->DrawPrimitive(D3DPT_LINELIST, 0, lineCount);
	}
	void RenderPoints()
	{

	}

	void Clear(uint32_t col)
	{
		d3ddev->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET,
			col,
			1.0f,
			0);
	}

	void Flip()
	{
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	void Begin()
	{
		d3ddev->BeginScene();
	}
	void End()
	{
		d3ddev->EndScene();
	}

};