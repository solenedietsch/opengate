/* --------------------------------------------------
   Copyright (C): OpenGATE Collaboration
   This software is distributed under the terms
   of the GNU Lesser General  Public Licence (LGPL)
   See LICENSE.md for further details
   -------------------------------------------------- */

#ifndef GamImageBox_h
#define GamImageBox_h

#include <pybind11/stl.h>
#include "G4Box.hh"

namespace py = pybind11;

#ifdef G4VIS_USE_OPENGL
    #ifdef __APPLE__
      #include <OpenGL/gl.h>
    #else
      #include <GL/gl.h>
    #endif
    #define G4VIS_BUILD_OPENGL_DRIVER
    #include "G4OpenGLSceneHandler.hh"
    #define GAMIMAGEBOX_USE_OPENGL 1
#endif /* G4VIS_USE_OPENGL */


class GamImageBox : public virtual G4Box {

public:
    GamImageBox(py::dict &user_info);
    ~GamImageBox();

    typedef float PixelType;

    void DescribeYourselfTo(G4VGraphicsScene& scene) const;
    //void InitialiseSlice(std::vector<PixelType> & sliceXY, std::vector<PixelType> & sliceXZ, std::vector<PixelType> & sliceYZ, const double resol_x, const double resol_y, const double resol_z);
    void InitialiseSlice(const double resol_x, const double resol_y, const double resol_z);

private:
#ifdef GAMIMAGEBOX_USE_OPENGL
    void DescribeYourselfTo(G4OpenGLSceneHandler& scene) const;

    GLubyte * convertToRGB(std::vector<PixelType> slice) const;
    GLuint genOpenGLTexture(const GLubyte * rgb, int width, int height) const;

    size_t position_x;
    size_t position_y;
    size_t position_z;
    size_t resolution_x;
    size_t resolution_y;
    size_t resolution_z;
    GLuint texture_xy;
    GLuint texture_xz;
    GLuint texture_yz;
#endif
};

#endif /* GamImageBox_h */
