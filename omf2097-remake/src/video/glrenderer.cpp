#include "glrenderer.h"

#include "glextloader.h"
#include "tools/exception.h"
#include "tools/log.h"
#include "video/texture.h"
#include "video/shader.h"
#include "video/shaderprogram.h"

#include <SFML/Window.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <string>
#include <algorithm>

#define NATIVE_W 320
#define NATIVE_H 200

using namespace std;

/**
  * Framebuffer state. Tanks to Teistiz for this one.
  */
const char* getFrameBufferStatusMessage(int code) {
    switch(code) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        return "attachment";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        return "missing attachment";
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT:
        return "layer count";
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        return "dimensions";
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        return "formats";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        return "draw_buffer";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        return "read_buffer";
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        return "unsupported";
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB:
        return "layer_targets";
    case GL_FRAMEBUFFER_COMPLETE:
        return "framebuffer complete";
    default:
        return "unknown error";
    }
}

GLRenderer::GLRenderer(sf::Window *sfApp,
                       int w, int h, bool fs,
                       bool vsync,
                       int fpslimit,
                       FilterMode filter) {
    this->sfApp = sfApp;
    this->w = w;
    this->h = h;

    // Open up window
    int mode = fs ? sf::Style::Fullscreen : sf::Style::Close;
    sfApp->Create(sf::VideoMode(w, h, 32), "One Must Fall 2097 Remake Project (OMFGv0.1a)", mode);
    if(!sfApp->IsOpened()) {
        THROW("Window was not opened!");
    }

    // output some awesome log crap
    LOG("OpenGL window opened (" << w << "x" << h << "x32).");
    LOG("OpenGL Info:");
    LOG("  Vendor       : " << (const char*) glGetString(GL_VENDOR));
    LOG("  Renderer     : " << (const char*) glGetString(GL_RENDERER));
    LOG("  Version      : " << (const char*) glGetString(GL_VERSION));
    LOG("  GLSL Version : " << (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Enable VSync or framerate limit
    if(vsync) {
        sfApp->UseVerticalSync(true);
        LOG("Using VSync!");
    } else {
        sfApp->SetFramerateLimit(fpslimit); // When there is no VSync available
        LOG("No VSync, using framerate limit of " << fpslimit << " !");
    }

    // OpenGL Extension loader (well, really a wrapper for GLEW
    this->extloader = new GLExtLoader();

    // Create a new shader program for filters
    try {
        this->shaders = new ShaderProgram();
        switch(filter) {
        case FILTER_HQX:
            this->shaders->attach(new Shader("shaders/hq2x.vsh", Shader::VERTEX));
            this->shaders->attach(new Shader("shaders/hq2x.psh", Shader::FRAGMENT));
            LOG("HQX Filter selected.");
            break;
        case FILTER_NONE:
            LOG("No Filter selected.");
            break;
        }
    } catch(Exception &ex) {
        RETHROW(ex);
    }

    // Disable depth testing and lighting (not needed), enable texturing
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    // Create framebuffer for offscreen rendering, and attach it
    try {
        this->create_offscreen_framebuffer();
    } catch(Exception &ex) {
        RETHROW(ex);
    }

    // Create a nice quad for rendering a texture into.
    this->fs_quad_id = glGenLists(1);
    glNewList(this->fs_quad_id, GL_COMPILE);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f); // Top Right
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f); // Top Left
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
    glEnd();
    glEndList();

    // Set background as none for now.
    this->background_tex = 0;
}

GLRenderer::~GLRenderer() {
    glDeleteLists(this->fs_quad_id, 1);
    this->delete_offscreen_framebuffer();
    if(this->background_tex) {
        delete this->background_tex;
    }
    this->shaders->use(false);
    delete this->shaders;
    delete this->extloader;
    sfApp->Close();
}

void GLRenderer::create_offscreen_framebuffer() {
    // Create texture for FBO
    glGenTextures(1, &fbo_color_tex_id);
    glBindTexture(GL_TEXTURE_2D, fbo_color_tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, NATIVE_W, NATIVE_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Depth/stencil RBO
    glGenRenderbuffers(1, &fbo_depth_rbo_id);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth_rbo_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, NATIVE_W, NATIVE_H);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Teistiz has awesome OpenGL skillz. And code for copypastin'. (make FB)
    glGenFramebuffers(1, &fbo_id);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_color_tex_id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth_rbo_id);
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        THROW("Error while trying to initialize framebuffer: " << getFrameBufferStatusMessage(status));
    }
}

void GLRenderer::delete_offscreen_framebuffer() {
    // Delete offscreen fb, rb, color tex
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo_id);
    glDeleteRenderbuffers(1, &fbo_depth_rbo_id);
    glDeleteTextures(1, &fbo_color_tex_id);
}


void GLRenderer::draw_image(Drawable *drawable) {
    this->objects.push_back(drawable);
}

void GLRenderer::set_background(Texture *tex) {
    this->background_tex = tex;
}

void GLRenderer::prepare() {
    // Switch to FBO rendering
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

    // Set state
    glEnable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);

    // Clear stuff
    glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glClearStencil(0);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, NATIVE_W, NATIVE_H);
    glLoadIdentity();

    // Draw background if any. This should be set as 0 in stencil buffer,
    // we want the background to stay "neutral"
    if(this->background_tex) {
        glStencilFunc(GL_ALWAYS, 0, 0);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        glBindTexture(GL_TEXTURE_2D, this->background_tex->id);
        glCallList(this->fs_quad_id);
    }
}

bool sort_drawables_by_index(GLRenderer::Drawable* a, GLRenderer::Drawable* b) {
    return (a->index < b->index);
}

void GLRenderer::finish() {
    // Sort Drawables by index, highest index should be drawn last (on top)
    sort(objects.begin(), objects.end(), sort_drawables_by_index);

    // Render objects
    vector<Drawable*>::iterator it;
    for(it = objects.begin(); it != objects.end(); it++) {
        Drawable *d = (*it);
        switch(d->bmode) {
        case BLEND_ADDITIVE:
            // Additive blending, so enable blending and disable alpha testing
            // This shouldn't touch the stencil buffer at all
            glEnable(GL_BLEND);
            glDisable(GL_ALPHA_TEST);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glStencilFunc(GL_EQUAL, 1, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            break;
        case BLEND_ALPHA:
            // Alpha blending. Well, not really blending; we just skip all data where alpha = 0.
            // Set all visible data as 1 on stencil buffer, so that all additive blending effects
            // works on these surfaces.
            glDisable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glStencilFunc(GL_ALWAYS, 1, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            break;
        }

        // Just draw the texture on screen to the right spot.
        glBindTexture(GL_TEXTURE_2D, d->tex->id);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(d->x+d->w, d->y+d->h, 0.0f); // Top Right
            glTexCoord2f(0.0f, 1.0f); glVertex3f(d->x, d->y+d->h, 0.0f); // Top Left
            glTexCoord2f(0.0f, 0.0f); glVertex3f(d->x, d->y, 0.0f); // Bottom Left
            glTexCoord2f(1.0f, 0.0f); glVertex3f(d->x+d->w, d->y, 0.0f); // Bottom Right
        glEnd();
    }
    // Clear old objects. Note: objects are deleted elsewhere.
    objects.clear();

    // Render to screen instead of FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clear stuff
    glDisable(GL_STENCIL_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, this->w, this->h);
    glLoadIdentity();

    // Disable blending & alpha testing
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);

    // Pick texture, select state
    glBindTexture(GL_TEXTURE_2D, fbo_color_tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Enable shader program
    if(this->shaders->get_attached_count() > 0) {
        try {
            this->shaders->use();
            this->shaders->set("textureSize", ShaderProgram::vec2f(NATIVE_W, NATIVE_H));
        } catch(Exception &ex) {
            RETHROW(ex);
        }
    }

    // Call in the quads!
    glCallList(this->fs_quad_id);

    // Stop using shaders.
    this->shaders->use(false);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Flip
    sfApp->Display();
}

string GLRenderer::get_error_string(int errid) {
    switch(errid) {
        case GL_NO_ERROR: return string("GL_NO_ERROR");
        case GL_INVALID_ENUM: return string("GL_INVALID_ENUM");
        case GL_INVALID_VALUE: return string("GL_INVALID_VALUE");
        case GL_INVALID_OPERATION: return string("GL_INVALID_OPERATION");
        case GL_STACK_OVERFLOW: return string("GL_STACK_OVERFLOW");
        case GL_STACK_UNDERFLOW: return string("GL_STACK_UNDERFLOW");
        case GL_OUT_OF_MEMORY: return string("GL_OUT_OF_MEMORY");
    }
    return string("");
}
