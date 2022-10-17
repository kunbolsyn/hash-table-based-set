#include <random>
#include <algorithm>
#include <sstream>
#include "set.h"

std::vector<std::string> randomvector(size_t k)
{
    std::vector<std::string> res;
    for (size_t i = 0; i != k; ++i)
    {
        std::ostringstream out;
        out << "string_" << std::abs(rand());
        res.push_back(out.str());
    }
    return res;
}

int main(int argc, char *argv[])
{
    set small;

    small.insert("if");
    small.insert("the");
    small.insert("value");
    std::cout << small << "\n";
    small.clear();
    std::cout << "this is the hash set: " << small << "\n";
    std::cout << "is small empty? " << small.empty() << "\n";
    set small3 = small;
    std::cout << small3 << "\n";

    std::cout << small.contains("xx") << "\n";
    std::cout << small.contains("yy") << "\n";

    std::initializer_list<std::string> init = {"abC", "aBc", "abcD", "key", "like"};
    set great(init, 25.0);
    std::cout << great << "\n";

#if 1
    small.insert("oskemen");
    std::cout << "oskemen was added to the small"
              << "\n";
    std::cout << "is small empty? " << small.empty() << "\n";
    std::cout << small.contains("kostanay") << "\n";
    std::cout << small << "\n";

    small.insert("atyrau");
    small.insert("nursultan");
    small.insert("almaty");
    small.insert("shymkent");
    small.insert("pavlodar");

    std::cout << small << "\n";
    std::cout << small.size() << "\n";

    small.remove("atyrau");

    auto small2 = small;
    std::cout << small2 << "\n";
    std::cout << "size = " << small2.size() << "\n";
    small2 = small;
    small = small;

#endif

    for (size_t s = 1000; s < 4096 * 1000; s = 2 * s)
    {
        std::cout << "----------------------------------------\n";
        std::vector<std::string> randvect = randomvector(s);

#if 0
        std::cout << "test vector: ";
        for( const auto& s : randvect )
            std::cout << s << " ";
        std::cout << "\n";
#endif

        set someset1(25);

        size_t nr_insertions = 0;
        for (const auto &s : randvect)
        {
            nr_insertions += someset1.insert(s);
            nr_insertions += someset1.insert(std::string("X") + s);
        }

        auto someset2 = someset1;

        if (nr_insertions != someset2.size())
        {
            std::cout << "nr = " << nr_insertions << "\n";
            std::cout << "size( ) = " << someset2.size() << "\n";
            throw std::runtime_error(
                "something went wrong with counting insertions");
        }

        std::cout << "number of insertions " << someset2.size() << "\n";
        std::cout << "finished test for " << s << "\n";
        someset2.printstatistics(std::cout);
    }
}
