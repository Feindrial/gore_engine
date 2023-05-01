#ifndef GORE_SCENE_NODE_H
#define GORE_SCENE_NODE_H

#include <vector>

#include "util/platform.h"

namespace gore
{
    class scene_node
    {
    public:
        scene_node() = default;
        virtual ~scene_node() = default;
        void add_child(scene_node* blck);
    protected:
        scene_node *parent_;
        std::vector<scene_node*> children_;
    private:
        std::vector<char> path_;
    };

    __FORCE_INLINE void scene_node::add_child(scene_node *blck)
    {
        blck->parent_ = this;
        children_.push_back(blck);
    }
}

#endif