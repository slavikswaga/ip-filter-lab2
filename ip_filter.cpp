#include "ip_filter.hpp"


void run_ip_filter(){
    try
    {
        std::vector<ip_tuple> ip_pool;
        std::string line;
        std::ifstream in("ip_filter.tsv");
        if(in.is_open()){
            while(std::getline(in, line))
            {
                std::vector<std::string> v = split(line, '\t');
                if (v[0].empty()) continue;
                std::vector<std::string> ip = split(v.at(0), '.');
                if(is_valid_ip(ip)) ip_pool.push_back(create_tuple(ip));
            }
        }
        else{
            throw std::runtime_error("Не удалось открыть ip_filter.tsv");
        }
        in.close();

        // TODO reverse lexicographically sort - DONE
        std::sort(ip_pool.begin(), ip_pool.end(), [](const auto& a, const auto& b){
            return a > b;
        });
        // calculation in compile time
        constexpr ip_tuple target_ip = generate_ip(981328);

        bool concur = search_ip_in_pool(target_ip, ip_pool);
        std::cout << "GENERATED IP - ";
        print_ip(target_ip);
        std::cout << (concur ? "ALREADY EXISTS IN IP_POOL" : "NOT IN THE IP_POOL YET") << "\n";

        // TODO filter by first byte and output - DONE
        std::cout <<"filter by first byte\n";
        // first byte equal 1
        filter(1, ip_pool);

        // TODO filter by first and second bytes and output - DONE
        std::cout <<"filter by first and second bytes\n";
        // first byte equal 46 and second byte equal 70
        filter(46, 70, ip_pool);

        // TODO filter by any byte and output - DONE
        std::cout << "filter by any byte\n";
        // any byte equal 46
        filter_any(46, ip_pool);

        
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << "\n";
    }
}

int main()
{
    run_ip_filter();

    return 0;
}
