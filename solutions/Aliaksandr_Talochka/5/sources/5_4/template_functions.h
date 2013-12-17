#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>
#include <boost/utility.hpp>
#include <boost/type_traits.hpp>


namespace task5_4
{
	template<typename Iterator>
	void deleter(Iterator start, Iterator end)
	{
		Iterator it;
		for(it = start; it != end; ++it)
		{
			delete *it;
		}
	}

	template< bool delete_first, typename Container >
	typename boost::enable_if_c< delete_first >::type clear_container( Container& cntnr)
	{
		deleter(cntnr.begin(), cntnr.end());

		cntnr.clear();		 
	}

	template< bool delete_first, typename Container >
	typename boost::enable_if_c< !delete_first >::type clear_container( Container& cntnr)
	{
		
		cntnr.clear();		 
	}

	template< bool delete_first, bool delete_second, typename Container >
	typename boost::enable_if_c< (delete_first) && (delete_second)>::type clear_container( Container& cntnr)
	{

		Container::iterator it;
		for(it = cntnr.begin(); it != cntnr.end(); ++it)
		{
				delete it->first;
				delete it->second;
		}
		
		cntnr.clear();
	}

	template< bool delete_first, bool delete_second, typename Container >
	typename boost::enable_if_c< (!delete_first) && (delete_second)>::type clear_container( Container& cntnr)
	{
		Container::iterator it;
		for(it = cntnr.begin(); it != cntnr.end(); ++it)
		{
				delete it->second;
		}
		cntnr.clear();
	}
	
	template< bool delete_first, bool delete_second, typename Container >
	typename boost::enable_if_c< (delete_first) && (!delete_second)>::type clear_container( Container& cntnr)
	{

		Container::iterator it;
		for(it = cntnr.begin(); it != cntnr.end(); ++it)
		{
				delete it->first;
		}
		cntnr.clear();
	}
	
	template< bool delete_first, bool delete_second, typename Container >
	typename boost::enable_if_c< (!delete_first) && (!delete_second)>::type clear_container( Container& cntnr)
	{

		cntnr.clear();
	}

}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

