#include "UUniqueID.h"

#include <random>


static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint32_t> s_UniformDistribution;


UUniqueID::UUniqueID() : m_UUID(s_UniformDistribution(s_Engine))
{
	
}

UUniqueID::UUniqueID(uint32_t uuid) : m_UUID(uuid)
{


}
