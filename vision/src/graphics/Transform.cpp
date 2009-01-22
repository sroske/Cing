/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Transform.h"

namespace Graphics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Transform::Transform() :	m_bIsValid( false )
{

};


/**
* @internal
* @brief Initializes the class so it becomes valid.
*
* @return true if the initialization was ok | false otherwise
*/
bool Transform::init()
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void Transform::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	// The class is not valid anymore
	m_bIsValid = false;
}

// Simple transformations methods
void Transform::translate(  float x, float y, float z )
{
	m_position += Vector( x, y, z );
}
void Transform::rotate(  float x, float y, float z )
{
	m_rotation += Vector( x, y, z );
}
void Transform::scale(  float x, float y, float z )
{
	m_scale += Vector( x, y, z );
}

} // namespace Graphics