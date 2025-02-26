// QYF Legal Description


#include "AbilitySystem/Data/AttributeInfo.h"
#include "GASGame250107/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInfos)
	{
		if (Info.AttributeTag == AttributeTag /** 也可以使用：Info.AttributeTag.MatchesTagExact(AttributeTag)*/)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FAuraAttributeInfo();
}
