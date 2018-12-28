#pragma once

#include "Platform.h"

// TODO - I would assume that there would be more things able to be drawn than just models. Revisit this...
class Model;
class Renderer;

class DrawList
{
public:
	void AddToRenderList(Model* model);
	void RemoveFromRenderList(Model* model);

private:
	struct DrawListNode
	{
		DrawListNode* next = nullptr;
		Model* model;
	};

public:

	class Iterator
	{
	public:
		Iterator(DrawList* list)
			:drawList(list),
			current(drawList->renderList)
		{
		}

		bool AtEnd() const { return current == nullptr; }

		Model* Current() const { return current->model; }

		void operator++() { current = current->next; }

	private:
		DrawList* drawList = nullptr;
		DrawListNode* current = nullptr;
	};
private:

	// Reserve of nodes so that we might not need to allocate more nodes
	DrawListNode* nodeReserve = nullptr;

	DrawListNode* renderList = nullptr;
	uint32 listSize = 0;
};