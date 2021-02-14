#ifndef FSM_SRC_TESTS_GTEST_FOR_EACH_TUPLE_H
#define FSM_SRC_TESTS_GTEST_FOR_EACH_TUPLE_H

#include <tuple>
#include <vector>

#include "for_each_tuple.h"

struct CarInfo
{
    const char* model;
    std::uint16_t year;
};

struct Maserati { static constexpr CarInfo info() { return { .model = "MC20", .year = 2021}; } };
struct AstonMartin { static constexpr CarInfo info() { return { .model = "DB11", .year = 2021}; } };
struct Ford { static constexpr CarInfo info() { return { .model = "GT40", .year = 2005}; } };

using namespace fsm::util::algo;

TEST(for_each_tuple, Creation) 
{

    auto cars = std::make_tuple(Maserati{}, AstonMartin{}, Ford{});

    std::vector<std::string_view> vec;

    for_each_tuple(cars, [&vec] (auto p_car, std::size_t) { vec.emplace_back(p_car.info().model); });

    ASSERT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0].data(), "MC20");
    EXPECT_EQ(vec[1].data(), "DB11");
    EXPECT_EQ(vec[2].data(), "GT40");
}

TEST(for_each_tuple, modifyValue) 
{

    auto cars_info = std::make_tuple(CarInfo{"A", 1999}, CarInfo{"B", 2010});

    for_each_tuple(cars_info, [] (auto& p_car_info, std::size_t) { p_car_info.year += 1; });

    EXPECT_EQ(std::get<0>(cars_info).year, 2000);
    EXPECT_EQ(std::get<1>(cars_info).year, 2011);
}

#endif // FSM_SRC_TESTS_GTEST_FOR_EACH_TUPLE_H