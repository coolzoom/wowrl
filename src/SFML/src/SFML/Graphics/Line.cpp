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
#include <SFML/Graphics/Line.hpp>
#include <SFML/Graphics/Renderer.hpp>
#include <utility>

namespace sf
{
////////////////////////////////////////////////////////////
Line::Line()
{

}


////////////////////////////////////////////////////////////
Line::Line(const Vector2f& p1, const Vector2f& p2, const Color& color) :
Drawable (Vector2f(0,0), Vector2f(1,1), 0, color),
myP1(p1),
myP2(p2)
{
}


////////////////////////////////////////////////////////////
Line::Line(const Vector2f& p1, const Color& c1, const Vector2f& p2, const Color& c2) :
Drawable (Vector2f(0,0), Vector2f(1,1), 0, Color::White),
myP1(p1),
myP2(p2),
myC1(c1),
myC2(c2)
{
}


////////////////////////////////////////////////////////////
void Line::SetColor(const Color& color)
{
    Drawable::SetColor(color);
}


////////////////////////////////////////////////////////////
void Line::SetStartPosition(const Vector2f& pos)
{
    myP1 = pos;
}

////////////////////////////////////////////////////////////
void Line::SetEndPosition(const Vector2f& pos)
{
    myP2 = pos;
}


////////////////////////////////////////////////////////////
void Line::SetStartColor(const Color& color)
{
    myC1 = color;
}

////////////////////////////////////////////////////////////
void Line::SetEndColor(const Color& color)
{
    myC2 = color;
}


////////////////////////////////////////////////////////////
const Vector2f& Line::GetStartPosition() const
{
    return myP1;
}

////////////////////////////////////////////////////////////
const Vector2f& Line::GetEndPosition() const
{
    return myP2;
}

////////////////////////////////////////////////////////////
const Color& Line::GetStartColor() const
{
    return myC1;
}

////////////////////////////////////////////////////////////
const Color& Line::GetEndColor() const
{
    return myC1;
}


////////////////////////////////////////////////////////////
void Line::Render(RenderTarget&, Renderer& renderer) const
{
    // Unbind the texture
    renderer.SetTexture(NULL);

    renderer.Begin(Renderer::LineList);

    renderer.AddVertex(myP1.x, myP1.y, myC1);
    renderer.AddVertex(myP2.x, myP2.y, myC2);

    renderer.End();
}

} // namespace sf
