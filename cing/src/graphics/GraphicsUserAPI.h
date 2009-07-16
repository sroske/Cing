/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

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

#ifndef _GraphicsUserAPI_h_
#define _GraphicsUserAPI_h_

#include "GraphicsPrereqs.h"
#include "MovableText.h"

#include "Color.h"
#include "Image.h"
#include "common/eString.h"
#include "common/CommonConstants.h"

#include <sstream>

namespace Cing
{

/**
 * @brief Graphics related functions that user can call
 */



//-----------------------------------------------------------------------------------
// Color/Settings
//-----------------------------------------------------------------------------------


/* The background() function sets the color used for the background of the window.
   The default background is light gray. In the draw() function, the background color
   is used to clear the display window at the beginning of each frame.
*/

void background( float gray );
void background( float gray, float alpha );
void background( float x, float y, float z );
void background( float x, float y, float z, float a );
void background( int rgb );
void background( int rgb, float alpha );
void background( const Color& theColor  );
void background( const Image& image ); // TODO: not implemented yet!	

void setBackgroundColor( const Color& color );

/* The stroke() function sets the color used to draw lines and borders around shapes. This color is either
   specified in terms of the RGB or HSB color depending on the current colorMode()
   (the default color space is RGB, with each value in the range from 0 to 255).
*/

void stroke( float gray );
void stroke( float gray, float alpha );
void stroke( float x, float y, float z );
void stroke( float x, float y, float z, float a );
void stroke( int rgb );
void stroke( int rgb, float alpha );
void stroke( const Color& color );

/* Changes the way Vision interprets color data. By default, the
   parameters for fill(), stroke(), background(), and color() are defined by values
   between 0 and 255 using the RGB color model. The colorMode() function is used to
   change the numerical range used for specifying colors and to switch color systems.
   For example, calling colorMode(RGB, 1.0) will specify that values are specified
   between 0 and 1. The limits for defining colors are altered by setting the
   parameters range1, range2, range3, and range 4.
*/

void colorMode(GraphicsType mode);
void colorMode(GraphicsType mode, float range);
void colorMode(GraphicsType mode, float range1, float range2, float range3);
void colorMode(GraphicsType mode, float range1, float range2, float range3, float range4);

/* Sets the color used to fill shapes. For example, if you run fill(204, 102, 0),
   all subsequent shapes will be filled with orange. This color is either specified
   in terms of the RGB or HSB color depending on the current colorMode() (the default
   color space is RGB, with each value in the range from 0 to 255).
*/

void fill( int gray );
void fill( int gray, int alpha );
void fill( int value1, int value2, int value3 );
void fill( int value1, int value2, int value3, int alpha );
void fill( const Color& color );
void fill( const Color& color, int alpha );
//void fill(hex)						NO HEX DATA TYPE
//void fill(hex, alpha)					NO HEX DATA TYPE


//-----------------------------------------------------------------------------------
// Shape / 2D Primitives
//-----------------------------------------------------------------------------------

void strokeWeight( int weight );

void noFill		();
void noStroke	();
void smooth		();
void noSmooth	();

void line			( float x1, float y1, float x2, float y2 );
void point		( float x1, float y1 );
void triangle	( float x1, float y1, float x2, float y2, float x3, float y3 );
void rect			( float x1, float y1, float x2, float y2 );
void quad			( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 );
void ellipse	( float x, float y, float width, float height);
void arc			( float x, float y,  float width, float height, float start, float stop );

void rectMode    ( int mode );
void ellipseMode ( int mode );

void pushStyle();
void popStyle ();
//-----------------------------------------------------------------------------------
// Shape / 3D Primitives
//-----------------------------------------------------------------------------------

void line( float x1, float y1, float z1, float x2, float y2, float z2);

void beginShape	();
void beginShape	(GraphicsType operation);
void endShape		();
void endShape		(GraphicsType operation);
void vertex			(float x, float y);
void vertex			(float x, float y, float z);

// Render related functions
void setRenderMode( RenderMode mode );

// Camera 3D related functions
void	useDefault3DCameraControl ( bool useDefault );
void	useMouseCameraControl		  ( bool value );
void	useKeyboardCameraControl	( bool value );

// Debug methods
void	showFps ( bool show );

// Image Create functions
Image createImage( int width, int height, GraphicsType format );
Image loadImage  ( const std::string& name );

// Read and writes to the global pixels variable.( slooooow for now!)
void  loadPixels();
void  updatePixels();

// To control at a global level the 2D/3D Transforms
void pushMatrix();
void popMatrix();
void resetMatrix();
void printMatrix();
void applyMatrix( float m00, float m01, float m02, float m03,
								  float m10, float m11, float m12, float m13,
									float m20, float m21, float m22, float m23,
									float m30, float m31, float m32, float m33 );

void translate( float x, float y );
void translate( float x, float y, float z );

void rotate		(	float x, float y, float z );
void rotate		(	float x );

void rotateX	(	float angle );
void rotateY	(	float angle );
void rotateZ	(	float angle );

void scale		(	float x, float y, float z );
void scale		(	float x, float y );

//Save frames
void save( const String& name );
//-----------------------------------------------------------------------------------
// Typography
//-----------------------------------------------------------------------------------
void text( const String& text, float x, float y );
void text( const String& text, float x, float y, float width, float height );



void text( const std::ostringstream& text, float x, float y );
void text( int	text, float x, float y );
void text( float text, float x, float y );
//void text(data, x, y, z)
//void text(stringdata, x, y, width, height)
//void text(stringdata, x, y, width, height, z)



void textFont		(const Font& font);
void textFont		(const Font& font, int size);
void textAlign		(int halign, int valign = TOP);
void textMode		(TextMode mode);
void textSize		(float size);


} // namespace Cing

#endif // _GraphicsUserAPI_h_