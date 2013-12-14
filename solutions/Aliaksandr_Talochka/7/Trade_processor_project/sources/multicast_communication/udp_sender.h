#include <iostream>
#include <fstream>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"


namespace multicast_communication
{
	class sender : virtual private boost::noncopyable
	{
	public:
	  sender(boost::asio::io_service& io_service,
		  const std::string& multicast_address, const short multicast_port, const std::string& input_FileName, const long u_sleep_time = 1e6)
		: endpoint_(boost::asio::ip::address::from_string(multicast_address), multicast_port),
		  socket_(io_service, endpoint_.protocol()),
		  timer_(io_service),
		  u_sleep_time_(u_sleep_time)
	  {

		  inFile.open(input_FileName.c_str(), std::ios::in|std::ios::binary);
		  if(inFile.is_open())
		  {
			message_ = read_file(inFile);

			  if(message_.size())
			  {
				  socket_.async_send_to(
					  boost::asio::buffer(message_), endpoint_,
					  boost::bind(&sender::handle_send_to, this,
						boost::asio::placeholders::error));
			  }
		  }
		  else
		  {
			  std::cout << "There is no file with the name: " << input_FileName.c_str() << std::endl;
		  }

	  }

	  ~sender()
	  {
		  if (inFile.is_open())
		  {
			  inFile.close();
		  }
	  }

	  void handle_send_to(const boost::system::error_code& error)
	  {
		if (!error)
		{
		  timer_.expires_from_now(boost::posix_time::microsec(u_sleep_time_));
		  timer_.async_wait(
			  boost::bind(&sender::handle_timeout, this,
				boost::asio::placeholders::error));
		}
	  }

	  void handle_timeout(const boost::system::error_code& error)
	  {
		if (!error)
		{
		  message_ = read_file(inFile);

		  if(message_.size())
		  {
			  socket_.async_send_to(
				  boost::asio::buffer(message_), endpoint_,
				  boost::bind(&sender::handle_send_to, this,
					boost::asio::placeholders::error));
		  }
		}
	  }

		std::string read_file(std::fstream &input_file)
		{
			const char begin_message = 0x01;
			const char end_message = 0x03;
			std::string ret = "";

			while (!input_file.eof())
			{
				char symbol;
				if ((symbol = input_file.get()) == begin_message)
				{
					ret.push_back(symbol);
					while ((symbol = input_file.get()) != end_message && !input_file.eof())
					{
						ret.push_back(symbol);   
					}
					ret.push_back(symbol);   
					break;
				}  
			}
			return ret;
		}

	private:
	  boost::asio::ip::udp::endpoint endpoint_;
	  boost::asio::ip::udp::socket socket_;
	  boost::asio::deadline_timer timer_;
	  boost::asio::ip::address multicast_address_;
	  std::string message_;
	  std::fstream inFile;
	  long u_sleep_time_;

	};
}