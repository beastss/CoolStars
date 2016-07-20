#include "PetSkill.h"
#include "cocos2d.h"
PetSkill *PetSkill::petSkillFactory(int skillId)
{
	switch (skillId)
	{
	case kEraseOnRow:
		return new PetSkillEraseOnRow();
	case kEraseOnColumn:
		return new PetSkillEraseOnColumn();
	case kAddSteps:
		return new PetSkillAddSteps();
	case kChangeColor:
		return new PetSkillChangeColor();
	default:
		assert(false && "no this skillId");
		return NULL;
		break;
	}
}

