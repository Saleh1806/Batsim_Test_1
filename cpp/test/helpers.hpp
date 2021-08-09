#pragma once

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <batprotocol.hpp>

void write_test_mb(batprotocol::MessageBuilder & mb);
void write_test_mb_binary(batprotocol::MessageBuilder & mb);
void write_test_mb_json_file(batprotocol::MessageBuilder & mb);

template <typename T>
void check_identical_vectors(
    const flatbuffers::Vector<T> * actual,
    const std::vector<T> & expected)
{
    EXPECT_NE(actual, nullptr);
    EXPECT_EQ(actual->size(), expected.size());

    for (unsigned int i = 0; i < actual->size(); ++i)
    {
        EXPECT_EQ(actual->Get(i), expected[i]) << "vectors differ at index=" << i;
    }
}

void check_identical_str_vectors(
    const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> * actual,
    const std::vector<std::string> & expected
);
