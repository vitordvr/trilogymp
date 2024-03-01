#pragma once

template <class singleton_t>
class Singleton
{
protected:
    Singleton() {}
    ~Singleton() {}

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

public:
    static singleton_t* GetSingletonPtr()
    {
        if (!instance_ptr)
            instance_ptr = new singleton_t;

        return instance_ptr;
    }

private:
    inline static singleton_t* instance_ptr;
};
