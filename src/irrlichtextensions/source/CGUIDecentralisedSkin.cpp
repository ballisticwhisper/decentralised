#include "CGUIDecentralisedSkin.h"

CGUIDecentralisedSkin::CGUIDecentralisedSkin(IVideoDriver* driver, std::map<std::wstring, std::wstring> &skin)
	: CGUISkin(EGUI_SKIN_TYPE::EGST_WINDOWS_CLASSIC, driver), skin_(skin)
{
	driver_ = driver;
}

CGUIDecentralisedSkin::~CGUIDecentralisedSkin()
{

}

void CGUIDecentralisedSkin::draw3DToolBar(IGUIElement* element,
	const core::rect<s32>& r,
	const core::rect<s32>* clip)
{
	if (!driver_)
		return;

	SColor backColor = getColor(EGUI_DEFAULT_COLOR::EGDC_3D_FACE);

	core::rect<s32> rect = r;

	rect.UpperLeftCorner.X = r.UpperLeftCorner.X;
	rect.UpperLeftCorner.Y = r.UpperLeftCorner.Y;
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	driver_->draw2DRectangle(backColor, rect, clip);
}

SColor CGUIDecentralisedSkin::getColorFromSkin(std::wstring name) const
{
	return toColor(skin_[name]);
}

SColor CGUIDecentralisedSkin::getColor(EGUI_DEFAULT_COLOR color) const
{
	switch (color)
	{
	case EGUI_DEFAULT_COLOR::EGDC_3D_HIGH_LIGHT:
		return SColor(255, 255, 255, 255);
	case EGUI_DEFAULT_COLOR::EGDC_3D_SHADOW:
		return SColor(255, 255, 0, 0);
	case EGUI_DEFAULT_COLOR::EGDC_3D_FACE:
		return toColor(skin_[L"MenuBar_BackgroundColor"]);
		//case EGUI_DEFAULT_COLOR::EGDC_3D_SHADOW:
		//	return SColor(255, 255, 0, 0);
	case EGUI_DEFAULT_COLOR::EGDC_BUTTON_TEXT:
	case EGUI_DEFAULT_COLOR::EGDC_HIGH_LIGHT_TEXT:
	case EGUI_DEFAULT_COLOR::EGDC_GRAY_TEXT:
		return toColor(skin_[L"MenuBar_TextColor"]);
	case EGUI_DEFAULT_COLOR::EGDC_3D_DARK_SHADOW:
		return SColor(255, 0, 0, 0);
	default:
		return SColor(255, 0, 0, 255);
	}
}

SColor CGUIDecentralisedSkin::toColor(std::wstring csv) const
{
	stringw csvText = stringw(csv.c_str());
	std::vector<stringw> csvSplit;
	csvText.split(csvSplit, L",");
	if (csvSplit.size() < 3)
		return SColor(0, 0, 0, 0);

	SColor result;
	stringc r = stringc(csvSplit[0].c_str());
	stringc g = stringc(csvSplit[1].c_str());
	stringc b = stringc(csvSplit[2].c_str());

	result.setAlpha(255);
	result.setRed((u32)strtol10(r.c_str()));
	result.setGreen((u32)strtol10(g.c_str()));
	result.setBlue((u32)strtol10(b.c_str()));

	return result;
}