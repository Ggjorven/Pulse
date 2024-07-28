#pragma once

#include <utility>

namespace Pulse
{

    // TODO: Add T2 support
    template<typename T>
    class Unique
    {
    public:
        // Constructors
        Unique() = default;
        Unique(T* ptr);
        Unique(Unique&& other);
        template<typename T2>
        Unique(Unique<T2>&& other);
        ~Unique();

        Unique(const Unique&) = delete;
        
        // Operators
        Unique& operator = (const Unique&) = delete;
        
        Unique& operator = (Unique&& other);
        template<typename T2>
        Unique& operator = (Unique<T2>&& other);

        inline T& operator * () { return *m_Instance; }
        inline const T& operator * () const { return *m_Instance; }

        inline T* operator -> () { return m_Instance; }
        inline const T* operator -> () const { return m_Instance; }

        inline operator bool() { return m_Instance != nullptr; }
        inline operator bool() const { return m_Instance != nullptr; }

        // Methods
        inline T* Raw() { return m_Instance; }
        inline const T* Raw() const { return m_Instance; }

        T* Release();
        void Reset(T* newPtr = nullptr);
        void Swap(Unique& other);

        template<typename T2>
        Unique<T2> As() const;

        template<typename... Args>
        static Unique<T> Create(Args&&... args);

    private:
        T* m_Instance = nullptr;

        template<typename T2>
        friend class Ref;
    };

    ///////////////////////////////////////////////////////////
    // Constructors
    ///////////////////////////////////////////////////////////
    template<typename T>
    Unique<T>::Unique(T* ptr)
        : m_Instance(ptr) {}

    template<typename T>
    Unique<T>::Unique(Unique&& other)
        : m_Instance(other.Release()) {}

    template<typename T>

    template<typename T2>
    Unique<T>::Unique(Unique<T2>&& other)
        : m_Instance((T*)other.Release()) {}

    template<typename T>
    Unique<T>::~Unique()
    {
        delete m_Instance;
    }

    ///////////////////////////////////////////////////////////
    // Operators
    ///////////////////////////////////////////////////////////
    template<typename T>
    Unique<T>& Unique<T>::operator = (Unique<T>&& other) 
    {
        if (this != &other)
            Reset(other.Release());
        
        return *this;
    }

    template<typename T>
    template<typename T2>
    Unique<T>& Unique<T>::operator = (Unique<T2>&& other)
    {
        m_Instance = other.Release();
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////
    template<typename T>
    T* Unique<T>::Release()
    {
        T* temp = m_Instance;
        m_Instance = nullptr;

        return temp;
    }

    template<typename T>
    void Unique<T>::Reset(T* newPtr)
    {
        T* oldPtr = m_Instance;
        m_Instance = newPtr;

        if (oldPtr)
            delete oldPtr;
    }

    template<typename T>
    void Unique<T>::Swap(Unique& other) 
    {
        std::swap(m_Instance, other.m_Instance);
    }

    template<typename T>
    template<typename T2>
    Unique<T2> Unique<T>::As() const
    {
        return Unique<T2>((T2*)Release());
    }

    template<typename T>
    template<typename ...Args>
    Unique<T> Unique<T>::Create(Args&& ...args)
    {
        return Unique<T>(new T(std::forward<Args>(args)...));
    }

}
