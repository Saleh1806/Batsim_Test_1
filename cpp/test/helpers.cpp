#include "helpers.hpp"

#include <cstdlib>
#include <fstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

using namespace std;
using namespace batprotocol;

void write_message_buffer_json_file(MessageBuilder & mb)
{
    auto json = mb.buffer_as_json();
    auto test_info = ::testing::UnitTest::GetInstance()->current_test_info();

    const std::string default_base_dir = ".";
    char * base_dir = getenv("BATPROTOCOL_JSON_OUTPUT_PATH");
    if (base_dir == nullptr)
    {
        base_dir = const_cast<char*>(default_base_dir.c_str());
    }

    char * filename;
    int ret = asprintf(&filename, "%s/%s-%s.json", base_dir, test_info->test_suite_name(), test_info->name());
    if (ret == -1)
        throw std::runtime_error("Cannot allocate filename string");

    std::ofstream out(filename, std::ios_base::trunc);
    if (!out.is_open())
    {
        fprintf(stderr, "Cannot create file '%s'\n", filename);
        fflush(stderr);
        free(filename);
        throw std::runtime_error("Cannot create json file");
    }
    out << *json;
    out.flush();
    free(filename);
}

