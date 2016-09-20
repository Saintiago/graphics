#pragma once

#include "ColoredShape.h"

class CGrid : public CColoredShape
{
public:
	CGrid() = default;
	~CGrid() = default;
	
protected:
	void Redraw()const override;

};

