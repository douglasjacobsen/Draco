//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   c4/C4_MPI_gather_scatter_pt.cc
 * \author Thomas M. Evans
 * \date   Mon Mar 25 14:44:54 2002
 * \brief  C4 MPI non-blocking send/recv instantiations.
 * \note   Copyright (C) 2002-2014 Los Alamos National Security, LLC.
 *         All rights reserved.
 */
//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

#include "c4/config.h"

#ifdef C4_MPI

#include "C4_MPI.t.hh"

namespace rtt_c4
{

//---------------------------------------------------------------------------//
// EXPLICIT INSTANTIATIONS OF GATHER/SCATTER
//---------------------------------------------------------------------------//

template DLL_PUBLIC 
int gather<unsigned>(unsigned * send_buffer,
                     unsigned * receive_buffer,
                     int        size);

template DLL_PUBLIC 
int gather<int>(int * send_buffer,
                int * receive_buffer,
                int   size);

template DLL_PUBLIC 
int gather<char>(char * send_buffer,
                 char * receive_buffer,
                 int          size);

//----------//

template DLL_PUBLIC 
int allgather<int>(int * send_buffer,
                   int * receive_buffer,
                   int   size);

//----------//

template
int gatherv<unsigned>(unsigned * send_buffer,
                      int        send_size,
                      unsigned * receive_buffer,
                      int      * receive_sizes,
                      int      * receive_displs);

template
int gatherv<int>(int * send_buffer,
                 int   send_size,
                 int * receive_buffer,
                 int * receive_sizes,
                 int * receive_displs);

template
int gatherv<double>(double * send_buffer,
                    int      send_size,
                    double * receive_buffer,
                    int    * receive_sizes,
                    int    * receive_displs);
template
int gatherv<char>(  char * send_buffer,
                    int      send_size,
                    char * receive_buffer,
                    int    * receive_sizes,
                    int    * receive_displs);

//----------//

template DLL_PUBLIC 
int scatter<unsigned>(unsigned * send_buffer,
                      unsigned * receive_buffer,
                      int        size);

template DLL_PUBLIC 
int scatter<int>(int * send_buffer,
                 int * receive_buffer,
                 int   size);

template
int scatterv<unsigned>(unsigned * send_buffer,
                       int      * send_sizes,
                       int      * send_displs,
                       unsigned * receive_buffer,
                       int        receive_size);

template
int scatterv<int>(int * send_buffer,
                  int * send_sizes,
                  int * send_displs,
                  int * receive_buffer,
                  int   receive_size);

template
int scatterv<double>(double * send_buffer,
                     int    * send_sizes,
                     int    * send_displs,
                     double * receive_buffer,
                     int      receive_size);

} // end namespace rtt_c4

#endif // C4_MPI

//---------------------------------------------------------------------------//
// end of C4_MPI_gather_scatter_pt.cc
//---------------------------------------------------------------------------//
