#pragma once

#include "vector"

template<typename T>
class Iterator {
public:
    virtual T& first() = 0;
    virtual T& next() = 0;
    virtual bool isDone() const = 0;
    virtual T& current() = 0;
    virtual ~Iterator() = default;
};

template<typename T>
class MyCollection;

template<typename T>
class CollectionIterator : public Iterator<T> {
public:
    CollectionIterator(const MyCollection<T>& c) :mc(c), index(0) {};
    virtual T& first() override {
        return mc.vct.front();
    };

    virtual T& next() override {
        index++;
        return mc.vct[index];
    };

    virtual bool isDone() const override {
        return index > mc.vct.size();
    };
    virtual T& current() override {
        return mc.vct[index];
    };

private:
    MyCollection<T> mc;
    int index;
};

template<typename T>
class MyCollection
{
public:
    Iterator<T>* GetIterator() {
        if (!iter) {
            iter = new CollectionIterator<T>(*this);
        }
        return iter;
    }

    void add(std::initializer_list<string> il) {
        for (auto& i : il) {
            vct.push_back(i);
        }
    }


private:
    std::vector<T> vct;
    Iterator<T>* iter;
};