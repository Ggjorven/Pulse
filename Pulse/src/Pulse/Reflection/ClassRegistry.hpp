#pragma once

#include "Pulse/Core/Defines.hpp"
#include "Pulse/Core/Logging.hpp"

#include "Pulse/Core/Ref.hpp"
#include "Pulse/Core/Unique.hpp"

#include "Pulse/Types/TypeUtils.hpp"

#include "Pulse/Utils/Hash.hpp"
#include "Pulse/Utils/Utils.hpp"
#include "Pulse/Utils/Macros.hpp"

#include <any>
#include <vector>
#include <utility>
#include <functional>

namespace Pulse::Reflection
{

    // Forward-declare Reflective
    class Reflective;

    ///////////////////////////////////////////////////////////
    // Type hashing utilities
    ///////////////////////////////////////////////////////////
    template<typename... Types>
    constexpr std::vector<u64> GetTypeHashes() { return { typeid(Types).hash_code()... }; }

    struct TypeHash
    {
    public:
        u64 operator () (const std::vector<u64>& typeHashes) const;
    };

    ///////////////////////////////////////////////////////////
    // Registry for class and member function information
    ///////////////////////////////////////////////////////////
    class ClassRegistry
    {
    public:
        using ValueContainer = Ref<Reflective>;
        using ArgsContainer = const std::vector<std::any>&;
        
        template<typename Key, typename Value, typename Hash = std::hash<Key>>
        using Map = std::unordered_map<Key, Value, Hash>;

    public:
        using Constructor = std::function<ValueContainer(ArgsContainer)>;
        using Destructor = std::function<void(Reflective*)>;
        using MemberFunction = std::function<std::any(Reflective*, ArgsContainer)>;

    public:
        void AddClass(const std::string& className, const std::vector<u64>& constructorTypeHashes, Constructor constructor, Destructor destructor);
        void AddMemFn(const std::string& className, const std::string& fnName, const std::vector<u64>& fnTypeHashes, MemberFunction func);

        ValueContainer Instantiate(const std::string& className, ArgsContainer args = {});
        std::any RunMember(const std::string& className, const std::string& fnName, Reflective* instance, ArgsContainer args = {});

    public:
        static ClassRegistry& Get();

    private:
        Map<std::string, Map<std::vector<u64>, Constructor, TypeHash>> m_Constructors = {};
        Map<std::string, Destructor> m_Destructors = {};
        Map<std::string, Map<std::string, Map<std::vector<u64>, MemberFunction, TypeHash>>> m_MemberFunctions = {};
    };



    ///////////////////////////////////////////////////////////
    // Helper function for creating reflection objects
    ///////////////////////////////////////////////////////////
    // Variadic template for argument unpacking in constructors
    template <typename T, typename... Args>
    ClassRegistry::ValueContainer CreateObject(const std::vector<std::any>& args)
    {
        return Utils::UseNativeArgTypesInFunc<decltype(&Ref<T>::template Create<Args...>), Args...>(&Ref<T>::Create, args);
    }



    ///////////////////////////////////////////////////////////
    // Macro for easy class reflection (must inherit from Reflective)
    ///////////////////////////////////////////////////////////
    
// Example usage: REFLECT_CLASS_CTOR(clsName, int, double);
#define REFLECT_CLASS_CTOR(cls, ...)                                                                                                    \
RUN_FUNCTION_NN(__COUNTER__,                                                                                                            \
    ::Pulse::Reflection::ClassRegistry::Get().AddClass, #cls,                                                                           \
    ::Pulse::Reflection::GetTypeHashes<__VA_ARGS__>(),                                                                                  \
    &::Pulse::Reflection::CreateObject<cls, __VA_ARGS__>,                                                                               \
    [](::Pulse::Reflection::Reflective* obj){ delete static_cast<cls*>(obj); }                                                          \
)


// Example usage: REFLECT_CLASS_MEMFN(clsName, MyMemFunc, char, int, char*);
#define REFLECT_CLASS_MEMFN(cls, fnName, retType, ...)                                                                                  \
RUN_FUNCTION_NN(__COUNTER__,                                                                                                            \
    ::Pulse::Reflection::ClassRegistry::Get().AddMemFn, #cls, #fnName,                                                                  \
    ::Pulse::Reflection::GetTypeHashes<__VA_ARGS__>(),                                                                                  \
    [](::Pulse::Reflection::Reflective* instance, const std::vector<std::any>& args) -> std::any                                        \
    {                                                                                                                                   \
        auto obj = static_cast<cls*>(instance);                                                                                         \
        auto func = ::Pulse::Utils::ToStatic<cls, retType, __VA_ARGS__>(&cls::fnName);                                                  \
                                                                                                                                        \
        if constexpr (std::is_void_v<retType>)                                                                                          \
        {                                                                                                                               \
            ::Pulse::Utils::UseNativeArgTypesInSMemFunc<decltype(func), cls, __VA_ARGS__>(std::move(func), obj, args);                  \
            return {};                                                                                                                  \
        }                                                                                                                               \
        else                                                                                                                            \
        {                                                                                                                               \
            return std::any(::Pulse::Utils::UseNativeArgTypesInSMemFunc<decltype(func), cls, __VA_ARGS__>(std::move(func), obj, args)); \
        }                                                                                                                               \
    }                                                                                                                                   \
)


}