#include "raphia/circ_buffer.hpp"
#include <catch2/catch.hpp>
#include <string>

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
    SECTION("back and front throw")
    {
        CHECK_THROWS(circ.front());
        CHECK_THROWS(circ.back());
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
    SECTION("back and front throw")
    {
        CHECK_THROWS(circ.front());
        CHECK_THROWS(circ.back());
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

TEST_CASE("circ_buffer::push_back()", "[modifier]")
{
    raphia::circ_buffer<char> circ(8);
    SECTION("push one element")
    {
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
        SECTION("front points to the element")
        {
            CHECK(circ.front() == 'h');
        }
        SECTION("back points to the element")
        {
            CHECK(circ.back() == 'h');
        }
    }
    SECTION("push range of elements to overflow the buffer")
    {
        std::string str = "Hello World";
        std::copy(str.begin(), str.end(), std::back_inserter(circ));
        SECTION("size is 8")
        {
            CHECK(circ.size() == 8);
        }
        SECTION("string has been overwritten")
        {
            CHECK(std::string(circ.begin(), circ.end()) == "lo World");
        }
    }
}

TEST_CASE("circ_buffer<class>::push_back()", "[modifier]")
{
    raphia::circ_buffer<std::shared_ptr<char>> circ(8);
    SECTION("push one element")
    {
        auto p = std::make_shared<char>();
        circ.push_back(p);
        SECTION("p use_count is 2")
        {
            CHECK(p.use_count() == 2);
            SECTION("pop front")
            {
                circ.pop_front();
                SECTION("element is destructed")
                {
                    CHECK(p.use_count() == 1);
                }
            }
            SECTION("pop back")
            {
                circ.pop_back();
                SECTION("element is destructed")
                {
                    CHECK(p.use_count() == 1);
                }
            }
            SECTION("clear")
            {
                circ.clear();
                SECTION("element is destructed")
                {
                    CHECK(p.use_count() == 1);
                }
            }
        }
    }
    SECTION("overflow buffer")
    {
        auto p = std::make_shared<char>();
        for (auto _ = 32; _--;)
            circ.push_back(p);
        SECTION("p use_count is circ capacity + 1")
        {
            CHECK((size_t)p.use_count() == circ.capacity() + 1);
        }
    }
}

TEST_CASE("circ_buffer::push_front()", "[modifier]")
{
    raphia::circ_buffer<char> circ(8);
    SECTION("add one element")
    {
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
    SECTION("push range of elements to overflow the buffer")
    {
        std::string str = "Hello World";
        std::copy(str.begin(), str.end(), std::front_inserter(circ));
        SECTION("size is 8")
        {
            CHECK(circ.size() == 8);
        }
        SECTION("string has been overwritten")
        {
            CHECK(std::string(circ.begin(), circ.end()) == "dlroW ol");
        }
    }
}

TEST_CASE("circ_buffer::emblace_front()", "[modifier]")
{
    class TestObject
    {
    public:
        TestObject(int a, int b, int c)
            : a_(a), b_(b), c_(c) {}
        int a_;
        int b_;
        int c_;
    };
    raphia::circ_buffer<TestObject> circ(8);

    SECTION("emblace one object")
    {
        circ.emblace_front(10, 20, 30);
        SECTION("the first element contains the values it was initialized with")
        {
            CHECK(circ.front().a_ == 10);
            CHECK(circ.front().b_ == 20);
            CHECK(circ.front().c_ == 30);
        }
    }
}

TEST_CASE("circ_buffer::emblace_back()", "[modifier]")
{
    class TestObject
    {
    public:
        TestObject(int a, int b, int c)
            : a_(a), b_(b), c_(c) {}
        int a_;
        int b_;
        int c_;
    };
    raphia::circ_buffer<TestObject> circ(8);

    SECTION("emblace one object")
    {
        circ.emblace_back(10, 20, 30);
        SECTION("the first element contains the values it was initialized with")
        {
            CHECK(circ.back().a_ == 10);
            CHECK(circ.back().b_ == 20);
            CHECK(circ.back().c_ == 30);
        }
    }
}

TEST_CASE("circ_buffer::resize()", "[modifier]")
{
    SECTION("we have a circ buffer of primitive values")
    {
        raphia::circ_buffer<char> circ(8);
        SECTION("fill buffer")
        {
            std::string str = "Hello";
            std::copy(str.begin(), str.end(), std::back_inserter(circ));
            SECTION("resize the buffer capacity")
            {
                circ.set_capacity(64);
                SECTION("content is ok")
                {
                    CHECK(std::string(circ.begin(), circ.end()) == "Hello");
                }
            }
            SECTION("read buffer and write more")
            {
                while (!circ.empty())
                    circ.pop_front();
                std::copy(str.begin(), str.end(), std::back_inserter(circ));
                circ.set_capacity(64);
                SECTION("content is ok")
                {
                    CHECK(std::string(circ.begin(), circ.end()) == "Hello");
                }
            }
        }
    }
    SECTION("we have a circ buffer of class objects")
    {
        raphia::circ_buffer<std::shared_ptr<char>> circ(8);
        SECTION("fill buffer")
        {
            std::shared_ptr<char> p = std::make_shared<char>();
            for (auto _ = 5; _--;)
                circ.push_back(p);
            SECTION("resize the buffer")
            {
                circ.set_capacity(64);
                SECTION("content is ok")
                {
                    CHECK(p.use_count() == 6);
                }
            }
        }
    }
}

TEST_CASE("circ_buffer::iterator::operator++", "[increment]")
{
    std::string str = "Hello World";
    raphia::circ_buffer<char> circ(str.begin(), str.end());
    auto iter = circ.begin();
    SECTION("increment iterator")
    {
        CHECK(*iter++ == 'H');
        CHECK(*iter++ == 'e');
        CHECK(*iter++ == 'l');
        CHECK(*iter++ == 'l');
        CHECK(*iter++ == 'o');
    }
}

TEST_CASE("circ_buffer::iterator::operator--", "[decremenent]")
{
    std::string str = "Hello World";
    raphia::circ_buffer<char> circ(str.begin(), str.end());
    auto iter = circ.end();
    --iter;
    SECTION("decrement iterator")
    {
        CHECK(*iter-- == 'd');
        CHECK(*iter-- == 'l');
        CHECK(*iter-- == 'r');
        CHECK(*iter-- == 'o');
        CHECK(*iter-- == 'W');
    }
}
