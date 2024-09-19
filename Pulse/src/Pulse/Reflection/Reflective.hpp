#pragma once

#include "Pulse/Core/Defines.hpp"
#include "Pulse/Core/Ref.hpp"

#include "Pulse/Reflection/ClassRegistry.hpp"

#include "Pulse/Types/TypeUtils.hpp"

#include <string>

namespace Pulse::Reflection
{

	// Interface from which all classes should inherit
	class Reflective : public RefCounted
	{
	public:
		Reflective() = default;
		virtual ~Reflective() = default;
	};

	class ReflectedClass
	{
	public:
		template<typename ...Args>
		ReflectedClass(const std::string& className, Args&& ...args)
			: m_ClassName(className), m_Instance(s_Allocator.Instantiate(className, { std::any_cast<Args>(args)... }))
		{
		}
		~ReflectedClass() = default;

	public:
		template<typename T>
		void Set(const std::string& varName, T value)
		{
			if constexpr (std::is_reference_v<T>)
				s_Allocator.SetMemVar(m_ClassName, varName, m_Instance, std::any(std::reference_wrapper<Types::Clean<T>>(value)));
			else
				s_Allocator.SetMemVar(m_ClassName, varName, m_Instance, std::any(value));
		}

		template<typename T>
		T& Get(const std::string& varName)
		{
			T& val = std::any_cast<std::reference_wrapper<Types::Clean<T>>>(s_Allocator.GetMemVar(m_ClassName, varName, m_Instance));
			return val;
		}

	public:
		template<typename T, typename ...Args>
		T Run(const std::string& fnName, Args&& ...args)
		{
			if constexpr (std::is_reference_v<T>)
			{
				T val = std::any_cast<std::reference_wrapper<Types::Clean<T>>>(s_Allocator.RunMember(m_ClassName, fnName, m_Instance, { std::any_cast<Args>(args)... }));
				return val;
			}
			else if constexpr (!std::is_void_v<T>)
				return std::any_cast<T>(s_Allocator.RunMember(m_ClassName, fnName, m_Instance, { std::any_cast<Args>(args)... }));
		}

		inline static void SetAllocator(ClassRegistry& registry) { s_Allocator = registry; }

	private:
		std::string m_ClassName;
		ClassRegistry::ValueContainer m_Instance;

		inline static ClassRegistry& s_Allocator = ClassRegistry::Get();
	};

}