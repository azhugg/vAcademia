
#pragma once

#include "ITool.h"
#include "Color4.h"

class CEraserTool : public ITool
{
public:
	CEraserTool();
	~CEraserTool();

	void SetDrawingTarget(IDrawingTarget* drawingTarget);
	void SetParameter(int paramID, void* param);

	void OnMouseMove(int x, int y);
	void OnLMouseDown(int x, int y);
	void OnLMouseUp(int x, int y);
	void OnRMouseDown(int x, int y);
	void OnRMouseUp(int x, int y);
	void OnChar(int keycode, int scancode);

	void OnSelected();
	void OnDeselected();
	void OnEndRestoreState();

	void RedoAction(void* data);

	int GetSynchRequiredType();

	void Destroy();

private:
	void Draw(int x, int y, bool isDrawAnyway = false);

	IDrawingTarget* m_drawingTarget;

	unsigned int m_eraserWidth;
	unsigned char m_eraserOpacity;

	bool m_isWorking;
	int m_lastMouseX, m_lastMouseY;
};