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

#ifndef SFML_QUAD_HPP
#define SFML_QUAD_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Resource.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>


namespace sf
{
class Image;

////////////////////////////////////////////////////////////
/// \brief Like Sprite, but allows control of the four
///        vertices that composes the Quad (position, color,
///        texture coordinates).
///
////////////////////////////////////////////////////////////
class SFML_API Quad : public Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty quad with no source image.
    ///
    ////////////////////////////////////////////////////////////
    Quad();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the quad from a source image
    ///
    /// \param image    Source image, that the sprite will display
    /// \param color    Global color of the sprite
    ///
    /// \see SetImage
    ///
    ////////////////////////////////////////////////////////////
    explicit Quad(const Image& image, const Color& color = Color(255, 255, 255, 255));

    ////////////////////////////////////////////////////////////
    /// \brief Change the source image of the quad
    ///
    /// The \a image argument refers to an image that must
    /// exist as long as the quad uses it. Indeed, the quad
    /// doesn't store its own copy of the image, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the source image is destroyed and the quad tries to
    /// use it, it may appear as a white rectangle.
    ///
    /// \param image New image
    ///
    /// \see GetImage
    ///
    ////////////////////////////////////////////////////////////
    void SetImage(const Image& image);

    ////////////////////////////////////////////////////////////
    /// \brief Flip the quad vertically
    ///
    /// \param flipped True to flip the sprite
    ///
    /// \see FlipX
    ///
    ////////////////////////////////////////////////////////////
    void FlipY(bool flipped);

    ////////////////////////////////////////////////////////////
    /// \brief Set the global color of the object
    ///
    /// This global color affects the entire object, and modulates
    /// (multiplies) its original pixels.
    /// The default color is white.
    ///
    /// \param color New color
    ///
    /// \see GetColor
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Change the position of a vertex
    ///
    /// The vertex position is relative to the sprite's position.
    ///
    /// \param index Index of the vertex
    /// \param color New color of the vertex
    ///
    ////////////////////////////////////////////////////////////
    void SetVertexPosition(unsigned int index, const Vector2f& pos);

    ////////////////////////////////////////////////////////////
    /// \brief Change the color of a vertex
    ///
    /// \param index Index of the vertex
    /// \param color New color of the vertex
    ///
    ////////////////////////////////////////////////////////////
    void SetVertexColor(unsigned int index, const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Change the uv coordinates of a vertex
    ///
    /// \param index Index of the vertex
    /// \param uv    The UV coordinates of the vertex
    ///
    ////////////////////////////////////////////////////////////
    void SetVertexUVs(unsigned int index, const Vector2f& uv);

    ////////////////////////////////////////////////////////////
    /// \brief Get the source image of the sprite
    ///
    /// If the sprite has no source image, or if the image
    /// doesn't exist anymore, a NULL pointer is returned.
    /// The returned pointer is const, which means that you can't
    /// modify the image when you retrieve it with this function.
    ///
    /// \return Pointer to the sprite's image
    ///
    /// \see SetImage
    ///
    ////////////////////////////////////////////////////////////
    const Image* GetImage() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of a vertex
    ///
    /// The vertex position on the screen is absolute and
    /// doesn't depend on the actual quad's position.
    ///
    /// \param index Index of the vertex
    ///
    /// \return Position of the vertex
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetVertexPosition(unsigned int index) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the color of a vertex
    ///
    /// \param index Index of the vertex
    ///
    /// \return Color of the vertex
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetVertexColor(unsigned int index) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the uv coordinates of a vertex
    ///
    /// \param index Index of the vertex
    ///
    /// \return UV coordiantes of the vertex
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetVertexUVs(unsigned int index) const;

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Draw the object to a render target
    ///
    /// \param target   Render target
    /// \param renderer Renderer providing low-level rendering commands
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& target, Renderer& renderer) const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    struct Vertex
    {
        Vertex(const Vector2f& pos = Vector2f(0, 0), const Vector2f& uv = Vector2f(0, 0), const Color& color = Color(255, 255, 255));

        Vector2f Position;
        Color    Col;
        Vector2f UV;
    };

    ResourcePtr<Image> myImage;      ///< Image used to draw the quad
    Vertex             myV[4];       ///< Array of vertices
    bool               myIsFlippedY; ///< Is the quad flipped on the Y axis ?
};

} // namespace sf


#endif // SFML_QUAD_HPP


////////////////////////////////////////////////////////////
/// \class sf::Quad
///
/// sf::Quad is a drawable class that allows to display an
/// image (or a part of it) on a render target just like
/// sf::Sprite, but with increased flexibility, at the cost
/// of ease of use.
///
/// Most functions from sf::Drawable are hidden: position,
/// rotation, scale and origin. Color and blend mode are
/// still available.
///
/// It is important to note that, like sf::Sprite, the
/// sf::Quad instance doesn't copy the image that it uses,
/// it only keeps a reference to it.
/// Thus, a sf::Image must not be destructed while it is
/// used by a sf::Quad (i.e. never write a function that
/// uses a local sf::Image instance for creating a quad).
///
/// Usage example:
/// \code
/// // Declare and load an image
/// sf::Image image;
/// image.LoadFromFile("image.png");
///
/// // Create a sprite
/// sf::Quad quad;
/// quad.SetImage(image);
/// quad.SetVertexPosition(0, sf::Vector2f(0, 0));
/// quad.SetVertexPosition(1, sf::Vector2f(16,0));
/// quad.SetVertexPosition(2, sf::Vector2f(0, 16));
/// quad.SetVertexPosition(3, sf::Vector2f(16,16));
///
/// // Display it
/// window.Draw(sprite); // window is a sf::RenderWindow
/// \endcode
///
/// \see sf::Image
///
////////////////////////////////////////////////////////////
