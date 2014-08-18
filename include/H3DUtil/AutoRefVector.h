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
/// \file AutoRefVector.h
/// Header file for AutoRefVector class.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __AUTOREFVECTOR_H__
#define __AUTOREFVECTOR_H__

#include <H3DUtil/H3DUtil.h>
#include <vector>
#include <algorithm>

namespace H3DUtil {
  /// This class is similar to the AutoRef class in the vector elements
  /// are Node * or pointers to subclasses of Node. Reference counting 
  /// will be upheld on all nodes in the vector.
  /// 
  template< class NodeClass >
  class AutoRefVector : private std::vector<NodeClass*> {
  public:
    /// The type of the Node, NodeClass, stored in the vector.
    typedef typename std::vector<NodeClass*>::value_type value_type;
    /// Pointer to NodeClass.
    typedef typename std::vector<NodeClass*>::pointer pointer;
    /// Const reference to NodeClass.
    typedef typename std::vector<NodeClass*>::const_reference const_reference;
    /// An unsigned integral type.
    typedef typename std::vector<NodeClass*>::size_type size_type;
    /// A signed integral type.
    typedef typename std::vector<NodeClass*>::difference_type difference_type; 
    /// Const iterator used to iterate through a vector.
    typedef typename std::vector<NodeClass*>::const_iterator const_iterator;
    /// Iterator used to iterate backwards through a vector.
    typedef typename std::vector<NodeClass*>::const_reverse_iterator 
    const_reverse_iterator;

    /// Creates an empty vector.
    inline AutoRefVector() {}

    /// Copy constructor from a vector class.
    inline AutoRefVector( const std::vector<NodeClass *> &v ) :
      std::vector<NodeClass*>( v ) {
      refAll();
    }

    /// Copy constructor
    inline AutoRefVector( const AutoRefVector<NodeClass> &v ) :
      std::vector<NodeClass*>( v ) {
      refAll();
    }

    /// Creates a vector with n elements.
    inline AutoRefVector( size_type n ):
      std::vector< NodeClass * >( n ) {}

    /// Destructor.
    inline virtual ~AutoRefVector() {
      clear();
    }

    /// Assignement operator.
    inline AutoRefVector<NodeClass> 
    &operator=( const AutoRefVector<NodeClass> &v ) {
      if( this != &v ) {
        unrefAll();      
        std::vector<NodeClass*>::operator=( v );
        refAll();
      }
      return *this;
    }

    /// Assignement operator.
    inline AutoRefVector<NodeClass> &operator=(
                                       const std::vector<NodeClass *> &v ) {
      // temporarily add an extra reference to the nodes in v so
      // they are not accidentally removed in unrefAll()
      for( typename std::vector< NodeClass * >::const_iterator i = v.begin();
           i != v.end();
           ++i ) 
        if(*i) (*i)->ref();
      unrefAll();
      std::vector<NodeClass*>::operator=( v );
      refAll();

      // remove the temporary references.
      for( typename std::vector< NodeClass * >::const_iterator i = v.begin();
           i != v.end();
           ++i ) 
        if(*i) (*i)->unref();
      return *this;
    }

    /// Returns a const_iterator pointing to the beginning of the vector.
    inline const_iterator begin() const { 
      return std::vector<NodeClass*>::begin();
    }
      
    /// Returns a const_iterator pointing to the end of the vector.
    inline const_iterator end() const { return std::vector<NodeClass*>::end(); }

        
    /// Returns a const_reverse_iterator pointing to the beginning of the
    /// reversed vector.
    inline const_reverse_iterator rbegin() const { 
      return std::vector<NodeClass*>::rbegin();
    }
      
    /// Returns a const_reverse_iterator pointing to the end of the reversed 
    /// vector.
    inline const_reverse_iterator rend() const { 
      return std::vector<NodeClass*>::rend(); 
    }

    /// Returns the size of the vector.
    inline size_type size() const { 
      return std::vector<NodeClass*>::size(); 
    }

    /// Returns the largest possible size of the vector.
    inline size_type max_size() const {
      return std::vector<NodeClass*>::max_size();
    }
        
    /// Number of elements for which memory has been allocated. capacity() 
    /// is always greater than or equal to size().
    inline size_type capacity() const { 
      return std::vector<NodeClass*>::capacity(); 
    }
        
    /// Swaps the contents of two vectors.
    inline void swap( AutoRefVector<NodeClass> &x ) {
      std::vector<NodeClass*>::swap( x );
    }

    /// Swaps the contents of two vectors.
    inline void swap( std::vector<NodeClass*> &x ) {
      unrefAll();
      std::vector<NodeClass*>::swap( x );
      refAll();
    }
        
    /// A request for allocation of additional memory. If s is less than
    /// or equal to capacity(), this call has no effect. 
    /// Otherwise, it is a request for allocation of additional memory. 
    /// If the request is successful, then capacity() is greater than or 
    /// equal to s; otherwise, capacity() is unchanged. In either case, 
    /// size() is unchanged.
    /// 
    inline void reserve( size_t s ) { std::vector<NodeClass*>::reserve( s ); }

    /// Inserts or erases elements at the end such that the size becomes n.
    inline virtual void resize( size_t n, NodeClass * t = NULL ) {
      if( size() > n ) {
        for( size_t i = n; i < size(); ++i )
          unref( std::vector<NodeClass*>::operator[]( i ) );
      }
      std::vector<NodeClass*>::resize( n, t );
    }

    /// true if the vector's size is 0.
    inline bool empty() const { return std::vector<NodeClass*>::empty(); }

    /// Returns the n'th element. We return a const_reference so that
    /// the values of the vector only can be changed using member 
    /// functions. To change the value of a specific index use
    /// the set( index, value ) function.
    inline const_reference operator[](size_type n) const {
      return std::vector<NodeClass*>::operator[]( n );
    }

    /// Set value at index i to v.
    inline void set( size_type i, const value_type &v ) {
      if( v != std::vector<NodeClass*>::operator[]( i ) ) {
        unref( std::vector<NodeClass*>::operator[]( i ) );
        ref( v );
        std::vector<NodeClass*>::operator[]( i ) = v;
      }
    }

    /// Returns the first element.
    inline const_reference front() const { return std::vector<NodeClass*>::front();}

    /// Returns the last element.
    inline const_reference back() const { return std::vector<NodeClass*>::back(); }

    /// Inserts a new element at the end.
    inline void push_back( const value_type &x ) {
      ref( x );
      std::vector< NodeClass * >::push_back( x );
    }

    /// Removed the last element.
    void pop_back() {
      unref( back() );
      std::vector< NodeClass * >::pop_back();
    }
        
    /// Erases all of the elements.
    inline void clear() {
      unrefAll();
      std::vector<NodeClass*>::clear();
    }

    /// Erase the first element equal to a.
    inline virtual void erase( NodeClass *a ) {
      typename std::vector<NodeClass * >::iterator i = 
        std::find( std::vector<NodeClass*>::begin(), 
                   std::vector<NodeClass*>::end(), 
                   a );
      if( i != end() ) {
        unref( *i );
        std::vector<NodeClass*>::erase( i );
      } 
    }

    /// Insert an element before the index given by pos.
    inline virtual void insert(unsigned int pos,
                               const value_type & x) {
      ref( x );
      std::vector< NodeClass*>::insert( std::vector< NodeClass*>::begin() + pos, x );
    }

    /// Removes the element at the index pos.
    inline virtual void erase(unsigned int pos ) {
      // nop if pos is outside range.
      if( pos >= size() ) return;

      unref( std::vector<NodeClass*>::operator[]( pos ) );
      std::vector< NodeClass*>::erase( std::vector< NodeClass*>::begin() + pos );
    }

  protected:
    /// Virtual function that is called when a Node is added to 
    /// the vector.
    inline virtual void ref( NodeClass *n ) const {
      if( n ) {
        n->ref();
      }
    }

    /// Virtual function that is called when a Node is removed from
    /// the vector.
    inline virtual void unref( NodeClass *n ) const {
      if( n ) {
        n->unref();
      }
    }

    /// Call ref () on all values in the vector.
    inline void refAll() const {
      for( const_iterator i = std::vector<NodeClass*>::begin(); 
           i != std::vector<NodeClass*>::end(); ++i ) 
        ref( *i );
    }

    /// Call unref () on all values in the vector.
    inline void unrefAll() const {
      for( const_iterator i = std::vector<NodeClass*>::begin(); 
           i != std::vector<NodeClass*>::end(); ++i ) 
        unref( *i );
    }
  };
}
    
#endif
