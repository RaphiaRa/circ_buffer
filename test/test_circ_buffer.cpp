#include "raphia/circ_buffer.hpp"
#include <catch2/catch.hpp>
#include <cstring>

TEST_CASE("circ_buffer::circ_buffer()", "[ctor]")
{
    raphia::circ_buffer<char> circ;
    SECTION("size is 0")
    {
        CHECK(circ.size() == 0);
    }
    SECTION("capacity is 0")
    {
        CHECK(circ.capacity() == 0);
    }
    SECTION("is empty")
    {
        CHECK(circ.empty());
    }
    SECTION("begin and end iterator coincide")
    {
        CHECK(circ.begin() == circ.end());
        CHECK(circ.cbegin() == circ.cend());
    }
}

TEST_CASE("circ_buffer::circ_buffer(size_type)", "[ctor]")
{
    raphia::circ_buffer<char> circ(512);
    SECTION("size is 0")
    {
        CHECK(circ.size() == 0);
    }
    SECTION("capacity is 512")
    {
        CHECK(circ.capacity() == 512);
    }
    SECTION("is empty")
    {
        CHECK(circ.empty());
    }
    SECTION("begin and end iterator coincide")
    {
        CHECK(circ.begin() == circ.end());
        CHECK(circ.cbegin() == circ.cend());
    }
}

TEST_CASE("circ_buffer::circ_buffer(const circ_buffer&)", "[ctor]")
{
    raphia::circ_buffer<char> circ;
    SECTION("size is 0")
    {
        CHECK(circ.size() == 0);
    }
    SECTION("capacity is 0")
    {
        CHECK(circ.capacity() == 0);
    }
    SECTION("is empty")
    {
        CHECK(circ.empty());
    }
    SECTION("begin and end iterator coincide")
    {
        CHECK(circ.begin() == circ.end());
        CHECK(circ.cbegin() == circ.cend());
    }
    CHECK_NOTHROW(circ = raphia::circ_buffer<char>(512));
    SECTION("size is 0")
    {
        CHECK(circ.size() == 0);
    }
    SECTION("capacity is 512")
    {
        CHECK(circ.capacity() == 512);
    }
    SECTION("is empty")
    {
        CHECK(circ.empty());
    }
    SECTION("begin and end iterator coincide")
    {
        CHECK(circ.begin() == circ.end());
        CHECK(circ.cbegin() == circ.cend());
    }
}

TEST_CASE("circ_buffer::push_back()", "[inserter]")
{
    raphia::circ_buffer<char> circ(8);
    circ.push_back('h');
    SECTION("size is 1")
    {
        CHECK(circ.size() == 1);
    }
    SECTION("is not empty")
    {
        CHECK(!circ.empty());
    }
    SECTION("begin points to first element")
    {
        CHECK(*circ.begin() == 'h');
        CHECK(*circ.cbegin() == 'h');
    }
}

TEST_CASE("circ_buffer::push_front()", "[inserter]")
{
    raphia::circ_buffer<char> circ(8);
    circ.push_front('h');
    SECTION("size is 1")
    {
        CHECK(circ.size() == 1);
    }
    SECTION("is not empty")
    {
        CHECK(!circ.empty());
    }
    SECTION("begin points to first element")
    {
        CHECK(*circ.begin() == 'h');
        CHECK(*circ.cbegin() == 'h');
    }
}
