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

#ifndef _Keyboard_H_
#define _Keyboard_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "InputPrereqs.h"
#include "BaseInputDevice.h"

// OIS
#include "OISKeyboard.h"

namespace Cing
{

/**
 * @internal
 * Manages keyboard input
 */
class Keyboard: public BaseInputDevice< OIS::KeyListener >, public OIS::KeyListener
{
public:

	// Constructor / Destructor
	 Keyboard();
	virtual ~Keyboard();

	// Init / Release / Update
	bool  init   ( OIS::InputManager* pOISInputManager );
	void  end    ();
	void  update ();

	// Query methods
	bool  isValid       ()  const { return m_bIsValid; }
	bool	isKeyDown			( OIS::KeyCode key ) const;
	bool	isModifierDown( OIS::Keyboard::Modifier modifier ) const;

  //bool  isKeyPressed  ( char c )  const;

private:


  // OIS KeyListener interface
  virtual bool keyPressed( const OIS::KeyEvent &arg );
  virtual bool keyReleased( const OIS::KeyEvent &arg );		

	// Attributes
  OIS::InputManager*  m_pOISInputManager; ///< Pointer to the OIS input manager
  OIS::Keyboard*      m_pOISKeyboard;     ///< OIS Keyboard
	bool                m_bIsValid;	        ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _Keyboard_H_
