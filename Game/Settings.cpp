#include "Settings.h"
#include "CSV.h"
#include "Event.h"
#include "Utility.h"
#include <stdint.h>

std::unordered_map<std::string, Settings::TypeUnion> Settings::sTable;

void Settings::Init(const std::string path)
{
	CSV csv(path, '=');
	TypeUnion un;
	std::string type;
	for (int i = 0; i < csv.GetRowCount(); i++)
	{
		try
		{
			type = csv.GetCell(i, 1);
			lower(type);

			if (type == "int")
			{
				un = TypeUnion(std::stoi(csv.GetCell(i, 2)));
			}
			else if (type == "float" || type == "flt")
			{
				un = TypeUnion(std::stof(csv.GetCell(i, 2)));
			}
			else if (type == "string" || type == "str")
			{
				un = TypeUnion(csv.GetCell(i, 2));
			}
			else
			{
				LOG_PUSH("can't parse setting type");
				continue;
			}

			sTable[csv.GetCell(i, 0)] = un;
		}
		catch (...)
		{
			LOG_PUSH("can't parse setting type (outer)");
			continue;
		}
	}
}

int Settings::TryGetI(const std::string& key)
{
	try
	{
		auto un = sTable.at(key);
		if (un.type == TypeUnion::Type::INT)
		{
			return un.i;
		}
		return INT_MIN;
	}
	catch (std::exception)
	{
		return INT_MIN;
	}
}

float Settings::TryGetF(const std::string& key)
{
	try
	{
		auto un = sTable.at(key);
		if (un.type == TypeUnion::Type::FLT)
		{
			return un.f;
		}
		return FLT_MIN;
	}
	catch (std::exception)
	{
		return FLT_MIN;
	}
}

const std::string& Settings::TryGetS(const std::string& key)
{
	try
	{
		auto un = sTable.at(key);
		if (un.type == TypeUnion::Type::STR)
		{
			return un.s;
		}
		return std::string();
	}
	catch (std::exception)
	{
		return std::string();
	}
}

void Settings::Set(const std::string& key, int val)
{
	sTable[key] = TypeUnion(val);
	EventBuilder::SettingsChanged();
}

void Settings::Set(const std::string& key, float val)
{
	sTable[key] = TypeUnion(val);
	EventBuilder::SettingsChanged();
}

void Settings::Set(const std::string& key, const std::string& val)
{
	sTable[key] = TypeUnion(val);
	EventBuilder::SettingsChanged();
}




Settings::TypeUnion::TypeUnion()
	: type(Type::INT), i(INT_MIN) {}

Settings::TypeUnion::TypeUnion(int i)
	: type(Type::INT), i(i) {}

Settings::TypeUnion::TypeUnion(float f)
	: type(Type::FLT), f(f) {}

Settings::TypeUnion::TypeUnion(const std::string& s)
	: type(Type::STR), s(s) {}

Settings::TypeUnion::TypeUnion(const TypeUnion& o)
	: type(o.type)
{
	if (type == Type::INT)
	{
		i = o.i;
	}
	else if (type == Type::FLT)
	{
		f = o.f;
	}
	else if (type == Type::STR)
	{
		s = o.s;
	}
	else
		SDL_assert(false);
}

Settings::TypeUnion::TypeUnion(TypeUnion&& o)
{
	if (type == Type::INT)
	{
		i = o.i;
	}
	else if (type == Type::FLT)
	{
		f = o.f;
	}
	else if (type == Type::STR)
	{
		s = std::move(o.s);
	}
	else
		SDL_assert(false);
}

Settings::TypeUnion& Settings::TypeUnion::operator=(const TypeUnion& o)
{
	if (this != &o)
	{
		if (type == Type::INT)
		{
			i = o.i;
		}
		else if (type == Type::FLT)
		{
			f = o.f;
		}
		else if (type == Type::STR)
		{
			s = o.s;
		}
		else
			SDL_assert(false);
	}
	return *this;
}

Settings::TypeUnion& Settings::TypeUnion::operator=(TypeUnion&& o)
{
	if (this != &o)
	{
		if (type == Type::INT)
		{
			i = o.i;
		}
		else if (type == Type::FLT)
		{
			f = o.f;
		}
		else if (type == Type::STR)
		{
			s = std::move(o.s);
		}
		else
			SDL_assert(false);
	}
	return *this;
}

Settings::TypeUnion::~TypeUnion()
{
	if (type == Type::STR)
	{
		s.~basic_string();
	}
	else 
	{
		// unsopportet type... add it 
		SDL_assert(type != Type::INT && type != Type::FLT);
	}
}
