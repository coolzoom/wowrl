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
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Renderer.hpp>
#include <utility>

namespace sf
{
////////////////////////////////////////////////////////////
Sprite::Sprite() :
mySubRect    (0, 0, 1, 1),
myIsFlippedX (false),
myIsFlippedY (false),
myUseVertices(false)
{

}


////////////////////////////////////////////////////////////
Sprite::Sprite(const Image& image, const Vector2f& position, const Vector2f& scale, float rotation, const Color& color) :
Drawable     (position, scale, rotation, color),
mySubRect    (0, 0, 1, 1),
myIsFlippedX (false),
myIsFlippedY (false),
myUseVertices(false)
{
    SetImage(image);
}


////////////////////////////////////////////////////////////
void Sprite::SetImage(const Image& image, bool adjustToNewSize)
{
    // If there was no valid image before, force adjusting to the new image size
    if (!myImage)
        adjustToNewSize = true;

    // Assign the new image
    myImage = &image;

    // If we want to adjust the size and the new image is valid, we adjust the source rectangle
    if (adjustToNewSize && (image.GetWidth() > 0) && (image.GetHeight() > 0))
    {
        SetSubRect(IntRect(0, 0, image.GetWidth(), image.GetHeight()));
    }
}


////////////////////////////////////////////////////////////
void Sprite::SetColor(const Color& color)
{
    Drawable::SetColor(color);
    for (unsigned int i = 0; i < 4; ++i)
        myV[i].Col = color;
}


////////////////////////////////////////////////////////////
void Sprite::SetSubRect(const IntRect& rectangle)
{
    mySubRect = rectangle;

    FloatRect coords;
    if (myImage)
        coords = myImage->GetTexCoords(mySubRect);

    float left   = coords.Left;
    float top    = coords.Top;
    float right  = coords.Left + coords.Width;
    float bottom = coords.Top + coords.Height;

    myV[0].UV = Vector2f(left,  top);
    myV[1].UV = Vector2f(right, top);
    myV[2].UV = Vector2f(left,  bottom);
    myV[3].UV = Vector2f(right, bottom);
}


////////////////////////////////////////////////////////////
void Sprite::Resize(float width, float height)
{
    if ((mySubRect.Width > 0) && (mySubRect.Height > 0))
    {
        SetScale(width / mySubRect.Width, height / mySubRect.Height);
        myUseVertices = false;
    }
}

////////////////////////////////////////////////////////////
void Sprite::Resize(const Vector2f& size)
{
    Resize(size.x, size.y);
    myUseVertices = false;
}


////////////////////////////////////////////////////////////
void Sprite::SetVertexPosition(unsigned int index, const Vector2f& pos)
{
    myV[index].Position = pos;
    myUseVertices = true;
}


////////////////////////////////////////////////////////////
void Sprite::SetVertexColor(unsigned int index, const Color& color)
{
    myV[index].Col = color;
    myUseVertices = true;
}


////////////////////////////////////////////////////////////
void Sprite::SetVertexUVs(unsigned int index, const Vector2f& uv)
{
    myV[index].UV = uv;
    myUseVertices = true;
}


////////////////////////////////////////////////////////////
void Sprite::FlipX(bool flipped)
{
    myIsFlippedX = flipped;
}


////////////////////////////////////////////////////////////
void Sprite::FlipY(bool flipped)
{
    myIsFlippedY = flipped;
}


////////////////////////////////////////////////////////////
const Image* Sprite::GetImage() const
{
    return myImage;
}


////////////////////////////////////////////////////////////
const IntRect& Sprite::GetSubRect() const
{
    return mySubRect;
}


////////////////////////////////////////////////////////////
Vector2f Sprite::GetSize() const
{
    return Vector2f(mySubRect.Width * GetScale().x, mySubRect.Height * GetScale().y);
}


////////////////////////////////////////////////////////////
const Vector2f& Sprite::GetVertexPosition(unsigned int index) const
{
    return myV[index].Position;
}

////////////////////////////////////////////////////////////
const Color& Sprite::GetVertexColor(unsigned int index) const
{
    return myV[index].Col;
}

////////////////////////////////////////////////////////////
const Vector2f& Sprite::GetVertexUVs(unsigned int index) const
{
    return myV[index].UV;
}


////////////////////////////////////////////////////////////
Color Sprite::GetPixel(unsigned int x, unsigned int y) const
{
    if (myImage)
    {
        unsigned int imageX = mySubRect.Left + x;
        unsigned int imageY = mySubRect.Top  + y;

        if (myIsFlippedX) imageX = mySubRect.Width  - imageX - 1;
        if (myIsFlippedY) imageY = mySubRect.Height - imageY - 1;

        return myImage->GetPixel(imageX, imageY) * GetColor();
    }
    else
    {
        return GetColor();
    }
}


////////////////////////////////////////////////////////////
void Sprite::Render(RenderTarget&, Renderer& renderer) const
{
    // Get the sprite size
    float width  = static_cast<float>(mySubRect.Width);
    float height = static_cast<float>(mySubRect.Height);

    // Check if the image is valid, and calculate the texture coordinates
    FloatRect coords;
    if (myImage)
        coords = myImage->GetTexCoords(mySubRect);

    // Compute the texture coordinates
    float left   = coords.Left;
    float top    = coords.Top;
    float right  = coords.Left + coords.Width;
    float bottom = coords.Top + coords.Height;
    if (myIsFlippedX) std::swap(left, right);
    if (myIsFlippedY) std::swap(top, bottom);

    // Bind the texture
    renderer.SetTexture(myImage);

    // Draw the sprite's geometry
    if (myUseVertices)
    {
        renderer.Begin(Renderer::TriangleStrip);
        for (unsigned int i = 0; i < 4; ++i)
            renderer.AddVertex(myV[i].Position.x, myV[i].Position.y, myV[i].UV.x, myV[i].UV.y, myV[i].Col);
        renderer.End();
    }
    else
    {
        renderer.Begin(Renderer::TriangleStrip);
            renderer.AddVertex(0,     0,      left,  top);
            renderer.AddVertex(width, 0,      right, top);
            renderer.AddVertex(0,     height, left,  bottom);
            renderer.AddVertex(width, height, right, bottom);
        renderer.End();
    }
}


////////////////////////////////////////////////////////////
Sprite::Vertex::Vertex(const Vector2f& pos, const Vector2f& uv, const Color& color) : Position(pos), Col(color), UV(uv)
{
}

} // namespace sf
