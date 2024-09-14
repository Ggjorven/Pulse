#include "plpch.h"
#include "ClassRegistry.hpp"

#include "Pulse/Core/Logging.hpp"

namespace Pulse::Reflection
{

	// Anonymous namespace to ensure it can't be accessed through extern
	namespace 
	{
		static Unique<ClassRegistry> s_Instance = Unique<ClassRegistry>::Create();
	}



	ClassRegistry& ClassRegistry::Get()
	{
		return *s_Instance;
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

	void ClassRegistry::CallMemberFunction(const std::string& className, const std::string& functionName, IReflectable* instance, const std::vector<std::any>& args)
	{
		if (m_MemberFunctions.contains(className))
			m_MemberFunctions[className][functionName](instance, args);
	}

}