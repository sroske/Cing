/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

  Copyright (c) 2008 XXX

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
#include "Image.h"

// Common
#include "common/Exception.h"
#include "common/ResourceManager.h"
#include "common/LogManager.h"

namespace Graphics
{

// Static member init
ImageThresholdFilter	Image::m_imgThresholdFilter;
ImageDifferenceFilter	Image::m_imgDiffFilter;

/**
 * @brief Constructor. This constructor does not load or creates any image.
 */
Image::Image():
	m_cvImage(NULL),
  m_bIsValid( false ),
	m_bUpdateTexture( false )
{
}

/**
 * @brief Creates an image with the same data as the received image
 *
 * @param
 */
Image::Image( const Image& img ):
	m_cvImage(NULL),
	m_bUpdateTexture( false )
{
	init( img );
}

/**
 * @brief Creates an image of the specified size, but does not load it from file.
 * Therefore, the image created is empty. However, this image can be modified afterwards.
 *
 * @param width  Width of the image to be created
 * @param height Height of the image to be created
 * @param format Format of the image to be created. Possible formats are: RGB, ARGB, GRAYSCALE
 */
Image::Image( int width, int height, ImageFormat format /*= RGB*/ ):
	m_cvImage(NULL),
	m_bUpdateTexture( false )
{
	init( width, height, format );
}

/**
 * @brief Creates an image from a file. The image loaded can be modified afterwards.
 * @note The image file specified should be placed on the data directory of the application
 *
 * @param name  Name of the file to be loaded. It must be placed in the data directory.
 */
Image::Image( const std::string& name ):
	m_cvImage(NULL)
{
	load( name );
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Image::~Image()
{
  // Release resources
  end();
}

/**
 * @brief Creates an image of the specified size, but does not load it from file.
 * Therefore, the image created is empty. However, this image can be modified afterwards.
 *
 * @param width  Width of the image to be created
 * @param height Height of the image to be created
 * @param format Format of the image to be created. Possible formats are: RGB, RGBA, GRAYSCALE
 */
void Image::init( int width, int height, ImageFormat format /*= RGB*/  )
{
  // Check if the class is already initialized
  if ( isValid() )
   THROW_EXCEPTION( "Image already initialized" );

	// Create the empty IplImage image
	int channels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
	m_cvImage    = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,channels);
		
	// Create the texture quad (to draw image)
	m_quad.init( m_cvImage->width, m_cvImage->height, format );

	// Load image data to texture
	updateTexture();

	// The class is now initialized
	m_bIsValid = true;
	m_bUpdateTexture = true;
}

/**
 * @brief Creates a copy of an image
 *
 * @param img Image to be copied
 */
void Image::init( const Image& img )
{
	this->operator =( img );

	m_bIsValid = true;
	m_bUpdateTexture = false;
}

/**
 * @brief Creates an image from a file. The image loaded can be modified afterwards.
 * 
 * @note The image file specified should be placed on the data directory of the application
 *
 * @note Supported image formats are: .bmp, .jpg, .gif, .raw, .png, .tga and .dds.
 *
 * @param name  Name of the file to be loaded. It must be placed in the data directory.
 */
void Image::load( const std::string& name  )
{
	// Load file from disk
	m_image.load( name, Common::ResourceManager::userResourcesGroupName );
	
	//Copy pixels from the OgreImage to the IplImage	
	if ( m_cvImage != NULL ) 
		cvReleaseImage( &m_cvImage );

	int channels = (int)Ogre::PixelUtil::getNumElemBytes( m_image.getFormat() );
	m_cvImage = cvCreateImage(cvSize((int)m_image.getWidth(),(int)m_image.getHeight()),IPL_DEPTH_8U,channels);
	m_cvImage->imageData = (char *)m_image.getData();

	// Create the texture quad (to draw image)
	m_quad.init( (int)m_image.getWidth(), (int)m_image.getWidth(), (ImageFormat)m_image.getFormat() );
  
	// Load image data to texture
	updateTexture();

	// The class is now initialized
	m_bIsValid = true;
	m_bUpdateTexture = false;
}

/**
 * @brief Saves the image to a file
 *
 * @note The type of image file that will be created depends on the extension passed
 * For example, if you call myImage.save( "myFile.jpg" ), the image will be saved to a jpeg file
 * with the name myFile.jpg
 *
 * @note The file will be placed in the application data folder
 *
 * @note Supported image formats are: .bmp, .jpg, .gif, .raw, .png, .tga and .dds.
 * @param name Name that will have the image file
 */
void Image::save( const std::string& name )
{
	// TODO: Pass data from IplImage to m_image to save the data

	// Add the user app data folder to the name
	m_image.save( Common::ResourceManager::userDataPath + name );
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void Image::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	m_quad.end();

	//Release IplImage
	// TODO check why this crashes.. 
	//cvReleaseImage(&m_cvImage);
	m_cvImage = NULL;

	// The class is not valid anymore
	m_bIsValid = false;

}

/**
 * @brief Sets the data of the image 
 *
 * @param imageData Data to set to the image
 * @param width			Width of the passed image data
 * @param height		Height of the passed image data
 * @param format		format Format of the image passed
 */
void Image::setData( char* imageData, int width, int height, ImageFormat format )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to set data to an invalid image (it has not been initialized)" );
		return;
	}

	// Check dimensions
	int channels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
	if ( (width != m_cvImage->width) || (height != m_cvImage->height) || (channels != m_cvImage->nChannels) )
	{
		LOG_ERROR( "Trying to set data with a wrong size of number of channels" );
		return;
	}

	// Set the data
	cvSetData( m_cvImage, imageData, m_cvImage->widthStep );

	// Make the image to be updated to texture in the next draw
	m_bUpdateTexture = true;
}


/**
 * @brief Returns image width 
 * @return the height of the image
 */
int Image::getWidth() const
{
	return (int)m_cvImage->width;
}

/**
 * @brief Returns image height
 * @return the height of the image
 */
int Image::getHeight() const
{
	return (int)m_cvImage->height;
}

/**
 * @brief Returns image format 
 * @return the imageFomat of the image
 */
ImageFormat Image::getFormat() const
{
	int channels = m_cvImage->nChannels;
	switch(channels)
	{
	case 1: return (ImageFormat)GRAYSCALE;
		break;
	case 3: return (ImageFormat)RGB;
		break;
	case 4: return (ImageFormat)RGBA;
		break;
	default: THROW_EXCEPTION( "Error in ImageFormat" );
	}
}

/**
 * @brief Get texture update state
 * @return the m_bUpdateTexture attribute
 */
bool Image::getUpdateTexture() const
{
		return m_bUpdateTexture;
}
/**
 * @brief Set if texture updates automatically every frame
 */
void	Image::setUpdateTexture(bool updateTextureFlag ) 
{
	m_bUpdateTexture = updateTextureFlag;
}

/**
 * @brief Draws the image in a specific position
 *
 * @param xPos x coordinate where the image should be drawn
 * @param yPos y coordinate where the image should be drawn
 * @param zPos z coordinate where the image should be drawn. If not passed, z will be set to zero
 */
void Image::draw( int xPos, int yPos, int zPos /*= 0*/ )
{
	if (m_bUpdateTexture)
	{
		updateTexture();
		m_bUpdateTexture = false;
	}

	m_quad.setPosition( xPos, yPos, zPos );
	m_quad.setVisible( true );
}

/**
 * @brief Copies the received image into the current image
 *
 * @param other Image to copy
 */
void Image::operator=( const Image& other )
{
	// Check the other image is valid
	if ( !other.isValid() )
		THROW_EXCEPTION( "Trying to copy an invalid image" );

	// TODO: Check speed of cvCloneImage, maybe is faster to use memcpy?
	// and solve a bug when the loaded image has different size.
	if ( m_cvImage )
		cvReleaseImage( &m_cvImage );

	m_cvImage = cvCloneImage(other.m_cvImage);
	m_quad    = other.m_quad;

	// Load image data to texture
	updateTexture();

	// Now this image is valid
	m_bIsValid = true;
}

/**
 * @brief  Duplicate an image, returns new Image object.
 *
 * @param other Image to copy
 */
Image* Image::clone()
{
	// Check the other image is valid
	if ( !isValid() )	
		THROW_EXCEPTION( "Trying to copy an invalid image" );

	return new Image( *this );
}

/**
 * @brief  Load image data to texture
 */
void Image::updateTexture()
{
	m_quad.updateTexture(	m_cvImage->imageData,
												m_cvImage->width,
												m_cvImage->height,
												getFormat());
}

/**
 * @brief Draws a triangle inside an image
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, second point
 * @param y2 y, second point
 * @param x3 x, third point
 * @param y3 y, third point
 */
void Image::triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();

	cvLine( m_cvImage,cvPoint(x1,y1),cvPoint(x2,y2),CV_RGB(color.r,color.g,color.b),
					strokeWeight,8,0); 
	cvLine( m_cvImage,cvPoint(x2,y2),cvPoint(x3,y3),CV_RGB(color.r,color.g,color.b),
		strokeWeight,8,0);
	cvLine( m_cvImage,cvPoint(x3,y3),cvPoint(x1,y1),CV_RGB(color.r,color.g,color.b),
		strokeWeight,8,0);

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws a line inside an image
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, end point
 * @param y2 y, end point
 */
void Image::line( float x1, float y1, float x2, float y2 )
{

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();

	// Draw a line
	cvLine(m_cvImage,
							cvPoint(x1,y1),
							cvPoint(x2,y2),
							CV_RGB(color.r,color.g,color.b),
							strokeWeight,	///< Thickness.
							8,	///< Type of the ellipse boundary, see cvLine description.
							0); ///< Number of fractional bits in the center coordinates and axes' values.

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws an arc inside an image. Arcs are drawn along the outer edge of an ellipse defined by the x, y,
 *				 width and height parameters. The start and stop parameters specify the angles at which to draw the arc.
 *
 * @param x x, first point
 * @param y y, first point
 * @param width  width
 * @param height height
 */
void Image::arc( float x, float y, float width, float height, float start, float end )
{

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();

	cvEllipse(	m_cvImage,							///-> Image.
							cvPoint(x,y),						///-> Center of the ellipse.
							cvSize(width,height),		///-> Length of the ellipse axes. 
							0,											///->	Rotation angle.
							start,											///-> Starting angle of the elliptic arc.
							end,										///-> Ending angle of the elliptic arc.
							CV_RGB(color.r,color.g,color.b),///-> Ellipse color.
							strokeWeight );										///-> Thickness of the ellipse arc.

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}
/**
 * @brief Draws a point ( one pixel )
 *
 * @param x x, point
 * @param y y, point
 */
void Image::point( float x, float y )
{

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();
	
	// Draw a pixel
	cvRectangle( m_cvImage,
							 cvPoint(x,y),
							 cvPoint(x,y),
							 CV_RGB(color.r,color.g,color.b),
							 strokeWeight);		///-> Thickness.

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws a quad, defined by four points
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, second point
 * @param y2 y, second point
 * @param x3 x, third point
 * @param y3 y, third point
 * @param x4 x, fourth point
 * @param y4 y, fourth point
 */
void Image::quad( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 )
{

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();

	// Draw a quad	
	int count = 4;
	CvPoint pt[4];

	pt[0] = cvPoint(x1,y1);
	pt[1] = cvPoint(x2,y2);
	pt[2] = cvPoint(x3,y3);
	pt[3] = cvPoint(x4,y4);

	CvPoint* rect = pt;

	//TODO: Check this, error if use pt variable
	cvPolyLine( m_cvImage,
							(CvPoint**)&rect,
							&count,
							1,
							1,
							CV_RGB(color.r,color.g,color.b),
							strokeWeight,
							CV_AA,
							0);

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}
/**
 * @brief Draws a text inside an image
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param text  string
 */
void Image::text( float x1, float y1,  const char* text )
{

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();

	cvPutText(m_cvImage, text, cvPoint(x1,y1), &(graphManager.getCvFont()), CV_RGB(color.r,color.g,color.b));

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws a rectangle inside an image
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, end point
 * @param y2 y, end point
 */
void Image::rect( float x1, float y1, float x2, float y2 )
{

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();

	// Draw a rectangle
	cvRectangle(m_cvImage,
							cvPoint(x1,y1),
							cvPoint(x2,y2),
							CV_RGB(color.r,color.g,color.b),
							strokeWeight);		///-> Thickness.
	
	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws a ellipse inside an image
 *
 * @param x x, first point
 * @param y y, first point
 * @param width  width
 * @param height height
 */
void Image::ellipse( float x, float y, float width, float height )
{

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();

	cvEllipse(	m_cvImage,							///-> Image.
							cvPoint(x,y),						///-> Center of the ellipse.
							cvSize(width,height),		///-> Length of the ellipse axes. 
							0,											///->	Rotation angle.
							0,											///-> Starting angle of the elliptic arc.
							360,										///-> Ending angle of the elliptic arc.
							CV_RGB(color.r,color.g,color.b),///-> Ellipse color.
							strokeWeight );										///-> Thickness of the ellipse arc.
	
	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Method to apply a variety of basic filters to this image.
 *
 * @param ImageProcessingFilters type
 */
void Image::filter( ImageProcessingFilters type )
{
	m_imgThresholdFilter.apply(  *m_cvImage );
}
} // namespace Graphics