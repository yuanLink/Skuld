#pragma once

#include "Stream.h"

namespace Skuld
{
	SKULD_EXPORT String ReadAllTextFromFile(const String& mFile);
	SKULD_EXPORT String ReadAllTextFromStream(Stream *mStream);
}