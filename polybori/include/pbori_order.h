// -*- c++ -*-
//*****************************************************************************
/** @file pbori_order.h
 *
 * @author Alexander Dreyer
 * @date 2006-05-23
 *
 * 
 *
 * @par Copyright:
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.1  2006/05/23 15:26:25  dreyer
 * CHANGE BoolePolyRing  can handle different orderings (only lex yet)
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

#include "OrderedManager.h"

// get all available orderings
#include "LexOrder.h"

BEGIN_NAMESPACE_PBORI

CTypes::manager_ptr
get_ordered_manager(CTypes::size_type nvars, CTypes::ordercode_type order) {

  if(order == CTypes::lp)
    return CTypes::manager_ptr(new OrderedManager<CTypes::manager_base,
    LexOrder>(nvars) );
  else                          // default is lex order
    return CTypes::manager_ptr(new OrderedManager<CTypes::manager_base,
    LexOrder>(nvars) );
}
END_NAMESPACE_PBORI
