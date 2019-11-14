// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Cyber_VitaeEditorTarget : TargetRules
{
	public Cyber_VitaeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Cyber_Vitae" } );
	}
}
