#include <QtCore/QDebug>

#include "afFunction_st.h"
#include "afSystem.h"
#include <QtCore/QDateTime>

using namespace AFlib;

int FunctionST::randomInt(int from, int to)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(from, to);
    return dis(gen);
}

std::string FunctionST::libraryName(std::string libraryName)
{
#if defined(Q_OS_WIN)
    return QString(libraryName + ".dll");
#elif defined(Q_OS_LINUX) || defined(Q_OS_ANDROID)
    return std::string("lib" + libraryName + ".so");
#endif
    // TODO add Mac
}
