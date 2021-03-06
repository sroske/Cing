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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "BaseLight.h"
#include "graphics/GraphicsManager.h"

// Ogre
#include "OgreStringConverter.h"
#include "OgreSceneManager.h"
#include "OgreBillboardSet.h"
#include "OgreBillboard.h"

// Common
#include "common/Exception.h"

// Framework
#include "framework/Application.h"

namespace Cing
{

	// Static member initialization
	const std::string  BaseLight::DEFAUTL_LIGHT_NAME  = "Light";
	long               BaseLight::m_lightCounter      = 0;

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	BaseLight::BaseLight():
	m_pLight    		( NULL  ),
		m_lightFlare		( NULL  ),
		m_lightFlareSet		( NULL  ),
		m_bIsValid			( false )
	{
	}

	/**
	* @internal
	* @brief Destructor. Class release.
	*/
	BaseLight::~BaseLight()
	{
		// Release resources
		end();
	}

	/**
	* @internal
	* @brief Creates the light and initializes its values
	*
	* @param[in] r Red component of the diffuse color of the light.    Value Range: 0..255
	* @param[in] g Green component of the diffuse color of the light.  Value Range: 0..255
	* @param[in] b Blue component of the diffuse color of the light.   Value Range: 0..255
	* @param[in] x x coordinate (horizontal axis) of the light in the scene
	* @param[in] y y coordinate (vertical axis) of the light in the scene
	* @param[in] z z coordinate (depth) of the light in the scene
	* @return true if the initialization was ok | false otherwise
	*/
	bool BaseLight::init( float r, float g, float b, float x, float y, float z )
	{
		// Check if the class is already initialized
		if ( isValid() )
			return true;

		// Check application correctly initialized (could not be if the user didn't calle size function)
		Application::getSingleton().checkSubsystemsInit();

		// Get the scene manager
		Ogre::SceneManager& scenaManager = GraphicsManager::getSingleton().getSceneManager();

		// Create the light (and its unique name)
		m_lightName = DEFAUTL_LIGHT_NAME + Ogre::StringConverter::toString( ++m_lightCounter );
		m_pLight = scenaManager.createLight( m_lightName );

		// Create a scene node for the light (to have a more convenient way of controlling it in the space)
		m_sceneNode = scenaManager.getRootSceneNode()->createChildSceneNode();
		m_sceneNode->attachObject( m_pLight );

		// Create billboard for debugging purposes
		m_lightFlareSet = GraphicsManager::getSingleton().getSceneManager().createBillboardSet( m_lightName + "_billboard", 1 );
		m_lightFlareSet->setMaterialName( "Examples/Flare" );
		m_lightFlare = m_lightFlareSet->createBillboard( Vector( 0, 0, 0 ) );
		m_lightFlare->setColour( Color( r, g, b, 255 ).normalized() );
		m_lightFlare->setDimensions(10, 10);
		m_lightFlareSet->setVisible( false );

		// Attach billboard to light scene node
		m_sceneNode->attachObject( m_lightFlareSet );

		// Set light position and diffuse color
		m_sceneNode->setPosition( x, y, z );
		m_pLight->setDiffuseColour( Color(r, g, b).normalized() );

		// Store initial colors
		m_diffuseColor.r = r;
		m_diffuseColor.g = g;
		m_diffuseColor.b = b;
		m_specularColor.r = 255; 
		m_specularColor.g = 255; 
		m_specularColor.b = 255; 

		// The class is now initialized
		m_bIsValid = true;

		return true;
	}

	/**
	* @internal
	* @brief Releases the class resources. 
	* After this method is called the class is not valid anymore.
	*/
	void BaseLight::end()
	{
		// Check if the class is already released
		if ( !isValid() )
			return;

		// Destroy the light and the scene node
		Ogre::SceneManager* sceneManager = GraphicsManager::getSingleton().getSceneManagerPtr();
		if ( sceneManager )
		{
			sceneManager->destroyLight( m_lightName );
			sceneManager->destroyBillboardSet( m_lightFlareSet );
		}

		// Just reset billboard pointers (ogre scene manager is suposed to delete them)
		m_lightFlareSet = NULL;
		m_lightFlare	= NULL;

		// The class is not valid anymore
		m_bIsValid = false;
	}

	/**
	 * @brief Enagles/disables the light
	 * @param[in] enable true will enable the light, false will disable it
	 */
	void BaseLight::enable( bool enable )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set direction in a Light not correctly initialized" );

		m_pLight->setVisible( enable );
	}

	/**
	 * @brief Returns true if the light is currently enabled, false otherwise
	 * @return true if the light is currently enabled, false otherwise
	 */
	bool BaseLight::isEnabled() const
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set direction in a Light not correctly initialized" );

		return m_pLight->isVisible();
	}


	/**
	* @internal
	* @brief Sets the new position of the light.
	* @param[in] x new x coordinate (horizontal axis) of the light in the scene
	* @param[in] y new y coordinate (vertical axis) of the light in the scene
	* @param[in] z new z coordinate (depth) of the light in the scene
	*/
	void BaseLight::setPosition( float x, float y, float z )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set position in a Light not correctly initialized" );

		// Set the absolute light position
		m_sceneNode->setPosition( x, y, z );
	}

	/**
	* @internal
	* @brief Sets the new position of the light. Z coordinate (depth) is set to zero
	* @param[in] x new x coordinate (horizontal axis) of the light in the scene
	* @param[in] y new y coordinate (vertical axis) of the light in the scene
	*/
	void BaseLight::setPosition( float x, float y )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set position in a Light not correctly initialized" );

		// Set the absolute light position
		m_sceneNode->setPosition( x, y, 0.0f );
	}

	/**
	* @internal
	* @brief Sets the new position of the light. Z coordinate (depth) is set to zero
	* @param[in] pos new position (horizontal axis) of the light in the scene
	*/
	void BaseLight::setPosition( const Vector& pos )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set position in a Light not correctly initialized" );

		// Set the absolute light position
		m_sceneNode->setPosition( pos.x, pos.y, pos.z );
	}

	/**
	* @internal
	* @brief Sets the new direction of the light.
	* @param[in] x new x coordinate (horizontal axis) of the light in the scene
	* @param[in] y new y coordinate (vertical axis) of the light in the scene
	* @param[in] z new z coordinate (depth) of the light in the scene
	*/
	void BaseLight::setDirection( float x, float y, float z )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set direction in a Light not correctly initialized" );

		// Normalize direction
		Vector dir(x, y, z);
		dir.normalise();

		// Set the absolute light direction
		m_pLight->setDirection( dir );
	}

	/**
	* @internal
	* @brief Sets the new direction of the light. Z coordinate (depth) is set to zero
	* @param[in] x new x coordinate (horizontal axis) of the light in the scene
	* @param[in] y new y coordinate (vertical axis) of the light in the scene
	*/
	void BaseLight::setDirection( float x, float y )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set direction in a Light not correctly initialized" );

		// Normalize direction
		Vector dir(x, y, 0.0f);
		dir.normalise();

		// Set the absolute light direction
		m_pLight->setDirection( dir );
	}

	/**
	* @internal
	* @brief Sets the new direction of the light. Z coordinate (depth) is set to zero
	* @param[in] dir new direction (horizontal axis) of the light in the scene
	*/
	void BaseLight::setDirection( const Vector& dir )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set direction in a Light not correctly initialized" );

		// Set the absolute light direction (normalized)
		m_pLight->setDirection( dir.normalisedCopy() );
	}

	/**
	* @internal
	* @brief Sets the diffuse (general) color of the light
	* @param[in] r Red component of the new color of the light.    Value Range: 0..255
	* @param[in] g Green component of the new color of the light.  Value Range: 0..255
	* @param[in] b Blue component of the new color of the light.   Value Range: 0..255
	*/
	void BaseLight::setDiffuseColor( float r, float g, float b )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set diffuse color in a Light not correctly initialized" );

		m_diffuseColor = Color( r, g, b );
		setDiffuseColor( m_diffuseColor );
	}


	/**
	* @internal
	* @brief Sets the diffuse (general) color of the light
	* @param[in] color New color of the light
	*/
	void BaseLight::setDiffuseColor( const Color& color )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set diffuse color in a Light not correctly initialized" );

		m_pLight->setDiffuseColour( color.normalized() );
		m_diffuseColor = color;
	}

	/**
	* @internal
	* @brief Sets the specular (shines) color of the light
	* @param[in] r Red component of the new specular color of the light.    Value Range: 0..255
	* @param[in] g Green component of the new specular color of the light.  Value Range: 0..255
	* @param[in] b Blue component of the new specular color of the light.   Value Range: 0..255
	*/
	void BaseLight::setSpecularColor( float r, float g, float b )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set specular color in a Light not correctly initialized" );

		m_specularColor = Color( r, g, b );
		setSpecularColor( m_specularColor );
	}


	/**
	* @internal
	* @brief Sets the specular (shines) color of the light
	* @param[in] color New color of the light
	*/
	void BaseLight::setSpecularColor( const Color& color )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set specular color in a Light not correctly initialized" );

		m_pLight->setSpecularColour( color.normalized() );
		m_specularColor = color;
	}

	/**
	* @internal
	* Changes the type of light.
	* Available types are:
	*  Light::LT_POINT:        point light source that emits light equally in all directions.
	*  Light::LT_DIRECTIONAL:  light source with no position, only direction. Light range is infinite.
	*  Light::LT_SPOTLIGHT:    light source with position and direction (and many other tunning possibilities).
	* @param[in] type Type to set to the light
	*/
	void BaseLight::setLightType( Light::LightTypes type )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to change the type of a Light not correctly initialized" );

		m_pLight->setType( type );
	}

	/**
	 * Sets the attenuation parameters of the light source i.e.how it diminishes with distance.
     * Lights normally get fainter the further they are away. Also, each light is given a maximum range 
	 * beyond which it cannot affect any objects. 
	 *
	 * Light attenuation is not applicable to directional lights since they have an infinite range and constant intensity. 
	 * This is a standard attenuation approach. More information: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point%20Light%20Attenuation
     *
	 * @param[in] range		The absolute upper range of the light in world units 
	 * @param[in] constant	The constant factor in the attenuation formula: 1.0 means never attenuate, 0.0 is complete attenuation 
	 * @param[in] linear	The linear factor in the attenuation formula: 1 means attenuate evenly over the distance 
	 * @param[in] quadratic The quadratic factor in the attenuation formula: adds a curvature to the attenuation formula. 
	 */
	void BaseLight::setAttenuation( float range, float constant, float linear, float quadratic )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set the light attenuation for a Light not correctly initialized" );
		
		//float r = m_pLight->getAttenuationRange();
		//float c = m_pLight->getAttenuationConstant();
		//float l = m_pLight->getAttenuationLinear();
		//float q = m_pLight->getAttenuationQuadric();
		m_pLight->setAttenuation(range, constant, linear, quadratic );
	}

	/**
	 * Helper function to set the attenuation parameters of the light so that it fades to disaapear at 'range' distance from the position
	 * Light attenuation is not applicable to directional lights since they have an infinite range and constant intensity. 
	 * This is a standard attenuation approach. More information: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point%20Light%20Attenuation
     *
	 * @param[in] range	The absolute upper range of the light in world units (the other attenuation parameters are automatically calculated in this method)
	 */
	void BaseLight::setAttenuation( float range )
	{		
		// Calculation based on: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Light+Attenuation+Shortcut
		setAttenuation( range, 1.0f, 4.5f/range, 75.0f/(range*range) );
	}

	/**
	* Configures the light to cast or not shadows
	* @param[in] cast if true, light will cast shadows
	*/
	void BaseLight::castShadows( bool cast )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to change the shadow settings on a Light not correctly initialized. init should be called first" );

		m_pLight->setCastShadows(cast);
	}

	/**
	* @brief Makes the light visible in the scene (for debugging purposes)
	*
	* @param draw if true, the light will be visible. If false, it will be invisible (although it will continue
	* lighting up the scene)
	*/
	void BaseLight::drawDebug( bool draw /*= true*/ )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to draw a Light not correctly initialized" );

		//m_pLight->setDebugDisplayEnabled( true );
		m_lightFlareSet->setVisible( draw );
	}

	/**
	* @brief Returns the position of the light
	* @return the position of the light
	*/
	const Vector& BaseLight::getPosition() const
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to get the position of a Light not correctly initialized" );

		return m_sceneNode->getPosition();
	}

} // namespace Cing