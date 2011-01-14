// ===========================================================================
//	MessageReceiverCommon		  (C)1999 Mark F. Coniglio. All rights reserved.
// ===========================================================================
//
//	Message Receiver common defines, structs, and typedefs

#ifndef _H_MessageReceiverCommon
#define _H_MessageReceiverCommon

#include "IsadoraTypes.h"

typedef	void*			MessageReceiverRef;		// oblique pointer = CMessageReceiver

#define	kOSCPortBase	30000					// base port used when receving OSC messages

typedef	UInt16			PortIndex;				// one based index to a midi input or output port (i.e., cable); 0 = any
typedef UInt16			PortCount;				// count of available midi input or output ports
typedef	UInt32			Signature;
typedef UInt8			MsgData;				// data passed to Message Receiver functions

typedef	UInt32			MessageMask;			// a message mask passed to CreateMessageReceiver_ function

#define	kNilPortIndex	0

// ---------------------------------------------------------------------------
// MouseMoved struct
//
// Passed as a parameter to when receiving kWantMouseMove messages

struct MouseMoved {
	Point			globalPoint;
	EventModifiers	modifiers;
	Boolean			insideStage;
	StageIndexT		insideStageIndex1;
	Point			insideStagePoint;
	Boolean			insideStageMouseState;
	Boolean			insideStageMouseStateChanged;
	Boolean			insideStageLeftClick;
	Boolean			insideStageRightClick;
	SInt32			insideStageWheelX;
	SInt32			insideStageWheelY;
	Boolean			isTabletDevice;
	TabletPointRec	tabletPointInfo;
};
typedef struct MouseMoved MouseMoved;

// ---------------------------------------------------------------------------
// SoundLevelInfo struct
//
// Passed as a parameter to when receiving kWantSoundLevel messages

struct SoundLevelInfo {
	SInt32			mCaptureIndex1;			// indicates which capture channel this came from
	SInt32			mVolumeChannels;		// number of channels in the mVolumeValues array
	const float*	mVolumeValues;			// pointer to an array of volume values - varying between 0 and 1
	SInt32			mFrequencyChannels;		// number of channels in the mFrequencyValues array
	const float*	mFrequencyValues;		// pointer to an array of frequency values - varying between 0 and 1
	float			mFrequencySpacing;		// frequency between each frequency chanel. (i.e., if mFrequencySpacing
											// is 500, then mFrequencyValues[0] is 0Hz, mFrequencyValues[1] = 500Hz,
											// mFrequencyValues[2] = 1000 Hz, etc.
};
typedef struct SoundLevelInfo SoundLevelInfo;

// ---------------------------------------------------------------------------
// Message Receiver Flags

enum
{
	kWantNothing			= 0,
	
	// the message ptr for all MIDI messages points to the actual MIDI data
	
	// channel messages
	kWantNoteOff			= (1L << 0),			// 0x80 Note Off Message
	kWantNoteOn				= (1L << 1),			// 0x90	Note On Message
	kWantPolyAftertouch		= (1L << 2),			// 0xA0 Poly Pressure Message
	kWantController			= (1L << 3),			// 0xB0	Control Change Message
	kWantProgramChange		= (1L << 4),			// 0xC0 Program Change Message
	kWantMonoAftertouch		= (1L << 5),			// 0xD0 Mono Aftertouch Message
	kWantPitchBend			= (1L << 6),			// 0xE0	Pitchbend Message
	
	kWantChannelMessages		= kWantNoteOff | kWantNoteOn | kWantPolyAftertouch | kWantController | kWantProgramChange | kWantMonoAftertouch | kWantPitchBend,
	
	//  (1L << 7) = kWantIzzyTick -> see below
	
	// system common messages
	kWantSysEx				= (1L << 8),			// 0xF0
													// 0xF1
	//  (1L << 9) = kWantFadeSceneTick -> see below
	
	kWantSongPosPtr			= (1L << 10),			// 0xF2	(song position ptr)
	kWantSongSel			= (1L << 11),			// 0xF3 (song select)
													// 0xF4
	//  (1L << 12) = kWantVideoFrameTick -> see below
	
	kWantCableSelect		= (1L << 13),			// 0xF5 (cable select)
	kWantTuneReq			= (1L << 14),			// 0xF6 (tune request)
													// 0xF7	(end of exclusive) 
	//  (1L << 15) = kWantIzzyController -> see below
	
	kWantSystemCommonMessage	 = 	kWantSysEx | kWantSongPosPtr | 	kWantSongSel | kWantCableSelect | kWantTuneReq,
	
	// real-time messages
	kWantTiming				= (1L << 16),			// 0xF8 - midi timing message
													// 0xF9
	//  (1L << 17) = kWantBroadcasterMsgs -> see below
	
	kWantSequenceStart		= (1L << 18),			// 0xFA - sequence start message
	kWantSequenceContinue	= (1L << 19),			// 0xFB - sequence continue message
	kWantSequenceStop		= (1L << 20),			// 0xFC - sequence stop message

	//  (1L << 21) = kWantSoundLevel -> see below
													// 0xFD
	kWantActiveSense		= (1L << 22),			// 0xFE - active sense message
	kWantReset				= (1L << 23),			// 0xFF - reset message
	
	// Real Time Message Mask
	kWantMIDIRealTimeMessage	= kWantTiming | kWantSequenceStart | kWantSequenceStop | kWantSequenceContinue | kWantActiveSense | kWantReset,
	
	// All MIDI Message Mask
	kWantAnyMIDIMessage			= kWantChannelMessages | kWantSystemCommonMessage | kWantMIDIRealTimeMessage,
	
	// computer messages
	kWantKeyDown			= (1L << 24),			// key pressed on computer keyboard - message: ptr to char = the character
	kWantKeyUp				= (1L << 25),			// key released on computer keyboard - message: ptr to char = the character
	kWantAnyKeyboardMessage	= kWantKeyDown | kWantKeyUp,
	
	kWantMouseMove			= (1L << 26),			// mouse moved (horizontal or vertical) - message: ptr to MouseMoved struct
	
	// video messages
	kWantVideoNewFrame		= (1L << 27),			// a new frame has arrived at the video input - message: ImageBufferPtr
	kWantStageUpdate		= (1L << 28),			// a stage needs to be updated - message: SInt32* = one based stage index
	kWantStageAboutToDispose= (1L << 29),			// a stage has been disposed - message: SInt32* = one based stage index

	// sound messages
	kWantSoundLevel			= (1L << 21),			// sound level changes - message: pointer to SoundLevelInfo struct
	
	// time messages
	kWantIzzyTick			= (1L << 7),			// want isadora millisecond tick - message: pointer to UInt32* = delta mS since last message
	kWantFadeSceneTick		= (1L << 9),			// want scene fade ticks - message = nil
	kWantVideoFrameTick		= (1L << 12),			// want video frame tick - message: pointer to UInt32* = current midi tick

	// isadora controller messages
	kWantIzzyController		= (1L << 15),			// want messages from controllers

	// broadcaster messages
	kWantBroadcasterMsgs	= (1L << 17),			// want messages transmitted by broadcasters - message: ValuePtr
	kWantIzzyStatusMessage	= (1L << 30),			// wants izzy status message - message: pointer to IzzyStatus struct

	
	// streaming raw data input (serial, tcp/ip, etc.)
	kWantStreamingData		= (1L << 31)			// serial data
};


#ifdef __cplusplus
extern "C" {
#endif

typedef void (MessageReceiveFunction)(
	struct IsadoraParameters*	ip,
	MessageMask					inMessageMask,
	PortIndex					inIndex1,
	const MsgData*				inData,
	UInt32						inLen,
	long						inRefCon);
	
#ifdef __cplusplus
}
#endif

#endif
