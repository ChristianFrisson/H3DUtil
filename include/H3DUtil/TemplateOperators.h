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
/// \file TemplateOperators.h
/// \brief Contains template operators for types defined in the ArithmeticTypes
/// namespace.
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __H3DTEMPLATEOPERATORS_H__
#define __H3DTEMPLATEOPERATORS_H__

#include <H3DUtil/H3DBasicTypes.h>

namespace H3DUtil {
  namespace ArithmeticTypes {
    /// \defgroup TemplateOperators Template operators.
    /// Generic operators defined with the use of other operators.
    /// This is to minimize the number of operators we need to define
    /// for each specific types. Most of the times it is enough to define
    /// ==, + and * operators for each class explicitly.
    /// \ingroup H3DUtilBasicTypes
    /// \{

    /// Not equals operator. Requires a == operator between
    /// the two types.
    template< class T, class U >
    inline bool operator!=( const T &a, const U &b ) {
      return ! ( a == b );
    }

    /// Division operator with float. Requires a * operator between
    /// T and float.
    template< class T >
    inline T operator/( const T &a, const float &b ) {
      return a * ( 1.0 / b );
    }

    /// Division operator with double. Requires a * operator between
    /// T and double.
    template< class T >
    inline T operator/( const T &a, const double &b ) {
      return a * ( 1.0 / b );
    }

    /// Division operator with int. Requires a * operator between
    /// T and int.
    template< class T >
    inline T operator/( const T &a, const int &b ) {
      return a * ( 1.0 / b );
    }

    /// Division operator with long. Requires a * operator between
    /// T and long.
    template< class T >
    inline T operator/( const T &a, const long &b ) {
      return a * ( 1.0 / b );
    }

#ifdef H3DUTIL_INT64
    /// Division operator with H3DInt64. Requires a * operator between
    /// T and long.
    template< class T >
    inline T operator/( const T &a, const H3DInt64 &b ) {
      return a * ( 1.0 / b );
    }
#endif

    /// Division operator with unsigned int. Requires a * operator between
    /// T and unsigned int.
    template< class T >
    inline T operator/( const T &a, const unsigned int &b ) {
      return a * ( 1.0 / b );
    }

    /// Division operator with unsigned long. Requires a * operator between
    /// T and unsigned long.
    template< class T >
    inline T operator/( const T &a, const unsigned long &b ) {
      return a * ( 1.0 / b );
    }

#ifdef H3DUTIL_INT64
    /// Division operator with unsigned long. Requires a * operator between
    /// T and unsigned long.
    template< class T >
    inline T operator/( const T &a, const H3DUInt64 &b ) {
      return a * ( 1.0 / b );
    }
#endif

    /// += operator. Requires the + operator between the types.
    template< class T, class U >
    inline void operator+=( T &a, const U &b ) {
      a = a + b;
    }

    /// -= operator. Requires the - operator between the types.
    template< class T, class U >
    inline void operator-=( T &a, const U &b ) {
      a = a - b;
    }

    /// *= operator. Requires the * operator between the types.
    template< class T, class U >
    inline void operator*=( T &a, const U &b ) {
      a = a * b;
    }

    /// /= operator. Requires the / operator between the types.
    template< class T, class U >
    inline void operator/=( T &a, const U &b ) {
      a = a / b;
    }

    /// \%= operator. Requires the \% operator between the types.
    template< class T, class U >
    inline void operator%=( T &a, const U &b ) {
      a = a % b;
    }

    /// \}
  }
}


#endif
