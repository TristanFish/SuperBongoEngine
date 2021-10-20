#include "Logger.h"

MessageType EngineLogger::currentSev = MessageType::TYPE_NONE;
std::string EngineLogger::outputName = "";
std::function<void(const std::string&)> EngineLogger::consoleCallback = nullptr;

void EngineLogger::SetCallback(const std::function<void(const std::string&)>& func)
{
	if(consoleCallback == nullptr)
	{
		consoleCallback = func;
	}
}

void EngineLogger::OnCreate(const std::string& name_)
{
	outputName = name_ + ".txt";
	std::fstream out;

	out.open(outputName.c_str(), std::ios::out);
	out.close();
	currentSev = MessageType::TYPE_INFO;
}

void EngineLogger::SetSeverity(MessageType type_)
{
	currentSev = type_;
}

std::string CreateTag(MessageTag tag)
{
	switch (tag)
	{
	case MessageTag::TYPE_NONE: 
		return std::string("");
	case MessageTag::TYPE_PHYSICS:
		return std::string("[PHYSICS]");
	case MessageTag::TYPE_SAVE: 
		return std::string("[SAVE]");
	case MessageTag::TYPE_LOAD: 
		return std::string("[LOAD]");
	case MessageTag::TYPE_NETWORK: 
		return std::string("[NETWORK]");
	case MessageTag::TYPE_GRAPHICS: 
		return std::string("[GRAPHICS]");
	case MessageTag::TYPE_AI: 
		return std::string("[AI]");
	default: 
		return std::string("");
	}
}

void EngineLogger::Info(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_INFO,  CreateTag(tag) + "[INFO]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::Trace(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_TRACE, CreateTag(tag) + "[TRACE]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::Warning(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_WARNING, CreateTag(tag) + "[WARNING]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::Error(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_ERROR, CreateTag(tag) + "[ERROR]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::FatalError(const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_FATAL_ERROR, CreateTag(tag) + "[FATAL_ERROR]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::Log(const MessageType type_, const std::string& message_, const std::string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	if (type_ <= currentSev && currentSev > MessageType::TYPE_NONE)
	{
		const std::string log = std::string(message_ + " in: " + fileName_ + " on line: " + std::to_string(line_) + "\n");
		std::cout << log;
		std::ofstream out;
		out.open(outputName.c_str(), std::ios::app | std::ios::out);
		if(consoleCallback != nullptr && sendToConsoleLog)
		{
			consoleCallback(log);
		}
		out << log;
		out.flush();
		out.close();
	}
}
