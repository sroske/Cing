/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef _GraphicsUserAPI_h_
#define _GraphicsUserAPI_h_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"
#include "MovableText.h"

#include "Color.h"
#include "Image.h"
#include "common/eString.h"
#include "common/CommonConstants.h"

#include <sstream>

namespace Cing
{

	// forward declarations
	class BaseRenderer2D;

/**
 * @brief Graphics related functions that user can call
 */
	
//-----------------------------------------------------------------------------------
// Render System related
//-----------------------------------------------------------------------------------
void setRenderer2D( BaseRenderer2D* newRenderer2D );


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

/* Changes the way Cing interprets color data. By default, the
   parameters for fill(), stroke(), background(), and color() are defined by values
   between 0 and 255 using the RGB color model. The colorMode() function is used to
   change the numerical range used for specifying colors and to switch color systems.
   For example, calling colorMode(RGB, 1.0) will specify that values are specified
   between 0 and 1. The limits for defining colors are altered by setting the
   parameters range1, range2, range3, and range 4.
*/

void colorMode(GraphicsType mode);

/* Sets the color used to fill shapes. For example, if you run fill(204, 102, 0),
   all subsequent shapes will be filled with orange. This color is either specified
   in terms of the RGB or HSB color depending on the current colorMode() (the default
   color space is RGB, with each value in the range from 0 to 255).
*/

void fill( float gray );
void fill( float gray, float alpha );
void fill( float value1, float value2, float value3 );
void fill( float value1, float value2, float value3, float alpha );
void fill( const Color& color );
//void fill(hex)						NO HEX DATA TYPE
//void fill(hex, alpha)					NO HEX DATA TYPE


//-----------------------------------------------------------------------------------
// Shape / 2D Primitives
//-----------------------------------------------------------------------------------

void strokeWeight	( int weight );

void noFill			();
void noStroke		();
void smooth			();
void noSmooth		();

void line			( int x1, int y1, int x2, int y2 );
void point			( int x, int y );
void triangle		( int x1, int y1, int x2, int y2, int x3, int y3 );
void rect			( int x1, int y1, int width, int height );
void quad			( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );
void ellipse		( int x, int y, int width, int height);
void arc			( int x, int y,  int width, int height, float start, float stop );

void imageMode		( int mode );
void rectMode		( int mode );
void ellipseMode	( int mode );

void pushStyle		();
void popStyle 		();
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
void	enableDefault3DCameraControl( bool value = true );
void	enableMouseCameraControl	( bool value = true );
void	enableKeyboardCameraControl	( bool value = true );

// Debugle methods
void	showFps ( bool show = true  );

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

void rotate		( float angleZRad );

void rotateX	( float angleRad );
void rotateY	( float angleRad );
void rotateZ	( float angleRad );

void scale		( float x, float y, float z );
void scale		( float x, float y );

// Coordinate systems
void applyCoordinateSystemTransform( const GraphicsType& coordSystem );

//Save frames
void saveCurrentFrame( const String& name );

//-----------------------------------------------------------------------------------
// Typography
//-----------------------------------------------------------------------------------
void text( const String& text, float x, float y );
void text( const String& text, float x, float y, float z);
void text( const String& text, float x, float y, float width, float height );
void text( const String& text, float x, float y, float z, float width, float height );



void text( const std::ostringstream& text, float x, float y );
void text( int	text, float x, float y );
void text( float text, float x, float y );
//void text(data, x, y, z)
//void text(stringdata, x, y, width, height)
//void text(stringdata, x, y, width, height, z)


// Text setters
void 	textFont		(Font& font);
void 	textFont		(Font& font, float size);
void 	textAlign		(int halign, int valign = TOP);
void 	textSize		(float size);
void	textResolution	(int resolution);
void	textLeading		(float leading);
void	textWordWrap	(bool wordWrap);

// Text getters
float	textWidth		( const std::string& text );
float	textHeight		( const std::string& text, float textBoxWidth = -1 );

std::vector< std::string > splitInLines( const std::string& text, float textBoxWidth );

//-----------------------------------------------------------------------------------
// Shadows
//-----------------------------------------------------------------------------------
void enableShadows	( ShadowTechnique technique );
void setShadowColor	( const Color& color );

//-----------------------------------------------------------------------------------
// Coordinate System related
//-----------------------------------------------------------------------------------

Vector2			worldToScreen			( const Vector& worldCoordinate );
Vector2			worldToScreenNormalized	( const Vector& worldCoordinate );
Vector			screenToWorld			( const Vector2& screenCoordinate, float distanceToCamera );
Vector			screenToWorld			( const Vector& screenCoordinate, float distanceToCamera );
Ogre::Entity*	pickEntity				( const Vector2& screenCoordinate );



//----------------------------------------------------------------------------------- 
// Helpers
//----------------------------------------------------------------------------------- 
unsigned int		numberOfChannels	( GraphicsType format );
unsigned int		imageBufferSize		( unsigned int width, unsigned int height, GraphicsType format );
GraphicsType		toCingPixelFormat	( Ogre::PixelFormat ogreFormat );
Ogre::PixelFormat	toOgrePixelFormat	( GraphicsType cingformat );


} // namespace Cing

#endif // _GraphicsUserAPI_h_
