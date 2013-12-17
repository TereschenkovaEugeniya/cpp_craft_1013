#include "solution.h"

void task4_5::solution::render(){
	for(size_t i = 0; i < data_.size(); i++){
		for(size_t j = 0; j < data_[i].size(); j++){
			std::cout << data_[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

task4_5::solution::solution( const data_type& data ):
	data_(data), min_(0), max_(0)
{
	if(!data_.size()){
		return;
	}

	boost::thread_group crawlersThreadsPool;
	std::vector<VectorIntMinMax> crawlersPool;
	VectorIntMinMax crawler;

	for(size_t i = 0; i < data.size(); i++){
		crawlersPool.push_back(crawler);
	}

	for(size_t i = 0; i < data.size(); i++){
		crawlersPool[i].setData(data[i]);
		crawlersThreadsPool.create_thread(boost::bind(&VectorIntMinMax::process, &crawlersPool[i]));
	}
	crawlersThreadsPool.join_all();

	std::vector<int> commonMinsMaxs;
	for(size_t i = 0; i < data.size(); i++){
		commonMinsMaxs.push_back(crawlersPool[i].getMax());
		commonMinsMaxs.push_back(crawlersPool[i].getMin());
	}
	crawler.setData(commonMinsMaxs);
	crawler.process();
	min_ = crawler.getMin();
	max_ = crawler.getMax();
}

int task4_5::solution::get_min() const
{
	return min_;
}
int task4_5::solution::get_max() const
{
	return max_;
}
