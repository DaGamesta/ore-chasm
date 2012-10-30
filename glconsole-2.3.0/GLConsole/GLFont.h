/*
    \file GLFont.h 

    This Code is covered under the LGPL.  See COPYING file for the license.

    $Id: GLFont.h 183 2010-07-18 15:20:20Z effer $
 */

#ifndef __GL_FONT_H__
#define __GL_FONT_H__

#ifdef HAVE_APPLE_OPENGL_FRAMEWORK
#    include <OpenGL/gl.h>
#    include <OpenGL/glu.h>
#    include <GLUT/glut.h>
#else
#    ifdef WIN32
#      include <windows.h>
#    endif
#    include <GL/gl.h>
#    include <GL/glu.h> 
#    include <GL/glut.h>
#endif

//#include <glutint.h>
//#include <GLUT/glutbitmap.h>

#include <assert.h>
#include <stdio.h>

#include <cstdarg>
#include <string>
#include <string.h>


#define MAX_TEXT_LENGTH 512

///
class GLFont
{
    friend inline bool GLFontCheckInit( GLFont* pFont );
	public:
        GLFont()
        {
            m_nNumLists = 96;
            m_nCharWidth = 8;
            m_nCharHeight = 13;
            m_bInitDone = false;
        }
        ~GLFont();        

        // printf style function take position to print to as well
        // NB: coordinates start from bottom left
        void glPrintf(int x, int y, const char *fmt, ...);
        void glPrintf(int x, int y, const std::string fmt, ...){ glPrintf(x,y, fmt.c_str()); }
        void glPrintfFast(int x, int y, const char *fmt, ...);
        void glPrintfFast(int x, int y, const std::string fmt, ...){ glPrintfFast(x,y, fmt.c_str()); }

        unsigned int   CharWidth() { return m_nCharWidth; }
        unsigned int   CharHeight() { return m_nCharHeight; }

    private:
        unsigned int   m_nCharWidth; // fixed width
        unsigned int   m_nCharHeight; // fixed width
        int            m_nNumLists;        // number of display lists
        int            m_nDisplayListBase; // base number for display lists
        bool           m_bInitDone;
};





typedef struct {
    const GLsizei width;
    const GLsizei height;
    const GLfloat xorig;
    const GLfloat yorig;
    const GLfloat advance;
    const GLubyte *bitmap;
} BitmapCharRec, *BitmapCharPtr;

typedef struct {
    const char *name;
    const int num_chars;
    const int first;
    const BitmapCharRec * const *ch;
} BitmapFontRec, *BitmapFontPtr;

typedef void *GLUTbitmapFont;















































/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees
 *      and is provided without guarantee or warrantee expressed or
 *      implied. This program is -not- in the public domain. */
                                                                                                                                                                      
//#include "glutint.h"
//#include "glutbitmap.h"
//
inline void glutBitmapCharacter(GLUTbitmapFont font, int c)
{
    printf("BITMAP CHAR, GLUT METHOD CALLED\n");
    const BitmapCharRec *ch;
    BitmapFontPtr fontinfo;
    GLint swapbytes, lsbfirst, rowlength;
    GLint skiprows, skippixels, alignment;
    
    #if defined(_WIN32) || defined(GLUT_IMPORT_LIB)
    fontinfo = (BitmapFontPtr) __glutFont(font);
    #else
    fontinfo = (BitmapFontPtr) font;
    #endif
    
    if (c < fontinfo->first ||
        c >= fontinfo->first + fontinfo->num_chars)
        return;
    ch = fontinfo->ch[c - fontinfo->first];
    if (ch) {
        /* Save current modes. */
        glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
        glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
        glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
        glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
        glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
        /* Little endian machines (DEC Alpha for example) could
         *          benefit from setting GL_UNPACK_LSB_FIRST to GL_TRUE
         *          instead of GL_FALSE, but this would require changing the
         *          generated bitmaps too. */
        glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
        glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glBitmap(ch->width, ch->height, ch->xorig, ch->yorig,
                 ch->advance, 0, ch->bitmap);
        /* Restore saved modes. */
        glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
        glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
        glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    }
}


////////////////////////////////////////////////////////////////////////////////
///
inline bool GLFontCheckInit( GLFont* pFont = NULL )
{
    // make sure glutInit has been called
    if( glutGet(GLUT_ELAPSED_TIME) <= 0 ){
        //fprintf( stderr, "WARNING: GLFontCheckInit failed after 'glutGet(GLUT_ELAPSED_TIME) <= 0' check\n" );
        //FIXME: GLUT FAILURE, SINCE I DON"T AND CANNOT USE GLUT...NEED A WAY TO SHOW A FONT
//        return false;
    }

    static int nDisplayListBase = -1;
    if( !pFont->m_bInitDone ) {
        assert( pFont != NULL );
        // GLUT bitmapped fonts...  
        pFont->m_nDisplayListBase = glGenLists( pFont->m_nNumLists );
        if( pFont->m_nDisplayListBase == 0 ) {
//    hmm, commented out for now because on my linux box w get here sometimes
//    even though glut hasn't been initialized.
//            fprintf( stderr, "%i", pFont->m_nNumLists );
            fprintf( stderr, "GLFontCheckInit() -- out of display lists\n");
            return false;
        }
        for( int nList = pFont->m_nDisplayListBase; 
                nList < pFont->m_nDisplayListBase + pFont->m_nNumLists; nList++ ) {
            glNewList( nList, GL_COMPILE );
            glutBitmapCharacter( GLUT_BITMAP_8_BY_13, nList+32-pFont->m_nDisplayListBase );
            glEndList();
        }

        nDisplayListBase = pFont->m_nDisplayListBase;
        pFont->m_bInitDone = true;
        return false;
    }
    else{
        assert( nDisplayListBase > 0 );
        pFont->m_nDisplayListBase = nDisplayListBase;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
inline GLFont::~GLFont()
{
    if( m_bInitDone && GLFontCheckInit(this) ) {
        glDeleteLists( m_nDisplayListBase, m_nDisplayListBase + m_nNumLists );
    } 
}
 
////////////////////////////////////////////////////////////////////////////////
// printf style print function
// NB: coordinates start from bottom left
inline void GLFont::glPrintf(int x, int y, const char *fmt, ...)   
{
    GLFontCheckInit(this);

    char        text[MAX_TEXT_LENGTH];                  // Holds Our String
    va_list     ap;                                     // Pointer To List Of Arguments

    if( fmt == NULL ) {                                 // If There's No Text
        return;                                         // Do Nothing
    }

    va_start( ap, fmt );                                // Parses The String For Variables
    vsnprintf( text, MAX_TEXT_LENGTH, fmt, ap );         // And Converts Symbols To Actual Numbers
    va_end( ap );                                       // Results Are Stored In Text

    glDisable(GL_DEPTH_TEST); //causes text not to clip with geometry
    //position text correctly...

    // This saves our transform (matrix) information and our current viewport information.
    glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
    // Use a new projection and modelview matrix to work with.
    glMatrixMode( GL_PROJECTION );              
    glPushMatrix();                                 
    glLoadIdentity();                               
    glMatrixMode( GL_MODELVIEW );                   
    glPushMatrix();                                     
    glLoadIdentity();                                   
    //create a viewport at x,y, but doesnt have any width (so we end up drawing there...)
    glViewport( x - 1, y - 1, 0, 0 );                   
    //This actually positions the text.
    glRasterPos4f( 0, 0, 0, 1 );
    //undo everything
    glPopMatrix();                                      // Pop the current modelview matrix off the stack
    glMatrixMode( GL_PROJECTION );                      // Go back into projection mode
    glPopMatrix();                                      // Pop the projection matrix off the stack
    glPopAttrib();                                      // This restores our TRANSFORM and VIEWPORT attributes

    //glRasterPos2f(x, y);

    glPushAttrib( GL_LIST_BIT );                        // Pushes The Display List Bits
    glListBase( m_nDisplayListBase - 32 );      // Sets The Base Character to 32
    //glScalef( 0.5, 0.5, 0.5 ); 
    glCallLists( strlen(text), GL_UNSIGNED_BYTE, text );// Draws The Display List Text
    glPopAttrib();                                      // Pops The Display List Bits
    glEnable(GL_DEPTH_TEST);
}

////////////////////////////////////////////////////////////////////////////////
//printf style print function
//NOTE: coordinates start from bottom left
//ASSUMES ORTHOGRAPHIC PROJECTION ALREADY SET UP...
inline void GLFont::glPrintfFast(int x, int y, const char *fmt, ...)   
{
    GLFontCheckInit(this);

    char        text[MAX_TEXT_LENGTH];// Holds Our String
    va_list     ap;                   // Pointer To List Of Arguments

    if( fmt == NULL ) {               // If There's No Text
        return;                       // Do Nothing
    }

    va_start( ap, fmt );                            // Parses The String For Variables
    vsnprintf( text, MAX_TEXT_LENGTH, fmt, ap );    // And Converts Symbols To Actual Numbers
    va_end( ap );                                   // Results Are Stored In Text

    glDisable( GL_DEPTH_TEST ); // Causes text not to clip with geometry
    glRasterPos2f( x, y );
    //glPushAttrib( GL_LIST_BIT );                        // Pushes The Display List Bits
    glListBase( m_nDisplayListBase - 32 );        // Sets The Base Character to 32
    glCallLists( strlen(text), GL_UNSIGNED_BYTE, text );  // Draws The Display List Text
    //glPopAttrib();                                      // Pops The Display List Bits
    glEnable( GL_DEPTH_TEST );
}


#endif
