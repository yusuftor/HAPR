#ifndef __TBSP_HH
#define __TBSP_HH
//
// Project :
// File    : TBSP.hh
// Purpose : encapsulation of BSP functions
// Author  : Ronald Kriemann
// License : GNU Lesser General Public License
//
// arch-tag: 91e3e5ed-0404-4f1e-a77b-b2b668081b75
//

#include <stdlib.h>
#include "types.hh"

namespace BSP
{

/////////////////////////////////////////////////////
//
// error codes
//

enum { NO_ERROR     =  0,    // no error occured
       ERR_ARG      = -1,    // error in argument
       ERR_RES      = -2,    // resource not available
       ERR_SYSCALL  = -3,    // error in system call/function
       ERR_INIT     = -4,    // not intialised
       ERR_MEM      = -5,    // memory allocation error
       ERR_COMM     = -6     // error during communication
};
       
/////////////////////////////////////////////////////
//
// datatype for a message
//
class TMessage
{
protected:
    // the real message
    void * _msg;
    
public:
    //
    // constructor and destructor
    //
    TMessage  ( void * m ) : _msg(m) {}
    TMessage  ( const TMessage & m ) { *this = m; }
    ~TMessage ()                     {} // if (_msg != NULL) free( _msg ); }
    
    //
    // return info about message
    //
    
    void *  data   () const;
    size_t  size   () const;
    uint    source () const;
    
    // copy operator
    TMessage & operator = ( const TMessage & m )
    { _msg = m._msg; return *this; }
};
    
////////////////////////////////////////
//
// initialization and ending
//

int init ( int * argc, char *** argv );
int done ();

////////////////////////////////////////
//
// properties of BSP machine
//

uint nprocs ();
uint pid    ();
    
////////////////////////////////////////
//
// global synchronisation
//

int sync ();
    
////////////////////////////////////////
//
// message passing
//

// send buf with tag to dest (buffered, unbuffered [hp])
int send   ( uint dest, const void * buf, size_t bsize );
int hpsend ( uint dest, const void * buf, size_t bsize );

// return and remove next message in queue
TMessage get_msg ();
    
// return number of messages in queue
uint nmsgs ();

////////////////////////////////////////
//
// direct (non-BSP) communication
// (blocking)
//

// direct send/recv
int dsend ( uint dest,   const void * buf, size_t bsize );
int drecv ( uint source,       void * buf, size_t bsize );

// return size of message to receive
size_t dprobe ( uint source );
               
////////////////////////////////////////
//
// misc.
//

// print statistics of BSP computation
void statistics ();

}// namespace

#endif  // __BSP_HH
