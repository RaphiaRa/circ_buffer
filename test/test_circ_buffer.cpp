#include "raphia/circ_buffer.hpp"
#include <catch2/catch.hpp>
#include <cstring>

TEST_CASE("circ::circ()", "[ctor]")
{
    raphia::circ_buffer<char> circ;
    SECTION("size is 0")
    {
        REQUIRE(circ.size() == 0);
    }
    SECTION("capacity is 0")
    {
        REQUIRE(circ.capacity() == 0);
    }
    SECTION("is empty")
    {
        REQUIRE(circ.empty());
    }
    SECTION("begin and end iterator coincide")
    {
        REQUIRE(circ.begin() == circ.end());
        REQUIRE(circ.cbegin() == circ.cend());
    }
}

TEST_CASE("circ::push_back()", "[inserter]")
{
    raphia::circ_buffer<char> circ(8);
    circ.push_back('h');
    SECTION("size is 1")
    {
        REQUIRE(circ.size() == 1);
    }
    SECTION("is not empty")
    {
        REQUIRE(!circ.empty());
    }
    SECTION("begin points to first element")
    {
        REQUIRE(*circ.begin() == 'h');
        REQUIRE(*circ.cbegin() == 'h');
    }
}

TEST_CASE("circ::push_front()", "[inserter]")
{
    raphia::circ_buffer<char> circ(8);
    circ.push_front('h');
    SECTION("size is 1")
    {
        REQUIRE(circ.size() == 1);
    }
    SECTION("is not empty")
    {
        REQUIRE(!circ.empty());
    }
    SECTION("begin points to first element")
    {
        REQUIRE(*circ.begin() == 'h');
        REQUIRE(*circ.cbegin() == 'h');
    }
}

