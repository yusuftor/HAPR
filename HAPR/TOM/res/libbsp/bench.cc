//
// Project : libbsp
// File    : bench.cc
// Purpose : benchmark
// Author  : Ronald Kriemann
// License : GNU Lesser General Public License
//
// arch-tag: 6b6f3edd-8ff0-478a-856c-9ac7759a5427
//

#include <sys/time.h>

#include <cstdlib>
#include <iostream>

#include "BSP.hh"

//
// measure walltime
//
static double
second ()
{
	struct timeval  tp;
	struct timezone tzp;

	gettimeofday( & tp, & tzp );
    
	return ((double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

//
// tree benchmark
//
static void
benchmark_tree ( uint n, uint loops )
{
    double  * data     = new double[n];
    uint      nprocs   = BSP::nprocs();
    uint      pid      = BSP::pid();
    double    msg_size = double(loops) * double(nprocs-1) * double(n) * double(sizeof(double));
    double    start, end;

    start = second();

    for ( uint i = 0; i < loops; ++i )
    {
        uint    nparts = nprocs;
        bool    active = true;
        uint    diff   = 1;
        
        while ( nparts > 1 )
        {
            if ( active )
            {
                if ( pid % (2*diff) != 0 )
                {
                    uint dest = pid - diff;
                    
                    active = false;
                    
                    BSP::dsend( dest, data, sizeof(double) * n );
                }// if
                else
                {
                    uint  source = pid + diff;
                    
                    BSP::drecv( source, data, sizeof(double) * n );
                }// else
            }// if
            
            diff   *= 2;
            nparts /= 2;
        }// while
    }// for

    end = second();
    if ( pid == 0 )
    {
        std::cout << "tree broadcasting:" << std::endl;
        std::cout << "  time for com = " << end-start << " sec" << std::endl;
        std::cout << "  bandwidth    = " << msg_size / (1024.0*1024.0*(end-start)) << " MB/s" << std::endl;
    }// if
}

//
// linear broadcasting benchmark
//
static void
benchmark_linear ( uint n, uint loops )
{
    double  * data     = new double[n];
    uint      nprocs   = BSP::nprocs();
    uint      pid      = BSP::pid();
    double    msg_size = double(loops) * double(nprocs-1) * double(n) * double(sizeof(double));
    double    start, end;

    start = second();

    for ( uint i = 0; i < loops; ++i )
    {
        if ( pid != 0 )
            BSP::dsend( 0, data, sizeof(double) * n );
        else
        {
            for ( uint j = 1; j < nprocs; j++ )
                BSP::drecv( j, data, sizeof(double) * n );
        }// else
            
    }// for

    end = second();
    if ( pid == 0 )
    {
        std::cout << "linear broadcasting:" << std::endl;
        std::cout << "  time for com = " << end-start << " sec" << std::endl;
        std::cout << "  bandwidth    = " << msg_size / (1024.0*1024.0*(end-start)) << " MB/s" << std::endl;
    }// if
}

//
// main function
//
int
main ( int argc, char ** argv )
{
    if ( BSP::init( & argc, & argv ) != BSP::NO_ERROR )
        return -1;

    if ( BSP::nprocs() == 1 )
    {
        std::cout << "at least 2 processors are needed" << std::endl;
        return -1;
    }// if
    
    uint n     = 10*1024*1024 / sizeof(double);
    uint loops = 10;

    if ( argc > 1 ) n     = atoi( argv[1] ) * 1024 * 1024 / sizeof(double);
    if ( argc > 2 ) loops = atoi( argv[2] );

    if ( BSP::pid() == 0 )
        std::cout << "running benchmark with " << double(n*sizeof(double))/(1024.0*1024.0) << " MB and "
                  << loops << " loops" << std::endl;
    
    benchmark_tree( n, loops );
    benchmark_linear( n, loops );

    BSP::done();

    return 0;
}
