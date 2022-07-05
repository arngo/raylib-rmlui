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
	rlDisableBackfaceCulling();
	/*
	   RMLUI_ASSERT(viewport_width > 0 && viewport_height > 0);
	   glViewport(0, 0, viewport_width, viewport_height);

	   glClearStencil(0);
	   glClearColor(0, 0, 0, 1);

	   glDisable(GL_CULL_FACE);

	   glEnable(GL_STENCIL_TEST);
	   glStencilFunc(GL_ALWAYS, 1, GLuint(-1));
	   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	   glEnable(GL_BLEND);
	   glBlendEquation(GL_FUNC_ADD);
	   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	   projection = Rml::Matrix4f::ProjectOrtho(0, (float)viewport_width, (float)viewport_height, 0, -10000, 10000);

	   SetTransform(nullptr);
	   */
}

void RenderInterface_Raylib::EndFrame() 
{
	rlSetTexture(0);
	rlDisableScissorTest();
	rlEnableBackfaceCulling();
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
	Texture* rlTexture = (Texture*)texture;

	unsigned int textureId = (rlTexture == nullptr) ? 0 : rlTexture->id;

	rlBegin(RL_TRIANGLES);

	for (unsigned int i = 0; i <= (num_indices - 3); i += 3)
	{
		if(rlCheckRenderBatchLimit(3))
		{
			rlBegin(RL_TRIANGLES);
			rlSetTexture(textureId);
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


	rlEnd();
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

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1)
struct TGAHeader {
	char idLength;
	char colourMapType;
	char dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char bitsPerPixel;
	char imageDescriptor;
};
// Restore packing
#pragma pack()

bool RenderInterface_Raylib::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source)
{
	/*
	   Rml::FileInterface* file_interface = Rml::GetFileInterface();
	   Rml::FileHandle file_handle = file_interface->Open(source);
	   if (!file_handle)
	   {
	   return false;
	   }

	   file_interface->Seek(file_handle, 0, SEEK_END);
	   size_t buffer_size = file_interface->Tell(file_handle);
	   file_interface->Seek(file_handle, 0, SEEK_SET);

	   if (buffer_size <= sizeof(TGAHeader))
	   {
	   Rml::Log::Message(Rml::Log::LT_ERROR, "Texture file size is smaller than TGAHeader, file is not a valid TGA image.");
	   file_interface->Close(file_handle);
	   return false;
	   }

	   using Rml::byte;
	   byte* buffer = new byte[buffer_size];
	   file_interface->Read(buffer, buffer_size, file_handle);
	   file_interface->Close(file_handle);

	   TGAHeader header;
	   memcpy(&header, buffer, sizeof(TGAHeader));

	   int color_mode = header.bitsPerPixel / 8;
	   int image_size = header.width * header.height * 4; // We always make 32bit textures

	   if (header.dataType != 2)
	   {
	   Rml::Log::Message(Rml::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
	   delete[] buffer;
	   return false;
	   }

	// Ensure we have at least 3 colors
	if (color_mode < 3)
	{
	Rml::Log::Message(Rml::Log::LT_ERROR, "Only 24 and 32bit textures are supported.");
	delete[] buffer;
	return false;
	}

	const byte* image_src = buffer + sizeof(TGAHeader);
	byte* image_dest = new byte[image_size];

	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; y++)
	{
	long read_index = y * header.width * color_mode;
	long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * 4;
	for (long x = 0; x < header.width; x++)
	{
	image_dest[write_index] = image_src[read_index + 2];
	image_dest[write_index + 1] = image_src[read_index + 1];
	image_dest[write_index + 2] = image_src[read_index];
	if (color_mode == 4)
	{
	const int alpha = image_src[read_index + 3];
#ifdef RMLUI_SRGB_PREMULTIPLIED_ALPHA
image_dest[write_index + 0] = (image_dest[write_index + 0] * alpha) / 255;
image_dest[write_index + 1] = (image_dest[write_index + 1] * alpha) / 255;
image_dest[write_index + 2] = (image_dest[write_index + 2] * alpha) / 255;
#endif
image_dest[write_index + 3] = (byte)alpha;
}
else
{
image_dest[write_index + 3] = 255;
}

write_index += 4;
read_index += color_mode;
}
}

texture_dimensions.x = header.width;
texture_dimensions.y = header.height;

bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);

delete[] image_dest;
delete[] buffer;

return success;
*/
return false;
}

bool RenderInterface_Raylib::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
{
	/*
	   GLuint texture_id = 0;
	   glGenTextures(1, &texture_id);
	   if (texture_id == 0)
	   {
	   Rml::Log::Message(Rml::Log::LT_ERROR, "Failed to generate texture.");
	   return false;
	   }

	   glBindTexture(GL_TEXTURE_2D, texture_id);

	   GLint internal_format = INTERNAL_FORMAT;
	   glTexImage2D(GL_TEXTURE_2D, 0, internal_format, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	   texture_handle = (Rml::TextureHandle)texture_id;

	   return true;
	   */
	return false;
}

void RenderInterface_Raylib::ReleaseTexture(Rml::TextureHandle texture_handle)
{
	//glDeleteTextures(1, (GLuint*)&texture_handle);
}

void RenderInterface_Raylib::SetTransform(const Rml::Matrix4f* new_transform)
{
	/*
	   transform_active = (new_transform != nullptr);
	   transform = projection * (new_transform ? *new_transform : Rml::Matrix4f::Identity());
	   transform_dirty_state = ProgramId::All;
	   */
}

//void RenderInterface_Raylib::SubmitTransformUniform(ProgramId program_id, int uniform_location)
//{
/*
   if ((int)program_id & (int)transform_dirty_state)
   {
   glUniformMatrix4fv(uniform_location, 1, false, transform.data());
   transform_dirty_state = ProgramId((int)transform_dirty_state & ~(int)program_id);
   }
   */
//}

