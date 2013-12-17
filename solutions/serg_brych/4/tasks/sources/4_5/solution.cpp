#include "solution.h"

task4_5::solution::solution( const data_type& data )
{
	if(!data.empty())
	{
		m_max = data[0][0];
		m_min = data[0][0];
		m_current_vector = data.size();
		calculate_result(data);
	}
	else
	{
		m_max = 0;
		m_min = 0;
	}
}

int task4_5::solution::get_min() const
{
	return m_min;
}
int task4_5::solution::get_max() const
{
	return m_max;
}

void task4_5::solution::calculate_result( const data_type& data )
{
	uint32_t count_of_cpu = boost::thread::hardware_concurrency() ? boost::thread::hardware_concurrency() : 1;
	int temp_init_value = m_max;
	
	for(uint32_t i = 0; i < count_of_cpu; i++)
	{
		m_threads.create_thread(boost::bind( &solution::process_thread, this, boost::ref(data), temp_init_value));
	}
	m_threads.join_all();
}


void task4_5::solution::process_thread(const data_type& data, const int init_value)
{
	int min = init_value, max = init_value;
	size_t current_container = 0;
	while(true)
	{
		{
			boost::lock_guard<boost::mutex> lg(m_get_data_mutex);
			if(m_current_vector < 1)
			{
				break;
			}
			m_current_vector--;
			current_container = m_current_vector;
			
		}

		size_t elements_count = data[current_container].size();
		for(size_t i = 0; i < elements_count; i++)
		{
			if(data[current_container][i] > max)
			{
				max = data[current_container][i];
			}
			if(data[current_container][i] < min)
			{
				min = data[current_container][i]; 
			}
		}
	}
	
	{
		boost::lock_guard<boost::mutex> ls(m_set_data_mutex);
		if(m_max < max)
			m_max = max; 
	
		if(m_min > min)
			m_min = min; 
	}


}