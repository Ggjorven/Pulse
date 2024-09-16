#pragma once

#include "Pulse/Core/Defines.hpp"
#include "Pulse/Core/Logging.hpp"

#include "Pulse/Types/Traits.hpp"

#include <any>
#include <vector>
#include <utility>
#include <functional>

namespace Pulse
{

    // TODO: Check MacOS support
    class Utils
    {
    public:
        // Converts a member function into a static-like function
        template <typename ObjType, typename ReturnType, typename... Args>
        static auto ToStatic(ReturnType(ObjType::* memFn)(Args...));

        ///////////////////////////////////////////////////////////
        // Single native arg type functions
        ///////////////////////////////////////////////////////////
        // Calls the constructor with the native type
        template<typename Type, typename ArgType>
        static Type UseNativeArgTypeInType(std::any arg);

        // Calls the function with the native type
        template<typename Func, typename ArgType>
        static auto UseNativeArgTypeInFunc(Func&& func, std::any arg) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;

        // Calls the function with the native type in a made ^static member function^
        template<typename Func, typename ClassType, typename ArgType>
        static auto UseNativeArgTypeInSMemFunc(Func&& func, ClassType* instance, std::any arg) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;

        ///////////////////////////////////////////////////////////
        // Multiple native arg types functions
        ///////////////////////////////////////////////////////////
        // Calls the constructor with all the native types
        template<typename Type, typename ...Args>
        static Type UseNativeArgTypesInType(const std::vector<std::any>& args);

        // Calls the function with all the native types
        template<typename Func, typename ...Args>
        static auto UseNativeArgTypesInFunc(Func&& func, const std::vector<std::any>& args) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;

        // Calls the function with all the native types in a made ^static member function^
        template<typename Func, typename ClassType, typename ...Args>
        static auto UseNativeArgTypesInSMemFunc(Func&& func, ClassType* instance, const std::vector<std::any>& args) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;
    };

    ///////////////////////////////////////////////////////////
    // Implementation
    ///////////////////////////////////////////////////////////
    template <typename ObjType, typename ReturnType, typename... Args>
    auto Utils::ToStatic(ReturnType(ObjType::* memFn)(Args...))
    {
        return [memFn](ObjType* obj, Args... args) -> ReturnType { return (obj->*memFn)(args...); };
    }

    template<typename Type, typename ArgType>
    Type Utils::UseNativeArgTypeInType(std::any arg)
    {
        return Type(std::any_cast<ArgType>(arg));
    }

    template<typename Func, typename ArgType>
    auto Utils::UseNativeArgTypeInFunc(Func&& func, std::any arg) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType
    {
        return func(std::any_cast<ArgType>(arg));
    }

    template<typename Func, typename ClassType, typename ArgType>
    auto Utils::UseNativeArgTypeInSMemFunc(Func&& func, ClassType* instance, std::any arg) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType
    {
        return func(instance, std::any_cast<ArgType>(arg));
    }

    template<typename Type, typename ...Args>
    Type Utils::UseNativeArgTypesInType(const std::vector<std::any>& args)
    {
        Logger::Assert((args.size() == sizeof...(Args)), "Argument size mismatch.");

        auto unpackArgs = [&]<std::size_t... Is>(std::index_sequence<Is...>) -> Type
        {
            return Type(std::any_cast<Args>(args[Is])...);
        };

        return unpackArgs(std::index_sequence_for<Args...>());
    }

    template<typename Func, typename ...Args>
    auto Utils::UseNativeArgTypesInFunc(Func&& func, const std::vector<std::any>& args) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType
    {
        Logger::Assert((args.size() == sizeof...(Args)), "Argument size mismatch.");

        auto unpackArgs = [&]<std::size_t... Is>(std::index_sequence<Is...>) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType
        {
            return func(std::any_cast<Args>(args[Is])...);
        };

        return unpackArgs(std::index_sequence_for<Args...>());
    }

    template<typename Func, typename ClassType, typename ...Args>
    auto Utils::UseNativeArgTypesInSMemFunc(Func&& func, ClassType* instance, const std::vector<std::any>& args) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType
    {
        Logger::Assert((args.size() == sizeof...(Args)), "Argument size mismatch.");

        using ReturnType = typename Pulse::Types::FunctionTraits<Func>::ReturnType;
        auto unpackArgs = [&]<std::size_t... Is>(ClassType* instance, std::index_sequence<Is...>) -> ReturnType
        {
            return func(instance, std::any_cast<Args>(args[Is])...);
        };

        return unpackArgs(instance, std::index_sequence_for<Args...>());
    }

}