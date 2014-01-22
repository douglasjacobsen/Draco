//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   c4/test/tstC4_Req.cc
 * \author Kelly Thompson
 * \date   Tue Nov  1 15:49:44 2005
 * \brief  Unit test for C4_Req class.
 * \note   Copyright (C) 2006-2014 Los Alamos National Security, LLC.
 *         All rights reserved.
 */
//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

#include "../global.hh"
#include "../SpinLock.hh"
#include "../ParallelUnitTest.hh"
#include "ds++/Release.hh"
#include <iostream>

using namespace std;

#define PASSMSG(A) ut.passes(A)
#define FAILMSG(A) ut.failure(A)
#define ITFAILS    ut.failure( __LINE__ )

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

void tstCopyConstructor(rtt_dsxx::UnitTest &ut)
{
    using rtt_c4::C4_Req;
    
    C4_Req requestA;
    C4_Req requestB( requestA );

    // The behavior of the copy constructor is not obvious.  If requestA has
    // not been used (inuse() returns 0) then requestA != requestB.
    
    if( requestA.inuse() == 0 && requestA == requestB  )
        FAILMSG("requestA.inuse()==0, so requestA cannot == requestB.");

    if( requestA.inuse() == 0 && requestA != requestB  )
        PASSMSG("requestA.inuse()==0 and requestA != requestB.");

    if( requestA.inuse() == 1 && requestA == requestB  )
        PASSMSG("requestA.inuse()=1 and requestA == requestB.");

    if( requestA.inuse() == 1 && requestA != requestB  )
        FAILMSG("requestA.inuse()=1, so requestA must == requestB.");

    if( ut.numFails == 0 ) 
        PASSMSG("tstCopyConstructor() is okay.");
    
    return;
}

//---------------------------------------------------------------------------//

void tstTraits(rtt_dsxx::UnitTest &ut)
{
    using std::cout;
    using std::endl;
    using rtt_c4::C4_Traits;

    {
        rtt_c4::HSyncSpinLock headsyncspinlock;
        
        if( C4_Traits<unsigned char>::tag  != 432 ) ITFAILS;
        if( C4_Traits<short>::tag          != 433 ) ITFAILS;
        if( C4_Traits<unsigned short>::tag != 434 ) ITFAILS;
        if( C4_Traits<unsigned int>::tag   != 436 ) ITFAILS;
        if( C4_Traits<unsigned long>::tag  != 438 ) ITFAILS;
        if( C4_Traits<long double>::tag    != 441 ) ITFAILS;
    }
#ifdef C4_MPI
    {
        using rtt_c4::MPI_Traits;
        rtt_c4::TSyncSpinLock tailsyncspinlock;
        
        if( MPI_Traits<unsigned char>::element_type()  != MPI_UNSIGNED_CHAR )  ITFAILS;
        if( MPI_Traits<short>::element_type()          != MPI_SHORT )          ITFAILS;
        if( MPI_Traits<unsigned short>::element_type() != MPI_UNSIGNED_SHORT ) ITFAILS;
        if( MPI_Traits<unsigned int>::element_type()   != MPI_UNSIGNED )       ITFAILS;
        if( MPI_Traits<unsigned long>::element_type()  != MPI_UNSIGNED_LONG )  ITFAILS;
        if( MPI_Traits<long double>::element_type()    != MPI_LONG_DOUBLE )    ITFAILS;
    }
#endif
    
    return;
}

//---------------------------------------------------------------------------------------//
void tstWait(rtt_dsxx::UnitTest &ut)
{
    using namespace rtt_c4;
    
    if (rtt_c4::node()>0)
    {
        cout << "sending from processor " << get_processor_name() << ':' << endl;
        int buffer[1];
        buffer[0] = node();
        C4_Req outgoing = send_async(buffer, 1U, 0);
        unsigned result = wait_any(1U, &outgoing);
        if (result!=0) ITFAILS;
    }
    else
    {
        cout << "receiving to processor " << get_processor_name() << ':' << endl;
        Check(rtt_c4::nodes()<5);
        C4_Req requests[4];
        bool done[4];
        for (int p=1; p<nodes(); ++p)
        {
            int buffer[4][1];
            requests[p] = receive_async(buffer[p], 1U, p);
            done[p] = false;
        }
        for (int c=1; c<nodes(); ++c)
        {
            unsigned result = wait_any(nodes(), requests);
            if (done[result]) ITFAILS;
            done[result] = true;
        }
        for (int p=1; p<nodes(); ++p)
            if (!done[p]) ITFAILS;
    }
    return;
}

//---------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
    rtt_c4::ParallelUnitTest ut(argc, argv, rtt_dsxx::release);
    try
    {
        // >>> UNIT TESTS
        tstCopyConstructor(ut);
        tstTraits(ut);
        tstWait(ut);
    }
    UT_EPILOG(ut);
}   

//---------------------------------------------------------------------------//
// end of tstC4_Req.cc
//---------------------------------------------------------------------------//
