#include "CoordinateSystemHandler.h"

#include "gui/XournalView.h"
#include "control/Control.h"
#include "undo/InsertUndoAction.h"
#include <cmath>

CoordinateSystemHandler::CoordinateSystemHandler(XournalView* xournal, XojPageView* redrawable, PageRef page)
 : BaseStrokeHandler(xournal, redrawable, page)
{
	XOJ_INIT_TYPE(CoordinateSystemHandler);
}

CoordinateSystemHandler::~CoordinateSystemHandler()
{
	XOJ_CHECK_TYPE(CoordinateSystemHandler);

	XOJ_RELEASE_TYPE(CoordinateSystemHandler);
}

/**
 * Draw a Cartesian coordinate system.
 * 
 * @param currentPoint The current point the mouse cursor is pointing to.
 * @param shiftDown Boolean to indicate if "shift" is currently pressed.
 *                  It is currently not used.
 */
void CoordinateSystemHandler::drawShape(Point& currentPoint, bool shiftDown)
{
	XOJ_CHECK_TYPE(CoordinateSystemHandler);

	/**
	 * Snap first point to grid (if enabled)
	 */
	if (!shiftDown && xournal->getControl()->getSettings()->isSnapGrid())
	{
		Point firstPoint = stroke->getPoint(0);
		snapToGrid(firstPoint.x,firstPoint.y);
		stroke->setFirstPoint(firstPoint.x,firstPoint.y);
	}
	int count = stroke->getPointCount();

	if (count < 1)
	{
		// Add first point of coordinate system
		stroke->addPoint(currentPoint);
	}
	else
	{
		// This is the starting point of the coordinate system
		Point startingPoint = stroke->getPoint(0);

		// This ensures that no past points of the coordinate system are drawn
		if (count > 2)
		{
			stroke->deletePoint(2);
			stroke->deletePoint(1);
		}
		if (xournal->getControl()->getSettings()->isSnapGrid())
		{
			snapToGrid(startingPoint.x,startingPoint.y);
			snapToGrid(currentPoint.x,currentPoint.y);
		}
		// Draw the other two points
		if (shiftDown || !xournal->getControl()->getSettings()->isSnapRotation())
		{
			stroke->addPoint(Point(startingPoint.x, currentPoint.y));
			stroke->addPoint(Point(currentPoint.x, currentPoint.y));
			// Can be equipped with some feature later.
		}
		else
		{
			stroke->addPoint(Point(startingPoint.x, currentPoint.y));
			stroke->addPoint(Point(currentPoint.x, currentPoint.y));
		}
	}
}
