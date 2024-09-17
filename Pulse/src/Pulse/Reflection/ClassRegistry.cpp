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



	u64 TypeHash::operator () (const std::vector<u64>& typeHashes) const
	{
		u64 seed = 0;
		for (u64 hash : typeHashes)
			Hash::Mix(seed, hash);

		return seed;
	}

	void ClassRegistry::AddClass(const std::string& className, const std::vector<u64>& constructorTypeHashes, Constructor&& constructor, Destructor&& destructor)
	{
		m_Constructors[className][constructorTypeHashes] = std::move(constructor);
		
		if (destructor) 
			m_Destructors[className] = std::move(destructor);
	}

	void ClassRegistry::AddMemFn(const std::string& className, const std::string& fnName, const std::vector<u64>& fnTypeHashes, MemberFunction func)
	{
		m_MemberFunctions[className][fnName][fnTypeHashes] = std::move(func);
	}

	void ClassRegistry::AddMemVar(const std::string& className, const std::string& varName, MemberSetFn&& setter, MemberGetFn&& getter)
	{
		m_MemberVarSetters[className][varName] = std::move(setter);
		m_MemberVarGetters[className][varName] = std::move(getter);
	}

	ClassRegistry::ValueContainer ClassRegistry::Instantiate(const std::string& className, ArgsContainer args)
	{
		// Retrieve the hash codes for types passed in
		std::vector<u64> hashes;
		hashes.reserve(args.size());

		for (const auto& arg : args)
			hashes.push_back(arg.type().hash_code());

		// Call the constructor
		if (m_Constructors.contains(className))
			return m_Constructors[className][hashes](args);

		return nullptr;
	}

	std::any ClassRegistry::RunMember(const std::string& className, const std::string& fnName, Reflective* instance, ArgsContainer args)
	{
		// Retrieve the hash codes for types passed in
		std::vector<u64> hashes;
		hashes.reserve(args.size());

		for (const auto& arg : args)
			hashes.push_back(arg.type().hash_code());

		// Call the function
		if (m_MemberFunctions.contains(className) && m_MemberFunctions.at(className).contains(fnName))
			return m_MemberFunctions[className][fnName][hashes](instance, args);

		return {};
	}

	void ClassRegistry::SetMemVar(const std::string& className, const std::string& varName, Reflective* instance, std::any value)
	{
		if (m_MemberVarSetters.contains(className) && m_MemberVarSetters.at(className).contains(varName))
			m_MemberVarSetters[className][varName](instance, value);
	}

	std::any ClassRegistry::GetMemVar(const std::string& className, const std::string& varName, Reflective* instance)
	{
		if (m_MemberVarGetters.contains(className) && m_MemberVarGetters.at(className).contains(varName))
			return m_MemberVarGetters[className][varName](instance);

		return {};
	}

	ClassRegistry& ClassRegistry::Get()
	{
		return GetRegistry();
	}

}