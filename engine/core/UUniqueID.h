#ifndef SUID_H
#define SUID_H

#include <xhash>

class UUniqueID
{
public:
	UUniqueID();
	UUniqueID(uint64_t uuid);


	operator uint64_t() const { return m_UUID; }
private:
	uint64_t m_UUID;

	
};

namespace std
{
	template<>
	struct hash<UUniqueID>
	{
		std::size_t operator()(const UUniqueID& uuid) const
		{
			return  hash<uint64_t>()((uint64_t)uuid);
		}
	};
}

#endif

