#pragma once

#include <vector>
#include <string>

namespace sf {
    class Window;
};
class Texture;
class GLExtLoader;
class Shader;
class ShaderProgram;

class GLRenderer {
public:
    enum BlendMode {
        BLEND_ADDITIVE = 0,
        BLEND_ALPHA
    };

    enum FilterMode {
        FILTER_NONE,
        FILTER_HQX
    };

    class Drawable {
    public:
        BlendMode bmode;
        float w,h,x,y;
        Texture *tex;
        int index;
    };

private:
    int w,h;
    Texture *background_tex;
    sf::Window *sfApp;
    std::vector<Drawable*> objects;
    GLExtLoader *extloader;

    // All things related to FBO
    unsigned int fbo_id;
    unsigned int fbo_color_tex_id;
    unsigned int fbo_depth_rbo_id;
    void create_offscreen_framebuffer();
    void delete_offscreen_framebuffer();

    // A nice fullscreen quad
    unsigned int fs_quad_id;

    // Shader stuff
    ShaderProgram *shaders;

public:
    GLRenderer(sf::Window *sfApp,
               int w, int h, bool fs,
               bool vsync = true,
               int fpslimit = 60,
               FilterMode filter = FILTER_HQX);
    ~GLRenderer();

    static std::string get_error_string(int errid);

    void draw_image(Drawable *drawable);
    void set_background(Texture *tex);
    void prepare();
    void finish();
    void set_window_title(std::string title);

    int getW() { return this->w; }
    int getH() { return this->h; }
};
