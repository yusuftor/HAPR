//
// Project   : libBSP
// File      : TArray.hh
// Copyright : Ronald Kriemann
// Purpose   : class for a dyn. array of templates
// License   : GNU Lesser General Public License
//
// arch-tag: 642802cd-0141-4ded-aba7-19c6ce1c2f25
//

//
// constructor with default size
//
template <class T> 
TArray<T>::TArray ( t_size size )
        : _size( size )
{
    if ( _size > 0 ) _data = new T[_size];
    else             _data = NULL;
}

//
// set a new size but keep the old elements
//
template <class T> 
void 
TArray<T>::set_size ( t_size  n )
{
    if ( n == _size )
        return;

    //
    // create new array and copy old data
    // (to be done: n <= 0)
    //
    
    t_size   i;
    T      * tmp = NULL;
    T      * p1, * p2;

    if (n > 0)
        tmp = new T[n];

    p1 = tmp;
    p2 = _data;
    
    if ( _size > n )
    {
        for ( i = 0; i < n; i++ )
            *p1++ = *p2++;
    }// if
    else
    {
        for ( i = 0; i < _size; i++ )
            *p1++ = *p2++;

        // the rest of the new data is undefinded
    }// else

    delete[] _data;

    _data = tmp;
    _size = n;
}

//
// copy operator
//
template <class T> 
TArray<T> & 
TArray<T>::operator = ( const TArray<T> & vec )
{
    t_size  i;

    if (vec._size == _size)
    {
        T * p1 = _data;
        T * p2 = vec._data;

        for (i = 0; i < _size; i++)
            *p1++ = *p2++;
    }// if
    else 
    {
        _size = vec._size;

        // set new size but no need for keeping old elements
        if (_data != NULL)
            delete[] _data;

        if (_size > 0)
            _data = new T[_size];
        else
            _data = NULL;

        if (_size > 0)
        {
            T * p1 = _data;
            T * p2 = vec._data;
            
            for (i = 0; i < _size; i++)
                *p1++ = *p2++;
        }// if
    }// else
    
    return (*this);
}

template <class T>
TArray<T> &
TArray<T>::copy ( T * data, t_size size )
{
    if ( size < 0 )
        size = 0;
    
    set_size( size );

    for ( uint i = 0; i < size; i++ )
        _data[i] = data[i];

    return *this;
}

