//
// Project   : libbsp
// File      : BSP_pub.cc
// Copyright : Ronald Kriemann
// Purpose   : encapsulation of BSP functions
// License   : GNU Lesser General Public License
//
// arch-tag: a5fad667-dd08-4ea9-819e-59a3a2884a85
//

#include <assert.h>

#include <iostream>

#include <pub.h>

#include "BSP.hh"

namespace BSP
{

using std::cout;
using std::endl;

////////////////////////////////////////
//
// local data structures
//

// flag which indicates that BSP was init.
static bool bsp_initialised = false;

// structure of the BSP machine
static int b_pid    = 0;
static int b_nprocs = 1;

// number of messages received in last sync
static int recv_msgs = 0;

// current message in queue
static int cur_msg = 0;

// global bsp-scope
static t_bsp global_bsp;

////////////////////////////////////////
//
// initialization and ending
//

int
init ( int * argc, char *** argv )
{
    if ( ! bsp_initialised )
    {
        bsplib_saveargs( argc, argv );
        bsplib_init( BSPLIB_STDPARAMS, & global_bsp );

        b_nprocs = bsp_nprocs( & global_bsp );
        b_pid    = bsp_pid( & global_bsp );
        
        bsp_initialised = true;
    }// if
    else
        cout << "(TBSP) init : already initialised" << endl;

    return NO_ERROR;
}

int
done ()
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    bsplib_done();

    return NO_ERROR;
}

////////////////////////////////////////
//
// properties of BSP machine
//

uint
nprocs ()
{
    return b_nprocs;
}

uint
pid ()
{
    return b_pid;
}
    
////////////////////////////////////////
//
// global synchronisation
//

int
sync ()
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    bsp_sync( & global_bsp );
    
    recv_msgs = bsp_nmsgs( & global_bsp );
    cur_msg   = 0;

    n_steps++;
    s_max_step_send = max( s_max_step_send, s_step_send );
    s_all_send     += double( s_step_send );
    s_step_send     = 0;

    return NO_ERROR;
}
    
////////////////////////////////////////
//
// message passing
//

//
// send buf with tag to dest
//
int
send  ( uint dest, const void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    bsp_send( & global_bsp, dest, (void*) buf, bsize );
    s_step_send += bsize;

    return NO_ERROR;
}

int
hpsend  ( uint dest, const void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    bsp_hpsend( & global_bsp, dest, (void*) buf, bsize );
    s_step_send += bsize;

    return NO_ERROR;
}

//
// return i'th message in queue
//
TMessage
get_msg ()
{
    if ( ! bsp_initialised )
        return TMessage( NULL );

    if ( cur_msg >= recv_msgs )
    {
        cout << '[' << b_pid << "] "
             << "(BSP) get_msg : no message in queue" << endl;
        return TMessage( NULL );
    }// if
    
    return TMessage( bsp_getmsg( & global_bsp, cur_msg++ ) );
}

//
// return number of messages in queue
//
uint
nmsgs ()
{
    if ( ! bsp_initialised )
        return 0;
            
    return recv_msgs - cur_msg;
}

//////////////////////////////////////////////////////////////////
//
// TMessage
//

void *
TMessage::data () const
{
    return bspmsg_data( (t_bspmsg *) _msg );
}

size_t
TMessage::size () const
{
    return bspmsg_size( (t_bspmsg *) _msg );
}

uint
TMessage::source () const
{
    return bspmsg_src( (t_bspmsg *) _msg );
}

}// namespace
