//==============================================================================
//         Copyright 2014          LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2014          LRI    UMR 8623 CNRS/Univ Paris Sud XI
//         Copyright 2014          MetaScale SAS
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef BOOST_NUMERIC_ODEINT_EXTERNAL_NT2_NT2_ALGEBRA_DISPATCHER_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_EXTERNAL_NT2_NT2_ALGEBRA_DISPATCHER_HPP_INCLUDED

#include <nt2/table.hpp>

namespace boost { namespace numeric { namespace odeint {
template<typename T, typename S>
struct algebra_dispatcher<nt2::container::table<T,S> >
{
  typedef vector_space_algebra algebra_type;
};
} } }

#endif
