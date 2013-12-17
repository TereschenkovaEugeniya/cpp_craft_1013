#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>

namespace task5_4
{
	template< bool delete_first, typename Container >
	class Cleaner
	{
	public:
		Cleaner(Container& cont)
		{
			cont.clear();
		}
	};

	template<typename Container>
	class Cleaner<true, Container>
	{
	public:
		Cleaner(Container& cont)
		{
			for (auto it = cont.begin(); it != cont.end(); ++it)
			{
				delete *it;
			}

			cont.clear();
		}
	};

	template< bool delete_first, bool delete_second, typename Container >
	class AssocCleaner
	{
	public:
		AssocCleaner(Container& cont)
		{
			cont.clear();
		}
	};

	template< typename Container >
	class AssocCleaner<true, true, Container>
	{
	public:
		AssocCleaner(Container& cont)
		{
			for (auto it = cont.begin(); it != cont.end(); ++it)
			{
				delete it->first;
				delete it->second;
			}
			cont.clear();
		}
	};

	template< typename Container >
	class AssocCleaner<true, false, Container>
	{
	public:
		AssocCleaner(Container& cont)
		{
			for (auto it = cont.begin(); it != cont.end(); ++it)
			{
				delete it->first;
			}
			cont.clear();
		}
	};

	template< typename Container >
	class AssocCleaner<false, true, Container>
	{
	public:
		AssocCleaner(Container& cont)
		{
			for (auto it = cont.begin(); it != cont.end(); ++it)
			{
				delete it->second;
			}
			cont.clear();
		}
	};

	template< bool delete_first, typename Container >
	void clear_container( Container& cont )
	{
		Cleaner<delete_first, Container> cleaner(cont);
	}

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& cont )
	{
		AssocCleaner<delete_first, delete_second, Container> cleaner(cont);
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

