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
/// \file Vec3d.h
/// \brief Header file for Vec3d.
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __VEC3D_H__
#define __VEC3D_H__

#include <H3DUtil/H3DUtil.h>
#include <H3DUtil/H3DBasicTypes.h>
#include <H3DUtil/H3DMath.h>
#include <H3DUtil/TemplateOperators.h>
#include <H3DUtil/Exception.h>
#include <H3DUtil/Vec3f.h>

namespace H3DUtil {
  namespace ArithmeticTypes {

    /// A Vec3d specifies a high-precision 3d vector. The values of the 
    /// Vec3d are H3DDouble.
    /// \ingroup H3DUtilBasicTypes
    struct H3DUTIL_API Vec3d {
      /// Exception thrown when error while normalizing vector types.
      H3D_API_EXCEPTION( Vec3dNormalizeError );

      /// Default constructor.
      Vec3d(): x(0), y(0), z(0) {}

      /// Constructor.
      Vec3d( H3DDouble _x,
             H3DDouble _y,
             H3DDouble _z ) : x(_x), y(_y), z(_z) {}

      /// Conversion from Vec3f.
      Vec3d( const Vec3f &v ) : x( v.x ), y( v.y ), z( v.z ) {}

      /// Returns the dot product between this Vec3d and v. The 
      /// \ref Vec3dDotProduct "operator*" operator can also be 
      /// used to get the dot product between two vectors.
      inline H3DDouble dotProduct( const Vec3d &v ) const {
        return x*v.x + y*v.y + z*v.z;
      }

      /// Returns the cross product between this Vec3d and v. The 
      /// \ref Vec3fCrossProduct "operator*" operator can also be
      /// used to get the dot product between two vectors.
      inline Vec3d crossProduct( const Vec3d &v ) const {
        return  Vec3d( y*v.z - z*v.y,
                       z*v.x - x*v.z,
                       x*v.y - y*v.x );
      }

      /// Normalize the vector to be of length 1.
      /// \throws NormalizeError If the vector is of zero length.
      ///
      inline void normalize() {
        H3DDouble l2 = x*x+y*y+z*z;
        // if the length already is 1 we don't have to do anything
        if( H3DAbs(l2-1) > Constants::d_epsilon ) {
          H3DDouble l = H3DSqrt( l2 );
          if( l == 0 ) {
            std::string s = "Trying to normalize zero length Vec3d.";
            throw Vec3dNormalizeError( s );
          } else {
            x /= l; 
            y /= l;
            z /= l;
          }
        }
      }

      /// Normalize the vector to be of length 1. If the vector is of 
      /// zero length nothing will be done.
      inline void normalizeSafe() {
        H3DDouble l2 = x*x+y*y+z*z;
        // if the length already is 1 we don't have to do anything
        if( H3DAbs(l2-1) > Constants::d_epsilon ) {
          H3DDouble l = H3DSqrt( l2 );
          if( H3DAbs(l) >= Constants::d_epsilon ) {
            x /= l; 
            y /= l;
            z /= l;
          }
        }
      }

      /// Returns the length squeared of the vector.
      inline H3DDouble lengthSqr() const {
        return x*x + y*y + z*z;
      }

      /// Returns the length of the vector.
      inline H3DDouble length() const {
        return H3DSqrt( x*x + y*y + z*z );
      }

      inline H3DDouble &operator[]( int i ) { 
        if( i == 0 ) return x;
        if( i == 1 ) return y;
        if( i == 2 ) return z;
        
        throw Exception::H3DAPIException( "Invalid index", 
                                          H3D_FULL_LOCATION );
      }

      inline const H3DDouble &operator[]( int i ) const { 
        if( i == 0 ) return x;
        if( i == 1 ) return y;
        if( i == 2 ) return z;
        
        throw Exception::H3DAPIException( "Invalid index", 
                                          H3D_FULL_LOCATION );
      }

      /// The public values of the vector.
      H3DDouble x, y, z;

      /// Per-element precision double comparison against an epsilon value.
      inline bool nearEqual( const Vec3d &rhs, const H3DDouble epsilon = std::numeric_limits< H3DDouble >::epsilon() ) const {
        return epsilonCompare( x, rhs.x, epsilon ) &&
               epsilonCompare( y, rhs.y, epsilon ) &&
               epsilonCompare( z, rhs.z, epsilon );
      }
    };

    /// \defgroup Vec3dOperators Vec3d operators.
    /// \brief Operators on Vec3d instances. See also the 
    /// \ref TemplateOperators "template operators" for more 
    /// operators automatically defined from the explicit ones 
    /// defined here.
    /// \ingroup H3DUtilBasicTypes
    /// \{
    
    /// Function for printing a Vec3d to an ostream.
    inline std::ostream& operator<<( std::ostream &os, const Vec3d &v ) {
      os << v.x << " " << v.y << " " << v.z;
      return os;
    } 
    /// Test two Vec3d for equality.
    inline bool operator==( const Vec3d &v1, const Vec3d &v2 ) {
      return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
    }
    /// Addition between two Vec3d.
    inline Vec3d operator+( const Vec3d &v1, const Vec3d &v2 ) {
      return Vec3d( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z );
    }

    /// Multiply a Vec3d with a double.
    inline Vec3d operator*( const Vec3d &v, const double &d ) {
      return Vec3d( v.x * d, v.y * d, v.z * d );
    } 

    /// Multiply a Vec3d with an int.
    inline Vec3d operator*( const Vec3d &v, const int &i ) {
      return Vec3d( v.x * i, v.y * i, v.z * i );
    }

    /// Multiply a Vec3d with a long.
    inline Vec3d operator*( const Vec3d &v, const long &i ) {
      return Vec3d( v.x * i, v.y * i, v.z * i );
    }

    /// Multiply a Vec3d with a float.
    inline Vec3d operator*( const Vec3d &v, const float &f ) {
      return Vec3d( v.x * f, v.y * f, v.z * f );
    }


    /// Returns the dot product of two Vec3d.
    /// \anchor Vec3dDotProduct
    inline H3DDouble operator*( const Vec3d &v1, const Vec3d &v2 ) {
      return v1.dotProduct( v2 );
    }

    /// Returns the cross product of two Vec3d.
    /// \anchor Vec3dCrossProduct
    inline Vec3d operator%( const Vec3d &v1, const Vec3d &v2 ) {
      return v1.crossProduct( v2 );
    }

    /// Multiplication with int.
    inline Vec3d operator*( const int &a, const Vec3d &b ) { return b * a; }

    /// Multiplication with long.
    inline Vec3d operator*( const long &a, const Vec3d &b ) { return b * a; }

    /// Multiplication with float.
    inline Vec3d operator*( const double &a, const Vec3d &b ) { return b * a; }

    /// Unary minus.
    inline Vec3d operator-( const Vec3d &b ) { return b * (double)-1; }

    /// Subtraction between two Vec3d.
    inline Vec3d operator-( const Vec3d &a, const Vec3d &b ) { return a + (-b); }

    // \}
  }
}

#endif
