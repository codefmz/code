#pragma once

#include <string>
#include <iostream>
#include <memory>

enum RequestType
{
    Type1 = 0,
    Type2,
    Type3,
    TypeNone
};

class Request
{
private:
    std::string desc;
    RequestType type;
public:
    Request(const std::string& _desc, RequestType _type) :desc(_desc), type(_type) {};
    RequestType getReqType() const {
        return type;
    }
    const std::string& getDesc() const {
        return desc;
    }
};

class Handle
{
protected:
    std::shared_ptr<Handle> nextHandle;
public:
    virtual ~Handle() = default;

    virtual bool handle(const Request& req) = 0;
    void setNextHandle(std::shared_ptr<Handle> handle) {
        this->nextHandle = handle;
    }
};


class JingLi : public Handle
{
public:
    virtual bool handle(const Request& req) {
        if (req.getReqType() == RequestType::Type1) {
            std::cout << "JingLi handle " << req.getDesc() << std::endl;
            return true;
        }

        if (nextHandle != nullptr) {
            return nextHandle->handle(req);
        }

        return false;
    }
};

class ZonJian : public Handle
{
public:
    virtual bool handle(const Request& req) {
        if (req.getReqType() <= RequestType::Type2) {
            std::cout << "ZonJian handle " << req.getDesc() << std::endl;
            return true;
        }

        if (nextHandle != nullptr) {
            return nextHandle->handle(req);
        }

        return false;
    }
};

class ZongJingLi : public Handle
{
public:
    virtual bool handle(const Request& req) {
        if (req.getReqType() <= RequestType::TypeNone) {
            std::cout << "ZongJingLi handle " << req.getDesc() << std::endl;
            return true;
        }

        if (nextHandle != nullptr) {
            return nextHandle->handle(req);
        }

        return false;
    }
};