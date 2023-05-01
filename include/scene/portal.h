#ifndef GORE_PORTAL_H
#define	GORE_PORTAL_H

#include "scene/area.h"

#include <array>

namespace gore
{
	class portal
	{
	public:
		portal();
		portal(area *area1, area *area2);
	private:
		std::array<area*, 2> areas_; //each portal has two sides therefore has two areas
	};
}

#endif