#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

template<typename T>
class Singleton {
protected:

    Singleton(){}

    static T* INSTANCE;

    virtual ~Singleton(){}

public:

    static T* getInstance(){
        if(!INSTANCE)
            INSTANCE = new T();
        return INSTANCE;
    }

    static void destroy() {
        delete INSTANCE;
        INSTANCE = NULL;
    }

    bool isInitialized(){
        return INSTANCE != NULL;
    }
};

#endif