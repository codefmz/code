#pragma once

#include <map>
#include <iostream>
#include <string>
#include <memory>

class User {
private:
    std::string name;
public:
    User(const std::string& _name) :name(_name) {};

    std::string getName() {
        return name;
    }
};

class WebSite {
public:
    virtual void Use(User &user) = 0;

    virtual ~WebSite() = default;
};

class ConcreteWebSite : public WebSite {
private:
    std::string name;
public:
    ConcreteWebSite(const std::string& _name) :name(_name) {};

    void Use(User &user) {
        std::cout << "网站分类：" << name << " 用户：" << user.getName() << std::endl;
    }

    virtual ~ConcreteWebSite() = default;
};

class FlyWightFactory {
private:
    std::map<std::string, std::shared_ptr<WebSite>> webSiteMap;
public:
    std::shared_ptr<WebSite> getWebSiteCategory(const std::string& key) {
        auto iter = webSiteMap.find(key);
        if (iter == webSiteMap.end()) {
            std::shared_ptr<WebSite> webSite = std::make_shared<ConcreteWebSite>(key);
            webSiteMap.insert({key, webSite});
            return webSite;
        }
        return iter->second;
    }

    int getWebSiteCount() {
        return webSiteMap.size();
    }
};