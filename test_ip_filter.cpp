#include <gtest/gtest.h>
#include "ip_filter.hpp"

TEST(is_valid_ip, invalid_ip)
{
    std::vector<std::string> vec = {"255", "255", "255", "-1"};
    EXPECT_THROW(is_valid_ip(vec), std::invalid_argument);
}

TEST(is_valid_ip, invalid_format_ip)
{
    std::vector<std::string> vec = {"192", "168", "0", "1", "1"};
    EXPECT_THROW(is_valid_ip(vec), std::invalid_argument);
}

TEST(is_valid_ip, valid_ip)
{
    std::vector<std::string> vec = {"192", "168", "0", "1"};
    EXPECT_TRUE(is_valid_ip(vec));
}
