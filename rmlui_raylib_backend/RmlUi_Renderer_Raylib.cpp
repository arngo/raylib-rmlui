/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#define RMLUI_STATIC_LIB

#include "RmlUi_Renderer_Raylib.h"
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Platform.h>
#include <raylib.h>
#include <rlgl.h>


void RenderInterface_Raylib::RaylibTriangleVert(Rml::Vertex& vert)
{
	Color* c;
	c = (Color*)&vert.colour;
	rlColor4ub(c->r, c->g, c->b, c->a);
	rlTexCoord2f(vert.tex_coord.x, vert.tex_coord.y);
	rlVertex2f(vert.position.x, vert.position.y);
}

RenderInterface_Raylib::RenderInterface_Raylib()
{
}

RenderInterface_Raylib::~RenderInterface_Raylib()
{
}

void RenderInterface_Raylib::SetViewport(int width, int height)
{
	viewport_width = width;
	viewport_height = height;
}

void RenderInterface_Raylib::BeginFrame()
{
	SetViewport(GetScreenWidth(), GetScreenHeight());
	rlDrawRenderBatchActive();
	//rlDisableBackfaceCulling();
}

void RenderInterface_Raylib::EndFrame() 
{
	rlDrawRenderBatchActive();
	rlSetTexture(0);
	rlDisableScissorTest();
	//rlEnableBackfaceCulling();
}


void RenderInterface_Raylib::RenderGeometry(
		Rml::Vertex* vertices,
		int num_vertices, // ignore?
		int* indices,
		int num_indices,
		const Rml::TextureHandle texture,
		const Rml::Vector2f& translation
		)
{
	rlPushMatrix();
	rlTranslatef(translation.x, translation.y, 0);
	rlBegin(RL_TRIANGLES);
	rlSetTexture(texture);

	for (unsigned int i = 0; i <= (num_indices - 3); i += 3)
	{
		if(rlCheckRenderBatchLimit(3))
		{
			rlBegin(RL_TRIANGLES);
			rlSetTexture(texture);
		}

		int indexA = indices[i];
		int indexB = indices[i + 1];
		int indexC = indices[i + 2];

		Rml::Vertex vertexA = vertices[indexA];
		Rml::Vertex vertexB = vertices[indexB];
		Rml::Vertex vertexC = vertices[indexC];

		RenderInterface_Raylib::RaylibTriangleVert(vertexA);
		RenderInterface_Raylib::RaylibTriangleVert(vertexB);
		RenderInterface_Raylib::RaylibTriangleVert(vertexC);
	}


	rlDrawRenderBatchActive();
	rlEnd();
	rlPopMatrix();
}


void RenderInterface_Raylib::EnableScissorRegion(bool enable)
{
	if(enable)
	{
		rlEnableScissorTest();
	}
	else
	{
		rlDisableScissorTest();
	}
}

void RenderInterface_Raylib::SetScissorRegion(int x, int y, int width, int height)
{
	rlScissor(x, viewport_height - (y + height), width, height);
}


bool RenderInterface_Raylib::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source)
{
	Texture texture = ::LoadTexture(source.c_str());
	if(texture.id == 0)
	{
		return false;
	}

	texture_dimensions.x = texture.width;
	texture_dimensions.y = texture.height;
	texture_handle = (Rml::TextureHandle) texture.id;

	return true;
}

bool RenderInterface_Raylib::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
{
	Image image = GenImageColor(source_dimensions.x, source_dimensions.y, BLANK);
	Texture texture = LoadTextureFromImage(image);

	if(texture.id == 0)
	{
		return false;
	}

	UpdateTexture(texture, source);
	texture_handle = (Rml::TextureHandle)texture.id;

	return true;
}

void RenderInterface_Raylib::ReleaseTexture(Rml::TextureHandle texture_handle)
{
	rlUnloadTexture(texture_handle);
}

void RenderInterface_Raylib::SetTransform(const Rml::Matrix4f* new_transform)
{
	/*
	   transform_active = (new_transform != nullptr);
	   transform = projection * (new_transform ? *new_transform : Rml::Matrix4f::Identity());
	   transform_dirty_state = ProgramId::All;
	   */

	/*
	    float matfloat[16] = {
		mat.m0, mat.m1, mat.m2, mat.m3,
		mat.m4, mat.m5, mat.m6, mat.m7,
		mat.m8, mat.m9, mat.m10, mat.m11,
		mat.m12, mat.m13, mat.m14, mat.m15
		};
		*/
	/*if(state.active_matrix) {
		rlPopMatrix();
		state.active_matrix = false;
	}*/

	if(new_transform == nullptr)
	{
		float matfloat[16] = { *Rml::Matrix4f::Identity().Transpose().data() };
		
		Matrix matrix = {
			//matfloat[0], matfloat[4], matfloat[8], matfloat[12], 
			//matfloat[1], matfloat[5], matfloat[9], matfloat[13], 
			//matfloat[2], matfloat[6], matfloat[10],matfloat[14],   
			//matfloat[3], matfloat[7], matfloat[11],matfloat[15],
			matfloat[0], matfloat[1], matfloat[2], matfloat[3], 
			matfloat[4], matfloat[5], matfloat[6], matfloat[7], 
			matfloat[8], matfloat[9], matfloat[10], matfloat[11], 
			matfloat[12], matfloat[13], matfloat[14], matfloat[15],
			/*
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
			*/
		};

		//::rlSetMatrixProjection(matrix);
		//rlMultMatrixf(matfloat);
		rlLoadIdentity();
	}
	else
	{
		//rlPushMatrix();
		//state.active_matrix = true;
		float matfloat[16] = { *(new_transform->Transpose().data()) };
		
		Matrix matrix = {
			//matfloat[0], matfloat[4], matfloat[8], matfloat[12], 
			//matfloat[1], matfloat[5], matfloat[9], matfloat[13], 
			//matfloat[2], matfloat[6], matfloat[10],matfloat[14],   
			//matfloat[3], matfloat[7], matfloat[11],matfloat[15],
			matfloat[0], matfloat[1], matfloat[2], matfloat[3], 
			matfloat[4], matfloat[5], matfloat[6], matfloat[7], 
			matfloat[8], matfloat[9], matfloat[10], matfloat[11], 
			matfloat[12], matfloat[13], matfloat[14], matfloat[15],
/*
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
*/
		};

		//::rlSetMatrixProjection(matrix);
		//::rlSetUniformMatrix(1,matrix);
		//::rlMultMatrixf(matfloat);
		rlMultMatrixf(matfloat);
		//rlLoadIdentity();
	}
}

