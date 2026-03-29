// Copyright 2024 Peteresi Games. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MathInvaderPROTarget : TargetRules
{
	public MathInvaderPROTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5Latest;
		ExtraModuleNames.Add("MathInvaderPRO");
	}
}
