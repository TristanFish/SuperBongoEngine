#include "Logger.h"
#include <fstream>
#include <iostream>

using namespace std;

MessageType EngineLogger::currentSev = MessageType::TYPE_NONE;
string EngineLogger::outputName;
function<void(const string&)> EngineLogger::consoleCallback = nullptr;


void EngineLogger::SetCallback(const function<void(const string&)>& func)
{
	if(consoleCallback == nullptr)
	{
		consoleCallback = func;
	}
}

void EngineLogger::OnCreate(const string& name_)
{
	outputName = name_ + ".txt";
	fstream out;

	out.open(outputName.c_str(), ios::out);
	out.close();
	currentSev = MessageType::TYPE_INFO;
}

void EngineLogger::SetSeverity(MessageType type_)
{
	currentSev = type_;
}

string CreateTag(MessageTag tag)
{
	switch (tag)
	{
	case MessageTag::TYPE_NONE: 
		return "";
	case MessageTag::TYPE_PHYSICS:
		return "[PHYSICS]";
	case MessageTag::TYPE_SAVE: 
		return "[SAVE]";
	case MessageTag::TYPE_LOAD: 
		return "[LOAD]";
	case MessageTag::TYPE_NETWORK: 
		return "[NETWORK]";
	case MessageTag::TYPE_GRAPHICS: 
		return "[GRAPHICS]";
	case MessageTag::TYPE_AI: 
		return "[AI]";
	case MessageTag::TYPE_MATH:
		return "[MATH]";
	}
	return "";
}

void EngineLogger::Info(const string& message_, const string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_INFO,  CreateTag(tag) + "[INFO]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::Trace(const string& message_, const string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_TRACE, CreateTag(tag) + "[TRACE]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::Warning(const string& message_, const string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_WARNING, CreateTag(tag) + "[WARNING]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::Error(const string& message_, const string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_ERROR, CreateTag(tag) + "[ERROR]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::FatalError(const string& message_, const string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	Log(MessageType::TYPE_FATAL_ERROR, CreateTag(tag) + "[FATAL_ERROR]: " + message_, fileName_, line_, tag, sendToConsoleLog);
}

void EngineLogger::Log(const MessageType type_, const string& message_, const string& fileName_, const int line_, MessageTag tag, bool sendToConsoleLog)
{
	if (type_ <= currentSev && currentSev > MessageType::TYPE_NONE)
	{
		const string log = string(message_ + " in: " + fileName_ + " on line: " + to_string(line_) + "\n");
		
		ofstream out;
		out.open(outputName.c_str(), ios::app | ios::out);
		if(consoleCallback != nullptr && sendToConsoleLog)
		{
			consoleCallback(log);
		} else
		{
			std::cout << log;
		}
		out << log;
		out.flush();
		out.close();
	}
}
