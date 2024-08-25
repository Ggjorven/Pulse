------------------------------------------------------------------------------
-- Solution
------------------------------------------------------------------------------
outputdir = "%{cfg.buildcfg}-%{cfg.system}"

MacOSVersion = "14.5"

workspace "Pulse"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

group "Pulse"
	include "Pulse"
group ""

include "Sandbox"
------------------------------------------------------------------------------