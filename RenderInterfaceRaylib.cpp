#include <RmlUi/Core.h>
#include <RmlUi/Core/RenderInterface.h>
#include <raylib/rlgl.h>

void RmlUiSFMLRenderer::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, const Rml::TextureHandle texture, const Rml::Vector2f& translation)
{
    MyWindow->pushGLStates();
    initViewport();

    rlTranslatef(translation.x, translation.y, 0);

    Rml::Vector<Rml::Vector2f> Positions(num_vertices);
    Rml::Vector<Rml::Colourb> Colors(num_vertices);
    Rml::Vector<Rml::Vector2f> TexCoords(num_vertices);

    for(int i = 0; i < num_vertices; i++)
    {
        Positions[i] = vertices[i].position;
        Colors[i] = vertices[i].colour;
        TexCoords[i] = vertices[i].tex_coord;
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, &Positions[0]);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &Colors[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &TexCoords[0]);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sf::Texture *sfTexture = (sf::Texture *)texture;

    if(sfTexture)
    {
        sf::Texture::bind(sfTexture);
    }
    else
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, 0);
    };

    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glColor4f(1, 1, 1, 1);

    MyWindow->popGLStates();
}

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
