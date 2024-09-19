#pragma once

#include "Pulse/Core/Defines.hpp"
#include "Pulse/Core/Logging.hpp"

#include "Pulse/Core/Ref.hpp"
#include "Pulse/Core/Unique.hpp"

#include "Pulse/Types/TypeUtils.hpp"

#include "Pulse/Utils/Hash.hpp"
#include "Pulse/Utils/Utils.hpp"
#include "Pulse/Utils/Macros.hpp"

#include <cstddef>
#include <any>
#include <vector>
#include <utility>
#include <functional>

namespace Pulse::Reflection
{

    typedef char Empty; // Used for :: resolution. (Ex. ::Pulse::Reflection::Empty)

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
        using ValueContainer = Reflective*;
        using ArgsContainer = const std::vector<std::any>&;

        template<typename Key, typename Value, typename Hash = std::hash<Key>>
        using Map = std::unordered_map<Key, Value, Hash>;

    public:
        using Constructor = std::function<ValueContainer(ArgsContainer)>;
        using Destructor = std::function<void(Reflective*)>;
        using MemberFunction = std::function<std::any(Reflective*, ArgsContainer)>;

        // The std::any holds a std::reference_wrapper to the type
        using MemberSetFn = std::function<void(Reflective*, std::any)>;
        using MemberGetFn = std::function<std::any(Reflective*)>;

    public:
        void AddClass(const std::string& className, const std::vector<u64>& constructorTypeHashes, Constructor&& constructor, Destructor&& destructor);
        void AddMemFn(const std::string& className, const std::string& fnName, const std::vector<u64>& fnTypeHashes, MemberFunction func);
        void AddMemVar(const std::string& className, const std::string& varName, MemberSetFn&& setter, MemberGetFn&& getter);

        ValueContainer Instantiate(const std::string& className, ArgsContainer args = {});
        std::any RunMember(const std::string& className, const std::string& fnName, Reflective* instance, ArgsContainer args = {});

        // Note: Returns an std::any that contains a std::reference_wrapper
        void SetMemVar(const std::string& className, const std::string& varName, Reflective* instance, std::any value);
        std::any GetMemVar(const std::string& className, const std::string& varName, Reflective* instance);

    public:
        static ClassRegistry& Get();

    private:
        // Class reflection info
        Map<std::string, Map<std::vector<u64>, Constructor, TypeHash>> m_Constructors = {};
        Map<std::string, Destructor> m_Destructors = {};
        Map<std::string, Map<std::string, Map<std::vector<u64>, MemberFunction, TypeHash>>> m_MemberFunctions = {};

        Map<std::string, Map<std::string, MemberSetFn>> m_MemberVarSetters = {};
        Map<std::string, Map<std::string, MemberGetFn>> m_MemberVarGetters = {};
    };

    // Utils for Dll importing/exporting reflected vars & functions.
    using GetClassRegistryFn = std::function<ClassRegistry*()>;
    PULSE_FULL_EXPORT inline ClassRegistry* _CGetClassRegistry() { return &ClassRegistry::Get(); }

    ///////////////////////////////////////////////////////////
    // Helper function for creating reflection objects
    ///////////////////////////////////////////////////////////
    // Variadic template for argument unpacking in constructors
    template <typename T, typename... Args>
    ClassRegistry::ValueContainer CreateObject(const std::vector<std::any>& args)
    {
        auto create = [](Args&& ...args) -> T* { return new T(std::forward<Args>(args)...); };
        return Utils::UseNativeArgTypesInFunc<decltype(create), Args...>(std::move(create), args);
    }



    ///////////////////////////////////////////////////////////
    // Macro for easy class reflection (must inherit from Reflective)
    ///////////////////////////////////////////////////////////
    // Note: Put all the reflection at the bottom or top of the class
    // as to not interfere with access specifiers
    ///////////////////////////////////////////////////////////
    // Example usage: REFLECT_CLASS_CTOR(clsName, int, double);
    #define PULSE_REFLECT_CLASS_CTOR(cls, ...)                                                                                              \
        private:                                                                                                                            \
        RUN_FUNCTION_NN(__COUNTER__,                                                                                                        \
        ::Pulse::Reflection::ClassRegistry::Get().AddClass, #cls,                                                                           \
        ::Pulse::Reflection::GetTypeHashes<__VA_ARGS__>(),                                                                                  \
        &::Pulse::Reflection::CreateObject<cls, __VA_ARGS__>,                                                                               \
        [](::Pulse::Reflection::Reflective* obj){ delete static_cast<cls*>(obj); }                                                          \
    )

    // Helper function that only works when retType is not void.
template <typename RetType, typename FuncType, typename ClsType, typename... Args>
std::enable_if_t<!std::is_void_v<RetType>, std::any>
CallWithReturn(FuncType&& func, ClsType* obj, const std::vector<std::any>& args)
{
    auto result = ::Pulse::Utils::UseNativeArgTypesInSMemFunc<std::decay_t<FuncType>, ClsType, Args...>(std::forward<FuncType>(func), obj, args);
    return std::make_any<decltype(result)>(result);
}

// Void overload
template <typename RetType, typename FuncType, typename ClsType, typename... Args>
std::enable_if_t<std::is_void_v<RetType>, std::any>
CallWithReturn(FuncType&& func, ClsType* obj, const std::vector<std::any>& args)
{
    ::Pulse::Utils::UseNativeArgTypesInSMemFunc<std::decay_t<FuncType>, ClsType, Args...>(std::forward<FuncType>(func), obj, args);
    return {}; // Return an empty std::any since there's no return value
}

    // Example usage: REFLECT_CLASS_MEMFN(clsName, MyMemFunc, char, int, char*);
    #define PULSE_REFLECT_CLASS_MEMFN(cls, fnName, retType, ...)                                                                            \
        private:                                                                                                                            \
        RUN_FUNCTION_NN(__COUNTER__,                                                                                                        \
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
                /* Doesn't work on linux? Even though there's a constexpr branch above it. */                                               \
                return CallWithReturn<retType>(std::move(func), obj, args);                                                                 \
            }                                                                                                                               \
                                                                                                                                            \
            return {};                                                                                                                      \
        }                                                                                                                                   \
    )

    ///////////////////////////////////////////////////////////
    // Member variables
    ///////////////////////////////////////////////////////////
    #define PULSE_REFLECT_CLASS_MEMVAR_GENERIC(cls, type, name)                                                                             \
        public:                                                                                                                             \
            void _Reflection_Set##name(const type& val)                                                                                     \
            {                                                                                                                               \
                name = val;                                                                                                                 \
            }                                                                                                                               \
                                                                                                                                            \
            type& _Reflection_Get##name()                                                                                                   \
            {                                                                                                                               \
                return name;                                                                                                                \
            }

    // Example usage: REFLECT_CLASS_MEMVAR(clsName, MyMemFunc, char, int, char*);
    #define PULSE_REFLECT_CLASS_MEMVAR(cls, type, name)                                                                                     \
        PULSE_REFLECT_CLASS_MEMVAR_GENERIC(cls, type, name)                                                                                 \
        private:                                                                                                                            \
        RUN_FUNCTION_NN(__COUNTER__,                                                                                                        \
        ::Pulse::Reflection::ClassRegistry::Get().AddMemVar, #cls, #name,                                                                   \
        [](::Pulse::Reflection::Reflective* instance, std::any value) -> void                                                               \
        {                                                                                                                                   \
            auto obj = static_cast<cls*>(instance);                                                                                         \
            auto func = ::Pulse::Utils::ToStatic<cls, void, const type&>(&cls::_Reflection_Set##name);                                      \
                                                                                                                                            \
            ::Pulse::Utils::UseNativeArgTypeInSMemFunc<decltype(func), cls, type>(std::move(func), obj, value);                             \
        },                                                                                                                                  \
        [](::Pulse::Reflection::Reflective* instance) -> std::any                                                                           \
        {                                                                                                                                   \
            auto obj = static_cast<cls*>(instance);                                                                                         \
            auto func = ::Pulse::Utils::ToStatic(&cls::_Reflection_Get##name);                                                              \
            return std::any(std::reference_wrapper(func(obj)));                                                                             \
        }                                                                                                                                   \
    )

}
