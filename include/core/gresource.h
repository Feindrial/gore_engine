#pragma once

namespace gore
{
    class resource_registry;

    class gresource
    {
        friend class resource_registry;
    public:
        typedef int gid;
    private:
        gid gid_;
    };
}