#ifndef __TYPES_HH
#define __TYPES_HH
//
// Project   : libBSP
// File      : types.hh
// Copyright : Ronald Kriemann
// Purpose   : Template class for a single concatenated list
// License   : GNU Lesser General Public License
//
// arch-tag: 396c7771-ca50-4570-80e9-03bbbb53ca88
//

//
// basic types
//

typedef  unsigned char       uchar;
typedef  unsigned short      ushort;
typedef  char                byte;
typedef  unsigned char       ubyte;
typedef  unsigned int        uint;
typedef  unsigned long       ulong;

typedef  double  real;

//
// some abbr.
//

// return minimal/maximal value
template< class T > T min ( const T & a, const T & b ) { return (a < b) ? a : b; }
template< class T > T max ( const T & a, const T & b ) { return (a > b) ? a : b; }

#endif			// __TYPES_HH
