/*
	Let There Be Light
	Copyright (C) 2012 Eric Laukien

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
		claim that you wrote the original software. If you use this software
		in a product, an acknowledgment in the product documentation would be
		appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
		misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
*/

#ifndef LTBL_SHADOWFIN_H
#define LTBL_SHADOWFIN_H

#include <SFML/OpenGL.hpp>
#include <lighting/Constructs/Vec2f.h>

namespace ltbl
{
	class ShadowFin
	{
	public:
		Vec2f m_rootPos;
		Vec2f m_umbra;
		Vec2f m_penumbra;

		float m_umbraBrightness;
		float m_penumbraBrightness;

		ShadowFin();
		~ShadowFin();

		void Render(float transparency);
	};
}

#endif