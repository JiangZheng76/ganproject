/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-27 20:44:40
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-27 20:54:26
 * @FilePath: /GanProject/Utils/generatethread.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef GENERATETHREAD_H
#define GENERATETHREAD_H
#include <thread.hh>
namespace gan{
class GenerateThread : public mysylar::Thread
{
public:
    typedef mysylar::SpinLock MutexType; 

    GenerateThread(std::function<void()> cb,const std::string& name);
    void mainWork();
    int getProc();
private:
    int m_process;
    MutexType m_mutex;

};
}


#endif // GENERATETHREAD_H
