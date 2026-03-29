// Copyright 2024 Peteresi Games. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MathInvaderPROEditorTarget : TargetRules
{
	public MathInvaderPROEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5Latest;
		ExtraModuleNames.Add("MathInvaderPRO");
	}
}
