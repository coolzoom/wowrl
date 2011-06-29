////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Point.hpp>
#include <SFML/Graphics/Renderer.hpp>
#include <utility>

namespace sf
{
////////////////////////////////////////////////////////////
Point::Point()
{

}

////////////////////////////////////////////////////////////
Point::Point(const Vector2f& position, const Color& color) :
Drawable (position, Vector2f(1,1), 0, color)
{
}

////////////////////////////////////////////////////////////
void Point::Render(RenderTarget&, Renderer& renderer) const
{
    // Unbind the texture if any
    renderer.SetTexture(NULL);

    renderer.Begin(Renderer::PointList);
    renderer.AddVertex(0, 0, GetColor());
    renderer.End();
}

} // namespace sf
