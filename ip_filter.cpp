#include "ip_filter.hpp"

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void print_ip(const ip_tuple& ip){
    std::cout << static_cast<int>(std::get<0>(ip)) << "."
              << static_cast<int>(std::get<1>(ip)) << "."
              << static_cast<int>(std::get<2>(ip)) << "."
              << static_cast<int>(std::get<3>(ip)) << "\n";
}

bool is_valid_ip(const std::vector<std::string>& ip){
    if(ip.size() != 4) throw std::invalid_argument("Неверный формат IP");
    for(size_t i = 0; i < 4; ++i){
        int octet = std::stoi(ip.at(i));
        if(octet > 255 || octet < 0) throw std::invalid_argument("Ошибка валидации октета");
    }
    return true;
}

ip_tuple create_tuple(const std::vector<std::string>& vec){
    uint8_t octets[4];
    for(size_t i = 0; i < 4; ++i){
        int octet = std::stoi(vec.at(i));
        octets[i] = static_cast<uint8_t>(octet);
    }
    std::tuple my_tuple(octets[0], octets[1], octets[2], octets[3]);
    return my_tuple;
}

void filter(const uint8_t& target, const auto& ip_pool){
    auto has_first_byte = [target](const auto& ip){
            return std::get<0>(ip) == target;
    };
    for(const auto& ip : ip_pool | std::views::filter(has_first_byte)){
        print_ip(ip);
    }
}

void filter(const uint8_t& target1, const uint8_t& target2, const auto& ip_pool){
    auto has_first_and_second_bytes = [target1, target2](const auto& ip){
        return std::get<0>(ip) == target1 && std::get<1>(ip) == target2;
    };
    for(const auto& ip : ip_pool | std::views::filter(has_first_and_second_bytes)){
        print_ip(ip);
    }
}

void filter_any(const uint8_t& target, const auto& ip_pool){
    auto has_any_byte = [target](const auto& ip){
        return std::get<0>(ip) == target || std::get<1>(ip) == target ||
        std::get<2>(ip) == target || std::get<3>(ip) == target;
    };
    for(const auto& ip : ip_pool | std::views::filter(has_any_byte)){
        print_ip(ip);
    }
}

constexpr ip_tuple generate_ip(uint32_t value)
{
    if (std::is_constant_evaluated()) {
        // Эта ветка работает только в compiletime
        for(int i = 0; i < 262144; ++i){
            value ^= value << 13;
            value ^= value >> 17;
            value *= 0x45D9F3B;
            value ^= value >> 16;
        }
        uint8_t A = static_cast<uint8_t>((value >> 24) & 255);
        uint8_t B = static_cast<uint8_t>((value >> 16) & 255);
        uint8_t C = static_cast<uint8_t>((value >> 8) & 255);
        uint8_t D = static_cast<uint8_t>(value & 255);
        std::tuple my_tuple(A, B, C, D);
        return my_tuple;
    } else {
        // Эта ветка — в runtime
        throw std::runtime_error("Не удалось выполнить вычисления в compiletime");
    }
     
}

bool search_ip_in_pool(const ip_tuple& target_ip, const std::vector<ip_tuple>& ip_pool) {
    if (ip_pool.empty()) return false;
    int low = 0, high = ip_pool.size() - 1;
    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(ip_pool.at(mid) == target_ip) {
            return true;
        }
        else if(ip_pool.at(mid) > target_ip) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    return false;
}

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
