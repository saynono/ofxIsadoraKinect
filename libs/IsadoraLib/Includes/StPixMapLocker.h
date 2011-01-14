// ---------------------------------------------------------------------------
//		StPixMapLocker
// ---------------------------------------------------------------------------
// Static class that locks pixel maps on construction, and unlocks them
// on destruction.

#ifndef _H_StPixMapLocker
#define _H_StPixMapLocker
#pragma once

class StPixMapLocker
{
public:
	enum RestoreFlag {
		kRestoreUnlocked,
		kRestoreLocked
	};
	
					StPixMapLocker(
						PixMapHandle	inPixMap);

					StPixMapLocker(
						GWorldPtr		inGWorld);

					~StPixMapLocker();
	
	void				Lock();
	void				Unlock();
	void				Restore();
	
	inline PixMapHandle	GetPixMap() const
							{ return mPixMap; }
	
	inline operator PixMapHandle()
							{ return mPixMap; }
	
	inline OSType		GetFormat() const
							{ return (**mPixMap).pixelFormat; }
							
	inline Ptr			GetBaseAddress()
							{ return ::GetPixBaseAddr(mPixMap); }
							
	inline SInt32		GetRowBytes()
							{ return ::GetPixRowBytes(mPixMap); }
	
	inline Rect			GetBounds() const
							{ return (**mPixMap).bounds; }
	
	inline bool			IsLocked() const
							{ return mIsLocked; }

private:
	void			Init(
						PixMapHandle	inPixMap);
private:
	PixMapHandle	mPixMap;
	RestoreFlag		mRestoreFlag;
	bool			mIsLocked;
};


#endif
