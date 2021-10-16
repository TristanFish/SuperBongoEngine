#include "Logger.h"

MessageType EngineLogger::currentSev = MessageType::TYPE_NONE;
std::string EngineLogger::outputName = "";

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

void EngineLogger::Info(const std::string message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_INFO, "[INFO]: " + message_, fileName_, line_);
}

void EngineLogger::Save(const std::string message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_SAVE, "[SAVE]: " + message_, fileName_, line_);
}

void EngineLogger::Trace(const std::string message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_TRACE, "[TRACE]: " + message_, fileName_, line_);
}

void EngineLogger::Warning(const std::string message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_WARNING, "[WARNING]: " + message_, fileName_, line_);
}

void EngineLogger::Error(const std::string message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_ERROR, "[ERROR]: " + message_, fileName_, line_);
}

void EngineLogger::FatalError(const std::string message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_FATAL_ERROR, "[FATAL_ERROR]: " + message_, fileName_, line_);
}

void EngineLogger::Log(const MessageType type_, const std::string message_, const std::string& fileName_, const int line_)
{
	if (type_ <= currentSev && currentSev > MessageType::TYPE_NONE)
	{
		std::cout << message_ << " in: " << fileName_ << " on line: " << line_ << std::endl;
		std::ofstream out;
		out.open(outputName.c_str(), std::ios::app | std::ios::out);
		out << message_ << " in: " << fileName_ << " on line: " << line_ << std::endl;
		out.flush();
		out.close();
	}
}
