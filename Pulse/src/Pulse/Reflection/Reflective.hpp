#pragma once

#include "Pulse/Core/Defines.hpp"

#include "Pulse/Core/Ref.hpp"

#include "Pulse/Reflection/ClassRegistry.hpp"

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
			: m_ClassName(className), m_Instance(ClassRegistry::Get().Instantiate(className, { std::any_cast<Args>(args)... }))
		{
		}
		~ReflectedClass() = default;

	public:
		template<typename T>
		void Set(const std::string& varName, T value)
		{
			if (std::is_reference_v<T>)
				ClassRegistry::Get().SetMemVar(m_ClassName, varName, m_Instance, std::any(std::reference_wrapper<std::decay_t<T>>(value)));
			else
				ClassRegistry::Get().SetMemVar(m_ClassName, varName, m_Instance, std::any(value));
		}

		template<typename T>
		T& Get(const std::string& varName)
		{
			T& val = std::any_cast<std::reference_wrapper<std::decay_t<T>>>(ClassRegistry::Get().GetMemVar(m_ClassName, varName, m_Instance));
			return val;
		}

	public:
		template<typename T, typename ...Args>
		T Run(const std::string& fnName, Args&& ...args)
		{
			if (std::is_reference_v<T>)
			{
				T val = std::any_cast<std::reference_wrapper<std::decay_t<T>>>(ClassRegistry::Get().RunMember(m_ClassName, fnName, m_Instance, { std::any_cast<Args>(args)... }));
				return val;
			}
			else
				return std::any_cast<T>(ClassRegistry::Get().RunMember(m_ClassName, fnName, m_Instance, { std::any_cast<Args>(args)... }));
		}

	private:
		std::string m_ClassName;
		ClassRegistry::ValueContainer m_Instance;
	};

}