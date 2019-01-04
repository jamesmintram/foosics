#pragma once

#include "../../foosics/debug/DebugRenderer.h"
#include "../../foosics/common/ph_assert.h"

#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/imgui_impl_dx9.h"

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
    float scale;
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

const DirectX::FXMVECTOR cube[] = {
	{ -0.5f, -0.5f,  0.5f,       1 },
	{  0.5f, -0.5f,  0.5f,       1 },
	{  0.5f,  0.5f,  0.5f,       1 },
	{ -0.5f,  0.5f,  0.5f,       1 },
	   
	{ -0.5f, -0.5f, -0.5f,       1 },
	{ 0.5f,  -0.5f, -0.5f,       1 },
	{ 0.5f,   0.5f, -0.5f,       1 },
	{ -0.5f,  0.5f, -0.5f,       1 },
};

const uint16_t cubeIndices[] = {
	// front
	0, 2, 1,
	2, 0, 3,
	// right
	1, 6, 5,
	6, 1, 2,
	// back
	7, 5, 6,
	5, 7, 4,
	// left
	4, 3, 0,
	3, 4, 7,
	// bottom
	4, 1, 5,
	1, 4, 0,
	// top
	3, 6, 2,
	6, 3, 7
};

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

class DebugRenderer : public IDebugRenderer
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;

	LPDIRECT3DVERTEXBUFFER9 triBuffer;
	LPDIRECT3DVERTEXBUFFER9 lineBuffer;
	LPDIRECT3DVERTEXBUFFER9 pointBuffer;

	LPDIRECT3DINDEXBUFFER9 triIndices;

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
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.hDeviceWindow = hWnd;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.BackBufferWidth = 1024;
		d3dpp.BackBufferHeight = 768;

		d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
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

		d3ddev->CreateIndexBuffer(
			kTriCount * 3,
			0,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&triIndices,
			NULL);

		ImGui_ImplDX9_Init(d3ddev);
		ImGui_ImplDX9_CreateDeviceObjects();
	}

	void Render()
	{
		Begin();
		Clear(0x6495ED); //ARGB

		d3ddev->SetViewport(&viewPort);

		d3ddev->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
		d3ddev->SetTransform(D3DTS_VIEW, &viewMatrix);

		d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);

		d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);


		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		d3ddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		RenderTriangles();
		RenderLines();
		RenderPoints();

		//Imgui

		d3ddev->SetRenderState(D3DRS_ZENABLE, false);
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		d3ddev->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

		static int lastFrameCount = 0;

		if (ImGui::GetFrameCount() > lastFrameCount)
		{
			lastFrameCount = ImGui::GetFrameCount();

			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}

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

	void DrawCube(float const *pos, float const *rot, float const scale)
	{
		//Transform verts and append to buffer
		RenderedCube cube;
		memcpy(&cube.pos, pos, sizeof(*pos) * 3);
		memcpy(&cube.rot, rot, sizeof(*rot) * 3);
		cube.color = penColor;
        cube.scale = scale;

		m_cubes.push_back(cube);
	}

private:
	void RenderTriangles()
	{
		uint16_t * indices = nullptr;
		triIndices->Lock(0, 0, (void**)&indices, 0);

		CUSTOMVERTEX * triBufferData = nullptr;
		triBuffer->Lock(0, 0, (void**)&triBufferData, 0);
		
		uint16_t *currentIndex = indices;
		CUSTOMVERTEX *currentVertex = triBufferData;

		// Cubes first
		const size_t cubeCount = m_cubes.size();

		for (size_t idx = 0; idx < cubeCount; idx++)
		{
			float const *pos = m_cubes[idx].pos;
			float const *rot = m_cubes[idx].rot;
            float const scale = m_cubes[idx].scale;
			uint32_t const col = m_cubes[idx].color;

			DirectX::XMMATRIX rotM = DirectX::XMMatrixRotationRollPitchYaw(rot[0], rot[1], rot[2]);
			DirectX::XMMATRIX trnM = DirectX::XMMatrixTranslation(pos[0], pos[1], pos[2]);
            DirectX::XMMATRIX sclM = DirectX::XMMatrixScaling(scale, scale, scale);

			DirectX::XMMATRIX cubeM = DirectX::XMMatrixMultiply(rotM, trnM);
            cubeM = DirectX::XMMatrixMultiply(sclM, cubeM);

			size_t startVert = currentVertex - triBufferData;


			//TODO: Push transformed verts into the buffer
			for (int idx = 0; idx < COUNT_OF(cube); idx++)
			{
				DirectX::XMVECTOR vertPos = DirectX::XMVector4Transform(cube[idx], cubeM);
				currentVertex->x = vertPos.m128_f32[0];
				currentVertex->y = vertPos.m128_f32[1];
				currentVertex->z = vertPos.m128_f32[2];
			
				currentVertex->color = col + idx * 10000;

				currentVertex++;
			}

			for (size_t idx = 0; idx < COUNT_OF(cubeIndices); idx++)
			{
				size_t index = cubeIndices[idx] + startVert;
				PH_ASSERT(index < 0xFFFF);

				*currentIndex = (uint16_t)index;
				currentIndex++;
			}
		}
		
		triBuffer->Unlock();
		triIndices->Unlock();

		m_cubes.clear();
		uint32_t const triCount = (uint32_t)((currentIndex - indices) / 3);
		uint32_t const vertCount = (uint32_t)(currentVertex - triBufferData);

		if (triCount == 0) return;

		d3ddev->SetFVF(CUSTOMFVF);
		d3ddev->SetStreamSource(0, triBuffer, 0, sizeof(CUSTOMVERTEX));
		d3ddev->SetIndices(triIndices);
		d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertCount, 0, triCount);
	}
	void RenderLines()
	{
		const uint32_t lineCount = (uint32_t)m_lines.size();
		if (lineCount == 0) return;

		CUSTOMVERTEX* lineBufferData = nullptr;
		lineBuffer->Lock(0, 0, (void**)&lineBufferData, 0);

		for (uint32_t idx = 0; idx < lineCount; idx++)
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
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
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