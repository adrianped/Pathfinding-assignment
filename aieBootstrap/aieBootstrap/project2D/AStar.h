#pragma once
#include <vector>
#include "AStarNode.h"

using namespace std;

class AStar
{
public:
	AStar();
	~AStar();

	bool CalcuatePath(AStarNode* pStart, AStarNode* pEnd, vector<AStarNode*>* finishedPath);

private:
	void SortOpenList(); //WONT NEED WHEN USING HEAP

	vector<AStarNode*> m_OpenList;//MAKE THIS A HEAP
	vector<bool> m_ClosedList;

};

