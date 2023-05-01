#ifndef GORE_GLOBJECT_H
#define GORE_GLOBJECT_H

#include "core/platform_macros.h"

namespace gore
{
    namespace gl
    {
        class globject
        {
        public:
            globject() = default;
            globject(unsigned int id) : id_{ id } {}
            __NO_DISCARD unsigned int id() const __NO_EXCEPT;

            friend bool operator==(const globject &obj1, const globject &obj2);
            friend bool operator!=(const globject &obj1, const globject &obj2);
        protected:
            unsigned int id_;
        };

        __NO_DISCARD __FORCE_INLINE unsigned int globject::id() const __NO_EXCEPT
        {
            return id_;
        }

        __FORCE_INLINE bool operator==(const globject &obj1, const globject &obj2)
        {
            return obj1.id_ == obj2.id_;
        }
        __FORCE_INLINE bool operator!=(const globject &obj1, const globject &obj2)
        {
            return !(operator==(obj1, obj2));
        }
        
    }
}

#endif