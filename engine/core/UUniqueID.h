#ifndef SUID_H
#define SUID_H

#include <xhash>

class UUniqueID
{
public:
	UUniqueID();
	UUniqueID(uint32_t uuid);


	operator uint32_t() const { return m_UUID; }
private:
	uint32_t m_UUID;

	
};

namespace std
{
	template<>
	struct hash<UUniqueID>
	{
		std::size_t operator()(const UUniqueID& uuid) const
		{
			return  hash<uint32_t>()((uint32_t)uuid);
		}
	};
}

#endif

