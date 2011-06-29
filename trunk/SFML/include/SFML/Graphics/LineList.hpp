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

#ifndef SFML_LINELIST_HPP
#define SFML_LINELIST_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Resource.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Line.hpp>
#include <vector>


namespace sf
{

////////////////////////////////////////////////////////////
/// \brief Like Sprite, but allows control of the four
///        vertices that composes the PointSet (position, color,
///        texture coordinates).
///
////////////////////////////////////////////////////////////
class SFML_API LineList : public Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty quad with no source image.
    ///
    ////////////////////////////////////////////////////////////
    LineList();

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty quad with no source image.
    ///
    ////////////////////////////////////////////////////////////
    LineList(size_t numpoint);

    ////////////////////////////////////////////////////////////
    /// \brief Add a new line to the list
    ///
    /// \param line The new line
    ///
    ////////////////////////////////////////////////////////////
    void AddLine(const Line& line);

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

    std::vector<Line> myLineList;
};

} // namespace sf


#endif // SFML_POINT_HPP


////////////////////////////////////////////////////////////
/// \class sf::Point
///
/// sf::PointSet is a drawable class that allows to display an
/// image (or a part of it) on a render target just like
/// sf::Sprite, but with increased flexibility, at the cost
/// of ease of use.
///
/// Most functions from sf::Drawable are hidden: position,
/// rotation, scale and origin. Color and blend mode are
/// still available.
///
/// It is important to note that, like sf::Sprite, the
/// sf::PointSet instance doesn't copy the image that it uses,
/// it only keeps a reference to it.
/// Thus, a sf::Image must not be destructed while it is
/// used by a sf::PointSet (i.e. never write a function that
/// uses a local sf::Image instance for creating a quad).
///
/// Usage example:
/// \code
/// // Declare and load an image
/// sf::Image image;
/// image.LoadFromFile("image.png");
///
/// // Create a sprite
/// sf::PointSet quad;
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
