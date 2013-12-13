#include "multicast_communication.h"
#include "market_data_processor.h"
#include "market_data_receiver.h"

#include <iostream>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <string>
#include <signal.h>
#include <fstream>

multicast_communication *mc_p_;
market_data_receiver *mdr_p_;

void postProcess(){
	std::cout << "[postProcessing]" << std::endl;

	std::ofstream ofs;
	ofs.open("market_data.dat");
	if(!ofs.is_open()){
		std::cout << "cannot open output file" << std::endl;
		return;
	}

	for (size_t i = 0; i < mdr_p_->quotes_.size(); ++i) {
		ofs << "Q " << mdr_p_->quotes_[i].security_symbol << std::fixed <<
				" " << mdr_p_->quotes_[i].bid_price << " " << mdr_p_->quotes_[i].bid_volume <<
				" " << mdr_p_->quotes_[i].offer_price << " " << mdr_p_->quotes_[i].offer_volume <<
				std::endl;
	}

	for (size_t i = 0; i < mdr_p_->trades_.size(); ++i) {
		ofs << "T " << mdr_p_->trades_[i].security_symbol << std::fixed <<
				" " << mdr_p_->trades_[i].price << " " << mdr_p_->trades_[i].volume <<
				std::endl;
	}

	ofs.close();

	std::cout << "DONE" << std::endl;
}

void signalHandler(int signalCode) {
	if (signalCode == SIGINT) {
		std::cout << "\ngot <CTRL+C>\nshutting down" << std::endl;
		mc_p_->kill();
	postProcess();
		exit(EXIT_SUCCESS);
	}
}

void readSettingsFromFile(size_t &tradeThreadsCount, size_t &quoteThreadsCount,
		udpEndpoints_t &tradesEps, udpEndpoints_t &quotesEps) {
	std::ifstream ifs;
	ifs.open("config.ini");
	if(ifs.is_open()){
		std::cout << "config file norm" << std::endl;
	}else{
		std::cout << "config file cannot be opened" << std::endl;

		tradeThreadsCount = 2;
		quoteThreadsCount = 2;

		tradesEps.push_back(udpEndpoint_t("233.200.79.128", 62128));
		tradesEps.push_back(udpEndpoint_t("233.200.79.129", 62129));
		tradesEps.push_back(udpEndpoint_t("233.200.79.130", 62130));
		tradesEps.push_back(udpEndpoint_t("233.200.79.131", 62131));

		quotesEps.push_back(udpEndpoint_t("233.200.79.0", 61000));
		quotesEps.push_back(udpEndpoint_t("233.200.79.1", 61001));
		quotesEps.push_back(udpEndpoint_t("233.200.79.2", 61002));
		quotesEps.push_back(udpEndpoint_t("233.200.79.3", 61003));

		return;
	}

	ifs >> tradeThreadsCount;
	ifs >> quoteThreadsCount;

	size_t ports_amount;
	std::string addrBuff;
	unsigned short portBuff;

	ifs >> ports_amount;
	for(size_t i = 0; i < ports_amount; i++){
		ifs >> addrBuff;
		ifs >> portBuff;
		tradesEps.push_back(udpEndpoint_t(addrBuff, portBuff));
	}

	ifs >> ports_amount;
	for(size_t i = 0; i < ports_amount; i++){
		ifs >> addrBuff;
		ifs >> portBuff;
		quotesEps.push_back(udpEndpoint_t(addrBuff, portBuff));
	}

}

int main() {
	std::cout << "udp client" << std::endl;
	std::cout.flush();

	size_t tradeThreadsCount;
	size_t quoteThreadsCount;
	udpEndpoints_t tradesEndpoints;
	udpEndpoints_t quotesEndpoints;
	readSettingsFromFile(tradeThreadsCount, quoteThreadsCount, tradesEndpoints, quotesEndpoints);
	std::cout << "got " << quotesEndpoints.size() << " qoutes eps" << std::endl;
	std::cout << "got " << tradesEndpoints.size() << " trades eps" << std::endl;

	std::cout << "create [multicast_communication] object" << std::endl;
	market_data_receiver mdr;
	multicast_communication mc(tradeThreadsCount, quoteThreadsCount,
			tradesEndpoints, quotesEndpoints, mdr);
	mc_p_ = &mc;
	mdr_p_ = &mdr;
	signal(SIGINT, &signalHandler);
	mc.run();

	std::cout << "\nOVER" << std::endl;
	return 0;
}
