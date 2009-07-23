#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows the use of the Cing::String class
 */

void setup()
{

	// Creating String object
	String p = "potato";

	// Comparing String objects, see reference below.
	if ( p.equals("potato") ) {
		println("Yes, the contents of p and potato are the same.");
	}

	// Modifying Strings
	p.toUpperCases();

	// Printing Strings
	println(p.toChar());

	// Searching indexes
	String a = "T";
	println("result: %i" , p.indexOf( a ));
	println("nChars: %i" , p.length());

}

void draw()
{

}

void end()
{
}

void mousePressed()
{
}

void mouseMoved()
{
}

void mouseReleased()
{
}

void keyPressed()
{
}