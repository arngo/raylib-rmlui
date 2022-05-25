#include <RmlUi/Core.h>
#include <RmlUi/Core/RenderInterface.h>
#include <raylib/rlgl.h>
#include <raylib/raylib.h>

// Called by RmlUi when it wants to render geometry that the application does not wish to optimise.
void RmlUiRaylibRenderer::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, const Rml::TextureHandle texture, const Rml::Vector2f& translation)
{
    // SDL uses shaders that we need to disable here  
    //glUseProgramObjectARB(0);
    rlDisableShader();
    //glPushMatrix();
    rlPushMatrix();
    //glTranslatef(translation.x, translation.y, 0);
    rlTranslatef(translation.x, translation.y, 0);
 
    Rml::Vector<Rml::Vector2f> Positions(num_vertices);
    Rml::Vector<Rml::Colourb> Colors(num_vertices);
    Rml::Vector<Rml::Vector2f> TexCoords(num_vertices);
    //float texw = 0.0f;
    //float texh = 0.0f;

    for(int i = 0; i < num_vertices; i++)
	{
		Positions[i] = vertices[i].position;
		Colors[i] = vertices[i].colour;
		TexCoords[i] = vertices[i].tex_coord;
	};

    //SDL_Texture* sdl_texture = nullptr;
    RenderTexture2D* rl_texture;
    if(texture)
    {
        //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        rlEnableStatePointer(GL_TEXTURE_COORD_ARRAY, &TexCoords[0]);
        //sdl_texture = (SDL_Texture *) texture;
        rl_texture = (RenderTexture2D *) texture;
        //SDL_GL_BindTexture(sdl_texture, &texw, &texh);
        rlEnableTexture(rl_texture.texture.id);
    }

    /*for(int  i = 0; i < num_vertices; i++) {
        Positions[i] = vertices[i].position;
        Colors[i] = vertices[i].colour;
        if (sdl_texture) {
            TexCoords[i].x = vertices[i].tex_coord.x * texw;
            TexCoords[i].y = vertices[i].tex_coord.y * texh;
        }
        else TexCoords[i] = vertices[i].tex_coord;
        TexCoords[i] = vertices[i].tex_coord;
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &Positions[0]);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &Colors[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &TexCoords[0]);
    */

    rlEnableStatePointer(GL_VERTEX_ARRAY, &Positions[0]);
    rlEnableStatePointer(GL_COLOR_ARRAY, &Colors[0]);

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //glEnable(GL_BLEND);
    rlEnableColorBlend();
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    rlSetBlendMode(RL_BLEND_ALPHA);
    //glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);
    rlDrawVertexArrayElements(0, num_indices, indices);
    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    rlDisableStatePointer(GL_VERTEX_ARRAY);
    rlDisableStatePointer(GL_COLOR_ARRAY);
 
    //if (sdl_texture) {
    if (rl_texture) {
        //SDL_GL_UnbindTexture(sdl_texture);
        rlDisableTexture();
        //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        rlDisableStatePointer(GL_TEXTURE_COORD_ARRAY);
    }
 
    //glColor4f(1.0, 1.0, 1.0, 1.0);
    rlColor4f(1.0, 1.0, 1.0, 1.0);
    //glPopMatrix();
    rlPopMatrix();
    /* Reset blending and draw a fake point just outside the screen to let SDL know that it needs to reset its state in case it wants to render a texture */
    //glDisable(GL_BLEND);
    rlDisableColorBlend();
    //SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_NONE);
    //SDL_RenderDrawPoint(mRenderer, -1, -1);
}

// Called by RmlUi when it wants to enable or disable scissoring to clip content.
void RmlUiRaylibRenderer::EnableScissorRegion(bool enable) {
    if (enable) {
        rlEnableScissorTest();
    } else {
        rlDisableScissorTest();
    }
}

// Called by RmlUi when it wants to change the scissor region.
void RmlUiRaylibRenderer::SetScissorRegion(int x, int y, int width, int height) {
    rlScissor(x, GetScreenHeight() - (y + height), width, height);
}


// Called by RmlUi when a texture is required by the library.
bool RmlUiRaylibRenderer::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source) {
    Texture2D rl_texture = LoadTexture(source);

    if (rl_texture) {
        texture_handle = (Rml::TextureHandle) &rl_texture;
        texture_dimensions = Rml::Vector2i(rl_texture.width, rl_texture.height);
        return true;
    }
    return false;
}

// Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
bool RmlUiRaylibRenderer::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) {
    RenderTexture2D rl_rendertexture = LoadRenderTexture(source_dimensions.x, source_dimensions.y);

    if (rl_rendertexture) {
        //rlUpdateTexture(rl_rendertexture.texture.id, 0, 0, source_dimensions.x, source_dimensions.y, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, source);
        UpdateTexture(rl_rendertexture.texture, source);
        texture_handle = (Rml::TextureHandle) &rl_rendertexture.texture;
        return true;
    }
    return false;
}

// Called by RmlUi when a loaded texture is no longer required.
void RmlUiRaylibRenderer::ReleaseTexture(Rml::TextureHandle texture_handle) {
    UnloadTexture((Texture2D *)texture_handle);
}
