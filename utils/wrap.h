/*
 * @Author: Johnathan 2440877322@qq.com
 * @Date: 2024-04-27 21:24:51
 * @LastEditors: Johnathan 2440877322@qq.com
 * @LastEditTime: 2024-05-04 14:53:42
 * @FilePath: /CppWrapGan/src/Wrap.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <string>
#include <vector>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class Wrap
{
public:
    Wrap(){}
    ~Wrap(){}
    virtual void run() = 0;
    virtual void initArags(){};
protected:
    std::string m_commend;
    // 需要设置的参数
    std::vector<std::vector<std::string>> args;
    // 结果路径
    std::vector<std::string> result;
};

class GenerateWrap : public Wrap{
public:
    virtual void run();
};

class InfiniteGenerateWrap : public Wrap{
public:
    virtual void run();
};

class InversionWrap : public Wrap{
    virtual void initArags();
    virtual void run();
};

class TrainWrap : public Wrap{
    virtual void initArags();
    virtual void run();
};
