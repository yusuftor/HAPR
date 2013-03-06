//
// Project   : libBSP
// File      : TSLL.cc
// Copyright : Ronald Kriemann
// Purpose   : Template class for a single concatenated list
// License   : GNU Lesser General Public License
//
// arch-tag: 6300f12b-3be5-4692-9ed8-73d64aaca5ba
//

template <class T>
TSLL<T> &
TSLL<T>::prepend ( const T & elem )
{
    TItem  * tmp = new TItem( elem, _first );
    
    _first = tmp;
    _size++;

    if (_last == NULL)
        _last = tmp;
    
    return *this;
}

template <class T>
TSLL<T> &
TSLL<T>::append ( const T & elem )
{
    TItem * tmp = new TItem( elem );
    
    if (_last)
        _last->_next = tmp;
    
    _last = tmp;
    
    if ( ! _first )
        _first = tmp;
    
    _size++;
    
    return *this;
}

template <class T>
void
TSLL<T>::remove ( const T & elem )
{
    TItem  * item = _first;
    TItem  * old  = NULL;

    while (item != NULL)
    {
        if (item->_data == elem)
        {
            if (item == _first)
                _first = item->_next;
            else // (item != first) => (old != NULL)
                old->_next = item->_next;

            if (item == _last)
                _last = old;
            
            delete item;
            _size--;

            if ( old == NULL )
                item = _first;
            else
                item = old->_next;
        }// if
        else
        {
            old  = item;
            item = item->_next;
        }// else
    }// while
}

template <class T>
void
TSLL<T>::remove_first ()
{
    if (_first)
    {
        TItem  * tmp = _first->_next;

        delete _first;

        if (tmp == NULL)
            _last = NULL;

        _first = tmp;

        _size--;
    }// if
}

template <class T>
T
TSLL<T>::behead ()
{
    assert( _first != NULL );

    T        ret = _first->_data;
    TItem  * tmp = _first->_next;

    delete _first;

    if (tmp == NULL)
        _last = NULL;
    
    _first = tmp;
    
    _size--;
    
    return ret;
}

template <class T>
void
TSLL<T>::remove_all ()
{
    if (_size == 0)
        return;
    
    TItem * tmp = _first;
    TItem * next;
    
    while (tmp != NULL)
    {
        next = tmp->_next;
        delete tmp;
        tmp = next;
    }// while
    
    _first = _last = NULL;
    _size = 0;
}

template <class T>
void
TSLL<T>::copy ( const TSLL< T > & list )
{
    TItem  * tmp  = list._first;
    TItem  * old  = NULL;
    TItem  * item = list._first;

    remove_all();
    
    while ( tmp != NULL )
    {
        item = new TItem( tmp->_data );

        if ( _first == NULL )
            _first = item;
        
        if ( old != NULL )
            old->_next = item;

        old = item;
        tmp = tmp->_next;
    }// while

    _last = old;
    _size = list._size;
}
    
template <class T>
bool
TSLL<T>::is_in ( const T & elem )
{
    TItem * tmp = _first;

    while ( tmp )
    {
        if (tmp->_data == elem)
            return true;

        tmp = tmp->_next;
    }// while

    return false;
}

/////////////////////////////////////////////////
//
// misc.
//

//
// return size in bytes used by this object
//
template <class T>
unsigned long
TSLL<T>::byte_size () const
{
    return sizeof(TItem*) + sizeof(TItem*) + sizeof(unsigned int) + (_size * sizeof(TItem));
}
    
