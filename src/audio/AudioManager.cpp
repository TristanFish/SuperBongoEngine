#include "AudioManager.h"



AudioManager::AudioManager() : system(nullptr)
{
	InitAudioManager();
}

void AudioManager::MonitorChannel(FMOD::Channel c)
{
	bool tb;
	c.isPlaying(&tb);
	std::cout  << tb << std::endl;
}

FMOD::Sound* AudioManager::RetrieveSoundObject(std::string soundName)
{
	//auto itr = soundPairs.find(soundName);
	auto itr = soundPairs.find(soundName);
	if (itr != soundPairs.end()) {
		return itr->second;
	}
	else { std::cout << "Map Key:" << " " << soundName << " " << "Not found, no sound object returned" << std::endl; }
}

void AudioManager::AddAudioSource(AudioSourceComponent& newComponent)
{
	//audioSources.emplace_back(newComponent);
}

void AudioManager::CreateChannelGroup(const char* groupName, FMOD::ChannelGroup* channelGroup)
{
	//A channel is a the road between files being a digital and being played on speakers, you can probably guess what a group is
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
	if (result != FMOD_OK) {
		std::cout << "FMOD system unable to create" << result << "\n" << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}

	//This actually initializes FMOD, first parameter is just a common amount of channels on a modern device.
	result = system->init(512, FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK) {
		std::cout << "FMOD did not init properly" << result << "\n" << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
	//Always have to load before we create
	LoadSounds();
	CreateSounds();
}

//Pass in a pointer to the new FMOD::Sound you've have created. 
void AudioManager::CreateSounds()
{
	for (int i = 0; i < soundPaths.size(); i++) {
		FMOD_RESULT r;
		r = system->createSound(soundPaths[i].c_str(), FMOD_3D, nullptr, &sounds[i]);
		if (r != FMOD_OK) {
			std::cout << r << FMOD_ErrorString(r) << std::endl;
		}
		soundPairs[soundNames[i]] = sounds[i];
		//soundPairs.insert(std::pair <const char*, FMOD::Sound*>(soundPaths[i], sounds[i]));
	}

}

void AudioManager::Create3DReverbAttributes(FMOD_VECTOR position)
{
	FMOD_VECTOR pos = position;
	float mindist = 10.0f;
	float maxdist = 1000.0f;
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
	soundPaths.emplace_back("src/sounds/tumbleweed.wav");
	//Sound object goes here
	sounds.emplace_back(leafCrunch);
	//String object that has name you intend to search through
	soundNames.emplace_back(leafCrunchString);

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
//		std::cout << r << FMOD_ErrorString(r) << std::endl;
//	}
//	r = G1->set3DAttributes(&audioSourcePos, NULL);
//	if (r != FMOD_OK) {
//		std::cout << r << FMOD_ErrorString(r) << std::endl;
//	}
//	
//}



void AudioManager::CreateAndPlaySound(const char* filename)
{
	system->playSound(newSound, nullptr, false, &C1);
	Create3DReverb();
	Update();
}

AudioManager::~AudioManager()
{
	//audioSources.clear();
	system->release();
	system = nullptr;
	newSound = nullptr;
	G1 = nullptr;
	C1 = nullptr;
	
}


