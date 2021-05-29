#include "AudioManager.h"
#include "core/Logger.h"


AudioManager::AudioManager() : system(nullptr)
{
	InitAudioManager();
}

bool AudioManager::MonitorChannel( FMOD::Channel& c) const
{
	bool tb;
	c.isPlaying(&tb);
	if (tb != 1) {
		//std::c << "Sound has finished playing" << std::endl;
		return true;
	}
	else return false;
}

FMOD::Sound* AudioManager::RetrieveSoundObject(std::string soundName)
{
	//auto itr = soundPairs.find(soundName);
	auto itr = soundPairs.find(soundName);
	if (itr != soundPairs.end()) {
		return itr->second;
	}
	else 
	{
		EngineLogger::Warning("Map Key: " + soundName + " Not found, no sound object returned", "AudioManager.cpp", __LINE__);
		return nullptr;
	}
}


void AudioManager::CreateChannelGroup(const char* groupName, FMOD::ChannelGroup* channelGroup)
{
	//A channel is a the road between files being digital and being played on speakers, you can probably guess what a group is
	system->createChannelGroup(groupName, &channelGroup);
	channelGroup->setMode(FMOD_3D);
}


void AudioManager::Update()
{
	system->update();
	
}

void AudioManager::InitAudioManager()
{
	//This is how ya boot FMOD system instances
	FMOD_RESULT result;
	result = FMOD::System_Create(&system);
	if (result != FMOD_OK) 
	{
		std::string err = "FMOD system unable to create ";
		err.append((FMOD_ErrorString(result)));

		EngineLogger::Error(err, "AudioManager.cpp", __LINE__);
		exit(-1);
	}

	//This actually initializes FMOD, first parameter is just a common amount of channels on a modern device.
	result = system->init(512, FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK) 
	{
		std::string err = "FMOD did not init properly ";
		err.append((FMOD_ErrorString(result)));

		EngineLogger::Error(err, "AudioManager.cpp", __LINE__);
		exit(-1);
	}
	//Always have to load before we create
	LoadSounds();
	CreateSounds();
}


void AudioManager::CreateSounds()
{
	for (size_t i = 0; i < soundPaths.size(); i++) 
	{
		const FMOD_RESULT r = system->createSound(soundPaths[i].c_str(), FMOD_3D, nullptr, &sounds[i]);
		if (r != FMOD_OK) 
		{
			EngineLogger::Warning(FMOD_ErrorString(r), "AudioManager.cpp", __LINE__);
		}
		soundPairs[soundNames[i]] = sounds[i];
		
	}

}

void AudioManager::Create3DReverbAttributes(FMOD_VECTOR position)
{
	FMOD_VECTOR pos = position;
	const float mindist = 10.0f;
	const float maxdist = 1000.0f;
	reverb->set3DAttributes(&pos, mindist, maxdist);	
}

void AudioManager::Create3DReverb()
{
	//FMOD uses properties/attributes of a specific struct to determine the DSP effects, if you look at a preset you'll see-
	//a series of arbitrary numbers that determine which DSP effects are altered

	FMOD_RESULT r;
	r = system->createReverb3D(&reverb);
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_CAVE;
	reverb->setProperties(&prop);
	
	//CreateAndSetPan(-1);
}

void AudioManager::CreateAndSetPan(float pan)
{
}

void AudioManager::LoadSounds()
{

	//Please keep all sounds keys in all lower case so we don't have to debug dumb stuff
	//Add any sounds you want in this function, just follow suit.
	//A map of any 2 objects you place here is inserted into inside CreateSound
	soundPaths.emplace_back("resources/sounds/tumbleweed.wav");
	//Sound object goes here
	sounds.emplace_back(leafCrunch);
	//String object that has name you intend to search through
	soundNames.emplace_back(leafCrunchString);

	soundPaths.emplace_back("resources/sounds/canary.wav");
	sounds.emplace_back(birdChirp);
	soundNames.emplace_back(birdChirpString);

}

//void AudioManager::SetAudioSourcePos(MATH::Vec3& sourcePos)
//{
//
//	C1->setChannelGroup(G1);
//	FMOD_RESULT r;
//	audioSourcePos = AudioUtility::Vec3ToFMODVec(sourcePos);
//	r = C1->set3DAttributes(&audioSourcePos, NULL);
//	
//	if (r != FMOD_OK) {
//		std::c << r << FMOD_ErrorString(r) << std::endl;
//	}
//	r = G1->set3DAttributes(&audioSourcePos, NULL);
//	if (r != FMOD_OK) {
//		std::c << r << FMOD_ErrorString(r) << std::endl;
//	}
//	
//}



AudioManager::~AudioManager()
{
	//audioSources.clear();
	system->release();
	system = nullptr;
	G1 = nullptr;
	C1 = nullptr;
	
}


