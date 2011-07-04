/*
 boost header: numeric/odeint/explicit_stepper_base.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky
 Copyright 2009 Andre Bergner

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_NUMERIC_ODEINT_EXPLICIT_STEPPER_BASE_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_EXPLICIT_STEPPER_BASE_HPP_INCLUDED

#include <iostream>

#include <boost/ref.hpp>

//#include <boost/numeric/odeint/util/size_adjuster.hpp>
//#include <boost/numeric/odeint/util/construct.hpp>
//#include <boost/numeric/odeint/util/destruct.hpp>
//#include <boost/numeric/odeint/util/copy.hpp>

#include <boost/numeric/odeint/util/state_wrapper.hpp>
#include <boost/numeric/odeint/util/resizer.hpp>

#include <boost/numeric/odeint/stepper/stepper_categories.hpp>


namespace boost {
namespace numeric {
namespace odeint {

/*
 * base class for explicit steppers
 * models the stepper concept
 */
template<
	class Stepper ,
	unsigned short Order ,
	class State ,
	class Value ,
	class Deriv ,
	class Time ,
	class Algebra ,
	class Operations ,
	class Resizer
>
class explicit_stepper_base
{
public:


	typedef State state_type;
	typedef Value value_type;
	typedef Deriv deriv_type;
	typedef Time time_type;
	typedef Algebra algebra_type;
	typedef Operations operations_type;
	typedef Resizer resizer_type;
	typedef Stepper stepper_type;
	typedef stepper_tag stepper_category;
	typedef state_wrapper< state_type > wrapped_state_type;
	typedef state_wrapper< deriv_type > wrapped_deriv_type;

	typedef explicit_stepper_base< Stepper , Order , State , Value , Deriv , Time , Algebra , Operations , Resizer > internal_stepper_base_type;

	typedef unsigned short order_type;
	static const order_type order_value = Order;


	order_type order( void ) const
    {
    	return order_value;
    }


/*	explicit_stepper_base( void ) : m_resizer() , m_dxdt()
	{ }

	explicit_stepper_base( const explicit_stepper_base &b ) : m_resizer() , m_dxdt( b.m_dxdt )
	{ }

	explicit_stepper_base& operator=( const explicit_stepper_base &b )
	{
		boost::numeric::odeint::copy( b.m_dxdt , m_dxdt );

		return *this;
	}
*/


	/*
	 * Version 1 : do_step( sys , x , t , dt )
	 *
	 * the two overloads are needed in order to solve the forwarding problem
	 */
	template< class System , class StateInOut >
	void do_step( System system , StateInOut &x , const time_type &t , const time_type &dt )
	{
		do_step_v1( system , x , t , dt );
	}

	template< class System , class StateInOut >
	void do_step( System system , const StateInOut &x , const time_type &t , const time_type &dt )
	{
		do_step_v1( system , x , t , dt );
	}


	/*
	 * Version 2 : do_step( sys , x , dxdt , t , dt )
	 *
	 * this version does not solve the forwarding problem, boost.range can not be used
	 */
	template< class System , class StateInOut , class DerivIn >
	void do_step( System system , StateInOut &x , const DerivIn &dxdt , const time_type &t , const time_type &dt )
	{
		this->stepper().do_step_impl( system , x , dxdt , t , x , dt );
	}


	/*
	 * Version 3 : do_step( sys , in , t , out , dt )
	 *
	 * this version does not solve the forwarding problem, boost.range can not be used
	 */
	template< class System , class StateIn , class StateOut >
	void do_step( System system , const StateIn &in , const time_type &t , StateOut &out , const time_type &dt )
	{
		typename boost::unwrap_reference< System >::type &sys = system;
		m_resizer.adjust_size( *this , in );
		sys( in , m_dxdt ,t );
		this->stepper().do_step_impl( system , in , m_dxdt , t , out , dt );
	}


	/*
	 * Version 4 : do_step( sys , in , dxdt , t , out , dt )
	 *
	 * this version does not solve the forwarding problem, boost.range can not be used
	 */
	template< class System , class StateIn , class DerivIn , class StateOut >
	void do_step( System system , const StateIn &in , const DerivIn &dxdt , const time_type &t , StateOut &out , const time_type &dt )
	{
		this->stepper().do_step_impl( system , in , dxdt , t , out , dt );
	}




	template< class StateType >
	bool adjust_size( const StateType &x )
	{
        return adjust_size_by_resizability( x , typename boost::numeric::odeint::is_resizeable< deriv_type >::type() );
	}


private:

	template< class System , class StateInOut >
	void do_step_v1( System system , StateInOut &x , const time_type &t , const time_type &dt )
	{
		typename boost::unwrap_reference< System >::type &sys = system;
		m_resizer.adjust_size( *this , x );
		sys( x , m_dxdt.m_v ,t );
		this->stepper().do_step_impl( system , x , m_dxdt.m_v , t , x , dt );
	}


	template< class StateType >
	bool adjust_size_by_resizability( const StateType &x , boost::true_type )
	{
	    if( !m_dxdt.same_size( x ) )
        {
            m_dxdt.resize( x );
            this->stepper().resize_impl( x );
            return true;
        } else {
            return false;
        }
	}

	template< class StateType >
    bool adjust_size_by_resizablity( const StateType &x , boost::false_type )
    {
	    return false;
    }


protected:

    stepper_type& stepper( void )
    {
    	return *static_cast< stepper_type* >( this );
    }

    const stepper_type& stepper( void ) const
    {
    	return *static_cast< const stepper_type* >( this );
    }


	resizer_type m_resizer;
	wrapped_deriv_type m_dxdt;
};


} // odeint
} // numeric
} // boost

#endif //BOOST_NUMERIC_ODEINT_EXPLICIT_STEPPER_BASE_HPP_INCLUDED
