#pragma once
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <stdint.h>
#include <algorithm>
#include <ranges>
#include <stdexcept>
#include <optional>

#define ip_tuple std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>

std::vector<std::string> split(const std::string &str, char d);
void print_ip(const ip_tuple& ip);
bool is_valid_ip(const std::vector<std::string>& ip);
ip_tuple create_tuple(const std::vector<std::string>& vec);
void filter(const uint8_t& target, const auto& ip_pool);
void filter(const uint8_t& target1, const uint8_t& target2, const auto& ip_pool);
void filter_any(const uint8_t& target, const auto& ip_pool);
consteval ip_tuple generate_ip(uint32_t value);
std::optional<int> search_ip_in_pool(const ip_tuple& target_ip, const std::vector<ip_tuple>& ip_pool);
void reading_file(std::vector<ip_tuple>& ip_pool, std::string name_file);

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

consteval ip_tuple generate_ip(uint32_t value)
{
    if (std::is_constant_evaluated()) {
        // Эта ветка работает только в compiletime
        for(int i = 0; i < 10000; ++i){
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

std::optional<int> search_ip_in_pool(const ip_tuple& target_ip, const std::vector<ip_tuple>& ip_pool) {
    if (ip_pool.empty()) return std::nullopt;
    int low = 0, high = ip_pool.size() - 1;
    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(ip_pool.at(mid) == target_ip) {
            return mid;
        }
        else if(ip_pool.at(mid) > target_ip) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    return std::nullopt;
}

void reading_file(std::vector<ip_tuple>& ip_pool, std::string name_file){
    std::string line;
        std::ifstream in(name_file);
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
}