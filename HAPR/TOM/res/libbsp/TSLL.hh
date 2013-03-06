#ifndef __TSLL_HH
#define __TSLL_HH
//
// Project   : libBSP
// File      : TSLL.hh
// Copyright : Ronald Kriemann
// Purpose   : Template class for a single concatenated list
// License   : GNU Lesser General Public License
//
// arch-tag: ec87ea39-c827-428a-8377-d68641f4c26a
//

#include <assert.h>

#include <iostream>

using std::cout;
using std::endl;

////////////////////////////////////////////////////
// 
// Template class list
//
template <class T> 
class TSLL
{
public:
    ////////////////////////////////////////////////////
    //
    // class for a single item in the list
    //

    class TItem
    {
    public:
        T        _data;
        TItem  * _next;
        
    public:
        TItem ( const T  & e, 
                TItem    * n = NULL )
                : _data( e ), _next( n )
        {}
    };

    //////////////////////////////////////////////////////////
    // 
    // class for an iterator for the previous list
    //

    class TIterator
    {
        friend class TSLL<T>;
        
    protected:
        // currently selected item
        TItem  * _item;
        
    public:
        //
        // constructor and destructor
        //

        TIterator ()                         : _item(NULL) {}
        TIterator ( TItem * item )           : _item(item) {}
        TIterator ( const TIterator & iter ) : _item(iter._item) {}
        
        // get status (end-of-list)
        bool eol () const { return (_item == NULL); }
        
        // increment
        TIterator & next () { return ++*this; }
        
        // access actual element
        T &       data ()       { assert(_item != NULL); return _item->_data; }
        const T & data () const { assert(_item != NULL); return _item->_data; }

        //
        // usual operators
        //

        // copy
        TIterator & operator = ( const TIterator & i ) { _item = i._item; return *this; }

        // access items
        T &         operator () ()       { assert(_item != NULL); return _item->_data; }
        const T &   operator () () const { assert(_item != NULL); return _item->_data; }

        // iterate (prefix/postfix)
        TIterator & operator ++ ()    { if (_item) _item = _item->_next; return *this; }
        TIterator   operator ++ (int) { TIterator tmp(*this); ++*this; return tmp; }

        // dereference
        T &         operator *  ()    { assert(_item != NULL); return   _item->_data; }
        T *         operator -> ()    { assert(_item != NULL); return & _item->_data; }

        // compare
        bool        operator == ( const TIterator & i ) const { return (_item == i._item); }
        bool        operator != ( const TIterator & i ) const { return (_item != i._item); }

    protected:
        // handle case when no item is present
        void no_item () const
        {
            cout << "TSLL::TIterator::no_item : I miss an item to play with" << std::endl;
        }
    };

protected:
    // first and last element of list
    TItem        * _first, * _last;
    
    // size of the list aka number of stored items
    unsigned int   _size;

public:
    /////////////////////////////////////////////////
    //
    // constructors and destructors
    //

    TSLL ()                    : _first(NULL), _last(NULL), _size(0) {}
    TSLL ( const TSLL<T> & l ) : _first(NULL), _last(NULL), _size(0) { copy( l ); }

    ~TSLL () { if (_size > 0) remove_all(); }


    /////////////////////////////////////////////////
    //
    // access member variables
    //

    unsigned int size () const { return _size; }

    TIterator       first ()       { return _first; }
    const TIterator first () const { return _first; }

    TIterator       last  ()       { return _last; }
    const TIterator last  () const { return _last; }


    // same as above but access real data
    T &       first_item ()       { assert(_first != NULL); return _first->_data; }
    const T & first_item () const { assert(_first != NULL); return _first->_data; }
    
    T &       last_item ()       { assert(_last != NULL); return _last->_data; }
    const T & last_item () const { assert(_last != NULL); return _last->_data; }
    
    /////////////////////////////////////////////////
    //
    // methods for manipulating list
    //

    // add a new object to the begin/end of list
    TSLL<T> & prepend ( const T & elem );
    TSLL<T> & append  ( const T & elem );

    // remove (next) element from list
    void remove ( const T & elem );
    void remove ( TIterator & i ) { assert(i._item != NULL); remove(i._item->_data); }
    void remove_first ();

    // return first item and remove it from list
    T behead ();
    
    // remove all elements from list
    void remove_all ();

    // copy given list into local
    void copy ( const TSLL< T > & list );
    
    /////////////////////////////////////////////////
    //
    // search/sort related methods
    //

    // look if element is in list
    bool is_in ( const T & elem );

    /////////////////////////////////////////////////
    //
    // misc.
    //
    
    // return size in bytes used by this object
    unsigned long byte_size () const;
    
protected:
    // handle case when no item is present
    void no_item () const
    {
        cout << "TSLL::no_item : I miss an item to play with" << std::endl;
    }
};

#include "TSLL.cc"

#endif   // __TSLL_HH
