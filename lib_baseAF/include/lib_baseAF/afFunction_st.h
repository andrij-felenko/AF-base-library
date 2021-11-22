#ifndef LIB_BASEAF_FUNCTION_ST_H
#define LIB_BASEAF_FUNCTION_ST_H

#include <array>
#include <list>
#include <random>
#include <charconv>

namespace AFlib {
class FunctionST
{
public:
    explicit FunctionST() = default;

    static std::string libraryName(std::string libraryName);
    // TODO add template
    static int randomInt(int from, int to);
//    static QDateTime nullDateTime();

    template <typename Return_arg, unsigned size>
    static std::pair <unsigned, Return_arg> findMaxElement(std::array <Return_arg, size> array)
    {
        auto maxValue = array[0];
        unsigned maxIndex = 0;
        for (unsigned i = 1; i< array.size(); i++)
            if (array[i] > maxValue){
                maxIndex = i;
                maxValue = array[i];
            }
        return std::make_pair (maxIndex, maxValue);
    }

    template <typename Return_arg, unsigned size>
    static std::pair <unsigned, Return_arg> findMinElement(std::array <Return_arg, size> array)
    {
        auto maxValue = array[0];
        unsigned maxIndex = 0;
        for (unsigned i = 1; i< array.size(); i++)
            if (array[i] < maxValue){
                maxIndex = i;
                maxValue = array[i];
            }
        return std::make_pair (maxIndex, maxValue);
    }

    template <typename Return_arg, unsigned size>
    static std::list <unsigned> findMaxIndexList(std::array <Return_arg, size> array)
    {
        auto maxValue = array[0];
        for (unsigned i = 1; i < array.size(); i++)
            if (array[i] > maxValue)
                maxValue = array[i];
        std::list <unsigned> list;
        for (unsigned i = 0; i < array.size(); i++)
            if (array[i] == maxValue)
                list.push_back(i);
        return list;
    }

    template <typename Return_arg, unsigned size>
    static std::list <unsigned> findMinIndexList(std::array <Return_arg, size> array)
    {
        auto minValue = array[0];
        for (unsigned i = 1; i < array.size(); i++)
            if (array[i] < minValue)
                minValue = array[i];
        std::list <unsigned> list;
        for (unsigned i = 0; i < array.size(); i++)
            if (array[i] == minValue)
                list.push_back(i);
        return list;
    }
};
}

typedef AFlib::FunctionST AFfunctionST;

#endif // LIB_BASEAF_FUNCTION_ST_H
