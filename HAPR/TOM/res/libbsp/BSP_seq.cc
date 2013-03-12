//
// Project :
// File    : BSP.cc
// Purpose : encapsulation of BSP functions
// Author  : Ronald Kriemann
// License : GNU Lesser General Public License
//
// arch-tag: cca65b6c-e636-4d41-91fb-d27e4caeb375
//

#include <iostream>

#include "TSLL.hh"
#include "TArray.hh"

#include "BSP.hh"

namespace BSP
{

using std::cout;
using std::endl;

////////////////////////////////////////
//
// local data types
//

// info about messages to send
typedef struct
{
    size_t  size;
    char  * data;
    bool    del;
} send_msg_info;

// info about messages received
typedef struct
{
    size_t  size;
    char  * data;
} recv_msg_info;

////////////////////////////////////////
//
// local data
//

// flag which indicates that BSP was init.
static bool bsp_initialised = false;

// structure of the BSP machine
static int bsp_pid    = 0;
static int bsp_nprocs = 1;

// messages to send and received messages
static TSLL< send_msg_info >    send_msgs;
static TSLL< recv_msg_info * >  recv_msgs, old_msgs;

////////////////////////////////////////
//
// initialization and ending
//

int
init ( int * argc, char *** argv )
{
    if ( ! bsp_initialised )
        bsp_initialised = true;
    else
        cout << "(BSP) init : already initialised" << endl;

    return NO_ERROR;
}

int
done ()
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    //
    // free old receive-messages
    //

    while ( recv_msgs.size() > 0 )
    {
        recv_msg_info  * info = recv_msgs.behead();
        
        delete[] info->data;
        delete   info;
    }// for
    
    while ( old_msgs.size() > 0 )
    {
        recv_msg_info  * info = old_msgs.behead();
        
        delete[] info->data;
        delete   info;
    }// for

    return NO_ERROR;
}

////////////////////////////////////////
//
// properties of BSP machine
//

uint
nprocs ()
{
    return bsp_nprocs;
}

uint
pid ()
{
    return bsp_pid;
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

    //
    // remove old messages
    //

    while ( recv_msgs.size() > 0 )
    {
        recv_msg_info  * info = recv_msgs.behead();
        
        delete[] info->data;
        delete   info;
    }// for
    
    while ( old_msgs.size() > 0 )
    {
        recv_msg_info  * info = old_msgs.behead();
        
        delete[] info->data;
        delete   info;
    }// for
    
    //
    // copy all messages from send to recv
    //

    while ( send_msgs.size() > 0 )
    {
        send_msg_info   sinfo = send_msgs.behead();
        recv_msg_info * rinfo = new recv_msg_info;

        if ( rinfo == NULL )
            return ERR_MEM;
        
        rinfo->size = sinfo.size;
        
        if ( sinfo.del )
        {
            // re-use pointer, because data was already copied
            rinfo->data = sinfo.data;
        }// if
        else
        {
            rinfo->data = new char[ sinfo.size ];

            if ( rinfo->data == NULL )
                return ERR_MEM;
        
            memcpy( rinfo->data, sinfo.data, sinfo.size );
        }// else

        recv_msgs.append( rinfo );
    }// while

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

    if ( dest != 0 )
    {
        fprintf( stderr, "(BSP) send : invalid destination\n" );
        return ERR_ARG;
    }// if
    
    send_msg_info  info;

    info.data = new char[ bsize ];

    if ( info.data == NULL )
        return ERR_MEM;
    
    info.size = bsize;
    info.del  = true;
    memcpy( info.data, buf, bsize );

    send_msgs.append( info );

    s_step_send += bsize;

    return NO_ERROR;
}

int
hpsend  ( uint dest, const void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    if ( dest != 0 )
    {
        fprintf( stderr, "(BSP) hpsend : invalid destination\n" );
        return ERR_ARG;
    }// if
    
    send_msg_info  info;

    info.data = (char*) buf;
    info.size = bsize;
    info.del  = false;

    send_msgs.append( info );

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

    if ( recv_msgs.size() == 0 )
        return TMessage( NULL );

    // move message into old-buffer
    recv_msg_info * info = recv_msgs.behead();

    old_msgs.append( info );
    
    return TMessage( info );
}

//
// return number of messages in queue
//
uint
nmsgs ()
{
    if ( ! bsp_initialised )
        return 0;

    return recv_msgs.size();
}

////////////////////////////////////////
//
// direct (non-BSP) communication
//

int
dsend ( unsigned int dest, const void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    if ( dest != 0 )
    {
        fprintf( stderr, "(BSP) dsend : invalid destination\n" );
        return ERR_ARG;
    }// if
    
    if ( dest == 0 )
    {
        fprintf( stderr, "(BSP) dsend : source = destination\n" );
        return ERR_ARG;
    }// if

    return NO_ERROR;
}

int
drecv ( unsigned int source, void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    if ( source != 0 )
    {
        fprintf( stderr, "(BSP) drecv : invalid source\n" );
        return ERR_ARG;
    }// if
    
    if ( source == 0 )
    {
        fprintf( stderr, "(BSP) drecv : source = destination\n" );
        return ERR_ARG;
    }// if

    return NO_ERROR;
}

//
// return size of message to receive
//
size_t
dprobe ( uint source )
{
    if ( ! bsp_initialised )
        return 0;

    if ( source != 0 )
    {
        fprintf( stderr, "(BSP) dprobe : invalid source\n" );
        return (size_t) ERR_ARG;
    }// if
    
    if ( source == 0 )
    {
        fprintf( stderr, "(BSP) dprobe : source = destination\n" );
        return (size_t) ERR_ARG;
    }// if

    return 0;
}

//////////////////////////////////////////////////////////////////
//
// TMessage
//

void *
TMessage::data () const
{
    return ((recv_msg_info *) _msg)->data;
}

size_t
TMessage::size () const
{
    return ((recv_msg_info *) _msg)->size;
}

uint
TMessage::source () const
{
    return 0;
}

}// namespace
