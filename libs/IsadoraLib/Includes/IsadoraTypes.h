// ===========================================================================
//	IsadoraTypes			 Â(C)1999 Mark F. Coniglio. All rights reserved.
// ===========================================================================
//
//	Defines and typedefs for Isadora

#ifndef _H_IsadoraTypes
#define _H_IsadoraTypes

#include "ArchitectureFlags.h"
#include "CPlusPlusMacros.h"
#include "CFloatXY.h"
#include "CFloatXYZ.h"

#ifdef __ISADORA_APP__
#define	CONST_		
#else
#define	CONST_	const
#endif

// ----------------------------------------------------------------------------------------
// DATA TYPES
// Predefined data types for values whose type is kData:
// ----------------------------------------------------------------------------------------
//

#define	kImageBufferDataType	FOUR_CHAR_CODE('imgb')			// bitmap video and pictures
#define	kCoreImageDataType		FOUR_CHAR_CODE('cimg')			// core image CIImage
#define	kCoreAudioDataType		FOUR_CHAR_CODE('csnd')			// core audio
#define kDigitalAudioDataType	FOUR_CHAR_CODE('snds')			// digital audio
#define kTextDataType			FOUR_CHAR_CODE('text')			// text (ptr to c-string)

// ----------------------------------------------------------------------------------------
// PLUGIN TYPES
// ----------------------------------------------------------------------------------------
typedef	OSType			PluginClassIDT;				// Plugin class ID
typedef OSType			PluginIDT;					// Plugin ID

// ----------------------------------------------------------------------------------------
// STAGE INDEX
// ----------------------------------------------------------------------------------------
#define	kInvalidStageIndex	((UInt32)(-1))
typedef UInt32				StageIndexT;

// ----------------------------------------------------------------------------------------
// STAGE WIDTH OPTIONS
// ----------------------------------------------------------------------------------------
enum StageRenderOption
{
	kStageRenderFull = 0,
	kStageRenderLeftHalf,
	kStageRenderRightHalf,
	kStageRenderLeftThird,
	kStageRenderMiddleThird,
	kStageRenderRightThird
};

// ----------------------------------------------------------------------------------------
// SCENE INDEX
// ----------------------------------------------------------------------------------------
// MFC 10/28/07: added this definition

typedef	UInt32					SceneIndexT;

// ----------------------------------------------------------------------------------------
// STREAMING PORT INDEX
// ----------------------------------------------------------------------------------------
typedef	UInt32					StreamingPortIndexT;
typedef	UInt32					StreamingProtocolCode;

// ----------------------------------------------------------------------------------------
// SERIAL PORT INDEX
// ----------------------------------------------------------------------------------------
// MFC 10/28/07: added this definition
#define kInvalidSerialStreamIndex		0
#define	kSerialStreamOffset				0x00000000
typedef	StreamingPortIndexT				SerialStreamIndexT;


// ----------------------------------------------------------------------------------------
// TCP PORT INDEX
// ----------------------------------------------------------------------------------------

#define kInvalidTCPStreamIndex			0
#define	kTCPStreamOffset				0x00000100
typedef	StreamingPortIndexT				TCPStreamIndexT;

// ----------------------------------------------------------------------------------------
// MATRIX VALUE INDEX
// ----------------------------------------------------------------------------------------

#define kInvalidMatrixValuetreamIndex	0
#define	kMatrixValueStreamOffset		0x00000200
typedef	StreamingPortIndexT				MatrixValueIndexT;

// ----------------------------------------------------------------------------------------
// HID VALUE INDEX
// ----------------------------------------------------------------------------------------

#define kInvalidHIDValuetreamIndex		0
#define	kHIDValueStreamOffset			0x00000300
typedef	StreamingPortIndexT				HIDValueIndexT;

// ----------------------------------------------------------------------------------------
// STREAMING PORT INDEX
// ----------------------------------------------------------------------------------------
// MFC 02/23/09: to allow input streaming from various
// communications protocols, offset flags were added
// to the streaming protocols above (Serial, TCP/IP, etc.)
// This macro will extract the real port number, which 
// now has a limit of 256 ports per protocol

#define	kExtractStreamingProtocol(p)	( (StreamingProtocolCode) ((p) & 0xFFFFFF00) )
#define	kExtractStreamingPortIndex(p)	( (StreamingPortIndexT) ((p) & 0xFF) )
#define kMaskStreamingPortIndex(p)		( (p) & 0xFF )
#define kEncodeSerialStream(p)			( (SerialStreamIndexT) (kSerialStreamOffset | (p)) )
#define kEncodeTCPStream(p)				( (TCPStreamIndexT) (kTCPStreamOffset | (p)) )
#define kEncodeMatrixValueStream(p)		( (MatrixValueIndexT) (kMatrixValueStreamOffset | (p)) )
#define kEncodeHIDValueStream(p)		( (HIDValueIndexT) (kHIDValueStreamOffset | (p)) )

// ----------------------------------------------------------------------------------------
// GL BITMAAP INDEX
// Index into the bitmap rendering contexts used with the 3D actors and the
// the 3D Renderer actor
// ----------------------------------------------------------------------------------------
// MFC 10/28/07: added this definition

typedef	UInt32				GLBitmapIndexT;

// ----------------------------------------------------------------------------------------
// MODEL INDEX
// Index used to select a 3D model from the list of models
// ----------------------------------------------------------------------------------------
// MFC 10/28/07: added this definition

typedef	UInt32				ModelIndexT;

// ----------------------------------------------------------------------------------------
// CALLBACK3D INDEX
// Index used to select a 3D callback from the list of callbacks
// ----------------------------------------------------------------------------------------
// MFC 10/28/07: added this definition

typedef	UInt32				Callback3DIndexT;

// ----------------------------------------------------------------------------------------
// TEXTURE3D INDEX
// Index used to select a 3D callback from the list of callbacks
// ----------------------------------------------------------------------------------------
// MFC 10/28/07: added this definition

typedef	UInt32				Texture3DDIndexT;

// ----------------------------------------------------------------------------------------
// IzzyTime
// The current Isadora global execution time slice in microseconds
// ----------------------------------------------------------------------------------------
// MFC 01/06/08: added this definition

typedef	UInt64				IzzyTime;

// ----------------------------------------------------------------------------------------
// CapDeviceIndexT
// Zero based index for a video or audio capture device
// ----------------------------------------------------------------------------------------
// MFC 01/24/08: added this definition

typedef	UInt16				CapDeviceIndexT;

#define	kInvalidCaptureDevice	((UInt16) -1)

// ----------------------------------------------------------------------------------------
// ACTOR TYPES
// Isadora Actors are divided into three basic categories:
//
// Watchers wait for some event to occur in the outside world,
// and then report that event. If such a thing has inputs, they
// generally are specify a range of values that define the events
// that this watcher will see.
//
// Actors manipulate data arriving at their inputs and send
// the result out of their outputs.
//
// Generators send data that has been created algorithm-
// ically using parameters specified at their inputs
//
enum {
	kClassWatcher	= FOUR_CHAR_CODE('watc'),		// Watcher
	kClassActor		= FOUR_CHAR_CODE('acto'),		// Actor
	kClassGenerator	= FOUR_CHAR_CODE('gene')		// Generator
};
typedef UInt32			ActorClasses;

enum {
	kGroupVideo						= FOUR_CHAR_CODE('vide'),

	kSubGroupvideoSources			= FOUR_CHAR_CODE('vdpp'),
	kSubGroupVideoRenderers			= FOUR_CHAR_CODE('vdrn'),
	kSubGroupVideoGeneral			= FOUR_CHAR_CODE('vdst'),
	kSubGroupVideoCoreImage			= FOUR_CHAR_CODE('vdci'),
	kSubGroupVideoQuartzComposer	= FOUR_CHAR_CODE('vdqc'),
	kSubGroupVideoFreeFrame			= FOUR_CHAR_CODE('vdff'),

	kGroupSound						= FOUR_CHAR_CODE('snd '),
	kGroupMidi						= FOUR_CHAR_CODE('midi'),
	kGroupGenerators				= FOUR_CHAR_CODE('gen '),
	kGroupMouseKeyboard				= FOUR_CHAR_CODE('mskb'),
	kGroupCalculation				= FOUR_CHAR_CODE('calc'),
	kGroupControl					= FOUR_CHAR_CODE('cntl'),
	kGroupCommunications			= FOUR_CHAR_CODE('comm'),
	kGroupText						= FOUR_CHAR_CODE('text'),
	kGroupSceneControl				= FOUR_CHAR_CODE('scne'),
	kGroupUser						= FOUR_CHAR_CODE('user')
	
};

typedef	PluginClassIDT		ActorClassIDT;				// Actor class ID
typedef PluginIDT			ActorIDT;					// Actor ID

typedef	UInt32				UATime;

// if we're compiling C++, make the UserActorIDT a C++ class
// otherwise -- it's just a struct.
#ifdef __cplusplus
class UserActorIDT
{
public:
	bool	operator==(
				const UserActorIDT&			inRhs) const
					{ return memcmp(this, &inRhs, sizeof(UserActorIDT)) == 0; }


#else
struct UserActorIDT
{
#endif

	PluginIDT	mPluginID;	// user actor ID
	UATime		mTime;		// gmtime at which created
};

#ifndef __cplusplus
typedef struct UserActorIDT UserActorIDT;
#endif

#define	IsUserActorPluginID(a) ( (((a).mPluginID >> 24) & 0xFF) == kGlobalUserActorIDPrefix\
 || (((a).mPluginID >> 24) & 0xFF) == kLocalUserActorIDPrefix\
 || (((a).mPluginID >> 24) & 0xFF) == kMacroIDPrefix )

#define	AreUserActorIDsEqual(a, b)	((a).mPluginID == (b).mPluginID && (a).mTime == (b).mTime)

// ----------------------------------------------------------------------------------------
// MATRIX VALUE struct
//
enum {
	MatrixValue_Action_Run,
	MatrixValue_Action_Deactivate
};

typedef struct {
	UInt32	mAction;			// MatrixValue_Action_Run or MatrixValue_Action_Deactivate
	UInt32	mSceneIndex1;		// scene number from which this message originated
	UInt32	mPort;				// port id of this matrix value message 
	float	mSceneIntensity;	// intensity of this scene
	float	mValue;				// intensity of this value
	UInt32	mChannelCount;		// number of channels to follow
	UInt32	mChannels[1];		// channel numbers
} MatrixValue;

// ----------------------------------------------------------------------------------------
// CONTROL TYPES
// Isadora Actors are divided into three basic categories:
//
typedef PluginClassIDT		ControlClassIDT;			// Control ID
typedef PluginIDT			ControlIDT;					// Control ID
typedef void*				IzzyCtlPtr;					// abstract ptr to isadora CIzzyControl object
typedef UInt32				IzzyCtlNum;					// control number

#define	kUnassignedControlNum		0
#define	kMinControlNum				1
#define kMaxControlNum				999

////////////////////////////////////////////////////////////////////////////
// IzzyControlMsg struct
////////////////////////////////////////////////////////////////////////////
//	This struct is passed to a ReceiveMessage function in the inData
//	parameter. The plugin should typecast inData to a type of IzzyControlMsg*
//	to make use of it.

struct FloatControlMsg
{
	float		mMin;					// minimum value - ignored if mDirect is true
	float		mMax;					// maximum value - ignored if mDirect is true
	float		mValue;					// current value
	Boolean		mDirect;				// when true, ignore min & max and don't scale
	
	Boolean		mIgnoreOutOfRange;		// only meaningful when mDirect = true
};

enum ControlMsgType {
	kIzzyCtl_FloatMessage,
	kIzzyCtl_StringMessage
};

struct IzzyControlMsg
{
	ControlMsgType	mType;				// message type -- see above
	IzzyCtlNum		mCtlNum;			// the control ID from which the message was received
	
	union {
		FloatControlMsg	mFloat;			// float message
		const char*		mString;		// string message
	} u;
};

typedef struct IzzyControlMsg IzzyControlMsg;

////////////////////////////////////////////////////////////////////////////
// PreloadInfo struct
////////////////////////////////////////////////////////////////////////////
//	This struct is passed to a the Media Command Queue via a
//  QueueMediaCommand_ call. It gives specific information about
//	how the media is to be queued

enum PreloadMode {
	kKeepUntilNextPlay = 0,
	kKeepUntilUnloaded = 1,
	kKeepUntilNextPlayInRAM = 2,
	kKeepUntilUnloadedInRAM = 3
};

struct PreloadInfo
{
	PreloadMode		mPreloadMode;				// see above
	SInt32			mSoundOut;					// 0 = std, 1 = e1-2, 2 = e3-4, etc.
	StageIndexT		mStageIndex1;				// 0 = to another actor, 1 = stage 1, 2 = stage 2, etc.
	Boolean			mLoadIntoRAM;				// false = don't load into RAM, true = load into RAM
	Boolean			mDirectToStageOutput;		// if true, will play direct to the stage
	Boolean			mCoreImageOutput;			// if true, will use CoreImage to play
	UInt32			mTrackOutputRoutingCount;	// 0 = for 1 track only, 1 to set the routing for specific tracks
	SInt32			mTrackOutputRouting[1];		// an array of track output routings
};

typedef struct PreloadInfo PreloadInfo;

////////////////////////////////////////////////////////////////////////////
// HSBCAdjust struct
////////////////////////////////////////////////////////////////////////////
//	Specifies adjustmens to the hue, saturation, brightness, and contrast

struct HSBCAdjust
{
	float					mHue;
	float					mSaturation;
	float					mBrightness;
	float					mContrast;
};

typedef struct HSBCAdjust HSBCAdjust;

////////////////////////////////////////////////////////////////////////////
// IzzyStatus struct
////////////////////////////////////////////////////////////////////////////
//	This struct is passed to Message Receivers who want the IzzyStatus
//	message. The comments to the right of each IzzyStatusMessage indicate
//	the meaning of the member. Check these comments for the meaning of the
//	mData parameter if it is used.

enum IzzyStatusMessage {
	kActivateScene			= (1 << 0),		// sent when the scene is activated and fading in	
	// kFadeInComplete		= (1 << 1),		// sent when the fade of a scene in is complete		
	// kStartFadeOut		= (1 << 2),		// sent when a scene starts to fade out				
	// kDeactivateScene		= (1 << 3),		// sent just immediately prior to deactivating a scene
	kMediaListAboutToChange	= (1 << 4),		// sent prior to the the media list being changed due to import or reordering
	kMediaListJustChanged	= (1 << 5)		// sent after to the the media list has been changed due to import or reordering
};

struct IzzyStatus
{
	IzzyStatusMessage	mStatus;		// see definitions above
	void*				mData;			// currently unused data pointer
};

typedef struct IzzyStatus IzzyStatus;

// ----------------------------------------------------------------------------------------
// THINGS

typedef	void*			ThingPtr;					// abstract ptr to an isadora CThing object
typedef UInt32			ThingID;					// unique ID for every CThing object	

enum {
	kJumpRelative = 1,
	kJumpAbsolute = 2
};
typedef	UInt16			JumpMode;					// 1 = relative, 2 = absolute

// ----------------------------------------------------------------------------------------
// PROPERTIES

enum {										// a property is either input or output
	kPropertyTypeInvalid = 0,
	kInputProperty,
	kOutputProperty
};
typedef UInt8			PropertyType;		// the type of an input property

typedef UInt16			PropertyIndex;		// index used to access a property
typedef UInt16			DestinationIndex;	// index used to access a link
typedef	UInt32			PropIDT;			// property ID code

typedef void*			PropertyPtr;		// abstract ptr to an Isadora CProperty object

// This structure is passed to properties whose type
// is changing to match another input or output whose
// value display is property defined. This allows the
// linked property to use that properties functions
// to convert from a value to a string and vice-versa
typedef struct {
	PluginIDT		mActorID;
	PropertyType	mPropertyType;
	PropIDT			mPropertyID;
} PropertyDefinedValueSource;

typedef struct PropDefinedValueSource	PropDefinedValueSource;

// PROPERTY FORMAT NUMBERS
// used to create the property display type bit mask below

enum {
	// 0 - 7
	kDisplayFormatNumberBit	= 0,		// displays number as integer or float
	kDisplayFormatOnOffBit,				// displays number as off or on (0-1)
	kDisplayFormatHexBit,				// displays number in hexidecimal format
	kDisplayFormatMIDINoteBit,			// displays number as a MIDI Note Name (0-127)
	kDisplayFormatCharacterBit,			// a alphanumeric character, from (0-255)
	kDisplayFormatTimecodeBit,			// displays a time percentage as timecode
	kDisplayFormatSecondsBit,			// display milliseconds value as integer
	kDisplayFormatMilliSecsBit,			// display milliseconds as milliseconds

	// 8 - 15
	kDisplayFormatHertzBit,				// display miillseconds as hertz (or cycles per second)
	kDisplayFormatTriggerBit,			// displays 0 as '-' and 1 as '*'
	kDisplayFormatColorBit,				// displays float as color
	kDisplayFormatVideoBit,				// displays as "video"
	kDisplayFormatSoundBit,				// displays as "sound"
	kDisplayFormatTextBit,				// displays as "text"
	kDisplayFormatFontBit,				// displays "text" as font
	kDisplayFormatMidiTicksBit,			// displays a MIDI Ticks			
	
	// 16-20
	kDisplayFormatMovieFileBit,			// displays movie file names		
	kDisplayFormatSoundFileBit,			// displays sound file names		
	kDisplayFormatPictFileBit,			// displays picture names		
	kDisplayFormatMidiFileBit,			// displays midi file names
	kDisplayFormat3DFileBit,			// displays 3D file names	
	
	kDisplayFormatCoreImageBit,			// displays as "image"
	kDisplayFormatCoreAudioBit,			// displays as "audio"
	
	// 23
	// 24
	
	// special display format flags
	kDisplayPropertyGrpConformIDBit = 25,	// these properties will be automatically grouped with the matching conform IDs
	kDisplayPropertyPopupMenuBit = 26,		// when user clicks on this item, call generate popup menu proc
	kDisplayPropertyCustomDataBit = 27,		// DataValueInfo struct is actuall CustomDataValueInfo with encoded name
	kDisplayPropertyNoConformBit = 28,		// prevents this value from conforming
	kDisplayPropertyDontSnapBit = 29,		// prevents this property from getting snapped by snapshot
	kDisplayPropertyDefinedBit = 30,		// display using actor supplied function
	kDisplayFormatMutableBit = 31			// property type changes to match it's input
};

// PROPERTY FORMAT FLAGS
// determines how a properties value will be displayed
enum {
	kDisplayFormatInvalid			= 0,
	kDisplayFormatNumber			= (1 << kDisplayFormatNumberBit),		// 0000 0001
	kDisplayFormatOnOff				= (1 << kDisplayFormatOnOffBit),		// 0000 0002
	kDisplayFormatHex				= (1 << kDisplayFormatHexBit),			// 0000 0004
	kDisplayFormatMIDINote			= (1 << kDisplayFormatMIDINoteBit),		// 0000 0008
	kDisplayFormatCharacter 		= (1 << kDisplayFormatCharacterBit),	// 0000 0010
	kDisplayFormatTimecode			= (1 << kDisplayFormatTimecodeBit),		// 0000 0020
	kDisplayFormatSeconds			= (1 << kDisplayFormatSecondsBit),		// 0000 0040
	kDisplayFormatMilliSecs			= (1 << kDisplayFormatMilliSecsBit),	// 0000 0080
	
	kDisplayFormatHertz				= (1 << kDisplayFormatHertzBit),		// 0000 0100
	kDisplayFormatTrigger			= (1 << kDisplayFormatTriggerBit),		// 0000 0200
	kDisplayFormatColor				= (1 << kDisplayFormatColorBit),		// 0000 0400
	kDisplayFormatVideo				= (1 << kDisplayFormatVideoBit),		// 0000 0800
	kDisplayFormatSound				= (1 << kDisplayFormatSoundBit),		// 0000 1000
	kDisplayFormatText				= (1 << kDisplayFormatTextBit),			// 0000 2000
	kDisplayFormatFont				= (1 << kDisplayFormatFontBit),			// 0000 4000
	kDisplayFormatMidiTicks			= (1 << kDisplayFormatMidiTicksBit),	// 0000 8000
	
	kDisplayFormatMovieFile			= (1 << kDisplayFormatMovieFileBit),	// 0001	0000
	kDisplayFormatSoundFile			= (1 << kDisplayFormatSoundFileBit),	// 0002	0000
	kDisplayFormatPictFile			= (1 << kDisplayFormatPictFileBit),		// 0004	0000
	kDisplayFormatMidiFile			= (1 << kDisplayFormatMidiFileBit),		// 0008	0000
	kDisplayFormat3DFile			= (1 << kDisplayFormat3DFileBit),		// 0010	0000
	
	kDisplayFormatMedia				= (kDisplayFormatMovieFile | kDisplayFormatSoundFile | kDisplayFormatPictFile | kDisplayFormatMidiFile | kDisplayFormat3DFile),
	
	kDisplayFormatCoreImage			= (1 << kDisplayFormatCoreImageBit),	// 0020	0000
	kDisplayFormatCoreAudio			= (1 << kDisplayFormatCoreAudioBit),	// 0040	0000
																			// 0080 0000
																			
																				// 0100 0000
																				// 0200 0000
	kDisplayFormatGrpConformID		= (1 << kDisplayPropertyGrpConformIDBit),	// 0400 0000
	kDisplayFormatPopupMenu			= (1 << kDisplayPropertyPopupMenuBit),		// 0400 0000
	kDisplayFormatCustomData		= (1 << kDisplayPropertyCustomDataBit),		// 0800 0000
	kDisplayPropertyNoConform		= (1 << kDisplayPropertyNoConformBit),		// 1000 0000
	kDisplayPropertyDontSnap		= (1 << kDisplayPropertyDontSnapBit),		// 2000 0000
	kDisplayPropertyDefined			= (1 << kDisplayPropertyDefinedBit),		// 4000 0000
	kDisplayFormatMutable			= (1 << kDisplayFormatMutableBit)			// 8000 0000
};

// the display format for a property
typedef	UInt32				PropertyDispFormat;		
typedef UInt32				PropertyOptions;
typedef UInt8				MutableGroup;

#define ExtractPropertyDisplayFormatBit(a, b)		( ((PropertyDispFormat) (a)) & ((PropertyDispFormat) (b)) )
#define MaskPropertyDisplayFormatBit(a, b)			( ((PropertyDispFormat) (a)) & ~((PropertyDispFormat) (b)) )
#define IsPropertyDisplayFormatBitSet(a, b)			( ExtractPropertyDisplayFormatBit(a, b) != 0 )

#define ExtractPropertyDisplayFormatMutableBit(a)	ExtractPropertyDisplayFormatBit(a, kDisplayFormatMutable)
#define MaskPropertyDisplayFormatMutableBit(a)		MaskPropertyDisplayFormatBit(a, kDisplayFormatMutable)
#define IsPropertyDisplayFormatMutableBitSet(a)		IsPropertyDisplayFormatBitSet(a, kDisplayFormatMutable)

#define	kIndMutableGroup	255

enum {
	kPropertyOption_DrawValueGraphically = (1 << 0)
};

#define	kTriggerIdleString		"-"
#define	kTriggerActiveString	"X"

// ----------------------------------------------------------------------------------------
// USER ACTOR CONSTANTS

#define	kGlobalUserActorIDPrefix		'$'
#define	kLocalUserActorIDPrefix			'#'
#define	kMacroIDPrefix					'@'
#define	kUserInPropIDPrefix				'i'
#define kUserOutPropIDPrefix			'o'

// ----------------------------------------------------------------------------------------
// MOD KEY CONSTANTS
// bit mask indicates which modifier key(s) are pressed

enum {
	kCommandMask = (1 << 0),
	kControlMask = (1 << 1),
	kOptionMask = (1 << 2),
	kShiftMask = (1 << 3),
	kShiftLockMask = (1 << 4)
};

// ----------------------------------------------------------------------------------------
// Measure/Beat/Tick conversion struct

typedef struct {
	UInt16	mMeasure;	// set on input
	UInt16	mBeat;		// set on input
	UInt16	mTick;		// set on input
} MBT;

// ----------------------------------------------------------------------------------------
// MediaPlayInfo

typedef struct {
	UInt16					mLoopStepIndex1;	// current loop step index - 0 if none
	UInt16					mRepeatIndex1;		// current loop repeat index - 0 if non
	UInt16					mLoopMode;			// 0 = forward, 1 = backward, 2 = palindrome
	float					mLoopRate;			// loop rate; +1.0 = forward, normal, -1.0 = backwards normal; +2.0 = forward twice normal, etc.
	UInt32					mLoopStart;			// current loop start point
	UInt32					mLoopDuration;		// current loop duration in milliseconds
} LoopStepInfo;

typedef struct {
	
	struct ImageBuffer*		mImageBuffer;		// returns pointer to media's ImageBuffer (if there is one)
	struct CoreImageBuffer*	mCoreImageBuffer;	// returns pointer to media's CoreImageBuffer (if there is one)
	float					mPlayPosition;		// returns pointer to current play position
	Boolean					mIsPlaying;			// returns true if media is playing now

	// only valid when using movie looper -- outputs giving loop status
	LoopStepInfo			mLoopStepInfo;
	
	// audio metering
#define kMaxAudioVolumeMeteringChannels	8
	Boolean					mAudioVolumeMeteringInfoValid;									// if true then the next two fields are valid
	UInt32					mNumAudioVolumeMeteringChannels;								// number of valid channels in the mAudioVolumeMeteringLevels array
	Float32					mAudioVolumeMeteringLevels[kMaxAudioVolumeMeteringChannels];	// levels for the audio metering
	
} MediaPlayInfo;

// ----------------------------------------------------------------------------------------
// Acquire ID

typedef	SInt32	AcquireIDT;

// ----------------------------------------------------------------------------------------
// OutputGeometry

typedef struct
{
	bool		mValid;							// when true -- data is valid
	float		mDirectOutputCenterH;			// direct output - image h center : 0.0 = centered; +/- 1.0 = full stage width
	float		mDirectOutputCenterV;			// direct output - image v center : 0.0 = centered; +/- 1.0 = full stage height
	float		mDirectOutputWidth;				// direct output - image width : 1.0 = 100%
	float		mDirectOutputHeight;			// direct output - image height : 1.0 = 100%
	float		mDirectOutputZoom;				// direct output - image zoom : 1.0 = 100%
	bool		mDirectOutputPreserveAspect;	// direct output - preserve aspect ratio if true

} OutputGeometry;

#define	InitOutputGeometry(og) { og.mValid = true; og.mDirectOutputCenterH = 0.0f; og.mDirectOutputCenterV = 0.0f; og.mDirectOutputWidth = 1.0f; og.mDirectOutputHeight = 1.0f; og.mDirectOutputZoom = 1.0f; og.mDirectOutputPreserveAspect = false; } 

// ----------------------------------------------------------------------------------------
// AcquireMediaInfo

typedef struct
{
	Boolean			mInRequestDirectOutput;			// input: true if requesting direct output to the stage
	Boolean			mInRequestCoreImageOutput;		// input: true if requesting core image output
	StageIndexT		mInStageIndex0;					// input: desired stage window index
	Boolean			mInLoadIntoRAM;					// input: if true, attempt to load into RAM
	OutputGeometry	mInDirectToOutputGeometry;		// input: only used if mRequestDirectOutput is true
} AcquireMovieInfo;

typedef struct {
	union {
		AcquireMovieInfo	movie;
	} u;
} AcquireMediaInfo;

// ----------------------------------------------------------------------------------------
// MediaIdleMode

enum {
	kMediaIdleNone = 0,
	kMediaIdleWaitBackgroundService,
	kMediaIdleWaitNewFrame
};

typedef	SInt32	MediaIdleMode;

// ----------------------------------------------------------------------------------------
// ShowMediaInfo

typedef struct
{
	TimeValue		mOutMovieDuration;				// output: duration of the movie
	TimeValue		mOutMovieTimeScale;				// output: time scale of the movie
	UInt32			mOutVideoTrackCount;			// output: number of video tracks
	UInt32			mOutAudioTrackCount;			// output: number of audio tracks	
} ShowMovieInfo;

typedef struct {
	union {
		ShowMovieInfo	movie;
	} u;
} ShowMediaInfo;

// ----------------------------------------------------------------------------------------
// MEDIA COMMANDS
//
// Media Commands are passed via the callback mechanism (one of
// QueueMediaCommand_, QueueMediaCommandReplace_, or
// ExecuteMediaCommand_) to tell Isadora to manipulate some
// piece of media. DoQueueMediaCommand and  QueueMediaCommandReplace_
// can be safely called at interrupt/thread time, regardless of the type of
// media. ExecuteMediaCommand_ should be called for functions that are
// interrupt/thread safe only.
//

enum
{
	// do nothing command
	kMediaNullCmd = 0,						// param1 = n/a									param2 = n/a
	
	// get play info for a time-based media object
	kMediaGetMediaPlayInfo,					// param1 = pointer to MediaPlayInfo			param2 = n/a
	
	// get the output image buffer for a visual media object
	kMediaGetImageBuffer,					// param1 = pointer to CImageBuffer*			param2 = n/a
	
	// retrieve name of a media object
	kMediaGetName,							// param1 = pointer to char buffer				param2 = size of buffer in bytes
	
	// note that show media will also start playing the media
	kMediaShowMedia,						// param1 = ShowMediaInfo*						param2 = non-zero to load into RAM
	kMediaShowMediaDirect,					// param1 = ShowMediaInfo*						param2 = non-zero to load into RAM
	kMediaHideMedia,						// param1 = n/a									param2 = n/a
	
	// commands to acquire and release a media object
	kMediaAcquire,							// param1 = Boolean* = successfully acquired	param2 = AcquireMediaInfo* (optional)
	kMediaRelease,							// param1 = n/a									param2 = n/a
	
	// commands to preload and unload preloaded media
	kMediaPreload,							// param1 = PreloadMode: kKeepUntilNextPlay(0)	param2 = n/a
											//			or kKeepUntilUnloaded(1)
	kMediaUnload,							// param1 = n/a									param2 = n/a
	
		// time based playback control
	kMediaPlay,								// param1 = fixed rate with 0x10000 = 1.0		param2 = sound channel for audio media, 0 for others
	kMediaStop,								// param1 = n/a									param2 = n/a
	kMediaRestart,							// param1 = n/a									param2 = n/a
	kMediaGetPlaying,						// param1 = Boolean* = true if playing			param2 = n/a
	kMediaGoToTime,							// param1 = millisecond location				param2 = n/a
	kMediaSetSpeed,							// param1 = UnsignedFixed speed: 0x10000 = 1.0	param2 = n/a
	
		// location
	kMediaGetDuration_mS_DEPRECATED,		// param1 = UInt32* = the position				param2 = n/a
	kMediaSetPosition_mS_DEPRECATED,		// param1 = locate position, in millseconds		param2 = n/a
	kMediaGetPosition_mS_DEPRECATED,		// param1 = UInt32* = the position				param2 = n/a
	
	kMediaSetPosition_Percent,				// param1 = locate position (float 0-100%)		param2 = n/a
	kMediaGetPosition_Percent,				// param1 = float* = the position				param2 = n/a
	kMediaSetPositionWithinLoop_Percent,	// param1 = locate position (float 0-100%)		param2 = n/a
	
		// looping
	kMediaSetLoopStart_mS_DEPRECATED,		// param1 = loop start time in milliseconds		param2 = n/a
	kMediaSetLoopStart_Percent,				// param1 = loop start as % of total dur		param2 = n/a
	kMediaSetLoopDuration_mS_DEPRECATED,	// param1 = loop dur in milliseconds			param2 = n/a
	kMediaSetLoopDuration_Percent,			// param1 = loop dur as % of total dur			param2 = n/a
	kMediaSetLoopStartDuration_Percent,		// param1 = loop start as % of total dur		param2 = n/a
											// param2 = loop duration as % of total dur		param2 = n/a
	kMediaSetLoopFlag,						// param1 = looping on or off (0 or 1)			param2 = palindromic looping on or off (0 or 1)
	
		// set crossfade frames
	kMediaSetCrossfadeFrames,				// param1 = number of frames					param2 = n/a
	
		// position of visual media on the stage
	kMediaSetHorizPosition,					// param1 = horizontal position in pixels		param2 = n/a
	kMediaSetVertPosition,					// param1 = vertical position in pixels			param2 = n/a
	kMediaSetWidth,							// param1 = width position in pixels			param2 = n/a
	kMediaSetHeight,						// param1 = height position in pixels			param2 = n/a

		// sound files
	kMediaSetVolume_Percent,				// param1 = float* = volume, from 0 to 100%		param2 = n/a
	kMediaSetPan_Percent,					// param1 = float* = panning, from 0 to 100%	param2 = n/a

		// external dv devices
	kMediaDVControlCommand,					// param1 = dv sub command						param2 = timecode location for locate
	
		// updating the stage
	kMediaUpdateStage,						// param1 = stage index (one-based)				param2 = n/a
	
		// audio chunk features
	kMediaSetChunkEnable,					// param1 = boolean 1 or 0						param2 = n/a
	kMediaSetChunkSizeFrames,				// param1 = chunk size in frames				param2 = n/a
	kMediaSetChunkSkipFrames,				// param1 = chunk skip in frames				param2 = n/a
	
		// Quicktime VR
	kMediaSetVRPan_Percent,					// param 1 = float* = pan in degrees			param2 = n/a
	kMediaSetVRTilt_Percent,				// param 1 = float* = tilt in degrees			param2 = n/a
	kMediaSetVRZoom_Percent,				// param 1 = float* = field of zoom in percent	param2 = n/a
	
		// access to object count
	kMediaGetObjectCount,					// param1 = UInt32* to receive count			param2 = n/a
	
		// set tempo for a time based media with BPM measurement
	kMediaSetTempo,							// param1 = float* = tempo						param2 = n/a

		// convert measures, beats, and ticks into a percentage
	kMediaMBTToPercent,						// param1 = MBT*								param2 = float*
	
		// force the output midi channel to the specified channel
	kMediaForceMidiChannel,					// param1 = 0 for assignments in MIDI file, 1-16 to force to that channel
	
		// convert milliseconds to a percentage for the specified media object
	kMediaMillisecondsToPercent,			// param1 = milliseconds						param2 = ptr to float

		// set the hit loop end proc for the specified time based media
	kMediaSetHitLoopEndProc,				// param1 = MediaHitLoopEndProc pointer			param2 = callback refcon
	
		// set sound output routing codes for media that can output sound
	kMediaSetExtSoundOutputRouting,			// param1 = 0 for built-in, 1 for outputs 1 & 2, 2 for outputs 3 & 4, etc.
	
	kMediaPlayAudioStream,					// param1 = CSoundProvider* - left chan			param2 = CSoundProvider* - right chan
	kMediaStopAudioStream,					// param1 = n/a									param2 = n/a
	
		// Sound Movie Settings
	kMediaSetTrackVolume_Percent,			// param1 = Track Index (one-based				param2 = float* = with volume, from 0 to 100%
	kMediaSetTrackPan_Percent,				// param1 = Track Index (one-based)				param2 = float* = panning, from 0 to 100%
	kMediaSetTrackExtSoundOutputRouting,	// param1 = Track Index (one-based				param2 = 0 for built-in, 1 for outputs 1 & 2, 2 for outputs 3 & 4, etc.
	
		// 3D Models
	kMedia3DAttachModel,					// param1 = ModelInfo*							param2 = n/a
	kMedia3DDetachModel,					// param1 = one based renderer channel			param2 = n/a

		// 3D Stage Orientation
	kMedia3DRotation,						// param1 = CFloatXYZ* = delta rotation			param2 = n/a
	kMedia3DTranslate,						// param1 = CFloatXYZ* = delta translation		param2 = n/a
	kMedia3DScale,							// param1 = CFloatXYZ* = delta translation		param2 = n/a
	kMedia3DWriteTextureMap,				// param1 = ImageBufferPtr						param2 = n/a
	kMedia3DSetTextureMapScaling,			// param1 = ptr to CFloatXYZ = scaling values	param2 = n/a
	kMedia3DSetUseAlpha,					// param1 = 0 to disable alpha, 1 to enable		param2 = n/a
	kMedia3DSetTransparency,				// param1 = float* - transparency from 0 to 1	param2 = n/a

		// 3D Renderes
	kMedia3DCreateRenderer,					// param1 = width								param2 = height
	kMedia3DDisposeRenderer,				// param1 = n/a									param2 = n/a
	kMedia3DRenderDraw,						// param1 = n/a									param2 = 1 for stage / 0 for bitmap renderer
	kMedia3DCameraRotation,					// param1 = delta rotation - ptr to CFloatXYZ	param2 = 1 for stage / 0 for bitmap renderer
	kMedia3DCameraTranslate,				// param1 = delta rotation - ptr to CFloatXYZ	param2 = 1 for stage / 0 for bitmap renderer
	kMedia3DGetImageBuffer,					// param1 = CImageBuffer**
	
		// 3D Drawing Callbacks
	kMedia3DAttachDrawCallback,				// param1 = one based renderer channel			param2 = ptr to Render3DInfo
	kMedia3DDetachDrawCallback,				// param1 = one based renderer channel			param2 = ptr to Render3DInfo
	kMedia3DContextNeedsDraw,				// param1 = n/a									param2 = 1 for stage / 0 for bitmap renderer
	kMedia3DCreateTextureMap,				// param1 = one based renderer channel			param2 = ptr to TextureMapInfo
	kMedia3DDisposeTextureMap,				// param1 = ptr to TextureMapInfo				param2 = n/a
	kMedia3DStoreTextureMapImage,			// param1 = ptr to TextureMapInfo				param2 = n/a
	kMedia3DPrepareTextureMapForRender,		// param1 = ptr to TextureMapInfo				param2 = n/a
	kMedia3DLightPositionAndColors,			// param1 = ptr to LightingInfo	  				param2 = 1 for stage / 0 for bitmap renderer
	
		// hue-saturation-brightness-contrast control (CoreImage Movies Only!)
	kMediaAdjustHSBC,						// param1 = ptr to HSBCAdjust struct			param2 = n/a
	
		// stage options
	kMediaSetStageScalingOptions,			// param1 = ptr to StageScalingOptions  		param2 = 1 for stage / 0 for bitmap renderer
	kMediaSetStageBackgroundOptions,		// param1 = ptr to StageBackgroundOptions		param2 = 1 for stage / 0 for bitmap renderer
	kMediaClearStageBackgroundOptions,		// param1 = ptr to StageBackgroundOptions 		param2 = 1 for stage / 0 for bitmap renderer
	kMediaUpdateStageBackgroundOptions,		// param1 = ptr to StageBackgroundOptions		param2 = 1 for stage / 0 for bitmap renderer
	
		// get core image buffer for a visual media object
	kMediaGetCoreImageBuffer,				// param1 = pointer to CoreImageBuffer*			param2 = n/a

		// get core image buffer for a visual media object
	kMediaSetLoopList,						// param1 = pointer to CLoopStepList			param2 = n/a
											// use nil ptr to clear the list


	kMediaIdle,								// param1 = MediaIdleMode						param2 = n/a
	
	kMediaEnableAudioFrequencyMonitor,		// param1 = 1 to enable, 0 to disable
	kMediaGetAudioFrequencyLevels,			// param1 = UInt32* - channels in level array	param2 = Float32* - array of levels
	
	kMediaCommandGroupStart,				// param1 = n/a									param2 = n/a
	kMediaCommandGroupEnd,					// param1 = n/a									param2 = n/a
	
	kMediaUpdateOutputGeometry,				// param1 = OutputGeometry*						param2 = n/a
	
	kMediaSetGLRenderOptions,				// param1 = ptr to GLRenderOptions				param2 = 1 for stage / 0 for bitmap renderer
	kMediaClearGLRenderOptions,				// param1 = ptr to GLRenderOptions				param2 = 1 for stage / 0 for bitmap renderer

	kMedia3DSetRenderBack,					// param1 = 0 to disable back face, 1 to enable	param2 = n/a

	kMediaActivate,							// param1 = n/a									param2 = n/a

	kMediaSetGlobalStageKeystoneOptions,	// param1 = ptr to StageKeystoneOptions			param2 = 1 for stage / 0 for bitmap renderer
	kMediaGetGlobalStageKeystoneOptions,	// param1 = ptr to StageKeystoneOptions			param2 = 1 for stage / 0 for bitmap renderer
	
	kMediaSetGlobalEdgeBlendOptions,		// param1 = ptr to StageEdgeBlendOptions		param2 = 1 for stage / 0 for bitmap renderer
	kMediaGetGlobalEdgeBlendOptions,		// param1 = ptr to StageEdgeBlendOptions		param2 = 1 for stage / 0 for bitmap renderer
	
		// set the hit loop end proc for the specified time based media
	kMediaSetTextOutputProc,				// param1 = MediaTextOutputProc pointer			param2 = TextOutputInfo*
	
		// marker for the last command
	kMediaLastCommand
};
typedef	UInt16				MediaCommand;

// ----------------------------------------------------------------------------------------
//	MediaCommandComplete Callback
//
//	Passed as a parameter to QueueMediaCommand_ or QueueMediaCommandReplace_ functions, this
//	callback will be called when the media command has been executed.

#ifdef __cplusplus
extern "C" {
#endif

typedef	void	(*MediaCommandCompleteProc)(UInt32	inQueuedCommandID);

#ifdef __cplusplus
}
#endif				

// ----------------------------------------------------------------------------------------
//	Render3DCallback Callback
//
// Passed as a parameter to inside of the Render3DInfo structure (see below),
// this callback allows plugins to render in 3D using OpenGL. Once attached to
// a specific stage using the kMedia3DAttachDrawCallback, this function will be
// called each time the stage needs to be rendered. At the time of the call, the
// OpenGL context is current, the camera has been positioned (as specified by the
// 3D Stage Orientation actor), the lighting has been positioned, and the scene
// is ready for rendering commands (i.e., a call to glBegin(...), glVertex, etc.)

#if __cplusplus
extern "C" {
#endif

typedef void	(*Render3DCallback)(struct IsadoraParameters* ip, void* inParam1, void* inParam2);

#if __cplusplus
}
#endif

// ----------------------------------------------------------------------------------------
//	Blend Mode Constants -- for use with all objects that create projection surfaces
//
enum {
	kBlendMode_Additive = 0,
	kBlendMode_Transparent = 1,
	kBlendMode_Opaque = 2
};

// ----------------------------------------------------------------------------------------
// Render3DInfo provides information necessary to call a 3D Rendering function
// that exists inside of an Isadora Plugin.
// 
// This info is passed to Isadora using the ExecuteMediaCommand_ function with a command
// of kMedia3DAttachDrawCallback. Once this is call is made, the callback will be called
// each time the stage needs to be rendered.
//
// When the plugin is done rendering, it should call ExecuteMediaCommand_ with a command of
// kMedia3DAttachDrawCallback
// 
struct Render3DInfo
{
	Render3DCallback			mCallback;
	SInt32						mLayer;
	ThingPtr					mThing;
	void*						mRefCon1;
	void*						mRefCon2;
};
typedef struct Render3DInfo Render3DInfo;
typedef const Render3DInfo* ConstRender3DInfoPtr;

// ----------------------------------------------------------------------------------------
// StageScalingOptions provides information necessary to set the scaling of imagery
// when rendering to the stage 
struct StageScalingOptions
{
	CFloatXY		mViewportOffset;		// viewport offset, 1.0 = one window width/height
	CFloatXY		mViewportSize;			// viewport scale, 1.0 = one window width/height
};
typedef struct StageScalingOptions StageScalingOptions;
typedef const StageScalingOptions* ConstStageScalingOptionsPtr;

// ----------------------------------------------------------------------------------------
// StageBackgroundOptions sets the background color of the stage and whether or not
// the stage will be erased before each rendering cycle
struct StageBackgroundOptions
{
	UInt32		mBkgColor;				// background color to use when rendering
	Boolean		mErase;					// if true, erase before each frame; if false, don't erase
	
	float		mIntensity;				// intensity level of these options, from 0.0 to 1.0; no effect when set to 0.0
										
	float		mFeedbackAmt;			// feedback intensity from 0.0 to 1.0; effectively "off" when 0.0
	float		mScaleH;				// 1.0 = no scaling; < 1.0 = smaller, > 1.0 = bigger
	float		mScaleV;				// ditto
	float		mOffsetH;				// < 0.0 = left, > 0.0 = right
	float		mOffsetV;				// < 0.0 = down, > 0.0 = up
	float		mRotation;				// rotation: -360 to +360
	float		mRotCenterH;			// rotation center point within the image
	float		mRotCenterV;
};
typedef struct StageBackgroundOptions StageBackgroundOptions;
typedef const StageBackgroundOptions* ConstStageBackgroundOptionsPtr;

// ----------------------------------------------------------------------------------------
// LStream - forward declaration for C++
#ifdef __cplusplus
class LStream;
#endif

// ----------------------------------------------------------------------------------------
// StageKeystoneOptions determines the keystoning of the image on the stage

#define MAX_STAGE_KEYSTONE_OFFSETS		8

CLASSHDR StageKeystoneOptions
{
PUBLIC
#ifdef __cplusplus
				StageKeystoneOptions()
				{
					mKeystoneFlipHorz = FALSE;
					mKeystoneFlipVert = FALSE;
					for (UInt32 i=0; i<8; i++) {
						mKeystoneOffsets[i] = 0.0f;
					}
				}
	
	UInt32		Read(LStream* inStream, UInt32 inFileVersion);
	UInt32		Write(LStream* inStream);

#endif

	Boolean		mKeystoneFlipHorz;
	Boolean		mKeystoneFlipVert;
	float		mKeystoneOffsets[MAX_STAGE_KEYSTONE_OFFSETS];	// bottom/left, bottom/right, top/right, top/left
};

// ----------------------------------------------------------------------------------------
// StageEdgeBlendOptions determines the masking of the image on the stage

#define MAX_EDGE_BLEND_EDGES	4

#include "CCurveSpec.h"

CLASSHDR EdgeSpec
{
PUBLIC
#ifdef __cplusplus
				EdgeSpec()
				{
					mPos = 0.0f;
					mRotation = 0.0f;
					mWidth = 0.0f;
				}
	
	UInt32		Read(LStream* inStream, UInt32 inFileVersion);
	UInt32		Write(LStream* inStream);
#endif
	
	// edge position, rotation, width
	float		mPos;
	float		mRotation;
	float		mWidth;
	
	// edge curve specification
	CCurveSpec	mCurve;
};

CLASSHDR StageEdgeBlendOptions
{
PUBLIC
#ifdef __cplusplus
	
				StageEdgeBlendOptions()
				{
					mWidth = 1.0f;
					mHeight = 1.0f;
					mOffsetH = 0.0f;
					mOffsetV = 0.0f;
					
					for (UInt32 i=0; i<MAX_EDGE_BLEND_EDGES; i++) {
						mEdges[i].mPos = 0.0f;
						mEdges[i].mRotation = 0.0f;
						mEdges[i].mWidth = 0.0f;
					}
					
					mEdges[0].mCurve.Init(TRUE,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.7f, 1.0f);
					mEdges[1].mCurve.Init(TRUE,  0.0f, 1.0f, 1.0f, 0.0f, 0.3f, 1.0f, 1.0f, 0.3f);
					mEdges[2].mCurve.Init(FALSE, 0.0f, 1.0f, 1.0f, 0.0f, 0.3f, 1.0f, 1.0f, 0.3f);
					mEdges[3].mCurve.Init(FALSE, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.7f, 0.3f, 0.0f);
				}
	
	UInt32		Read(LStream* inStream, UInt32 inFileVersion);
	UInt32		Write(LStream* inStream);
#endif

	float		mWidth;				// width of mask, as percentage of stage (0.0 - 1.0)
	float		mHeight;			// height of mask, as percentage of stage (0.0 - 1.0)
	float		mOffsetH;			// horizontal center of mask, as percentage of stage (0.0 - 1.0)
	float		mOffsetV;			// vertical center of mask, as percentage of stage (0.0 - 1.0)
	
	EdgeSpec	mEdges[MAX_EDGE_BLEND_EDGES];
};

// ----------------------------------------------------------------------------------------
// GLRenderOptions sets sets options for how 3D objects will be rendered

struct GLRenderOptions
{
	Boolean	mZClipEnable;
	float	mZClipNear;
	float	mZClipFar;
};
typedef struct GLRenderOptions GLRenderOptions;
typedef const GLRenderOptions* ConstGLRenderOptionsPtr;

// ----------------------------------------------------------------------------------------
// ModelInfo provides the information necessary to render a 3D Model. This struct is passed
// to as the first parameter to the kMedia3DAttachModel command.
//
struct ModelInfo
{
	SInt32						mRenderChannelIndex1;	// positive = render to offscreen gl context
														// negative = render to stage (stage index = -mRenderChannelIndex1)
	SInt32						mRenderLayer;			// render layer controls order of model rendering
	Boolean						mUseLighting;			// if true -- use lighting when rendering the object
};
typedef struct ModelInfo ModelInfo;
typedef const ModelInfo* ModelInfoPtr;

// ----------------------------------------------------------------------------------------
// TextureMapInfo provides the information necessary to create an OpenGL texture map from
// an ImageBuffer.
//
// The caller creates the texture map by calling ExecuteMediaCommand_ with a command of
// kMedia3DCreateTextureMap. When calling this function, the user need only set
// mImageBuffer or mCoreImageBuffer (one or the other, but not both) and the mRepeat
// input -- Isadora will fill in the remaining fields upon successful
// complettion of the call.
// 
// From this point on, you can use the TextureMapInfo with the
// kMedia3DStoreTextureMapImage and/or kMedia3DPrepareTextureMapForRender commands
// to the ExecuteMediaCommand_. This allows you to bind the texture map to the object
// being rendered.
//
// Once the plugin is done with the texture map, it must destroy it by calling
// ExecuteMediaCommand_ with a command of kMedia3DDisposeTextureMap
// 
struct TextureMapInfo
{
	// filled in by isadora
	UInt32						mRenderChannelIndex0;		// zero-based open gl render channel or stage index
	Boolean						mGLTextureAttached;
	void*						mGLTexture;
	int							mTextureName;
	UInt32						mTextureMappingMode;
	Boolean						mTexMapCoordsArePixels;
	
	// the remaining fields must be filled in by the user before
	// using any of the texture map commands passed to ExecuteMediaCommand_

	// only one of the image inputs members be filled in, not both!
	struct ImageBuffer*			mImageBuffer;
	struct CoreImageBuffer*		mCoreImageBuffer;

	Boolean						mRepeat;			// if true, use GL_REPEAT for GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T
													// if false, use GL_CLAMP
};
typedef struct TextureMapInfo TextureMapInfo;
typedef const TextureMapInfo* TextureMapInfoPtr;

// ----------------------------------------------------------------------------------------
// LightingInfo - Used to specify the global lighting for a specific stage or render channel.
//
// The caller sets the lighting informat map by calling ExecuteMediaCommand_ with a command of
// kMedia3DLightPositionAndColors. 
//
struct LightingInfo
{
	CFloatXYZ		mRotation;
	float			mDistance;
	UInt32			mAmbientColor;
	UInt32			mDiffuseColor;
	UInt32			mSpecularColor;
};
typedef struct LightingInfo LightingInfo;
typedef const LightingInfo* LightingInfoPtr;

// ----------------------------------------------------------------------------------------
// MediaCode defines the media that can be accessed via
// the QueueMediaCommand_, QueueMediaCommandReplace_
// and ExecuteMediaCommand_ callback functions.
// 
// The first four entries match the four media types that
// can be added into the Media Window. The remainder are
// other kinds of "media" that may be controlled, e.g., a
// DV Device.
enum
{
	kMediaTypeNone = 0,		// invalid media type
	kMediaTypeMovie,		// a quicktime movie
	kMediaTypeAudio,		// an AIFF file
	kMediaTypeMidi,			// a standard midi file
	kMediaTypePicture,		// a picture,
	
	kMediaTypeDVControl,	// an external DV camera

	kMediaType3D,			// a 3D object
	kMediaType3DRenderer	// a 3D renderer
};
typedef	UInt16				MediaCode;

// ----------------------------------------------------------------------------------------
// external dv camera sub commands
enum {
	kDVSubCommandPlay,							
	kDVSubCommandPause,
	kDVSubCommandStop,
	kDVSubCommandLocate
};

// ----------------------------------------------------------------------------------------
// Constant passed to an actor's DrawActorDefinedArea proc
// to indicate which part to draw.
enum
{
	kActorDefinedAreaTop		= 1,
	kActorDefinedAreaBottom		= 2
};
typedef	UInt16				ActorDefinedAreaPart;

////////////////////////////////////////////////////////////////////////////
// CONTROL/ACTOR DEFINED AREA MOUSE MESSAGES DEFINES
////////////////////////////////////////////////////////////////////////////
// These messages are passed to the mouse message proc of either a control
// or of an actor with an interface element in the Actor Defined Area.
//
// To receive these messages in a Control, you must set the
// mControlMouseMessageProc member of the ControlInfo param when the
// Control's GetControlInfo function is called.
//
// To receive these messages in an actor, you must set the
// mMouseTrackInActorDefinedAreaProc member of the ActorInfo
// param when the Actor's GetActorInfo function is called.
//

enum {
	kIzzyMouseEnter,	// mouse has entered the control/actor defined area
	kIzzyMouseMove,		// mouse is moving within the control/actor defined area
	kIzzyMouseLeave,	// mouse is leaving the control/actor defined area
	
	kIzzyMouseDown,		// when mouse is clicked in the control/actor defined area
	kIzzyMouseTrack,	// after click in control/actor defined area, reports movements of the mouse
	kIzzyMouseUp,		// after click in control/actor defined area, reports mouse up
	
	kIzzyDoubleClick,	// double click (sent upon mouse down after down/track/up)
	
	kIzzyMouseHit		// mouse function should return true if mouse point is within user area
};
typedef	UInt8	IzzyMouseMessage;
	
////////////////////////////////////////////////////////////////////////////
// ACTOR MESSAGE DEFINES
////////////////////////////////////////////////////////////////////////////

enum {
	kIzzyActorHandleUndo,
	kIzzyActorHandleRedo,
	kIzzyActorDisposeUndoRedo
};
typedef	UInt32	IzzyActorMessage;
	
////////////////////////////////////////////////////////////////////////////
// CONTROL MESSAGE DEFINES
////////////////////////////////////////////////////////////////////////////

enum {
	kIzzyControlHandleUndo,
	kIzzyControlHandleRedo,
	kIzzyControlDisposeUndoRedo,
	kIzzyControlMediaListChanged
};
typedef	UInt32	IzzyControlMessage;
	
////////////////////////////////////////////////////////////////////////////
// CAPTURE MODE DEFINES
////////////////////////////////////////////////////////////////////////////

typedef	UInt8	CaptureModeFlags;

enum {
	kIzzyCaptureNone = 0,
	kIzzyCaptureVideo = 1,
	kIzzyCaptureAudio = 2
};

typedef	UInt8	VideoCaptureInputType;
enum {
	kIzzyVidCaptureDefaultInput,
	kIzzyVidCaptureComposite,
	kIzzyVidCaptureSVideo,
	kIzzyVidCaptureComponent,
	kIzzyVidCaptureTV
};

typedef	UInt8	AudioCaptureInputType;
enum {
	kIzzyAudCaptureDefaultInput,
	kIzzyAudCaptureLineIn,
	kIzzyAudCaptureInternalMicrophone,
	kIzzyAudCaptureDeviceInput1,
	kIzzyAudCaptureDeviceInput2,
	kIzzyAudCaptureDeviceInput3,
	kIzzyAudCaptureDeviceInput4,
	kIzzyAudCaptureDeviceInput5,
	kIzzyAudCaptureDeviceInput6
};

// MFC 10/28/07: changed type to UInt16 since negative values are invalid
typedef	UInt16	CaptureChannelIndex;

// MFC 10/28/07: changed type to UInt16 since negative values are invalid
typedef UInt16	VideoMediaIndex;
	
// ---------------------------------------------------------------
// User Image Processing Preferences
// 
// The user sets these preferences to determine what to do when
// two or more images are being combined in some way (i.e.,
// crossfades, keying, etc.)
//
// kImageProcessLargerSlower indicates that the user prefers that you
// scale the smaller image up to match the largest one, thus preserving
// quality at the expense of speed
//
// kImageProcessSmallerFaster indicates that the user wishes to scale
// larger images down to match the smallest image size, thus reducing
// quality but increasing processing speed
//
// kImageProcessFixedSize indicates that the user has set a fixed
// height and width at which all images should be processed. If this
// option is chosen, video output for an actor should always be scaled
// to the user specified size.
//
// kImageProcessMatchFormatOnly indicates that an intermediary buffer
// should only be created if the formats don't match -- but the images
// will never be scaled
//
enum {
	kImageProcessLargerSlower,					// scale the smaller up to match the larger
	kImageProcessSmallerFaster,					// scale the larger down to match the smaller
	kImageProcessFixedSize,						// process at user specified fixed size
	kImageProcessUseDefault,					// parameter passed to IsIntermediaryImageBufferRequired/CreateComplimentaryImageBuffer
												// to indicate that default should be used
	kImageProcessMatchFormatOnly				// created buffer to match formats, but don't scale
};
typedef SInt16	ImageProcessingMode;

#define	kMinimumImageProcessingDepthRGB		32
#define	kMinimumImageProcessingDepthYUV		16

struct ImageProcessingPrefs
{
	ImageProcessingMode			mImgProcMode;			// see Image Processing Preferences above...
	SInt16						mFixedSizeWidth;		// internal video processing width if mImgProcPref == kImageProcessFixedSize
	SInt16						mFixedSizeHeight;		// internal video processing height if mImgProcPref == kImageProcessFixedSize
	SInt16						mFixedBitDepth;			// internal video processing height if mImgProcPref == kImageProcessFixedSize
};
typedef struct ImageProcessingPrefs ImageProcessingPrefs;

#endif	// #ifndef _H_IsadoraTypes
