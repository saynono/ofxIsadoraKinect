// ===========================================================================
//	CCurveSpec					 (C)2009 Mark F. Coniglio. All rights reserved.
// ===========================================================================

#ifndef _H_CCURVESPEC
#define _H_CCURVESPEC

#include "CPlusPlusMacros.h"
#include "CFloatXY.h"

#ifdef __cplusplus
	const UInt32	EDGE_TABLE_POINTS	= 41;
	const UInt32	EDGE_DVISIONS		= EDGE_TABLE_POINTS - 1;
#else
	#define			EDGE_TABLE_POINTS	41
	#define			EDGE_DVISIONS		(EDGE_TABLE_POINTS - 1)
#endif

CLASSHDR CurveSpecValuesOld
{
PUBLIC
	float		mMin;
	float		mMax;

	float		mX1;
	float		mY1;
	float		mX2;
	float		mY2;
};

enum {
	kCurveSpecCurrentVersion = 1
};

CLASSHDR CurveSpecValues
{
PUBLIC
	UInt32		mVersion;
	
	float		mMinX;
	float		mMaxX;
	float		mMinY;
	float		mMaxY;

	float		mX1;
	float		mY1;
	float		mX2;
	float		mY2;
};

CLASSHDR CCurveSpec
{
PUBLIC

#ifdef __cplusplus
				CCurveSpec()
				{
					mVersion = kCurveSpecCurrentVersion;
				}
	
	void		Init(
					Boolean					inHorizontal,
					const CurveSpecValues&	v)
				{
					mVersion = kCurveSpecCurrentVersion;
					Init(inHorizontal, v.mMinX, v.mMaxX, v.mMinY, v.mMaxY, v.mX1, v.mY1, v.mX2, v.mY2);
				}
				
	void		Init(
					Boolean	inHorizontal,
					float	inMinX,
					float	inMaxX,
					float	inMinY,
					float	inMaxY,
					float	inX1,
					float	inY1,
					float	inX2,
					float	inY2)
				{
					mHorizontal	= inHorizontal;
					mMinX		= inMinX;
					mMaxX		= inMaxX;
					mMinY		= inMinY;
					mMaxY		= inMaxY;
					mX1			= inX1;
					mY1			= inY1;
					mX2			= inX2;
					mY2			= inY2;

					mNeedRecalcValues = TRUE;
					
					UpdateValues();
				}
	
#ifdef __ISADORA_APP__
	UInt32		Read(
					LStream*			inStream,
					UInt32				inFileVersion);
	
	UInt32		Write(
					LStream*			inStream);
#endif

	CFloatXY	ScaleToCurve(
					double				mu);
					
	double		LookupValueHoriz(
					double				inValue,
					CFloatXY*			inTable,
					UInt32				inTableSize);
					
	double		LookupValueVert(
					double				inValue,
					CFloatXY*			inTable,
					UInt32				inTableSize);
					
	void		RecalcTableValues();
	
	void		SetSize(
					SInt32				inWidth,
					SInt32				inHeight);
									
	void		UpdateValues();
				

	inline void			ForceRecalcValues() { mNeedRecalcValues = TRUE; }
	
	inline Boolean		GetHorizontal() const { return mHorizontal; }
	
	inline SInt32		GetUIMinX() const { return mUIMinX; }
	inline SInt32		GetUIMaxX() const { return mUIMaxX; }
	inline SInt32		GetUIMinY() const { return mUIMinY; }
	inline SInt32		GetUIMaxY() const { return mUIMaxY; }
	inline Point		GetCtlPt1() const { return mCtlPt1; }
	inline Point		GetCtlPt2() const { return mCtlPt2; }
	
	inline double		GetTableAtIndex(SInt32 i) { return mTable[i]; }
	inline double		GetTableAtInvertedIndex(SInt32 i) { return mTable[(EDGE_TABLE_POINTS-1) - i]; }
	
	inline void			SetTableAtIndex(SInt32 i, double value) { mTable[i] = value; }
	
	inline void			SetMinX(float inMinX) { mMinX = inMinX; mNeedRecalcValues = TRUE; }
	inline void			SetMaxX(float inMaxX) { mMaxX = inMaxX; mNeedRecalcValues = TRUE; }
	inline void			SetMinY(float inMinY) { mMinY = inMinY; mNeedRecalcValues = TRUE; }
	inline void			SetMaxY(float inMaxY) { mMaxY = inMaxY; mNeedRecalcValues = TRUE; }
	inline void			SetX1(float inX) { mX1 = inX; mNeedRecalcValues = TRUE; }
	inline void			SetY1(float inY) { mY1 = inY; mNeedRecalcValues = TRUE; }
	inline void			SetX2(float inX) { mX2 = inX; mNeedRecalcValues = TRUE; }
	inline void			SetY2(float inY) { mY2 = inY; mNeedRecalcValues = TRUE; }
	inline void			SetX1Y1(float inX, float inY) { mX1 = inX; mY1 = inY; mNeedRecalcValues = TRUE; }
	inline void			SetX2Y2(float inX, float inY) { mX2 = inX; mY2 = inY; mNeedRecalcValues = TRUE; }
	
	inline float		GetX1() const { return mX1; }
	inline float		GetY1() const { return mY1; }
	inline float		GetX2() const { return mX2; }
	inline float		GetY2() const { return mY2; }
	inline float		GetMinX() const { return mMinX; }
	inline float		GetMaxX() const { return mMaxX; }
	inline float		GetMinY() const { return mMinY; }
	inline float		GetMaxY() const { return mMaxY; }

#endif

PRIVATE
	UInt32				mVersion;
	
	float				mMinX;
	float				mMaxX;
	float				mMinY;
	float				mMaxY;
	
	float				mX1;
	float				mY1;
	float				mX2;
	float				mY2;

	Boolean				mHorizontal;
	SInt32				mWidth;
	SInt32				mHeight;
	Point				mCtlPt1;
	Point				mCtlPt2;
	SInt32				mUIMinX;
	SInt32				mUIMaxX;
	SInt32				mUIMinY;
	SInt32				mUIMaxY;

	double				mTable[EDGE_TABLE_POINTS];

	Boolean				mNeedRecalcValues;

};

CLASSHDR CCurveSpecArray
{
PUBLIC
	CCurveSpecArray()
	{
		mCurves[0].Init(TRUE,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.7f, 1.0f);
		mCurves[1].Init(TRUE,  0.0f, 1.0f, 1.0f, 0.0f, 0.3f, 1.0f, 1.0f, 0.3f);
		mCurves[2].Init(FALSE, 0.0f, 1.0f, 1.0f, 0.0f, 0.3f, 1.0f, 1.0f, 0.3f);
		mCurves[3].Init(FALSE, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.7f, 0.3f, 0.0f);
	}

	CCurveSpec	mCurves[4];
};

#endif