
#include "UI.hpp"
#include "UIInternal.hpp"
#include "UIContext.hpp"

namespace UI
{

static Context* context = nullptr;

namespace Internal
{
void SetMusaUI(Context& uiContext)
{
	context = &uiContext;
}
}

void AddWidgetToView(Widget& widget)
{
	context->AddWidget(widget);
}

void RemoveWidgetFromView(Widget& widget)
{
	context->RemoveWidget(widget);
}

}