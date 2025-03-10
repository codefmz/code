#pragma once
#include <memory>
#include <iostream>

class Music {
public:
    virtual ~Music() = default;
    //业务逻辑2
    virtual void playSound() = 0;
};

class Message {
public:
    virtual ~Message() = default;
    Message(std::shared_ptr<Music> music) : music(music) {
    }
    //业务逻辑1
    virtual void normal() = 0;
    virtual void special() = 0;
protected:
    std::shared_ptr<Music> music;
};

class DingDing :public Music
{
public:
    virtual void playSound() override {
        std::cout << "DingDing~~~" << std::endl;
    }
};

class DangDang :public Music
{
public:
    virtual void playSound() override {
        std::cout << "DangDang~~~" << std::endl;
    }
};


class MessageLite : public Message
{
public:
    MessageLite(std::shared_ptr<Music> music) : Message(music) {
    };
    virtual void normal() override {
        music->playSound();
        std::cout << "MessageLite :: normal" << std::endl;
    }
    virtual void special() override {
        music->playSound();
        std::cout << "MessageLite :: special" << std::endl;
    }
};

class MessagePrefect : public Message
{
public:
    MessagePrefect(std::shared_ptr<Music> music) : Message(music) {};

    virtual void normal() override {
        music->playSound();
        std::cout << "MessagePrefect :: normal" << std::endl;
    }
    virtual void special() override {
        music->playSound();
        std::cout << "MessagePrefect :: special" << std::endl;
    }
};