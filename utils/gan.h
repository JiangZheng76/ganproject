/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-23 11:10:11
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-25 16:34:34
 * @FilePath: /GanProject/Utils/gan.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef GAN_H
#define GAN_H

#define YAML_PATH "/home/sysu/GanProject/config/test.yaml"
#define LAST_PATH "/home/sysu/GanProject/config/template.yaml"

#include <log.h>
#include <config.hpp>

namespace gan{

using GanLogger = mysylar::Logger;
using GanConfigVarBase =  mysylar::ConfigVarBase;

template<class T>
using GanConfigVar = mysylar::ConfigVar<T>;
}

#endif // GAN_H
