#ifndef DENOMINATOR_H_
#define DENOMINATOR_H_

#include <map>
#include <boost/noncopyable.hpp>

class denominator: public boost::noncopyable {
public:
	denominator();
	virtual ~denominator();

	static const std::map<const char, double> denominators;
	double getDenominatorByChar(const char denominatorChar);
};

#endif /* DENOMINATOR_H_ */
