
#ifdef _DEV
////////////////////////////////////////////////////////////////////////////////
#include <djengine.h>
#include "gamedef.h"
#include "debug.h"

void BoundingBoxCollection::Init()
{
	m_pMaterial = (DJMaterial*) theResourceManager.GetResource("entity_boudingbox", DJResource::TYPE_MATERIAL);
	DJ_ASSERT(m_pMaterial != NULL);

	theSpriteEngine.GetLayer(LAYER_BOUNDING_BOX)->AddNode(this);

	for (int i = 0; i < 100; i++)
	{
		BoundingBox *pBox = new BoundingBox;
		pBox->bActive = false;
		m_boxes.AddLast(pBox);
	}

	m_bShow = false;
}

void BoundingBoxCollection::Term()
{
	m_boxes.Clear(DJTRUE);
	theSpriteEngine.GetLayer(LAYER_BOUNDING_BOX)->RemoveNode(this);
	theResourceManager.ReleaseResource(m_pMaterial);
}


void BoundingBoxCollection::QueueBoundingBox(const DJRECT &rect)
{
	if (!m_bShow) return;

	DJLinkedListLink<BoundingBox> *link = m_boxes.LastOutLink();
	BoundingBox *pBox = link->Data();
	pBox->rect = rect;
	pBox->bActive = true;
	m_boxes.AddFirstLink(link);
}

void BoundingBoxCollection::OnPaint(const DJ2DRenderContext &rc)
{
	pTheRenderDevice->SetWorldTransform(DJMatrix::Identity());
	pTheRenderDevice->SetViewTransform(DJMatrix::Identity());

	DJMatrix tm, tm2;
	DJMatrix2D tm2D;
	GetLocalTransform(tm2D);
	tm2D = rc.m_mTransform * tm2D;
	tm2D.GetMatrix(&tm);
	//rc.m_pLayer->GetLocalTransform();
	rc.m_mLayerTransform.GetMatrix(&tm2);
	pTheRenderDevice->SetViewTransform(tm2);

	pTheRenderDevice->SetViewTransform(DJMatrix::Identity());

	pTheRenderDevice->SetMaterial(m_pMaterial);
	pTheRenderDevice->UpdateUniforms(m_pMaterial->GetMaterialMode(0)->GetLocalUniformList());
	pTheRenderDevice->SetTexture(m_pMaterial->GetTextureStage("T1")->m_pTexture);
	
	DJLinkedListIter<BoundingBox> iter(m_boxes);
	for (BoundingBox *pBox = iter.GetStep(); pBox; pBox = iter.GetStep())
	{
		if (!pBox->bActive) continue;
		pBox->bActive = false;
		DJVector2 topLeft((float) pBox->rect.nX, (float) pBox->rect.nY);
		DJVector2 botRight((float) pBox->rect.nX + pBox->rect.nW - 1, (float) pBox->rect.nY + pBox->rect.nH - 1);

		RectVertex vtx[4];
		vtx[0].x = topLeft[0]; vtx[0].y = topLeft[1]; vtx[0].z = 0.0f; vtx[0].u = 0.0f; vtx[0].v = 0.0f; vtx[0].c = DJColor(1.0f, 1.0f, 1.0f, 0.7f);
		vtx[1].x = botRight[0]; vtx[1].y = topLeft[1]; vtx[1].z = 0.0f; vtx[1].u = 1.0f; vtx[1].v = 0.0f; vtx[1].c = DJColor(1.0f, 1.0f, 1.0f, 0.7f);
		vtx[2].x = botRight[0]; vtx[2].y = botRight[1]; vtx[2].z = 0.0f; vtx[2].u = 1.0f; vtx[2].v = 1.0f; vtx[2].c = DJColor(1.0f, 1.0f, 1.0f, 0.7f);
		vtx[3].x = topLeft[0]; vtx[3].y = botRight[1]; vtx[3].z = 0.0f; vtx[3].u = 0.0f; vtx[3].v = 1.0f;  vtx[3].c = DJColor(1.0f, 1.0f, 1.0f, 0.7f);
		pTheRenderDevice->RenderPrimitiveRaw(DJRenderDevice::ptTriangleFan, 2, vtx, RectVertex_DescCount, RectVertex_Descs, sizeof(RectVertex));
	}
}

void BoundingBoxCollection::Toggle()
{
	m_bShow = !m_bShow;
}

BoundingBoxCollection theBoundingBoxCollection;

////////////////////////////////////////////////////////////////////////////////

#endif //_DEV