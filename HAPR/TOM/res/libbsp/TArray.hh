#ifndef __TARRAY_HH
#define __TARRAY_HH
//
// Project   : libBSP
// File      : TArray.hh
// Copyright : Ronald Kriemann
// Purpose   : class for a dyn. array of templates
// License   : GNU Lesser General Public License
//
// arch-tag: 642cccb4-0d26-4340-900b-04145904dd36
//

#include <assert.h>

#include <iostream>

#include "types.hh"

using std::cout;
using std::endl;

//
// class for a template-vector
//
template <class T> 
class TArray
{
public:
    ///////////////////////////////////////////////
    //
    // local types
    //

    typedef unsigned int t_size;
    
protected:
    T       * _data;         // the array with the stored elements
    t_size    _size;         // number of the element with the max. index

public:
    /////////////////////////////////////////
    //
    // constructor and destructor
    //

    TArray ( t_size size = 0 );
    TArray ( const TArray<T> & vec ) : _data(NULL), _size(0) { (*this) = vec; }

    ~TArray () { delete[] _data; }

    /////////////////////////////////////////
    //
    // manipulate array
    //
    
    // get/set size of the vector
    t_size  size     () const { return _size; }
    void    set_size ( t_size  n );
    
    // copy operator
    TArray<T> & operator = ( const TArray<T> & vec );

    /////////////////////////////////////////
    //
    // access array
    //
    
    T &       operator [] ( t_size  i )       { assert( i < _size ); return _data[i]; }
    const T & operator [] ( t_size  i ) const { assert( i < _size ); return _data[i]; }

    // return c-style array
    T *       c_array ()       { return _data; }
    const T * c_array () const { return _data; }

    // copy c-style array
    TArray &  copy ( T * data, t_size size );

    /////////////////////////////////////////
    //
    // misc.
    //

    // return size in bytes used by this object
    ulong byte_size () const { return sizeof(t_size) + (_size * sizeof(T)); }
};

#include "TArray.cc"

#endif // __TARRAY_HH
