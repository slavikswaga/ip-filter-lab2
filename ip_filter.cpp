#include "ip_filter.hpp"

int main()
{
    try{
        std::vector<ip_tuple> ip_pool;
        reading_file(ip_pool, "ip_filter.tsv");
    
        //reverse lexicographically sort
        std::sort(ip_pool.begin(), ip_pool.end(), [](const auto& a, const auto& b){
                return a > b;
        });

        // calculation in compile time
        constexpr ip_tuple target_ip = generate_ip(981328);
        bool concur = search_ip_in_pool(target_ip, ip_pool);
            std::cout << "GENERATED IP - ";
            print_ip(target_ip);
            std::cout << (concur ? "ALREADY EXISTS IN IP_POOL" : "NOT IN THE IP_POOL YET") << "\n";
        
        //filter by first byte and output
        std::cout <<"filter by first byte\n";
        // first byte equal 1
        filter(1, ip_pool);

        //filter by first and second bytes and output
        std::cout <<"filter by first and second bytes\n";
        // first byte equal 46 and second byte equal 70
        filter(46, 70, ip_pool);

        //filter by any byte and output
        std::cout << "filter by any byte\n";
        // any byte equal 46
        filter_any(46, ip_pool);
        }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << "\n";
    }
    return 0;
}
