/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-18 11:05:24
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-20 16:14:22
 * @FilePath: /GanProject/Utils/Config.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "config.h"
#include "unistd.h"
namespace gan{

ConfigManager::ConfigManager()
    :m_randpath("")
    ,m_curpath(""){
}

std::string ConfigManager::saveToRandYaml(){
    if(m_randpath.empty()){
        if(m_curpath.empty()){
            // curpath empty ,we choose current path
            if(!getcwd(&m_randpath[0],1024)){
                
            }
        }else {
            // cur path 不为空 获取当前的 path 路径
            size_t len = m_curpath.size();
            for(size_t i = len-1;i>=0;i--){
                if(m_curpath[i] == '/'){
                    if(i > 0 && m_curpath[i-1] == '/'){
                        i--;
                        continue;
                    }
                    m_randpath = m_curpath.substr(0,i+1);
                    break;
                }
            }
        }
    }
}
void ConfigManager::loadFromYaml(const std::string& path){
    
}
void ConfigManager::overrideCurYaml(){

}


}