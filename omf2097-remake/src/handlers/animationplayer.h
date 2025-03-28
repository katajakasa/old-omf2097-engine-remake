#ifndef ANIMATIONPLAYER_H
#define ANIMATIONPLAYER_H

#include <list>
#include <vector>

#include "handlers/stringengine.h"
#include "handlers/animationsource.h"
#include "handlers/animation.h"
#include "video/glrenderer.h"
#include "video/shader.h"

class Texture;
class Background;
class Music;
class SampleLoader;

class AnimationPlayer : public StringEngine {
public:
    class GameObjects {
    public:
        GLRenderer *renderer;
        Music *music;
        SampleLoader *sloader;
    };

    class FrameFix {
    public:
        virtual void fix(int anim_no, int frame_no, Animation::Frame *frame);
    };

    class MoveAction {
    public:
        int x,y;
        MoveAction(int x, int y) { this->x = x; this->y = y; }
        bool operator==(const MoveAction &b) const { return (x == b.x && y == b.y); }
        bool operator!=(const MoveAction &b) const { return (x != b.x || y != b.y); }
        bool operator>=(const MoveAction &b) const { return (x >= b.x && y >= b.y); }
        bool operator>(const MoveAction &b) const  { return (x > b.x && y > b.y);   }
        int diffx(const MoveAction &b) { return x - b.x; }
        int diffy(const MoveAction &b) { return y - b.y; }
    };

    /* Entity coloring & lighting */
    class EntityCnL {
    protected:


    public:
        int bpd, bps, bpn, bpb, tint;

    };

private:
    int intx, inty, intn, intp;

    // Animation related stuff
    GLRenderer::Drawable *object;
    Animation *animation;
    AnimationSource *anisrc;

    // Hack
    FrameFix *fixer;

    // Parent player, children players
    AnimationPlayer *parent;
    std::list<AnimationPlayer*> children;

    // Contains stuff from outside the scene
    GameObjects gobj;

    // String engine position + ticks counting
    int pos;
    double ticks, wait;

    // Animation frame moves
    MoveAction *moveact;

    // Some internal convenience functions
    void incinerate_object();
    void incinerate_children();
    int get_anim_no();
    bool create_child(int anim_no, int sintx, int sinty, int sintn, int sintp);

public:
    bool delete_child(int anim_no, AnimationPlayer *caller);
    void set_int_xy(int x, int y) { this->intx = x; this->inty = y; }
    void set_int_np(int n, int p) { this->intn = n; this->intp = p; }

    AnimationPlayer(GameObjects gobj, AnimationSource *anisrc, Animation *ani, AnimationPlayer *parent = 0);
    ~AnimationPlayer();

    bool tick(double elapsed);
    void render();

    void reset() { pos = ticks = wait = 0; }
    void set_frame_fixer(FrameFix *fixer) { this->fixer = fixer; }
};

#endif // ANIMATIONPLAYER_H
