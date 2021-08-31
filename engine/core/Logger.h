#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <fstream>

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

class EngineLogger
{
public:

	EngineLogger() = delete;
	EngineLogger(const EngineLogger&) = delete;
	EngineLogger(EngineLogger&&) = delete;
	EngineLogger& operator=(const EngineLogger&) = delete;
	EngineLogger& operator=(EngineLogger&&) = delete;
	~EngineLogger() = delete;

	static void OnCreate(const std::string& name_ = "EngineLog");
	static void SetSeverity(MessageType type_);
	static void Info(const std::string message_, const std::string& fileName_, const int line_);
	static void Save(const std::string message_, const std::string& fileName_, const int line_);
	static void Trace(const std::string message_, const std::string& fileName_, const int line_);
	static void Warning(const std::string message_, const std::string& fileName_, const int line_);
	static void Error(const std::string message_, const std::string& fileName_, const int line_);
	static void FatalError(const std::string message_, const std::string& fileName_, const int line_);

private:

	static void Log(const MessageType type_, const std::string message_, const std::string& fileName_, const int line_);

	static MessageType currentSev;
	static std::string outputName;
};

#endif