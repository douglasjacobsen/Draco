//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   ds++/Soft_Equivalence.hh
 * \author Thomas M. Evans and Todd Urbatsch
 * \date   Wed Nov  7 14:10:55 2001
 * \brief  Soft_Equivalence functions for floating point comparisons.
 * \note   Copyright (C) 2010-2012 Los Alamos National Security, LLC.
 *         All rights reserved.
 */
//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

#ifndef __ds_Soft_Equivalence_hh__
#define __ds_Soft_Equivalence_hh__

//===========================================================================//
// Soft_Equivalence
//
// Purpose : checks that two reals or fields of reals are within a tolerance
// of each other.
//
// revision history:
// -----------------
// 0) original
// 1) 01-24-02 : modified the soft_equiv function to allow using an absolute 
//               error check if the reference value is within 1.0e-14 of zero.
//
//===========================================================================//

#include "Assert.hh"
#include <cmath>
#include <iterator>

namespace rtt_dsxx
{

//===========================================================================//
// SCALAR SOFT EQUIVALENCE FUNCTIONS
//===========================================================================//
/*!
 * \brief Compare two floating point scalars for equivalence to a specified
 * tolerance. 
 *
 * \param value scalar floating point value
 *
 * \param reference scalar floating point reference to which value is
 * compared
 * 
 * \param precision tolerance of relative error (default 1.0e-12)
 *
 * \return true if values are the same within relative error specified by
 * precision, false if otherwise
 *
 * \todo Should we be using numeric_limits instead of hard coded vales for
 *       e-12 and e-14?
 */
template<typename FPT>
inline bool soft_equiv( const FPT &value, 
                        const FPT &reference, 
                        const FPT precision = 1.0e-12)
{
    using std::fabs;
    bool passed = false;

    if (fabs(value - reference) < precision * fabs(reference))
        passed = true;
    else
        passed = false;
    
    // second chance for passing if reference is within machine error of zero
    if (!passed && (fabs(reference) < 1.0e-14))
        if (fabs(value) < precision) passed = true;
    
    return passed;
}

//---------------------------------------------------------------------------//

template<>
inline bool soft_equiv( const int & /* value */, 
                        const int & /* reference */,
                        const int   /* precision */ )
{
    Insist (0, "Can't do a soft compare with integers!");
    return false;
}

//===========================================================================//
// FIELD SOFT EQUIVALENCE FUNCTIONS
//===========================================================================//
/*!
 * \brief Object that allows multilevel STL containers of floating point
 * values to be compared within a tolerance.
 *
 * \param Depth levels of containers for analysis (2 for vector<vector<T>>).
 *
 * This class provides a template recursion object that allows two STL
 * containers to be compared element-by-element no matter how many levels of
 * containers exist.  The value and reference fields must have STL-type
 * iterators.  The value-types of both fields must be the same or a
 * compile-time error will result.
 *
 * This class is specialized for Depth=1.  This is the lowest level of
 * recursion and is the level where actual numeric comparison occurs.
 *
 * Typical use:
 *
 * \code
 * vector<double> const ref = { 0.1, 0.2 };
 * vector<double> val       = { 0.1, 0.19999999999};
 * if( soft_equiv_deep<1>().equiv( val.begin(), val.end(),
 *     ref.begin(), ref.end(), 0.000001 ) )
 *   cout << "arrays match" << endl;
 * \endcode
 */
template<unsigned Depth, typename FPT=double >
class soft_equiv_deep
{
  public:
    // Constructor
    soft_equiv_deep(void) { /* empty */ }

    /*!
     * \brief Compare two multi-level floating point fields for equivalence to a
     * specified tolerance.  
     *
     * \param value floating point field of values
     * \param value_end one past the end of the floating point field of values
     * \param reference floating point field to which values are compared
     * \param reference_end one past the end of the floating point field to which values are compared
     * \param precision tolerance of relative error (default 1.0e-12)
     * \return true if values are the same within relative error specified by
     * precision and the fields are the same size, false if otherwise
     */
     template<typename Value_Iterator, typename Ref_Iterator>
     bool equiv(
        Value_Iterator value,
        Value_Iterator value_end,
        Ref_Iterator ref,
        Ref_Iterator ref_end,
        FPT const precision = 1.0e-12 )
    {
        // first check that the sizes are equivalent
        if (std::distance(value, value_end) != std::distance(ref, ref_end))
            return false;
        
        // if the sizes are the same, loop through and check each element
        bool passed = true;
        while (value != value_end && passed == true)
        {
            passed = soft_equiv_deep<Depth-1,FPT>().equiv(
                (*value).begin(), (*value).end(),
                (*ref).begin(), (*ref).end(), precision);
            value++;
            ref++;
        }
        return passed;   
    }
};

//! Specialization for Depth=1 case:
template<typename FPT>
class soft_equiv_deep<1,FPT>
{
  public:
    // Constructor
    soft_equiv_deep<1,FPT>(void) { /* empty */ }
    template<typename Value_Iterator, typename Ref_Iterator>
    bool equiv(
        Value_Iterator value,
        Value_Iterator value_end,
        Ref_Iterator ref,
        Ref_Iterator ref_end,
        FPT const precision = 1.0e-12 )
    {
        // first check that the sizes are equivalent
        if (std::distance(value, value_end) != std::distance(ref, ref_end))
            return false;
        
        // if the sizes are the same, loop through and check each element
        bool passed = true;
        while (value != value_end && passed == true)
        {
            passed = soft_equiv(*value, *ref, precision);
            value++;
            ref++;
        }
        return passed;   
    }
};

//===========================================================================//
// FIELD SOFT EQUIVALENCE FUNCTIONS
//===========================================================================//
/*!
 * \brief Compare two floating point fields for equivalence to a specified
 * tolerance. 
 *
 * \param value  floating point field of values
 *
 * \param reference floating point field to which values are compared
 * 
 * \param precision tolerance of relative error (default 1.0e-12)
 *
 * \return true if values are the same within relative error specified by
 * precision and the fields are the same size, false if otherwise
 *
 * The field soft_equiv check is an element-by-element check of two
 * single-dimension fields.  The precision is the same type as the value
 * field.  The value and reference fields must have STL-type iterators.  The
 * value-types of both fields must be the same or a compile-time error will
 * result. 
 */
template<typename Value_Iterator, typename Ref_Iterator>
inline bool soft_equiv(
    Value_Iterator value,
    Value_Iterator value_end,
    Ref_Iterator ref,
    Ref_Iterator ref_end,
    typename std::iterator_traits<Value_Iterator>::value_type const precision 
    = 1.0e-12)
{
    typedef typename std::iterator_traits<Value_Iterator>::value_type FPT;
    return soft_equiv_deep<1,FPT>().equiv(value, value_end,
                                          ref, ref_end, precision);
}

} // end namespace rtt_dsxx

#endif // __ds_Soft_Equivalence_hh__

//---------------------------------------------------------------------------//
// end of ds++/Soft_Equivalence.hh
//---------------------------------------------------------------------------//
