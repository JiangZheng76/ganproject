/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-27 20:44:40
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-27 20:54:11
 * @FilePath: /GanProject/Utils/generatethread.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "generatethread.h"
namespace gan{
GenerateThread::GenerateThread(std::function<void()> cb,const std::string& name)
    :Thread(cb,name){
    
}
void GenerateThread::mainWork(){
    
}

int GenerateThread::getProc(){
    MutexType::Lock lock(m_mutex);
    return m_process;
}
}









