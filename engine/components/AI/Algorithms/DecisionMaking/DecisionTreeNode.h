#pragma once
#include <string>
#include "core/Logger.h"

class DecisionTreeNode
{
public:

	virtual ~DecisionTreeNode() {}

	virtual void MakeDecision() = 0;

private:
	
};

class Action : public DecisionTreeNode
{
public:
	virtual void MakeDecision() override = 0;
};

class Decision : public DecisionTreeNode
{
public:
	DecisionTreeNode* trueNode;
	DecisionTreeNode* falseNode;

	void AddTrueNode(DecisionTreeNode* node)
	{
		trueNode = node;
	}

	void AddFalseNode(DecisionTreeNode* node)
	{
		falseNode = node;
	}

	void MakeDecision() override
	{
		DecisionTreeNode* branch = GetBranch();

		if(branch == nullptr)
		{
			return;
		}
		
		branch->MakeDecision();
	}
	


	virtual bool TestValue() = 0;
	DecisionTreeNode* GetBranch()
	{
		bool state = TestValue();

		if(state)
		{
			return trueNode;
		} 
		else
		{
			return falseNode;
		}
	}

	virtual ~Decision()
	{
		if(trueNode)
		{
			delete trueNode;
		}

		if(falseNode)
		{
			delete falseNode;
		}
	}
	
private:
	
};

class DecisionTree
{
public:
	DecisionTreeNode* d;

	void EvaluateDecisionTree()
	{
		d->MakeDecision();
	}

	~DecisionTree()
	{
		delete d;
	}	
};

struct FloatDecision : public Decision
{
	float minValue;
	float maxValue;
	float* testFloat;

	FloatDecision(float min, float max, float* input)
	{
		minValue = min;
		maxValue = max;
		testFloat = input;
	}

	bool TestValue() override
	{
		if(minValue < *testFloat && *testFloat < maxValue) return true;

		return false;
	}

};

struct IntDecision : public Decision
{
	int minValue;
	int maxValue;
	int* testInt;

	IntDecision(int min, int max, int* input)
	{
		minValue = min;
		maxValue = max;
		testInt = input;
	}

	bool TestValue() override
	{
		if(minValue < *testInt && *testInt < maxValue) return true;

		return false;
	}
};

struct StringDecision : public Decision
{
	std::string acceptedValue;
	std::string* testString;

	StringDecision(std::string accValue, std::string* input)
	{
		acceptedValue = accValue;
		testString = input;
	}

	bool TestValue() override
	{
		if(acceptedValue == *testString) return true;
		
		return false;
	}
};

struct BoolDecision : public Decision
{
	bool* testBool;

	BoolDecision(bool* input)
	{
		testBool = input;
	}

	bool TestValue() override
	{
		if(*testBool) return true;

		return false;
	}
};

struct PrintAction : public Action
{
	std::string printingValue;

	PrintAction(const char* printValue)
	{
		printingValue = printValue;
	}

	void MakeDecision() override
	{
		EngineLogger::Info(printingValue, "DecisionTreeNode.h", __LINE__, MessageTag::TYPE_AI);
	}
};