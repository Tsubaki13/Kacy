// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

using UnrealBuildTool;
using System.Collections.Generic;

public class KacyTarget : TargetRules
{
	public KacyTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Kacy" } );
	}
}
