//
// Project   : libbsp
// File      : BSP.cc
// Copyright : Ronald Kriemann
// Purpose   : encapsulation of BSP functions
// License   : GNU Lesser General Public License
//
// arch-tag: bf2e5734-5187-46d7-9736-36e2f0670070
//

#include <unistd.h>

#define SEQ  1
#define PUB  2
#define MPI  3
#define SHM  4

#include "BSP.hh"

//
// local variables for statistics
//

namespace BSP
{

// communication statistics
static double   s_all_send      = 0.0;
static size_t   s_step_send     = 0;
static size_t   s_max_step_send = 0;

// step statistics
static unsigned n_steps         = 0;

}

//
// decide what kind of BSP implementation should be used
//

#if BSP_TYPE == SEQ
#include "BSP_seq.cc"
#endif

#if BSP_TYPE == PUB
#include "BSP_pub.cc"
#endif

#if BSP_TYPE == MPI
#include "BSP_mpi.cc"
#endif

#if BSP_TYPE == SHM
#include "BSP_shm.cc"
#endif

//
// local functions
// 

namespace BSP
{

//
// print statistics
//
void
statistics ()
{
    printf( "(BSP) statistics : steps / last / all / max = %d / %d / %.2e / %d\n",
            n_steps,
            s_step_send,
            s_all_send / (1024.0 * 1024.0),
            s_max_step_send );

    // reset statistics
    s_all_send      = 0.0;
    s_step_send     = 0;
    s_max_step_send = 0;
    n_steps         = 0;
}

}// namespace
