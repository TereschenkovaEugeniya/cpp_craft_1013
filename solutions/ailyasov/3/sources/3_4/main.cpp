#include <vector>
#include <string>

#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>

#include "trades_filter.h"

int main() {
    std::vector<std::string> inputs;

    std::string directory(SOURCE_DIR "/tests/");
    boost::regex rx("input_34_([0-9]{3})[.]txt", boost::regex::extended);
    boost::smatch match;

    namespace fs = boost::filesystem;

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
            output << SOURCE_DIR "/tests/output_34_";
            output << match[1] << ".txt";
            t_group.create_thread(
                    boost::bind( &TradesFilter::filter_trades_and_save, directory + input, output.str() ) );
        }
    }
    t_group.join_all();

    return 0;

} 
