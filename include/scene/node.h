#ifndef GORE_NODE_H
#define GORE_NODE_H

#include "scene/spatial.h"

#include <vector>

namespace gore
{
    class node : public spatial
    {
    public:
        void add_child(spatial *spt);
        void notify_children();
    private:
        spatial parent_;
        std::vector<spatial> children_;
    };

}

#endif