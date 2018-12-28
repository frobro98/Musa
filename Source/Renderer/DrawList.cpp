#include "DrawList.h"
#include "Model/Model.h"

void DrawList::AddToRenderList(Model* model)
{
	// TODO - This is called every time a model is activated as well....change this
	model->SetOwningDrawList(this);
	if (nodeReserve == nullptr)
	{
		DrawListNode* node = new DrawListNode;
		node->model = model;
		node->next = renderList;
		renderList = node;
	}
	else
	{
		DrawListNode* node = nodeReserve;
		nodeReserve = nodeReserve->next;
		node->model = model;
		node->next = renderList;
		renderList = node;
	}
}

void DrawList::RemoveFromRenderList(Model* model)
{
	DrawListNode* current = renderList;

	// Test out head of list
	if (current->model == model)
	{
		DrawListNode* node = renderList;
		renderList = renderList->next;

		node->model = nullptr;
		node->next = nodeReserve;
		nodeReserve = node;
	}
	else
	{
		bool found = false;
		while (current->next != nullptr)
		{
			if (current->next->model == model)
			{
				DrawListNode* node = current->next;
				node->model = nullptr;

				current->next = node->next;
				node->next = nodeReserve;
				nodeReserve = node;

				found = true;
				break;
			}

			current = current->next;
		}

		assert(found);
	}
}
