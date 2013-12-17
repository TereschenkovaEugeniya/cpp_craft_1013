#ifndef VECTORINTMINMAX_H_
#define VECTORINTMINMAX_H_

#include <vector>
#include <iostream>

class VectorIntMinMax {
private:
	int min_;
	int max_;
	std::vector<int> data_;
public:
	VectorIntMinMax();
	VectorIntMinMax(const std::vector<int>& data);
	~VectorIntMinMax();

	void process();
	void setData(const std::vector<int>& data);
	int getMin();
	int getMax();
};

#endif /* VECTORINTMINMAX_H_ */
