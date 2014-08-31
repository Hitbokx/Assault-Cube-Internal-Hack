#include "glDraw.h"

void GL::setupOrtho( )
{
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glPushMatrix( );
	GLint viewPort[4];
	glGetIntegerv( GL_VIEWPORT, viewPort );
	glViewport( 0, 0, viewPort[2], viewPort[3] );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	glOrtho( 0, viewPort[2], viewPort[3], 0, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glDisable( GL_DEPTH_TEST );
}

void GL::restoreGL( )
{
	glPopMatrix( );
	glPopAttrib( );
}

void GL::drawFilledRect( float x, float y, float width, float height, const GLubyte colour[3] )
{ // Immediate Mode
	glColor3ub( colour[0], colour[1], colour[2] );
	glBegin( GL_QUADS );
	glVertex2f( x, y );
	glVertex2f( x + width, y );
	glVertex2f( x + width, y + height );
	glVertex2f( x, y + height );
	glEnd( );
}

void GL::drawOutline( float x, float y, float width, float height, float lineWidth, const GLubyte colour[3] )
{
	glLineWidth( lineWidth );
	glBegin( GL_LINE_STRIP );
	glColor3ub( colour[0], colour[1], colour[2] );

	glVertex2f( x - 0.5f, y - 0.5f );
	glVertex2f( x + width + 0.5f, y - 0.5f );
	glVertex2f( x + width + 0.5f, y + height + 0.5f );
	glVertex2f( x - 0.5f, y + height + 0.5f );
	glVertex2f( x - 0.5f, y - 0.5f );

	glEnd( );
}
