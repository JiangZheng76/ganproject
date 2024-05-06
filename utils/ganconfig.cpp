#include <algorithm>
#include "ganconfig.h"
#include "unordered_map"
 
namespace gan{

static GanConfigVar<int>::ptr g_init_first_param 
            = GanConfig::Lookup<int>("first_param",1,"first params");

static void ListAllMember(const std::string& prefix,const YAML::Node& node
    ,std::unordered_map<std::string,const YAML::Node>& output){
    // 检查是否有不合法的字符
    if(prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz._/:1234567890") != std::string::npos){
        // SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << " config invalid name "<< prefix << " : "<< node ;
        qCritical() << "ListAllMember() config invalid name " << QString::fromStdString(prefix);
        return;
    }
    output.insert(std::make_pair(prefix,node));
    // 本节点是一个键值对时
    if(node.IsMap()){
        for(auto it= node.begin();
            it!=node.end();it++){
            // 每一个node 都有一个 Scalar（）表示自身的值,然后他的下面可以有不同的类型
            // 如果 map 下面还有 map ，那么scalar可以是 map 的名字，也可以是 map 的 value。
            // 键值对类型的 node 【prefix : node】
            if(it->first.Scalar().empty()){
                continue;
            }
            ListAllMember(prefix.empty()?
                it->first.Scalar():prefix+"." + it->first.Scalar()
                ,it->second,output);
        }
    }else if(node.IsSequence()){
        // 本节点是一个队列时
        for(size_t i = 0;i<node.size();i++){
            if(node[i].Scalar().empty()){
                continue;
            }
            ListAllMember(prefix.empty()? 
                node[i].Scalar() + "_" + std::to_string(i) : prefix+node[i].Scalar()+"_"+ std::to_string(i),node[i],output);
        }
    }
}
bool GanConfig::LoadFromYaml(const std::string& path,bool isOverride){
    YAML::Node node = YAML::LoadFile(path);
    if(!node.IsDefined()){
        return false;
    }
    if(isOverride){
        GetSdatas().clear();
    }
    std::unordered_map<std::string,const YAML::Node> all_node;
    ListAllMember("",node,all_node);
    for(auto & i: all_node){ 
        auto key = i.first;
        
        if(key.empty() || key[key.size()-1] == '.'){
            continue;
        }
        qInfo() <<"LoadFromYaml()" << QString::fromStdString(key) << " " << QString::fromStdString(i.second.Scalar());
        // 统一将prefix变成 lower 小写
        std::transform(key.begin(),key.end(),key.begin(),::tolower);
        auto it = GetSdatas().find(key);
        GanConfigVarBase::ptr var;
        if(it == GetSdatas().end()){
            // 原来是没有的话就添加进去
            if(!i.second.Scalar().empty()){
                var = std::dynamic_pointer_cast<GanConfigVarBase>(GanConfig::Lookup<std::string>(key,i.second.Scalar(),key));
            }
            continue;
        }else {
            var = it->second;
        }
         
        // 如果存在约定就更新里面的值,那什么时候插入新的值呢？？？（约定优于配置）
        // 不插入新的，程序启动的时候有什么，就使用什么，
        if(var){
            // 表示是一个 value，不是一个 key
            if(i.second.IsScalar()){
                // 从 string状态的 scalar 转换成对应类型的 value
                var->fromString(i.second.Scalar());
            }else {
                std::stringstream ss;
                //  这个不是很懂，map传进去然后也可以恢复成 map是么？？？
                // 如果不是scalar 类型，那么就将这个 node 转换成字符串，再送进去转换成 value
                ss << i.second;
                var->fromString(ss.str());
            }
        }
    }
    return true;

}
/**
 * @brief 通过名字 获取对应的 YAML：：Node 引用，用来修改里面的值
 * @param {Node&} node
 * @param {string&} nname
 * @description: 
 * @return {*}
 */
YAML::Node GetYamlNode(YAML::Node node,std::string& nname){
    // qDebug() << "GetYamlNode() " << QString::fromStdString(nname) ;
    size_t dotIdx = nname.find_first_of('.');
    size_t _idx = nname.find_first_of('_');
    if(dotIdx == std::string::npos && _idx == std::string::npos){
        // 说明当前的节点是最后一个了
        return node;
    }
    if(dotIdx != std::string::npos && _idx != std::string::npos){
        // dot 在后表示目前先是 sequence fsadf_123.adsfasf
        bool type = dotIdx > _idx;
        if(type){
            // sequence
            std::string subName = nname.substr(0,_idx+1);
            size_t len = nname.size();
            int numIdx = -1;
            size_t i;
            // 获取sequence 中的 numIdx 
            for(i=_idx;i<len;i++){
                char a = nname[i];
                if(a >= 48 && a <= 57){
                    if(numIdx == -1){
                        numIdx = static_cast<int>(a-48);
                        continue;
                    }else {
                        numIdx *= 10;
                        numIdx += static_cast<int>(a-48);
                    }
                }else {
                    break;
                }
            }
            // 数字之后的都是剩余名
            std::string remainName = nname.substr(i);
            return GetYamlNode(node[subName][numIdx],remainName);
        }else {
            // map
            std::string subName = nname.substr(0,dotIdx+1);
            std::string remainName = nname.substr(dotIdx + 1);
            return GetYamlNode(node[subName],remainName);
        }
    }else if(_idx != std::string::npos){
        // sequence
        std::string subName = nname.substr(0,_idx+1);
        size_t len = nname.size();
        int numIdx = -1;
        size_t i;
        // 获取sequence 中的 numIdx 
        for(i=_idx;i<len;i++){
            char a = nname[i];
            if(a >= 48 && a <= 57){
                if(numIdx == -1){
                    numIdx = static_cast<int>(a-48);
                    continue;
                }else {
                    numIdx *= 10;
                    numIdx += static_cast<int>(a-48);
                }
            }else {
                break;
            }
        }
        // 数字之后的都是剩余名
        std::string remainName = nname.substr(i);
        return GetYamlNode(node[subName][numIdx],remainName); 
    }else if (dotIdx != std::string::npos){
        // map        
        std::string subName = nname.substr(0,dotIdx+1);
        std::string remainName = nname.substr(dotIdx + 1);
        return GetYamlNode(node[subName],remainName);
    }
    SYLAR_ASSERT2(false,"GetYamlNode() error nname="+nname);
}

// 字典树
struct YamlTrie{
    typedef std::shared_ptr<YamlTrie> ptr;
    std::string key;
    std::string val;
    std::unordered_map<std::string,YamlTrie::ptr> nodes;
    bool isNode;
    YamlTrie()
        :isNode(false){}
    YamlTrie::ptr find(const std::string& subKey){
        if(nodes.find(subKey) == nodes.end()){
            nodes.insert(std::make_pair(subKey,new YamlTrie()));
        }
        return nodes[subKey];
    }
};

bool EmmiterYamlNode(YAML::Emitter& emitter){
    YamlTrie::ptr root(new YamlTrie());
    for(auto p : GanConfig::GetSdatas()){
        auto key = p.first;
        auto val = p.second;
        int dotIdx = 0;
        int startIdx = 0;
        YamlTrie::ptr node = root;
        qInfo() << "EmmiterYamlNode() " << QString::fromStdString(key);
        // while((dotIdx = key.find_first_of('.',startIdx)) 
        //     != std::string::npos){
        //     std::string sub = key.substr(startIdx,dotIdx - startIdx);
        //     qInfo() << "EmmiterYamlNode() " << QString::fromStdString(sub);
        //     auto sub_node = node->find(sub);
        //     node = sub_node;
        //     startIdx = dotIdx+1;
        // }
        std::string sub = key.substr(startIdx);
        YamlTrie::ptr sub_node = node->find(sub);
        if(!sub_node){
            qWarning() << "EmmiterYamlNode() " << "sub_node is nullptr.";
            return false;
        }
        sub_node->isNode = true;
        sub_node->key = sub;
        sub_node->val = val->toString();
        qInfo() << "EmmiterYamlNode() " << QString::fromStdString(sub_node->key) << " " << QString::fromStdString(sub_node->val); 
    }
}
/**
 * @brief 修改之后覆盖原有 Yaml
 * @description: 
 * @return {*}
 */
void GanConfig::OverrideCurYaml(){
    // YAML::Node node = YAML::LoadFile(YAML_PATH);
    // for(auto d : GetSdatas()){
    //     std::string uname = d.first;
    //     QString qname =  QString::fromStdString(uname);
    //     QString qval = QString::fromStdString(d.second->toString());
    //     // 感觉这里会有问题？？？
    //     YAML::Node subnode = GetYamlNode(node,uname);
    //     qDebug() << qname << ": [after:" << qval << "]" 
    //             << "[before:" 
    //             << QString::fromStdString(subnode.Scalar()) << "]";
    //     subnode = d.second->toString();
    // }
    std::ofstream fout(YAML_PATH);
    YAML::Emitter emitter(fout);
    EmmiterYamlNode(emitter);
    // emitter << node;
    qInfo() << "Overrider current YAML success.";
    return ;
}

bool GanConfig::SaveLastYaml(){

}

const std::string GanConfig::dump(){
    std::stringstream ss;
    if(GetSdatas().empty()){
        ss << "Gan Config without any params!";
        return ss.str();
    }
    ss << "show all params :\n";
    for(auto& p : GetSdatas()){
        ss << p.first << "," << p.second->toString() << "\n";
    }
    return ss.str();
}


}
