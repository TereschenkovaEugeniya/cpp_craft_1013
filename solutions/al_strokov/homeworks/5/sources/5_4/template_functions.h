#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>
#include <cstddef>
#include <set>
#include <list>
#include <map>

namespace task5_4
{
	template<typename Container, typename HelperValue>
	void helper_clear_container(Container& C, HelperValue){
		C.clear();
	}

	template<typename Container, typename HelperValue>
	void helper_clear_container(Container& C, HelperValue*){
		typename Container::iterator it;
		for(it = C.begin(); it != C.end(); ++it){
			delete *it;
		}
		C.clear();
	}

	template<typename Container>
	void clear_container(Container& C){
		if(C.size()){
			helper_clear_container(C, *C.begin());
		}
	}

	template<typename F, typename S>
	void clear_container(std::map<F,S>& m){
		m.clear();
	}

	template<typename F, typename S>
	void clear_container(std::map<F*,S>& m){
		typename std::map<F*,S>::iterator it;
		for(it = m.begin(); it != m.end(); it++){
			delete it->first;
		}
		m.clear();
	}

	template<typename F, typename S>
	void clear_container(std::map<F,S*>& m){
		typename std::map<F,S*>::iterator it;
		for(it = m.begin(); it != m.end(); it++){
			delete it->second;
		}
		m.clear();
	}

	template<typename F, typename S>
	void clear_container(std::map<F*,S*>& m){
		typename std::map<F*,S*>::iterator it;
		for(it = m.begin(); it != m.end(); it++){
			delete it->first;
			delete it->second;
		}
		m.clear();
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

