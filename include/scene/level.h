#ifndef GORE_LEVEL_H
#define GORE_LEVEL_H

#include "scene/portal.h"

#include <vector>

namespace gore
{
	class level
	{
	private:
		std::vector<portal> portals_;
	};
}

#endif