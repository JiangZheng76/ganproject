/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-20 15:24:48
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-20 15:26:03
 * @FilePath: /GanProject/Utils/singleton.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
namespace gan{

template<class T>
class SingletonPtr{
public:
    static std::shared_ptr<T> GetInstance(){
        static std::shared_ptr<T> v(new T());
        return v;
    }
};

}