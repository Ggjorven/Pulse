#pragma once

#include "Pulse/Core/Defines.hpp"
#include "Pulse/Core/Logging.hpp"

#include "Pulse/Core/Ref.hpp"
#include "Pulse/Core/Unique.hpp"

#include <any>
#include <functional>

namespace Pulse::Reflection
{

    // Interface from which all classes should inherit
    class IReflectable : public RefCounted 
    { 
    public:
        virtual ~IReflectable() = default; 
    };



    // Registry for class and member function information
    class ClassRegistry
    {
    public:
        using ValueContainer = Ref<IReflectable>;
        using ArgsContainer = const std::vector<std::any>&;
        
        template<typename Key, typename Value>
        using Map = std::unordered_map<Key, Value>;

    public:
        using Constructor = std::function<ValueContainer(ArgsContainer)>;
        using Destructor = std::function<void(IReflectable*)>;
        using MemberFunction = std::function<void(IReflectable*, ArgsContainer)>;

    public:
        static ClassRegistry& Get();

    public:
        void RegisterClass(const std::string& className, Constructor ctor, Destructor dtor);
        void RegisterMemberFunction(const std::string& className, const std::string& functionName, MemberFunction func);

        ValueContainer CreateInstance(const std::string& className, const std::vector<std::any>& args = {});

        void CallMemberFunction(const std::string& className, const std::string& functionName, IReflectable* instance, const std::vector<std::any>& args = {});

    private:
        ClassRegistry() = default;

        Map<std::string, Constructor> m_Constructors = {};
        Map<std::string, Destructor> m_Destructors = {};
        Map<std::string, Map<std::string, MemberFunction>> m_MemberFunctions = {};

        friend Unique<ClassRegistry>;
    };

    // Variadic template for argument unpacking in constructors
    template <typename T, typename... Args>
    ClassRegistry::ValueContainer CreateObject(const std::vector<std::any>& args)
    {
        Logger::Assert((args.size() == sizeof...(Args)), "Argument size mismatch for class constructor");

        // Use a helper lambda to unpack and cast the arguments from std::any
        auto unpackArgs = [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            return Ref<T>::Create(std::any_cast<Args>(args[Is])...);
        };

        return unpackArgs(std::index_sequence_for<Args...>{});
    }

    // Macro to register a class with a constructor that takes arguments
    #define PULSE_REFLECT(cls, ...) \
public: \
        static Pulse::Ref<IReflectable> Create(const std::vector<std::any>& args) \
    { \
        return Pulse::Reflection::CreateObject<cls, __VA_ARGS__>(args); \
    } \
        static void Destroy(IReflectable* obj) { delete static_cast<cls*>(obj); } \
        static void RegisterReflection() { \
        \
            Pulse::Reflection::ClassRegistry::Get().RegisterClass(#cls, Create, Destroy); \
    }

}