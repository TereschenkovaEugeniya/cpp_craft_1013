#include "VectorIntMinMax.h"

VectorIntMinMax::VectorIntMinMax():
min_(-1),
max_(1) {

}

VectorIntMinMax::VectorIntMinMax(const std::vector<int>& data):
	min_(-1),
	max_(1),
	data_(data){

}

VectorIntMinMax::~VectorIntMinMax() {
}

void VectorIntMinMax::process() {
	if(!data_.size()){
		return;
	}
	min_ = data_[0];
	max_ = data_[0];

	for(std::vector<int>::const_iterator it = data_.begin() + 1; it < data_.end(); ++it){
		min_ = std::min(min_, *it);
		max_ = std::max(max_, *it);
	}
}

void VectorIntMinMax::setData(const std::vector<int>& data){
	data_ = data;
}

int VectorIntMinMax::getMin() {
	return min_;
}

int VectorIntMinMax::getMax() {
	return max_;
}
