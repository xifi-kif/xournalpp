#include "SwapUndoAction.h"

#include "control/Control.h"
#include "model/Document.h"

#include <i18n.h>

SwapUndoAction::SwapUndoAction(size_t pageNr, bool moveUp, PageRef swappedPage, PageRef otherPage)
 : UndoAction("SwapUndoAction")
{
	XOJ_INIT_TYPE(SwapUndoAction);

	this->pageNr = pageNr;
	this->moveUp = moveUp;
	this->swappedPage = swappedPage;
	this->otherPage = otherPage;
}

SwapUndoAction::~SwapUndoAction()
{
	XOJ_CHECK_TYPE(SwapUndoAction);

	XOJ_RELEASE_TYPE(SwapUndoAction);
}

bool SwapUndoAction::undo(Control* control)
{
	XOJ_CHECK_TYPE(SwapUndoAction);

	swap(control);
	this->undone = true;

	return true;
}

bool SwapUndoAction::redo(Control* control)
{
	XOJ_CHECK_TYPE(SwapUndoAction);

	swap(control);
	this->undone = false;

	return true;
}

void SwapUndoAction::swap(Control* control)
{
	XOJ_CHECK_TYPE(SwapUndoAction);

	Document* doc = control->getDocument();

	doc->unlock();

	gint insertPos = this->pageNr;
	gint deletePos = this->pageNr + 1;

	if (moveUp != this->undone)
	{
		std::swap(insertPos, deletePos);
	}

	doc->deletePage(deletePos);
	doc->insertPage(this->swappedPage, insertPos);

	control->firePageDeleted(deletePos);
	control->firePageInserted(insertPos);
	control->firePageSelected(insertPos);

	control->getScrollHandler()->scrollToPage(insertPos);

	doc->lock();
}

vector<PageRef> SwapUndoAction::getPages()
{
	XOJ_CHECK_TYPE(SwapUndoAction);

	vector<PageRef> pages;
	pages.push_back(this->swappedPage);
	pages.push_back(this->otherPage);
	return pages;
}

string SwapUndoAction::getText()
{
	XOJ_CHECK_TYPE(SwapUndoAction);

	return moveUp ? _("Move page upwards") : _("Move page downwards");
}
