////////////////////////////////////////////////////////////////////////////////

#ifndef __DEV_H__
#define __DEV_H__

#ifdef _DEV
////////////////////////////////////////////////////////////////////////////////

#include <djengine.h>
#include <djapp.h>
#include <dj2d.h>
#include <djuinode.h>


#include "monkeyking.h"
#include "util.h"

struct BoundingBox
{
	DJRECT rect;
	bool bActive;
};

/*
 * WARNING: this must NOT be in a layer in which real DJ2DSprites
 * are contained, since they directly paint to the render device.
 */
class BoundingBoxCollection : public DJ2DNode
{
protected:
	DJMaterial*					m_pMaterial;
	DJLinkedList<BoundingBox>	m_boxes;
	bool						m_bShow;

public:
	void Init();
	void Term();

	// Queue a bounding box to paint on the next flush
	void QueueBoundingBox(const DJRECT &rect);

	virtual void OnPaint(const DJ2DRenderContext &rc);

	void Toggle();
};

extern BoundingBoxCollection theBoundingBoxCollection;

#endif //_DEV

#endif