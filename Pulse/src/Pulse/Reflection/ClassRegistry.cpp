#include "plpch.h"
#include "ClassRegistry.hpp"

#include "Pulse/Core/Logging.hpp"

#include "Pulse/Reflection/Reflective.hpp"

namespace Pulse::Reflection
{

	// Anonymous namespace to ensure it can't be accessed through extern
	namespace 
	{
		// Note: We use functions to make sure that the memory is initialized the first time we call it.
		// If we use just static variables they are not initialized in time.
		static ClassRegistry& GetRegistry()
		{
			static ClassRegistry registry = {};
			return registry;
		}
	}



	ClassRegistry& ClassRegistry::Get()
	{
		return GetRegistry();
	}

	void ClassRegistry::RegisterClass(const std::string& className, ClassRegistry::Constructor ctor, ClassRegistry::Destructor dtor)
	{
		m_Constructors[className] = ctor;
		m_Destructors[className] = dtor;
	}

	void ClassRegistry::RegisterMemberFunction(const std::string& className, const std::string& functionName, ClassRegistry::MemberFunction func)
	{
		m_MemberFunctions[className][functionName] = func;
	}

	ClassRegistry::ValueContainer ClassRegistry::CreateInstance(const std::string& className, const std::vector<std::any>& args)
	{
		if (m_Constructors.contains(className))
			return m_Constructors[className](args);

		return nullptr;
	}

	std::any ClassRegistry::CallMemberFunction(const std::string& className, const std::string& functionName, Reflective* instance, const std::vector<std::any>& args)
	{
		if (m_MemberFunctions.contains(className))
			return m_MemberFunctions[className][functionName](instance, args);

		return {};
	}

}