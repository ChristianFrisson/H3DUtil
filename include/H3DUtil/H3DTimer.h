//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file TimeStamp.h
/// \brief Routines to handle time stamping of the field network
///
/// TimeStamp stores an internal time value that represents the seconds
/// elapsed since January 1, 1970.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DTIMER_H__
#define __H3DTIMER_H__

#include <sstream>
#include <H3DUtil/H3DUtil.h>

#ifdef HAVE_PROFILER
#include <sofa/helper/AdvancedTimer.h>

namespace H3DUtil{

 class H3DUTIL_API H3DTimer:public sofa::helper::AdvancedTimer{
   
public:
    H3DTimer() : AdvancedTimer(  ) {}

    static void end(IdTimer id, std::stringstream& profiledResult);
  };
  
}
#endif

// Macros for convenience.
//
// Using these macros means that client code can
// compile with and without the profiler enabled 
// and will not incur any overhead when it is disabled.
#ifdef HAVE_PROFILER
#define H3DTIMER_BEGIN(name)      H3DTimer::stepBegin ( name );
#define H3DTIMER_END(name)        H3DTimer::stepEnd ( name );
#define H3DTIMER_NEXT(prev,next)  H3DTimer::stepNext ( prev, next );
#else
#define H3DTIMER_BEGIN(name)
#define H3DTIMER_END(name)
#define H3DTIMER_NEXT(prev,next)
#endif

#endif
