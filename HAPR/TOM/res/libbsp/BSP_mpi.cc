//
// Project :
// File    : TBSP.cc
// Purpose : encapsulation of BSP functions
// Author  : Ronald Kriemann
// License : GNU Lesser General Public License
//
// arch-tag: d1cdd686-1d20-463a-b79d-72fcca9d1593
//

#include <iostream>

// avoid inclusion of C++ code in MPICH
#define MPICH_SKIP_MPICXX

#include <mpi.h>

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
    MPI_Request  req;
    char       * data;
} send_msg_info;

// info about messages received
typedef struct 
{
    size_t  size;
    int     source;
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

// holds number of messages to send to all nodes
static TArray< int >            send_no;

////////////////////////////////////////
//
// static functions
//

static std::string MPI_error ( int rc );

////////////////////////////////////////
//
// initialization and ending
//

int
init ( int * argc, char *** argv )
{
    if ( ! bsp_initialised )
    {
        int rc;
        
        if (( rc = MPI_Init( argc, argv )) != MPI_SUCCESS )
        {
            fprintf( stderr, "(BSP) init : error in \"MPI_Init\" : %s\n", MPI_error( rc ).c_str() );
            return ERR_SYSCALL;
        }// if

        if (( rc = MPI_Comm_size( MPI_COMM_WORLD, & bsp_nprocs )) != MPI_SUCCESS )
        {
            fprintf( stderr, "(BSP) init : error in \"MPI_Comm_size\" : %s\n", MPI_error( rc ).c_str() );
            return ERR_SYSCALL;
        }// if

        if (( rc = MPI_Comm_rank( MPI_COMM_WORLD, & bsp_pid )) != MPI_SUCCESS )
        {
            fprintf( stderr, "(BSP) init : error in \"MPI_Comm_rank\" : %s\n", MPI_error( rc ).c_str() );
            return ERR_SYSCALL;
        }// if

        send_no.set_size( bsp_nprocs );
        for ( int i = 0; i < bsp_nprocs; i++ )
            send_no[i] = 0;
        
        bsp_initialised = true;
    }// if

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

    int  rc;
    
    if (( rc = MPI_Finalize()) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) done : error in \"MPI_Finalize\" : %s\n", MPI_error( rc ).c_str() );
        return ERR_SYSCALL;
    }// if

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
    
    //
    // exchange info about how many messages to receive
    //

    TArray< int >  recv_no( bsp_nprocs );
    uint           nmsgs = 0;
    int            rc;
    
    if (( rc = MPI_Alltoall( send_no.c_array(), 1, MPI_INT,
                             recv_no.c_array(), 1, MPI_INT,
                             MPI_COMM_WORLD )) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) sync : error in \"MPI_Alltoall\" : %s\n", MPI_error( rc ).c_str() );
        return ERR_SYSCALL;
    }// if
    
    // reset send-information and count messages to recv
    for ( int i = 0; i < bsp_nprocs; i++ )
    {
        send_no[i] = 0;
        nmsgs += recv_no[i];
    }// for

    //
    // receive messages from all other nodes
    //

    MPI_Status  status;
    
    nmsgs = 0;
    
    for ( int i = 0; i < bsp_nprocs; i++ )
    {
        if ( recv_no[i] == 0 )
            continue;

        for ( int j = 0; j < recv_no[i]; j++ )
        {
            recv_msg_info * info = new recv_msg_info;
            int             size, nsize;

            if ( info == NULL )
                return ERR_MEM;
            
            if (( rc = MPI_Probe( i, 0, MPI_COMM_WORLD, & status )) != MPI_SUCCESS )
            {
                fprintf( stderr, "(BSP) sync : error in \"MPI_Probe\" : %s\n", MPI_error( rc ).c_str() );
                return ERR_SYSCALL;
            }// if
            
            if (( rc = MPI_Get_count( & status, MPI_BYTE, & size )) != MPI_SUCCESS )
            {
                fprintf( stderr, "(BSP) sync : error in \"MPI_Get_count\" : %s\n", MPI_error( rc ).c_str() );
                return ERR_SYSCALL;
            }// if

            info->data = new char[ size ];

            if ( info->data == NULL )
                return ERR_MEM;
            
            if (( rc = MPI_Recv( info->data, size, MPI_BYTE, i, 0,
                                 MPI_COMM_WORLD, & status )) != MPI_SUCCESS )
            {
                fprintf( stderr, "(BSP) sync : error in \"MPI_Recv\" : %s\n", MPI_error( rc ).c_str() );
                return ERR_SYSCALL;
            }// if
            
            if (( rc = MPI_Get_count( & status, MPI_BYTE, & nsize )) != MPI_SUCCESS )
            {
                fprintf( stderr, "(BSP) sync : error in \"MPI_Get_count\" : %s\n", MPI_error( rc ).c_str() );
                return ERR_SYSCALL;
            }// if

            if ( size != nsize )
            {
                fprintf( stderr, "(BSP) sync : different size in <probe> and <recv>\n" );
                return ERR_COMM;
            }// if

            info->size   = nsize;
            info->source = i;
            recv_msgs.append( info );
        }// for
    }// for

    //
    // finish send requests
    //

    while ( send_msgs.size() > 0 )
    {
        //
        // maybe a simple free would also do (TRY IT)
        //

        send_msg_info  info = send_msgs.behead();
        
        if (( rc = MPI_Wait( & info.req, & status )) != MPI_SUCCESS )
        {
            fprintf( stderr, "(BSP) sync : error in \"MPI_Wait\" : %s\n", MPI_error( rc ).c_str() );
            return ERR_SYSCALL;
        }// if
        
        delete info.data;
    }// while

    //
    // global synchronisation
    //

    if (( rc = MPI_Barrier( MPI_COMM_WORLD )) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) sync : error in \"MPI_Barrier\" : %s\n", MPI_error( rc ).c_str() );
        return ERR_SYSCALL;
    }// if

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

    send_msg_info  info;
    int            rc;

    info.data = new char[ bsize ];

    if ( info.data == NULL )
        return ERR_MEM;
    
    memcpy( info.data, buf, bsize );

    if (( rc = MPI_Isend( info.data, bsize, MPI_BYTE, dest, 0,
                          MPI_COMM_WORLD, & info.req )) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) send : error in \"MPI_Isend\" : %s\n", MPI_error( rc ).c_str() );
        return ERR_SYSCALL;
    }// if

    send_no[ dest ]++;

    send_msgs.append( info );

    s_step_send += bsize;

    return NO_ERROR;
}

int
hpsend  ( uint dest, const void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    send_msg_info  info;
    int            rc;

    info.data = NULL;
    
    if (( rc = MPI_Isend( (void*) buf, bsize, MPI_BYTE, dest, 0,
                          MPI_COMM_WORLD, & info.req )) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) hpsend : error in \"MPI_Isend\" : %s\n", MPI_error( rc ).c_str() );
        return ERR_SYSCALL;
    }// if

    send_no[ dest ]++;

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
dsend ( uint dest, const void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    int  rc;
    
    if (( rc = MPI_Send( (void *) buf, bsize, MPI_BYTE, dest, 0, MPI_COMM_WORLD )) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) dsend : error in \"MPI_Send\" : %s\n", MPI_error( rc ).c_str() );
        return ERR_SYSCALL;
    }// if

    return NO_ERROR;
}

int
drecv ( uint source, void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    MPI_Status  status;
    int         rc;

    if (( rc = MPI_Recv( buf, bsize, MPI_BYTE, source, 0, MPI_COMM_WORLD, & status )) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) drecv : error in \"MPI_Recv\" : %s\n", MPI_error( rc ).c_str() );
        return ERR_SYSCALL;
    }// if
    
    int  count;

    if (( rc = MPI_Get_count( & status, MPI_BYTE, & count )) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) drecv : error in \"MPI_Get_count\" : %s\n", MPI_error( rc ).c_str() );
        return ERR_SYSCALL;
    }// if

    if ( ((size_t) count) != bsize )
    {
        fprintf( stderr, "(BSP) drecv : available and requested size differ, %ld <> %ld\n",
                 (long) count, (long) bsize );
        return ERR_COMM;
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

    MPI_Status  status;
    int         count;
    int         rc;

    if (( rc = MPI_Probe( source, 0, MPI_COMM_WORLD, & status )) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) dprobe : error in \"MPI_Probe\" : %s\n", MPI_error( rc ).c_str() );
        return (size_t) ERR_SYSCALL;
    }// if

    if (( rc = MPI_Get_count( & status, MPI_BYTE, & count )) != MPI_SUCCESS )
    {
        fprintf( stderr, "(BSP) dprobe : error in \"MPI_Get_count\" : %s\n", MPI_error( rc ).c_str() );
        return (size_t) ERR_SYSCALL;
    }// if

    return count;
}

//////////////////////////////////////////////////////////////////
//
// BSP::TMessage
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
    return ((recv_msg_info *) _msg)->source;
}

//////////////////////////////////////////////////////////////////
//
// misc
//

//
// common error output for MPI functions
//
static std::string
MPI_error ( int rc )
{
    switch ( rc )
    {
    case MPI_ERR_COMM  : return ", invalid communcator";
    case MPI_ERR_COUNT : return ", invalid count argument";
    case MPI_ERR_TYPE  : return ", invalid datatype";
    case MPI_ERR_TAG   : return ", invalid tag argument";
    case MPI_ERR_RANK  : return ", invalid destination";
    }// switch

    return ", unknown error";
}

}// namespace
