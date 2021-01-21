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
#include <SFML/Graphics/Quad.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Renderer.hpp>
#include <utility>

namespace sf
{
////////////////////////////////////////////////////////////
Quad::Quad() :
myIsFlippedY (false)
{

}


////////////////////////////////////////////////////////////
Quad::Quad(const Image& image, const Color& color) :
Drawable (Vector2f(0,0), Vector2f(1,1), 0, color),
myIsFlippedY (false)
{
    SetImage(image);
}


////////////////////////////////////////////////////////////
void Quad::SetImage(const Image& image)
{
    // If there was no valid image before, force adjusting to the new image size
    bool adjustToNewSize = false;
    if (!myImage)
        adjustToNewSize = true;

    // Assign the new image
    myImage = &image;

    // If we want to adjust the size and the new image is valid, we adjust the source rectangle
    if (adjustToNewSize && (image.GetWidth() > 0) && (image.GetHeight() > 0))
    {
        FloatRect coords = myImage->GetTexCoords(IntRect(0, 0, image.GetWidth(), image.GetHeight()));

        float left   = coords.Left;
        float top    = coords.Top;
        float right  = coords.Left + coords.Width;
        float bottom = coords.Top + coords.Height;

        myV[0].UV = Vector2f(left,  top);
        myV[1].UV = Vector2f(right, top);
        myV[2].UV = Vector2f(left,  bottom);
        myV[3].UV = Vector2f(right, bottom);
    }
}


////////////////////////////////////////////////////////////
void Quad::FlipY(bool flipped)
{
    myIsFlippedY = flipped;
}


////////////////////////////////////////////////////////////
void Quad::SetColor(const Color& color)
{
    Drawable::SetColor(color);
}


////////////////////////////////////////////////////////////
void Quad::SetVertexPosition(unsigned int index, const Vector2f& pos)
{
    myV[index].Position = pos;
}


////////////////////////////////////////////////////////////
void Quad::SetVertexColor(unsigned int index, const Color& color)
{
    myV[index].Col = color;
}


////////////////////////////////////////////////////////////
void Quad::SetVertexUVs(unsigned int index, const Vector2f& uv)
{
    myV[index].UV = uv;
}


////////////////////////////////////////////////////////////
const Image* Quad::GetImage() const
{
    return myImage;
}


////////////////////////////////////////////////////////////
const Vector2f& Quad::GetVertexPosition(unsigned int index) const
{
    return myV[index].Position;
}

////////////////////////////////////////////////////////////
const Color& Quad::GetVertexColor(unsigned int index) const
{
    return myV[index].Col;
}

////////////////////////////////////////////////////////////
const Vector2f& Quad::GetVertexUVs(unsigned int index) const
{
    return myV[index].UV;
}


////////////////////////////////////////////////////////////
void Quad::Render(RenderTarget&, Renderer& renderer) const
{
    // Bind the texture
    renderer.SetTexture(myImage);

    renderer.Begin(Renderer::TriangleStrip);
    if (myIsFlippedY)
    {
        for (unsigned int i = 0; i < 4; ++i)
        {
            int j = (i+2) % 4;
            renderer.AddVertex(myV[i].Position.x, myV[i].Position.y, myV[j].UV.x, myV[j].UV.y, myV[j].Col);
        }
    }
    else
    {
        for (unsigned int i = 0; i < 4; ++i)
            renderer.AddVertex(myV[i].Position.x, myV[i].Position.y, myV[i].UV.x, myV[i].UV.y, myV[i].Col);
    }
    renderer.End();
}


////////////////////////////////////////////////////////////
Quad::Vertex::Vertex(const Vector2f& pos, const Vector2f& uv, const Color& color) : Position(pos), Col(color), UV(uv)
{
}

} // namespace sf
