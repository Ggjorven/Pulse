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
        // Calls the constructor with all the native types
        template<typename Type, typename ...Args>
        static Type UseNativeArgTypesInType(const std::vector<std::any>& args);

        // Calls the function with all the native types
        template<typename Func, typename ...Args>
        static auto UseNativeArgTypesInFunc(Func&& func, const std::vector<std::any>& args) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;

        // Converts a member function into a static-like function
        template <typename ObjType, typename ReturnType, typename... Args>
        static auto ToStatic(ReturnType(ObjType::* memFn)(Args...));

        // Calls the function with all the native types in a made ^static member function^
        template<typename Func, typename ClassType, typename ...Args>
        static auto UseNativeArgTypesInSMemFunc(Func&& func, ClassType* instance, const std::vector<std::any>& args) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;
    };

    ///////////////////////////////////////////////////////////
    // Implementation
    ///////////////////////////////////////////////////////////
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

    template <typename ObjType, typename ReturnType, typename... Args>
    auto Utils::ToStatic(ReturnType(ObjType::* memFn)(Args...)) 
    {
        return [memFn](ObjType* obj, Args... args) -> ReturnType { return (obj->*memFn)(args...); };
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