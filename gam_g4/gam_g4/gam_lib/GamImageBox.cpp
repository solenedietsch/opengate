/* --------------------------------------------------
   Copyright (C): OpenGATE Collaboration
   This software is distributed under the terms
   of the GNU Lesser General  Public Licence (LGPL)
   See LICENSE.md for further details
   -------------------------------------------------- */

#include "GamImageBox.h"

#include "G4Polyhedron.hh"
#include "G4VGraphicsScene.hh"
#include "G4VisManager.hh"
#include "GamHelpersDict.h"

#include <typeinfo>

//-----------------------------------------------------------------------------
GamImageBox::GamImageBox(py::dict &user_info) : G4Box(DictGetStr(user_info, "name"), DictGetDouble(user_info, "pX"), DictGetDouble(user_info, "pY"), DictGetDouble(user_info, "pZ")) {
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GamImageBox::~GamImageBox() {}
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
void GamImageBox::DescribeYourselfTo(G4VGraphicsScene& scene) const{
#ifdef GAMIMAGEBOX_USE_OPENGL
    try
    {
        G4OpenGLSceneHandler& opengl = dynamic_cast<G4OpenGLSceneHandler&>(scene);

        scene.AddSolid (*this);
        DescribeYourselfTo(opengl);
    }
    catch(std::bad_cast exp)
    {
        scene.AddSolid (*this);
    }
#else
    scene.AddSolid (*this);
#endif

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifdef GAMIMAGEBOX_USE_OPENGL
void GamImageBox::DescribeYourselfTo(G4OpenGLSceneHandler& scene) const{

    scene.BeginPrimitives(scene.GetObjectTransformation());

    GLfloat xHalfLength = GetXHalfLength();
    GLfloat yHalfLength = GetYHalfLength();
    GLfloat zHalfLength = GetZHalfLength();

    GLfloat x = position_x * xHalfLength * 2 / resolution_x - xHalfLength;
    GLfloat y = position_y * yHalfLength * 2 / resolution_y - yHalfLength;
    GLfloat z = position_z * zHalfLength * 2 / resolution_z - zHalfLength;

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    GLfloat * color = new GLfloat[4];
    glGetFloatv(GL_CURRENT_COLOR, color);

    glColor3f(1.f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture_xy);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0); glVertex3f(-xHalfLength, -yHalfLength, z);
    glTexCoord2d(0, 1); glVertex3f(-xHalfLength,  yHalfLength, z);
    glTexCoord2d(1, 1); glVertex3f( xHalfLength,  yHalfLength, z);
    glTexCoord2d(1, 0); glVertex3f( xHalfLength, -yHalfLength, z);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture_yz);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0); glVertex3f(x, -yHalfLength, -zHalfLength);
    glTexCoord2d(1, 0); glVertex3f(x,  yHalfLength, -zHalfLength);
    glTexCoord2d(1, 1); glVertex3f(x,  yHalfLength,  zHalfLength);
    glTexCoord2d(0, 1); glVertex3f(x, -yHalfLength,  zHalfLength);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture_xz);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0); glVertex3f(-xHalfLength, y, -zHalfLength);
    glTexCoord2d(0, 1); glVertex3f(-xHalfLength, y,  zHalfLength);
    glTexCoord2d(1, 1); glVertex3f( xHalfLength, y,  zHalfLength);
    glTexCoord2d(1, 0); glVertex3f( xHalfLength, y, -zHalfLength);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-xHalfLength, -yHalfLength, z);
    glVertex3f(-xHalfLength,  yHalfLength, z);
    glVertex3f( xHalfLength,  yHalfLength, z);
    glVertex3f( xHalfLength, -yHalfLength, z);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(x, -yHalfLength, -zHalfLength);
    glVertex3f(x,  yHalfLength, -zHalfLength);
    glVertex3f(x,  yHalfLength,  zHalfLength);
    glVertex3f(x, -yHalfLength,  zHalfLength);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(-xHalfLength, y, -zHalfLength);
    glVertex3f(-xHalfLength, y,  zHalfLength);
    glVertex3f( xHalfLength, y,  zHalfLength);
    glVertex3f( xHalfLength, y, -zHalfLength);
    glEnd();

    glColor3fv(color);
    delete[] color;

    glPopAttrib();

    scene.EndPrimitives();
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
GLubyte * GamImageBox::convertToRGB(std::vector<PixelType> slice) const{
    GLubyte * rgb = new GLubyte[slice.size() * 3];

    int i = 0;
    for(std::vector<PixelType>::iterator it = slice.begin(); it != slice.end(); ++it) {
        PixelType pixel = *it;
        pixel *= std::numeric_limits<GLubyte>::max();

        GLubyte gray = static_cast<GLubyte>(pixel);
        rgb[i] = gray;
        rgb[i+1] = gray;
        rgb[i+2] = gray;
        i += 3;
    }

    return rgb;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GLuint GamImageBox::genOpenGLTexture(const GLubyte * rgb, int width, int height) const{
        std::cout << "coucouc00" << std::endl;
    GLuint texture(0);
   GLuint balance[5] = {1000, 2, 3, 17, 50};
   GLuint* p;

   p = balance;
 
    std::cout << "coucouc01" << std::endl;
        std::cout << texture << std::endl;    std::cout << "coucouc01" << std::endl;
    glGenTextures(1, p);
        std::cout << "coucouc01" << std::endl;
    std::cout << "coucouc02" << std::endl;
    glBindTexture(GL_TEXTURE_2D, texture);
    std::cout << "coucouc03" << std::endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    std::cout << "coucouc04" << std::endl;
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    std::cout << "coucouc05" << std::endl;
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) rgb);
    std::cout << "coucouc06" << std::endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    std::cout << "coucouc07" << std::endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    std::cout << "coucouc08" << std::endl;
    return texture;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//void GamImageBox::InitialiseSlice(std::vector<PixelType> & sliceXY, std::vector<PixelType> & sliceXZ, std::vector<PixelType> & sliceYZ, const double resol_x, const double resol_y, const double resol_z) {
void GamImageBox::InitialiseSlice(const double resol_x, const double resol_y, const double resol_z) {
    std::vector<PixelType> tmp = { 12.0, 1.2 };
    {
        GLubyte * rgb = convertToRGB(tmp);
    std::cout << "coucouc1" << std::endl;
        texture_xy = genOpenGLTexture(rgb, resol_x, resol_y);
    std::cout << "coucouc2" << std::endl;
        delete [] rgb;
    }

    {
        GLubyte * rgb = convertToRGB(tmp);
        texture_xz = genOpenGLTexture(rgb, resol_x, resol_z);
        delete [] rgb;
    }

    {
        GLubyte * rgb = convertToRGB(tmp);
        texture_yz = genOpenGLTexture(rgb, resol_y, resol_z);
        delete [] rgb;
    }

}
//-----------------------------------------------------------------------------


#endif


