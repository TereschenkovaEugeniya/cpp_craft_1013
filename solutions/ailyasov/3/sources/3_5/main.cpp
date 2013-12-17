#include <vector>
#include <string>

#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>

#include "small_buffer.h"

int main() {

    namespace fs = boost::filesystem;

    std::string directory(SOURCE_DIR "/tests/");

    boost::regex rx("input_35_([0-9]{3})[.]txt", boost::regex::extended);
    boost::smatch match;

    std::vector<std::string> inputs;


    if( fs::exists( directory ) )
    {
        for(fs::directory_iterator it = fs::directory_iterator( directory ); it != fs::directory_iterator(); ++it)
        {
            if(boost::regex_match(it->leaf(), match, rx))
            {
                inputs.push_back(it->leaf());
            }
        }
    }

    boost::thread_group t_group;
    for(std::vector<std::string>::iterator it = inputs.begin(); it !=
            inputs.end(); ++it)
    {
        std::string input = *it;
        if(boost::regex_search(input, match, rx))
        {
            std::stringstream output;
            output << SOURCE_DIR "/tests/output_35_";
            output << match[1] << ".txt";
            t_group.create_thread(
                    boost::bind( &SmallBuffer::read_to_buff_and_save_stats, directory + input, output.str() ) );
        }
    }
    t_group.join_all();
    return 0;
}

