#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <fstream>
#include <functional>

enum class MessageType : unsigned short
{
	TYPE_NONE = 0,
	TYPE_FATAL_ERROR,
	TYPE_ERROR,
	TYPE_WARNING,
	TYPE_TRACE,
	TYPE_SAVE,
	TYPE_INFO,
};

enum class MessageTag
{
	TYPE_NONE,
	TYPE_PHYSICS,
	TYPE_SAVE,
	TYPE_LOAD,
	TYPE_NETWORK,
	TYPE_GRAPHICS,
	TYPE_AI
};

class EngineLogger
{
public:

	EngineLogger() = delete;
	EngineLogger(const EngineLogger&) = delete;
	EngineLogger(EngineLogger&&) = delete;
	EngineLogger& operator=(const EngineLogger&) = delete;
	EngineLogger& operator=(EngineLogger&&) = delete;
	~EngineLogger() = delete;

	static void SetCallback(const std::function<void(const std::string&)>& func);

	static void OnCreate(const std::string& name_ = "EngineLog");
	static void SetSeverity(MessageType type_);
	static void Info(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag = MessageTag::TYPE_NONE, bool sendToConsoleLog = true);
	static void Trace(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag = MessageTag::TYPE_NONE, bool sendToConsoleLog = true);
	static void Warning(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag = MessageTag::TYPE_NONE, bool sendToConsoleLog = true);
	static void Error(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag = MessageTag::TYPE_NONE, bool sendToConsoleLog = true);
	static void FatalError(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag = MessageTag::TYPE_NONE, bool sendToConsoleLog = true);

private:

	static void Log(const MessageType type_, const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog);

	static MessageType currentSev;
	static std::string outputName;
	static std::function<void(const std::string&)> consoleCallback;
};

#endif