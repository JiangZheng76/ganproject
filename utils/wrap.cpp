/*
 * @Author: Johnathan 2440877322@qq.com
 * @Date: 2024-04-27 21:24:51
 * @LastEditors: Johnathan 2440877322@qq.com
 * @LastEditTime: 2024-05-04 15:48:30
 * @FilePath: /CppWrapGan/src/Wrap.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: Johnathan 2440877322@qq.com
 * @Date: 2024-04-27 21:24:51
 * @LastEditors: Johnathan 2440877322@qq.com
 * @LastEditTime: 2024-04-28 15:01:22
 * @FilePath: /CppWrapGan/src/Wrap.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "wrap.h"
#include <stdio.h>
#include <iostream>
#include <pybind11/pytypes.h>

using namespace std;

void GenerateWrap::run(){

    py::scoped_interpreter guard{};

    py::module argarse = py::module::import("argparse");
    py::object parser = argarse.attr("ArgumentParser")();

    // 想将这个 py 语句转换为下面这个c++语句，对于 type=str我不知道怎么实现
    // parser.add_argument("--model-config", type=str)

    parser.attr("add_argument")("--model-config"
        // , py::arg("type") = py::str()
        , py::arg("default") = py::str("abc")
    );

    parser.attr("add_argument")("--test-config"
        // , py::arg("type") = py::str("")
    );
    parser.attr("add_argument")("--exp-suffix"
        // , py::arg("type") = "str"
        , py::arg("default") = py::none()
    );

    parser.attr("add_argument")("--ckpt"
        // , py::arg("type") = "str"
        , py::arg("default") = py::none()
    );

    parser.attr("add_argument")("--seed"
        // , py::arg("type") = "int"
        , py::arg("default") = py::none()
    );
    
    parser.attr("add_argument")("--interactive"
       , py::arg("action") = py::str("store_true")
    );

    parser.attr("add_argument")("--override-save-idx"
        // , py::arg("type") = "int"
        , py::arg("default") = py::none()
    );

    parser.attr("add_argument")("--speed-benchmark"
        , py::arg("action") = py::str("store_true")
    );
    
    parser.attr("add_argument")("--calc-flops"
        , py::arg("action") = py::str("store_true")
    );

    // // Flag for inversion distributed testing
    parser.attr("add_argument")("--inv-start-idx"
        // , py::arg("type") = "int"
        , py::arg("default") = py::none()
    );

    parser.attr("add_argument")("--try-restrict-memory"
        // , py::arg("type") = "float"
        , py::arg("default") = py::float_(1.0)
    );

    // // Load from inversion
    parser.attr("add_argument")("--inv-records"
        // , py::arg("type") = "str"
        , py::arg("default") = py::none()   
    );

    parser.attr("add_argument")("--inv-placements"
        // , py::arg("type") = "str"
        , py::arg("default") = py::none()       
    );

    // // Interactive recover from ckpt
    parser.attr("add_argument")("--inter-ckpt"
        // , py::arg("type") = "str"
        , py::arg("default") = py::none()       
    );

    parser.attr("add_argument")("--debug"
        , py::arg("action") = py::str("store_true")
    );

    parser.attr("add_argument")("--verbose"
        , py::arg("action") = py::str("store_true")
    );
    // argument.attr("action") = "store_true";

    // argument = parser.attr("add_argument")("--archive-mode");
    // argument.attr("action") = "store_true";

    // argument = parser.attr("add_argument")("--clear-fid-cache");
    // argument.attr("action") = "store_true";

    py::list args;
    // args.append("CUDA_VISIBLE_DEVICES='0,1'");
    args.append("--model-config");
    args.append("./configs/model/InfinityGAN.yaml");
    args.append("--test-config");
    args.append("./configs/test/infinite_gen_1024x1024.yaml");
    // args.append("--model-config='./configs/model/InfinityGAN.yaml'");
    py::object parse_args = parser.attr("parse_args")(args);

    std::cout << parse_args.attr("model_config").cast<std::string>() << " "
            << parse_args.attr("test_config").cast<std::string>();
    // 找到我们自己的 path 
    py::module sys = py::module::import("sys");
    sys.attr("path").attr("append")("/home/jiangz/jz/gan/infinityGAN");

    py::module gen = py::module::import("test");
    gen.attr("generate")(parse_args);
}





void InfiniteGenerateWrap::run(){
    py::scoped_interpreter guard{};
    py::module sys = py::module::import("sys");
    sys.attr("path").attr("insert")(0,"/home/jiangz/jz/gan/infinityGAN");
    py::module gen = py::module::import("wrap");
    // py::module test = py::module::import("test");
    std::string model_config = "./configs/model/InfinityGAN_imgs.yaml";
    std::string test_config = "./configs/test/infinite_gen_imgs_1024x1024.yaml";
    std::string ckpt = "/home/jiangz/jz/gan/CppWrapGan/logs/InfinityGAN_imgs/ckpt/inter_00006000.pth.tar";

    // 目标路径
    std::string des = "";
    gen.attr("infinite_generate")(model_config,test_config,ckpt);

    
}

void InversionWrap::run(){
    py::scoped_interpreter guard{};
    py::module sys = py::module::import("sys");
    sys.attr("path").attr("insert")(0,"/home/jiangz/jz/gan/infinityGAN");
    py::module invension = py::module::import("wrap");

}
void InversionWrap::initArags(){

}
void TrainWrap::initArags(){
    
}

void TrainWrap::run(){
    py::scoped_interpreter guard{};
    py::module sys = py::module::import("sys");
    sys.attr("path").attr("insert")(0,"/home/jiangz/jz/gan/infinityGAN");
    py::module train = py::module::import("wrap");

    

    std::string model_config = "./configs/model/InfinityGAN_imgs.yaml";

    
    train.attr("train")(model_config);
}