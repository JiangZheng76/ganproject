/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-23 11:08:43
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-27 15:46:35
 * @FilePath: /GanProject/Utils/ganconfig.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef GANCONFIG_H
#define GANCONFIG_H

#include <log.h>
#include <config.hpp>
#include <QDebug>

#include "gan.h"

namespace gan{
static GanLogger::ptr g_logger  = SYLAR_LOG_ROOT();

/**
 * @brief 纯静态的管理器
 * @description: 
 * 这样写的好处是绝对是最上面一层，不会出现其他的情况
 * @return {*}
 */
class GanConfig{
public:
    using GanConfigVarMap = mysylar::Config::ConfigVarMap;
    // 从 yaml 文件中读取然后展示出来
    // 将数据转化成 yaml 文件

    static bool LoadFromYaml(const std::string& path,bool isOverride = true);
    static void OverrideCurYaml();
    static bool SaveLastYaml();

    template<class T>
    static typename GanConfigVar<T>::ptr Lookup(const std::string& name){
        auto it = GetSdatas().find(name);
        if(it == GetSdatas().end()){
            // SYLAR_LOG_WARN(g_logger) << "Lookup(" << name<< ") could not find.";
            // qWarning() << "Lookup(" << QString::fromStdString(name) << ") could not find.";
            return nullptr;
        }
        return std::dynamic_pointer_cast<GanConfigVar<T>>(it->second);
    }

    template<class T>
    static typename GanConfigVar<T>::ptr Lookup(const std::string& name
        ,const T& default_val,const std::string& descriptor){
        /**
         * 不能像下面那么写，会有问题，不能推导出正确的类型
         */
        // typename GanConfigVar<T>::ptr var = Lookup<T>(name);
        // if(!var){
        //     var = new GanConfigVar<T>(name,default_val,descriptor);
        // }
        auto ptr = Lookup<T>(name);
        // 需要进行一层类型装换？？？为什么
        typename GanConfigVar<T>::ptr var = std::dynamic_pointer_cast<GanConfigVar<T>>(ptr);
        if(!var){
            typename GanConfigVar<T>::ptr v(new GanConfigVar<T>(name,default_val,descriptor));
            var = v;
            // SYLAR_LOG_INFO(g_logger) << "Lookup name = " << name << " create a new one.";
            qInfo() << "Lookup name = " << QString::fromStdString(name) << " create a new one.";
        }else {
            // SYLAR_LOG_INFO(g_logger) << "Lookup name = " << name << " exist.";
            var->setVal(default_val);
            qInfo() << "Lookup name = " << QString::fromStdString(name) << " exist and update.";
        }
        GetSdatas()[name] = var;
        // int size = GetSdatas().size();
        // qInfo() << size;
        return var;
    }

    static const std::string dump();

    static std::string& GetSCurPath(){
        static std::string s_curPath = "";
        return s_curPath;
    }
    static GanConfigVarMap& GetSdatas(){
        static GanConfigVarMap s_datas;
        return s_datas;
    }
    
};
}

#endif // GANCONFIG_H
