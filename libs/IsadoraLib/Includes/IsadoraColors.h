// ===========================================================================
//	IsadoraColors			 (C)1999 Mark F. Coniglio. All rights reserved.
// ===========================================================================
//
//	Defines the colors for this program

#ifndef _H_IsadoraColors
#define _H_IsadoraColors

#define	ISADORA_NEW_COLORS		FALSE
#define ISADORA_OLD_GUI			TRUE

RGBColor*	RGBBlack();
RGBColor*	RGBWhite();

RGBColor*	RGBEditorFocusedColor();
RGBColor*	RGBEditorUnfocusedColor();
RGBColor*	RGBEditorBackgroundColor();
RGBColor*	RGBEditorGridColorSel();
RGBColor*	RGBEditorGridColorUnsel();

RGBColor*	RGBPropValueSelected();
RGBColor*	RGBPropValueUnselected();
RGBColor*	RGBThingSelected();
RGBColor*	RGBThingUnselected();

RGBColor*	RGBSelRect();
RGBColor*	RGBTrackLinkColor();

RGBColor*	RGBTrackLinkColor();
RGBColor*	RGBLinkColor(bool inActive, bool inSelected);

RGBColor*	RGBPropMenuBkg();

RGBColor*	RGBValueBoxInactive();
RGBColor*	RGBValueBoxActive();

RGBColor*	RGBTooltipColor();

Pattern*	BlackPat();

#ifdef __ISADORA_APP__
void
SetBackColorOfFirstColorEraseAttachment(
	LAttachable*	inAttachable,
	const RGBColor&	inColor);
#endif

#endif
