/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-18 11:05:18
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-20 15:59:50
 * @FilePath: /GanProject/Utils/Config.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <boost/lexical_cast.hpp>
#include <memory>
#include <string>
#include <map>
#include <singleton.h>
namespace gan{

template<class F,class T>
class LexicalCast{
public:
    T operator()(const F& v){
        return boost::lexical_cast<T>(v);
    }
};


class ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name,const std::string& descriptor)
    :m_name(name),m_descriptor(descriptor){
    }
    const std::string& getName(){return m_name;}
    const std::string& getDescriptor(){return m_descriptor;}

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string& str)=0;

    virtual const std::string& getType()=0;
private:
    std::string m_name;
    std::string m_descriptor;
};

template<class T,class FromStr=LexicalCast<std::string,T>
    ,class ToStr=LexicalCast<T,std::string>>
class ConfigVar : public ConfigVarBase{
private:
    T m_val;
    std::map<uint64_t,on_change_cb> m_cbs;
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void()> on_change_cb;
    ConfigVar(const std::string& name, T& default_val,const std::string& descriptor)
        :ConfigVarBase(name,descriptor)
        ,m_val(default_val){
    }

    void setVal(T& val){
        m_val = val;
        // 更新之后调用回调函数
        for(auto& cb : m_cbs){
            cb();
        }
    }

    T getVal(){
        return m_val;
    }
    std::string toString() override {
        try{
            return ToStr(m_val);
        }catch(std::exception& e){
            std::cerr << e.what() << '\n';
        }
        return "";
    }
    bool fromString(const std::string& str) override {
        try
        {
            setVal(FromStr()(str));
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return false;
    }
    const std::string& getType() override {
        return typeid(T).name();
    }
    uint64_t addListener(on_change_cb cb){
        static uint64_t s_cb_id = 0;
        s_cb_id++;
        m_cbs[s_cb_id] = cb;
        return s_cb_id;
    }
    void delListener(uint64_t cb_id){
        m_cbs.erase(cb_id);
    }
    on_change_cb getListener(uint64_t cb_id){
        if(m_cbs.find(cb_id) != m_cbs.end()){
            return m_cbs[cb_id];
        }
        return nullptr;
    }
};

class ConfigManager {
    // 从 yaml 文件中读取然后展示出来
    // 将数据转化成 yaml 文件
    typedef std::map<std::string,ConfigVarBase::ptr> ConfigVarMap;
    typedef std::shared_ptr<ConfigManager> ptr;

    ConfigManager();

    std::string saveToRandYaml();
    void loadFromYaml(const std::string& path);
    void overrideCurYaml();

    
    // 寻找但是并不修改
    // 没有找到返回 nullptr
    template<class T>
    typename ConfigVar<T>::ptr lookup(const std::string& name){
        auto it = m_datas.find(name);
        if(it == m_datas.end()){
            return nullptr;
        }
        // 安全的转换，没有转换成功会返回 nullptr
        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }

    template<class T>
    typename ConfigVar<T>::ptr lookup(const std::string& name
        , const T& default_val,const std::string& descriptor){

        // 检查名字异常
        if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._0123456789")
            != std::string::npos){
            throw std::invalid_argument(name); 
        }
        auto var_ptr = lookup<T>(name);
        if(var_ptr){
            var_ptr->setVal(default_val);
            return var_ptr;
        }else {
            var_ptr = make_shared<ConfigVar<T>>(name,default_val,descriptor);
            m_datas.insert(make_pair(name,var_ptr));
            return var_ptr;
        }

    }   
private:
    std::string m_randpath;
    std::string m_curpath;
    
    ConfigVarMap m_datas;
    
};

typedef SingletonPtr<ConfigManager>  cfgMgr;


}