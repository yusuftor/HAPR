//
// Project :
// File    : BSP.cc
// Purpose : encapsulation of BSP functions
// Author  : Ronald Kriemann
// License : GNU Lesser General Public License
//
// arch-tag: b36252ef-7324-4b0d-b52f-53eb94a7d216
//

#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>

#include "TSLL.hh"
#include "TArray.hh"

#include "BSP.hh"

namespace BSP
{

////////////////////////////////////////
//
// local definitions
//

// define for massive debug output
//#define DEBUG_MSG

#ifdef DEBUG_MSG
#define PRINT_MSG( msg ) cout << '[' << bsp_pid << "] " << msg << endl
#else
#define PRINT_MSG( msg )
#endif

////////////////////////////////////////
//
// local data types
//

// info about messages to send
typedef struct
{
    size_t size;
    char * data;
    bool   del;
} send_msg_info;

// info about messages received
typedef struct
{
    size_t  size;
    uint    source;
    char  * data;
} recv_msg_info;

////////////////////////////////////////
//
// local data
//

// flag which indicates that BSP was init.
static bool bsp_initialised = false;

// structure of the BSP machine
static uint bsp_pid    = 0;
static uint bsp_nprocs = 1;

// messages to send and received messages
static TArray< TSLL< send_msg_info > >  send_msgs;
static TSLL< recv_msg_info * >          recv_msgs, old_msgs;

// matrix of dimension nprocs * nprocs holding
// a pipe for each processor-pair
static TArray< int > pipe_read_matrix;
static TArray< int > pipe_write_matrix;

#define READ_PM(  i, j )  pipe_read_matrix[ ((i) * bsp_nprocs) + (j) ]
#define WRITE_PM( i, j )  pipe_write_matrix[ ((i) * bsp_nprocs) + (j) ]

#define UNDEF_PIPE  -1

// communication pattern for data exchange in sync
static uint          no_of_comm_it = 0;
static TArray< int > comm_matrix;

#define UNDEF_PROC  ((uint) -1)

#define COMM( i, j )  comm_matrix[ ((i) * bsp_nprocs) + (j) ]

// array of pid of child-processes
TArray< pid_t >  child_pids;

// received status for direct communication
TArray< bool >    drecv_size;
TArray< size_t >  drecv_msg_size;

////////////////////////////////////////
//
// local signal handler
//

extern "C" void
sig_handler ( int signo )
{
    if ( signo == SIGPIPE )
    {
        printf( "received SIGPIPE; inconsistent BSP steps ?" );
        exit( 1 );
    }// if
}

////////////////////////////////////////
//
// initialization and ending
//

int
init ( int * argc, char *** argv )
{
    if ( ! bsp_initialised )
    {
        //
        // read BSP_PROCS environment variable
        // and fork nprocs-1 processes
        //

        char * env = getenv( "BSP_PROCS" );

        if ( env != NULL )
        {
            bsp_nprocs = max( 1, atoi( env ) );

            if ( bsp_nprocs > 64 )
            {
                fprintf( stderr, "(BSP) init : too many processors requested, max = 64, requested = %d\n",
                         bsp_nprocs );
                return ERR_ARG;
            }// if
        }// if

        // pid of first process
        bsp_pid = 0;
            
        if ( bsp_nprocs > 1 )
        {
            child_pids.set_size( bsp_nprocs );

            // setup signal handler
            signal( SIGPIPE, sig_handler );
            
            //
            // set number of open files to higher value
            // to set up all pipes in process 0
            //
            
            struct rlimit  rl_old, rl_new;
            bool           rl_changed = false;

            getrlimit( RLIMIT_NOFILE, & rl_old );

            PRINT_MSG( "(BSP) init: no of open files cur/max = " <<
                       rl_old.rlim_cur << '/' << rl_old.rlim_max );

            if ( rlim_t(2*bsp_nprocs*bsp_nprocs) > rl_old.rlim_cur )
            {
                if ( rlim_t(2*bsp_nprocs*bsp_nprocs) > rl_old.rlim_max-3 )
                {
                    fprintf( stderr, "(BSP) init : can not open %d files, max = %d\n",
                             2*bsp_nprocs*bsp_nprocs, (int) rl_old.rlim_max );
                    return ERR_RES;
                }// if
                
                rl_changed = true;
                
                rl_new.rlim_cur = min( rlim_t(2*bsp_nprocs*bsp_nprocs), rl_old.rlim_max );
                rl_new.rlim_max = rl_old.rlim_max;

                if ( setrlimit( RLIMIT_NOFILE, & rl_new ) < 0 )
                {
                    fprintf( stderr, "(BSP) init : error in \"setrlimit\"\n" );
                    return ERR_SYSCALL;
                }// if
            }// if
            
            // 
            // build pipe-matrix
            //
            
            pipe_read_matrix.set_size( bsp_nprocs * bsp_nprocs );
            pipe_write_matrix.set_size( bsp_nprocs * bsp_nprocs );

            for ( uint i = 0; i < bsp_nprocs; i++ )
            {
                for ( uint j = 0; j < bsp_nprocs; j++ )
                {
                    READ_PM( i, j ) = WRITE_PM( i, j ) = UNDEF_PIPE;

                    if ( i == j )
                        continue;
                    
                    int  pd[2];
                    
                    if ( pipe( pd ) != 0 )
                    {
                        fprintf( stderr, "(BSP) init : error in \"pipe\"\n" );
                        return ERR_SYSCALL;
                    }// if

                    READ_PM(  i, j ) = pd[0];
                    WRITE_PM( i, j ) = pd[1];
                }// for
            }// for

            //
            // build communication pattern
            //

            uint                    it = 0;
            bool                    left = false;
            TArray< bool >          visited( bsp_nprocs * bsp_nprocs );
            TSLL< TArray< uint > >  pattern;

            for ( uint i = 0; i < bsp_nprocs*bsp_nprocs; i++ )
                visited[i] = false;
            
            while ( true )
            {
                TArray< bool >  used( bsp_nprocs );
                TArray< uint  > pat( bsp_nprocs );
                bool            have_comm = false;

                for ( uint i = 0; i < bsp_nprocs; i++ )
                {
                    used[i] = false;
                    pat[i]  = UNDEF_PROC;
                }// for
                
                //
                // for each processor, determine left/right neighbour
                // which was not yet visited
                //

                for ( uint i = 0; i < bsp_nprocs; i++ )
                {
                    uint  dest;
                    
                    // check if already assigned
                    if ( pat[ i ] != UNDEF_PROC )
                        continue;
                    
                    if ( left ) dest = (i + bsp_nprocs - 1) % bsp_nprocs;
                    else        dest = (i + bsp_nprocs + 1) % bsp_nprocs;

                    // check if dest. was visited before or
                    // if not accessible this turn
                    while ( visited[ (i * bsp_nprocs) + dest ] || used[ dest ] )
                    {
                        if ( left ) dest = (dest + bsp_nprocs - 1) % bsp_nprocs;
                        else        dest = (dest + bsp_nprocs + 1) % bsp_nprocs;

                        if ( dest == i )
                        {
                            // one complete turn, nothing to do
                            dest = UNDEF_PROC;
                            break;
                        }// if
                    }// while

                    // set communication pattern
                    if ( dest != UNDEF_PROC )
                    {
                        have_comm = true;
                        
                        // mark node as visited and used
                        used[ i ] = used[ dest ]         = true;
                        visited[ (i*bsp_nprocs) + dest ] = true;
                        visited[ (dest*bsp_nprocs) + i ] = true;
                        pat[ dest ]                      = i;
                    }// if
                    
                    pat[ i ] = dest;
                }// for
                
                // check if there was something to communicate
                // or stop iteration if not
                if ( have_comm )
                    pattern.append( pat );
                else
                    break;

                // alternate directions
                left = ! left;

                it++;
            }// while

            //
            // copy list of patterns into matrix
            //

            no_of_comm_it = it;
            
            comm_matrix.set_size( bsp_nprocs * it );

            it = 0;
            
            while ( pattern.size() > 0 )
            {
                TArray< uint >  pat = pattern.behead();

                for ( uint i = 0; i < bsp_nprocs; i++ )
                    COMM( it, i ) = pat[i];

                ++it;
            }// while
            
#ifdef DEBUG_MSG
            cout << "read:" << endl;
            for ( uint i = 0; i < bsp_nprocs; i++ )
            {
                for ( uint j = 0; j < bsp_nprocs; j++ )
                    printf( "%4d ", READ_PM(  i, j ) );
                cout << endl;
            }// for
            cout << endl;
            
            cout << "write:" << endl;
            for ( uint i = 0; i < bsp_nprocs; i++ )
            {
                for ( uint j = 0; j < bsp_nprocs; j++ )
                    printf( "%4d ", WRITE_PM(  i, j ) );
                cout << endl;
            }// for
            cout << endl;

            cout << "comm:" << endl;
            for ( uint i = 0; i < no_of_comm_it; i++ )
            {
                for ( uint j = 0; j < bsp_nprocs; j++ )
                    printf( "%4d ", COMM( i, j ) );
                cout << endl;
            }// for
            cout << endl;
#endif
            
            //
            // fork sup processes
            //

            for ( uint i = 1; i < bsp_nprocs; i++ )
            {
                pid_t  id = fork();

                if ( id  < 0 )
                {
                    fprintf( stderr, "(BSP) init : error in \"fork\"\n" );
                    return ERR_SYSCALL;
                }// if

                if ( id == 0 )
                {
                    bsp_pid = i;
                    break;
                }// if
                else
                    child_pids[i] = id;
            }// for
            
            //
            // close all unneeded pipes
            //
            
            for ( uint i = 0; i < bsp_nprocs; i++ )
            {
                for ( uint j = 0; j < bsp_nprocs; j++ )
                {
                    if ( j != bsp_pid )
                    {
                        close( READ_PM( i, j ) );
                        READ_PM( i, j ) = UNDEF_PIPE;
                    }// if
                    
                    if ( i != bsp_pid )
                    {
                        close( WRITE_PM( i, j ) );
                        WRITE_PM( i, j ) = UNDEF_PIPE;
                    }// if
                }// for
            }// for

            //
            // reset file usage
            //
            
            if ( rl_changed )
            {
                if ( setrlimit( RLIMIT_NOFILE, & rl_old ) < 0 )
                {
                    fprintf( stderr, "(BSP) init : error in \"setrlimit\"\n" );
                    return ERR_SYSCALL;
                }// if
            }// if
        }// if

        //
        // set up local variables
        //

        send_msgs.set_size( bsp_nprocs );

        drecv_size.set_size( bsp_nprocs );
        drecv_msg_size.set_size( bsp_nprocs );

        for ( uint i = 0; i < bsp_nprocs; i++ )
            drecv_size[i] = false;

        // finally initialised
        bsp_initialised = true;
    }// if
    else
        cout << '[' << bsp_pid << "] "
             << "(BSP) init : already initialised" << endl;

    return NO_ERROR;
}

int
done ()
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    //
    // close all open pipes
    //

    if ( bsp_nprocs > 1 )
    {
        for ( uint i = 0; i < bsp_nprocs; i++ )
        {
            for ( uint j = 0; j < bsp_nprocs; j++ )
            {
                if ( READ_PM( i, j ) != UNDEF_PIPE )
                {
                    close( READ_PM( i, j ) );
                    READ_PM( i, j ) = READ_PM( j, i ) = UNDEF_PIPE;
                }// if
            }// for
        }// for
    }// if

    //
    // free old received messages
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
    // wait for children to finish
    //

    if ( bsp_pid == 0 )
    {
        for ( uint i = 1; i < bsp_nprocs; i++ )
        {
            if ( waitpid( child_pids[i], NULL, 0 ) < 0 )
            {
                fprintf( stderr, "(BSP) done : error in \"waitpid\"\n" );
                return ERR_SYSCALL;
            }// if
        }// for
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

#define CHECK_WRITE( fn )  if ( ret < 0 ) { fprintf( stderr, "error in \"write\"\n" ); return ERR_SYSCALL; }
#define CHECK_READ(  fn )  if ( ret < 0 ) { fprintf( stderr, "error in \"read\"\n" );  return ERR_SYSCALL; }

int
sync ()
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    //
    // free old received messages
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
    // handle message from this to this processor
    // (just copy the content)
    //
            
    while ( send_msgs[bsp_pid].size() > 0 )
    {
        send_msg_info   sinfo = send_msgs[bsp_pid].behead();
        recv_msg_info * rinfo = new recv_msg_info;

        if ( rinfo == NULL )
            return ERR_MEM;
        
        rinfo->size = sinfo.size;
        rinfo->source = bsp_pid;

        if ( sinfo.del )
        {
            // re-use pointer, because we already copied the message
            rinfo->data = sinfo.data;
            sinfo.del   = false;
        }// if
        else
        {
            // real copy the content
            rinfo->data = new char[ rinfo->size ];

            if ( rinfo->data == NULL )
                return ERR_MEM;
        
            memcpy( rinfo->data, sinfo.data, rinfo->size );
        }// else
        
        recv_msgs.append( rinfo );
    }// while

    //
    // receive messages from all other nodes
    // use communication pattern and exchange data
    // with other processors
    //

    uint type[2];
    
    for ( uint it = 0; it < no_of_comm_it; it++ )
    {
        uint     dest = COMM( it, bsp_pid );
        int      n;
        ssize_t  ret;
        size_t   sum;

        // check if we have to skip this iteration
        if ( dest == UNDEF_PROC )
            continue;
        
        //
        // read messages from pipe and send messages to pipe
        // - avoid deadlocks by changing read/send for i < pid, i > pid
        // - send size of message first, then message itself
        //
        
        if ( dest < bsp_pid ) { type[0] = 1; type[1] = 2; }
        else                  { type[0] = 2; type[1] = 1; }

        for ( uint t = 0; t < 2; t++ )
        {
            if ( type[t] == 1 )
            {
                //
                // read from other processor
                //

                PRINT_MSG( "(BSP) sync : reading on pipe " << READ_PM( dest, bsp_pid ) );
                
                ret = read( READ_PM( dest, bsp_pid ), & n, sizeof( n ) );
                CHECK_READ( "(BSP) sync" );

                PRINT_MSG( "(BSP) sync : receiving " << n << " msgs from proc " << dest );
                
                for ( int j = 0; j < n; j++ )
                {
                    recv_msg_info * info = new recv_msg_info;

                    PRINT_MSG( "(BSP) sync : receving size from proc " << dest );
                    
                    ret = read( READ_PM( dest, bsp_pid ), & info->size, sizeof( info->size ) );
                    CHECK_READ( "(BSP) sync" );

                    info->source = dest;
                    info->data   = new char[ info->size ];

                    if ( info->data == NULL )
                        return ERR_MEM;
                    
                    PRINT_MSG( "(BSP) sync : reading " << info->size << " bytes from proc "
                               << dest );

                    // read in loop because message might be truncated
                    sum = 0;
                    while ( sum < info->size )
                    {
                        ret = read( READ_PM( dest, bsp_pid ), info->data + sum, info->size - sum );
                        CHECK_READ( "(BSP) sync" );

                        sum += ret;
                    }// while

                    PRINT_MSG( "(BSP) sync : data received from proc " << dest );
                    
                    recv_msgs.append( info );
                }// for
            }// if

            if ( type[t] == 2 )
            {
                //
                // write to other processor
                //

                PRINT_MSG( "(BSP) sync : writing to pipe " << WRITE_PM( bsp_pid, dest ) ); 
                
                n = send_msgs[dest].size();

                PRINT_MSG( "(BSP) sync : sending " << n << " msgs to proc " << dest );
                
                ret = write( WRITE_PM( bsp_pid, dest ), & n, sizeof( n ) );
                CHECK_WRITE( "(BSP) sync" );
                
                while ( send_msgs[dest].size() > 0 )
                {
                    send_msg_info  info = send_msgs[dest].behead();

                    PRINT_MSG( "(BSP) sync : sending size to proc " << dest );
                    
                    ret = write( WRITE_PM( bsp_pid, dest ), & info.size, sizeof( info.size ) );
                    CHECK_WRITE( "(BSP) sync" );

                    PRINT_MSG( "(BSP) sync : sending " << info.size << " bytes to proc " << dest );

                    ret = write( WRITE_PM( bsp_pid, dest ), info.data, info.size );
                    CHECK_WRITE( "(BSP) sync" );

                    // check if whole message was sent
                    if (( ret >= 0 ) && ( size_t(ret) != info.size ))
                    {
                        fprintf( stderr, "(BSP) sync : sent wrong number of bytes, %ld <> %ld\n",
                                 (long) ret, (long) info.size );
                        return ERR_COMM;
                    }// if

                    PRINT_MSG( "(BSP) sync : data sent to proc " << dest );
                    
                    if ( info.del )
                        delete info.data;
                }// while
            }// if
        }// for
    }// for

    //
    // global synchronisation
    //

    PRINT_MSG( "(BSP) sync : finished sync" );

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

    if ( dest >= uint(bsp_nprocs) )
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

    send_msgs[ dest ].append( info );

    s_step_send += bsize;

    return NO_ERROR;
}

int
hpsend  ( uint dest, const void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    if ( dest >= uint(bsp_nprocs) )
    {
        fprintf( stderr, "(BSP) hpsend : invalid destination\n" );
        return ERR_ARG;
    }// if
    
    send_msg_info  info;

    info.data = (char*) buf;
    info.size = bsize;
    info.del  = false;

    send_msgs[ dest ].append( info );

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

    if ( dest >= uint(bsp_nprocs) )
    {
        fprintf( stderr, "(BSP) dsend : invalid destination\n" );
        return ERR_ARG;
    }// if
    
    if ( dest == bsp_pid )
    {
        fprintf( stderr, "(BSP) dsend : source = destination\n" );
        return ERR_ARG;
    }// if
    
    int  ret;

    // first send size information for consistency checks
    ret = write( WRITE_PM( bsp_pid, dest ), & bsize, sizeof( size_t ) );
    CHECK_WRITE( "(BSP) dsend" );

    // now send message
    ret = write( WRITE_PM( bsp_pid, dest ), buf, bsize );
    CHECK_WRITE( "(BSP) dsend" );
    
    // check if whole message was sent
    if (( ret >= 0 ) && ( size_t(ret) != bsize ))
    {
        fprintf( stderr, "(BSP) dsend : sent wrong number of bytes, sent = %ld, req. = %ld\n",
                 (long) ret, (long) bsize );
        return ERR_COMM;
    }// if

    return NO_ERROR;
}

int
drecv ( unsigned int source, void * buf, size_t bsize )
{
    if ( ! bsp_initialised )
        return ERR_INIT;

    if ( source >= uint(bsp_nprocs) )
    {
        fprintf( stderr, "(BSP) drecv : invalid source\n" );
        return ERR_ARG;
    }// if
    
    if ( source == bsp_pid )
    {
        fprintf( stderr, "(BSP) drecv : source = destination\n" );
        return ERR_ARG;
    }// if
    
    int     ret;
    size_t  n;

    // first read size of message for consistency checks
    if ( drecv_size[ source ] == true )
        n = drecv_msg_size[ source ];
    else
    {
        ret = read( READ_PM( source, bsp_pid ), & n, sizeof( n ) );
        CHECK_READ( "(BSP) drecv" );
    }// else

    if ( n != bsize )
    {
        fprintf( stderr, "(BSP) drecv : available and requested size differ, %ld <> %ld\n",
                 (long) n, (long) bsize );
        return ERR_COMM;
    }// if

    // read in loop because message might be truncated
    n = 0;
    while ( n < bsize )
    {
        ret = read( READ_PM( source, bsp_pid ), ((char*) buf) + n, bsize - n );
        CHECK_READ( "(BSP) drecv" );
        
        n += ret;
    }// while

    // reset received status
    drecv_size[ source ] = false;

    return NO_ERROR;
}

//
// return size of message to receive
//
size_t
dprobe ( uint source )
{
    if ( ! bsp_initialised )
        return (size_t) ERR_INIT;

    if ( source >= uint(bsp_nprocs) )
    {
        fprintf( stderr, "(BSP) dprobe : invalid source\n" );
        return (size_t) ERR_ARG;
    }// if
    
    if ( source == bsp_pid )
    {
        fprintf( stderr, "(BSP) dprobe : source = destination\n" );
        return (size_t) ERR_ARG;
    }// if
    
    if ( drecv_size[ source ] == true )
        return drecv_msg_size[ source ];
    
    int     ret;
    size_t  n;

    ret = read( READ_PM( source, bsp_pid ), & n, sizeof( n ) );

    if ( ret < 0 )
    {
        fprintf( stderr, "error in \"read\"\n" );
        return 0;
    }// if

    drecv_size[ source ]     = true;
    drecv_msg_size[ source ] = n;
    
    return n;
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
    return ((recv_msg_info *) _msg)->source;
}

}// namespace
