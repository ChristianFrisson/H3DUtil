//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    This file is part of H3DUtil.
//
//    H3DUtil is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3DUtil is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3DUtil; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file FreeImageImage.cpp
/// \brief .cpp file for FreeImageImage.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "H3DUtil/FreeImageImage.h"

using namespace H3DUtil;

#ifdef HAVE_FREEIMAGE

#include <FreeImage.h>

  /// Thrown when the FIBITMAP * we encapsulate has a color type we 
  /// don't support.
  H3D_VALUE_EXCEPTION( FREE_IMAGE_COLOR_TYPE, 
                       UnsupportedFreeImageColorType ); 
  /// Thrown when the FIBITMAP * we encapsulate has a image type we 
  /// don't support.
  H3D_VALUE_EXCEPTION( FREE_IMAGE_TYPE, UnsupportedFreeImageImageType ); 

FreeImageImage::PixelComponentType FreeImageImage::pixelComponentType() {
  FREE_IMAGE_TYPE t = FreeImage_GetImageType( bitmap );
  switch( t ) {
  case FIT_BITMAP:
  case FIT_UINT16: 
  case FIT_UINT32: return UNSIGNED;
  case FIT_INT16: 
  case FIT_INT32: return SIGNED;
  case FIT_DOUBLE:
  case FIT_FLOAT: return RATIONAL;
  default: 
    throw UnsupportedFreeImageImageType( t, "", H3D_FULL_LOCATION );
  }
}

FreeImageImage::PixelType FreeImageImage::pixelType() {
  FREE_IMAGE_COLOR_TYPE t = FreeImage_GetColorType( bitmap );

  switch( t ) {
  case FIC_MINISBLACK: 
  case FIC_MINISWHITE: return LUMINANCE;
#if defined(FREEIMAGE_COLORORDER) && FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
  case FIC_RGB: return RGB;
  case FIC_RGBALPHA: return RGBA;
#else
  case FIC_RGB: return BGR;
  case FIC_RGBALPHA: return BGRA;
#endif
  default: 
    throw UnsupportedFreeImageColorType( t, "", H3D_FULL_LOCATION );
  }
}

/// Destructor.
FreeImageImage::~FreeImageImage() {
  if( bitmap ) FreeImage_Unload( bitmap );
}

void *FreeImageImage::getImageData() {
  return FreeImage_GetBits( bitmap );
}

unsigned int FreeImageImage::bitsPerPixel() {
  return FreeImage_GetBPP( bitmap );
}

/// Returns the height of the image in pixels.
unsigned int FreeImageImage::height() {
  return FreeImage_GetHeight( bitmap );
}

/// Returns the width of the image in pixels.
unsigned int FreeImageImage::width() {
  return FreeImage_GetWidth( bitmap );
}

unsigned DLL_CALLCONV
_ReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
  istream& is= *static_cast<istream*>(handle);

  streampos a= is.tellg();
  is.read ( static_cast<char*>(buffer), size*count );
  streampos b= is.tellg();

  return static_cast<unsigned int>(b-a);
}

inline int DLL_CALLCONV
_SeekProc(fi_handle handle, long offset, int origin) {
  istream& is= *static_cast<istream*>(handle);
  switch ( origin ) {
  case SEEK_SET:
    is.seekg ( offset );
    break;
  case SEEK_CUR:
    is.seekg ( offset,ios_base::cur );
    break;
  case SEEK_END:
    is.seekg ( offset,ios_base::end );
    break;
  }
 
  return -1;
}
 
inline long DLL_CALLCONV
_TellProc(fi_handle handle) {
  istream& is= *static_cast<istream*>(handle);
  return static_cast<long>(is.tellg());
}

FreeImageIO* FreeImageImage::getIStreamIO () {
  static FreeImageIO io;
  io.read_proc= _ReadProc;
  io.write_proc= NULL;
  io.seek_proc= _SeekProc;
  io.tell_proc= _TellProc;
  return &io;
}

#endif
