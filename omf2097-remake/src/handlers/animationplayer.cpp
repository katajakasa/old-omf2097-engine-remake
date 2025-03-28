#include "animationplayer.h"

#include "handlers/stringengine.h"
#include "video/texture.h"
#include "audio/music.h"
#include "audio/audiosample.h"
#include "audio/sampleloader.h"
#include "handlers/background.h"
#include "handlers/animation.h"
#include "video/glrenderer.h"
#include "tools/log.h"

#include <string>

using namespace std;

// Hack fix stuff, fugly but whatever.
void AnimationPlayer::FrameFix::fix(int anim_no, int frame_no, Animation::Frame *frame) {}

AnimationPlayer::AnimationPlayer(GameObjects gobj, AnimationSource *anisrc, Animation *ani, AnimationPlayer *parent) : StringEngine() {
    pos = ticks = wait = 0;
    this->gobj = gobj;
    this->animation = ani;
    this->parent = parent;
    this->anisrc = anisrc;
    this->intx = this->inty = 0;
    this->intp = this->intn = 0;
    this->fixer = 0;
    this->moveact = 0;
    this->object = 0;
}

AnimationPlayer::~AnimationPlayer() {
    this->incinerate_children();
    this->incinerate_object();
    this->parent = 0;
    this->fixer = 0;
}

void AnimationPlayer::render() {
    // Render this object (if there is anything to render)
    if(this->object != 0) {
        object->tex->validate();
        gobj.renderer->draw_image(this->object);
    }

    // Render children
    list<AnimationPlayer*>::iterator it;
    for(it = this->children.begin(); it != this->children.end(); it++) {
        (*it)->render();
    }
}

void AnimationPlayer::incinerate_object() {
    if(this->object != 0) {
        delete this->object->tex;
        delete this->object;
        this->object = 0;
    }
}

void AnimationPlayer::incinerate_children() {
    list<AnimationPlayer*>::iterator it;
    for(it = this->children.begin(); it != this->children.end(); it++) {
        delete (*it);
        it = this->children.erase(it);
    }
}

int AnimationPlayer::get_anim_no() {
    return animation->anim_no;
}

bool AnimationPlayer::delete_child(int anim_no, AnimationPlayer *caller) {
    list<AnimationPlayer*>::iterator it;
    for(it = this->children.begin(); it != this->children.end(); it++) {
        if((*it) == caller) {
            continue;
        }
        if((*it)->get_anim_no() == anim_no) {
            delete (*it);
            this->children.erase(it);
            return true;
        }
    }

    // Okay, animation not found on this node. Let's try to find it from parent
    if(parent != 0) {
        return parent->delete_child(anim_no, this);
    }

    // No result
    return false;
}

bool AnimationPlayer::create_child(int anim_no, int sintx, int sinty, int sintn, int sintp) {
    Animation* animation = this->anisrc->seek_animation(anim_no);
    if(animation != 0) {
        AnimationPlayer *p = new AnimationPlayer(gobj, anisrc, animation, this);
        p->set_int_xy(sintx, sinty);
        p->set_int_np(sintn, sintp);
        p->set_frame_fixer(this->fixer); // Hack!!!1
        this->children.push_back(p);
        return true;
    }
    return false;
}

bool AnimationPlayer::tick(double elapsed) {
    // Handle moves
    if(this->object != 0 && this->moveact != 0) {
        double tper = this->ticks / this->wait;

    }

    // See if something needs to be done
    if(ticks >= wait) {
        // We are dead, just stop here.
        if(this->pos == -1) {
            cout << "Anim: " << this->get_anim_no() << " has finished!" << endl;
            return false;
        }

        // Parse next part of animation string
        this->pos = parse(animation->anim_string, this->pos);

        // Some debugging info
        LOG("Anim: " << this->get_anim_no() << " parsed: " << this->last_str);

        // Delete last animation frame
        this->incinerate_object();

        // If we have a valid frame
        bool frame_found = animation->has_frame(frame_sel);
        if(frame_found) {
            Animation::Frame *f = animation->frames.at(frame_sel);
            if(this->fixer != 0) {
                this->fixer->fix(this->get_anim_no(), frame_sel, f);
            }

            int initialx = f->x + this->intx;
            int initialy = f->y + this->inty;
            float xpos = -1.0f + 2*(initialx / 320.0f) + 0.005;
            float ypos = -1.0f + 2*(initialy / 200.0f) + 0.002;

            // Draw requested object
            GLRenderer::Drawable *so = new GLRenderer::Drawable();
            so->w = 2 * (f->w / 320.0f);
            so->h = 2 * (f->h / 200.0f);
            so->x = xpos;
            so->y = 0.0f - (so->h + ypos);
            so->bmode = blend_additive ? GLRenderer::BLEND_ADDITIVE : GLRenderer::BLEND_ALPHA;
            so->index = this->intp;
            so->tex = f->getTexture(anisrc->get_palette(0));
            this->object = so;
        }

        // Handle move actions
        if(this->pix != 0xFFFF && !frame_found && this->object) {
            cout << "Setting this->object->x = " << this->pix << endl;
            this->object->x = this->pix;
        }
        if(this->piy != 0xFFFF && !frame_found && this->object) {
            cout << "Setting this->object->y = " << this->piy << endl;
            this->object->y = this->piy;
        }

        if(frame_found && this->object && (this->pix != 0xFFFF || this->piy != 0xFFFF)) {
            if(this->moveact != 0) {
                delete this->moveact;
            }
            float xpos = -1.0f + 2*(this->pix / 320.0f) + 0.005;
            float ypos = -1.0f + 2*(this->piy / 200.0f) + 0.002;
            this->moveact = new MoveAction(
                ((this->pix != 0xFFFF) ? xpos : this->object->x),
                ((this->piy != 0xFFFF) ? ypos : this->object->y)
            );
            cout << "Move to: x = " << ((this->pix != 0xFFFF) ? xpos : this->object->x) << ", y = " <<  ((this->piy != 0xFFFF) ? ypos : this->object->y) << endl;;
        }

        // Add animation if requested by string engine
        if(mcre != -1) {
            LOG("Create new animation: " << mcre << " p: " << mp);
            this->create_child(mcre, mx, my, mn, mp);
        }

        // Delete animation if requested by string engine
        if(mdel != -1) {
            LOG("Delete animation: " << mdel);
            this->delete_child(mdel, this);
        }

        // Play audio sample
        if(sample_id != -1) {
            AudioSample *sample = gobj.sloader->getSample(sample_id+13);
            sample->playRate(sf*-80, sp);
        }

        // Play music if asked
        if(music_id != -1) {
            gobj.music->load(music_id);
            gobj.music->Play();
        }

        // Set position if requested
        if(this->d_set != -1) {
            this->pos = animation->find_str_descriptor_pos(this->d_set);
            LOG("pos: " << this->pos);
        }

        // See to timeouts
        wait = timeout_sel*9;
        ticks = 0;
    }

    // Tick this AnimationPlayer
    this->ticks += elapsed * 1000;

    // Tick children. If child is dead, just kill it here.
    list<AnimationPlayer*>::iterator it;
    for(it = this->children.begin(); it != this->children.end(); it++) {
        if(!(*it)->tick(elapsed)) {
            delete (*it);
            it = this->children.erase(it);
        }
    }

    // Return true for "still have something to do"
    return true;
}
