#pragma once
#include <unordered_map>
#include <string>

struct Settings
{
private:
	struct TypeUnion
	{
		enum Type : uint8_t
		{
			INT,
			FLT,
			STR
		};
		Type type;
		union 
		{
			int i;
			float f;
			std::string s;
		};
		TypeUnion();
		TypeUnion(int i);
		TypeUnion(float f);
		TypeUnion(const std::string& s);

		TypeUnion(const TypeUnion& o);
		TypeUnion(TypeUnion&& o);
		TypeUnion& operator=(const TypeUnion& o);
		TypeUnion& operator=(TypeUnion&& o);

		~TypeUnion();
	};

	static std::unordered_map<std::string, TypeUnion> sTable;
public:	
	static void Init(const std::string path); // to be Tested
	
	// returns INT_MIN on fail
	static int TryGetI(const std::string& key);
	// returns FLT_MIN on fail
	static float TryGetF(const std::string& key);
	// returns empty str on fail
	static const std::string& TryGetS(const std::string& key);

	static void Set(const std::string& key, int val);
	static void Set(const std::string& key, float val);
	static void Set(const std::string& key, const std::string& val);
};