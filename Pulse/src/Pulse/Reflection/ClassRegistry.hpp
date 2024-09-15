#pragma once

#include "Pulse/Core/Defines.hpp"
#include "Pulse/Core/Logging.hpp"

#include "Pulse/Core/Ref.hpp"
#include "Pulse/Core/Unique.hpp"

#include "Pulse/Utils/Hash.hpp"

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
    std::vector<u64> GetTypeHashes()
    {
        return { typeid(Types).hash_code()... };
    }

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
        // Can be called multiple times for multiple constructors
        void AddClass(const std::string& className, const std::vector<u64>& constructorTypeHashes, Constructor constructor, Destructor destructor = nullptr);

        ValueContainer Instantiate(const std::string& className, ArgsContainer args = {});

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
        Logger::Assert((args.size() == sizeof...(Args)), "Argument size mismatch for class constructor");

        // Use a helper lambda to unpack and cast the arguments from std::any
        auto unpackArgs = [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            return Ref<T>::Create(std::any_cast<Args>(args[Is])...);
        };

        return unpackArgs(std::index_sequence_for<Args...>{});
    }



    ///////////////////////////////////////////////////////////
    // Macro for easy class reflection (must inherit from Reflective)
    ///////////////////////////////////////////////////////////
    // TODO: Support multiple constructors and member functions
    #define REFLECT_CLASS(cls, ...)                                                                     \
    public:                                                                                             \
        static Pulse::Ref<Pulse::Reflection::Reflective> _Create(const std::vector<std::any>& args)     \
        {                                                                                               \
            return Pulse::Reflection::CreateObject<cls, __VA_ARGS__>(args);                             \
        }                                                                                               \
                                                                                                        \
        static void _Destroy(Reflective* obj)                                                           \
        {                                                                                               \
            delete static_cast<cls*>(obj);                                                              \
        }                                                                                               \
                                                                                                        \
        static void _RegisterReflection()                                                               \
        {                                                                                               \
            const std::vector<Pulse::u64> typeHashes = Pulse::Reflection::GetTypeHashes<__VA_ARGS__>(); \
            Pulse::Reflection::ClassRegistry::Get().AddClass(#cls, typeHashes, &_Create, &_Destroy);    \
        }

}