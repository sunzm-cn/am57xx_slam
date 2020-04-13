#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template<class T>
class Singleton
{
public:
    static T& Inst() {
        static T instance;
        return instance;
    }
protected:
    Singleton() {};
    ~Singleton() {};
};

#endif