// ===========================================================================
//	IsadoraCallbacks			 Â(C)1999 Mark F. Coniglio. All rights reserved.
// ===========================================================================
//
//	Defines callback functions useful to Isadora plugins

#ifndef _H_IsadoraCallbacks
#define	_H_IsadoraCallbacks

// ---------------------------------------------------------------
// Inline Versions of QuickTime Functions

#include "GraphicsHelpers.h"

// #include "_tchar.h"

// ----------------------------------------------------------------------------
// PLUGIN DEBUGGING
//
// After this, NDEBUG, DEBUG, _DEBUG, ISADORA_DEBUG, and ISADORA_NDEBUG, are 
// all consistent with eachother and defined as such:
//
//                       Debug     Release
//   NDEBUG                  0           1
//   DEBUG                   1           0
//   _DEBUG            defined   undefined
//   ISADORA_DEBUG     defined   undefined
//   ISADORA_NDEBUG  undefined     defined
//
// Be careful not to use #ifdef with DEBUG or NDEBUG, as they are still 
// "defined" even when they are defined as 0. If you just stick with #if 
// everywhere you'll be fine.
//
// IMPORTANT: This is the same logic as the internal DebugFlags.h (not a file 
// in the plugin SDK). If this logic is updated here, it should be changed in 
// that file as well!

#if defined(_DEBUG) || DEBUG || defined(ISADORA_DEBUG)
#  undef NDEBUG
#  define NDEBUG 0
#else
#  if !(NDEBUG || defined(ISADORA_NDEBUG))
#    pragma message ("Warning: NDEBUG, DEBUG, _DEBUG, ISADORA_DEBUG, and ISADORA_NDEBUG not set -- assuming NDEBUG.")
#  endif
#  undef NDEBUG
#  define NDEBUG 1
#endif

#undef DEBUG
#define DEBUG (!NDEBUG)

// These are either defined or undefined, not 0 or 1.
#undef _DEBUG
#undef ISADORA_DEBUG
#undef ISADORA_NDEBUG
#if NDEBUG
#  define ISADORA_NDEBUG
#else
#  define _DEBUG
#  define ISADORA_DEBUG
#endif

//-----------------------------------------------------------------------------

#include "IsadoraTypes.h"
#include "ImageBufferCommon.h"
#include "TextBufferCommon.h"
#include "MessageReceiverCommon.h"
#include "CaptureConstants.h"
#include "ColorRefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ValueCommon.h"

#ifdef __cplusplus
class UGraphics;
#else
struct UGraphics;
#endif

/*

--------------------------
PROPERTY DEFINITION STRING
--------------------------

The GetActorParameterStringProc function is called to obtain the parameter 
string used to build the GUI representation of the actor and its properties. 

It is given as one long c-string, with carriage returns ('\r') to the mark
the end of lines. Each line has the following format, with capitalized
items being chosen from a list of possible constants.

PROPTYPE namestr id TYPE DISPLAYFMT minval maxval initval

(Note that the parser for the parameter does not care about case, so
'INT' is the same as 'int', etc.)

A description of each parameter follows.

Item		Description
----------- -------------------------------------------------------------------
PROPTYPE	is either INPROP or OUTPROP to indicate the type of
			property. You should list all of the input properties
			first, followed by all of the output properties

namestr		is the name of the property that will be displayed to the user.
			Note that since the name must be one continuous string, you must use
			underlines (_) to specify spaces. Thus "my_input" is shown as
			"my input" to the user

id			an identifier, unique to this actor, up to four characters long. This
			ID allows Isadora to identify a property in subsequent versions
			of your plugin, even if you change the order of inputs or outputs.
			If you delete a property, then you should retire its ID so that
			if a user attempts to load a file that was created with the older
			version of your plugin, it will know that it should remove the
			deleted property from the earlier version of the file.

TYPE		indicates the property type. it should be one of

			BOOL	= a boolean, 0 or 1 (off or on) value
			INT		= a 32 bit signed integer
			FLOAT	= a 32 bit floating point number
			RANGE	= two 16 bit signed integers indicating a range of values
			STRING	= a pointer to a String struct, which holds a pointer
					  to a c-string and a use count.
			DATA	= private data defined by the user. in this case, the
					  value must point to a block of data that begins with
					  DataValueInfo structure (see IsadoraTypes.h.) The first
					  member of that structure is an OSType that indicates the
					  type of data contained in the block of memory.

DISPLAYFMT	Indicates the one  or more possible formats that can be used to display
			the property value. It can be one or more of the following possibilities,
			with each separated by commas. The first item in the list of display
			formats will be the default when creating a new object.

			MUTEABLE	= Value will adapt to match type of input
			NUMBER		= Display as an signed integer
			ONOFF		= Displays as "on" or "off" for non-zero and zero values, respectively
			PITCH		= Displays value as a MIDI pitch, from C-2 to G8
			CHAR		= Displays value as a character surrounded by single quotes,
						  i.e., decimal 65 would be shown as 'A'
			SECONDS		= Displays value in seconds, with the actual value being an
						  an integer or float that represents milliseconds
			MILLISECS	= Displays value in milliseconds, with the actual value being
						  an integer or float that represents milliseconds
			HERTZ		= Displays value in hertz, with the actual value being
						  an integer or float that represents milliseconds.
			TIMECODE	= Displays a timecode value for position in a movie or 
						  other piece of media - it is up to the actor to figure
						  out how to determine this value.
			TEXT		= Valid only when TYPE is STRING, will display the value as a string
			FONT		= Valid only when TYPE is STRING, displays value as a string, and will
						  allow user to choose a font when they click on the input property
						  value edit box.
			TRIG		= Special display for types of BOOL. With this option, the value 
						  is normally displayed as a dash (-) to indicate that nothing is
						  happening. When a non-zero value is received, the display
						  changes momentarily to an 'X', returning to the dash after
						  100 milliseconds of inactivity.
			PROPDEF		= The actor will create the display string. If this option
						  is chosen, then the actor should define the 
						  PropertyValueToString function to convert from an numeric
						  value to a string that Isadora can display.

minval		the property's minimum allowable value. Use an asterisk (*) to
			indicate the default lowest possible minimum value for the
			property's type, i.e., for an INT the * here would mean -(2^32).
			The minimum and maximum value for a STRING are ignored.

maxval		the property's maximum allowable value. Use an asterisk (*) to
			indicate the default highest possible minimum value for the
			property's type, i.e., for an INT an * here would mean (2^32-1).
			The minimum and maximum value for a STRING are ignored.

initval		the default value for the property when creating a new actor. This
			value can be text for STRING property types. Note however, that if you
			initialize the value with a string, it cannot contain a space.

An example string might be

const char* MyDefintionString = 
	"INPROP		trigger		trig	bool		trig				0		1		0\r"
	"INPROP		frequency	f		int			hertz				0		100		0\r"
	"OUTPROP	output		oval	float		number				0		100		0\r"
	"OUTPROP	end_trigger	otrg	bool		trig				0		1		0\r";
*/

////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////
#define	FloatToRawLong(floatValue)			(*((SInt32*) &floatValue))			// returns a float as a long integer without conversion
#define FloatFromRawLong(floatValueAsLong)	(*((float*) &floatValueAsLong))		// returns a float that was stored in a long integer

////////////////////////////////////////////////////////////////////////////
// TYPES
////////////////////////////////////////////////////////////////////////////
// Actor Defined Area Types
typedef	UInt16				ActorAreaDrawFlagsT;
#define	kDrawAllActorAreas	0xFFFF

////////////////////////////////////////////////////////////////////////////
// ACTOR FUNCTIONS
////////////////////////////////////////////////////////////////////////////
// Below you will find typedefs for the functions that Isadora will call
// inside of your actor. Some of them are required, others are optional.
// Descriptions of what your actor should do when these actors are called
// are listed below.

	// GetActorInfoProc (Required)
	// 
	// When your actor's GetActorInfoProc is called, it must fill in the
	// ActorInfo structures with the calls that the actor supports.

typedef	void		(*GetActorInfoProc)(
						const void*					inGetActorInfoParam,
						struct ActorInfo*			outActorParams);

	// GetActorParameterStringProc (Required)
	//
	// The GetActorParameterStringProc function is called to obtain the parameter 
	// string used to build the GUI representation of the actor and its properties. 
	// See "Property Definition String" above for more information

typedef	const char*	(*GetActorParameterStringProc)(
						struct IsadoraParameters*	inIzzyParams,
						struct ActorInfo*			outActorParams);
	
	// CreateActorProc (Required)
	//
	// Create a new instance of the actor. Do not allocate or
	// load media resources required to execute the actor (that should be 
	// handled by the ActivateActorProc.) Instead, preallocate memory 
	// for your actor, initialize its internal values and setup
	// the ActorInfo struct with all of the required information.
typedef	void	(*CreateActorProc)(
					struct IsadoraParameters*	inIzzyParams,		
					struct ActorInfo*			outActorParams);
	
	// DisposeActorProc (Required)
	//
	// Dispose an existing instance of the actor created by
	// the CreateActorProc.
typedef	void	(*DisposeActorProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams);

	// ExecuteActorProc (Optional)
	//
	// This function allows your actor to specify which
	// inputs will receive an Execute() when a specific
	// output receives its execute message.
	//
	// The default behavior is that, when an actor receives
	// an execute message, the outputs of all of actors that
	// are linked to the inputs of this actor receive an Execute()
	// message. Your actor should override this behavior if 
	// executing some of its outputs should execute specific inputs.
	//
	// An input would be the Selector actor, which routes
	// one input of many inputs to a single output. In this spepcific
	// case, this function would set the outActualActivatedInputCount
	// to 3, returning the first two inptus (the 'select' input and the
	// 'inputs' input, which should always be active) and the one-based
	// index of the selected input, i.e., the one input that should
	// should receive an Execute() message based on the current setting
	// of the 'select' input.
	//
	// The inMaxActivatedInputCount indicates the size of the
	// array pointed to by outArrayOfActivatedInputIndexes. It
	// will always be set to the number of inputs on your actor, so
	// there should be no reason to exeed this value.
	//
	// Your function should set the outActualActivatedInputCount
	// value to the number of input indexes in the outArrayOfActivatedInputIndexes
	// array, and then set the elements of that array to the 
	// one-based index of the inputs that should receive activation
	// messages.
typedef	bool	(*ExecuteActorOutputProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorInfo,
					const IzzyTime*				inIzzyTime,		
					UInt32						inOutputIndex1,
					UInt32						inMaxActivatedInputCount,
					UInt32*						outActualActivatedInputCount,
					UInt32*						outArrayOfActivatedInputIndexes);
	
	// HandleIzzyStatusMessageProc (Optional)
	//
	// This function allows an actor to be notified in certain
	// changes of status. Primarily this is used to be notified
	// of the activation of a scene, but you may also receive
	// notifications of changes to the Media List (i.e., the 
	// Media Window) and other messages. See IzzyStatusMessage
	// in IsadoraTypes.h for more details on the messsages you
	// can receive

typedef	void	(*HandleIzzyStatusMessageProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorInfo,
					IzzyStatusMessage			inIzzyStatusMessage,
					const void*					inData);

	// set this define to 1 to have plugins use the IzzyStatusMessage mechanism
	#define USE_IZZY_STATUS_MESSAGE			1
	
	
	// PrepareActorForFirstUseProc (Optional)
	//
	// This function is called the _first_ time that the user
	// grabs the actor from the toolbox and drags it into the
	// Scene Editor. This is called immediately after the 
	// CreateActorProc, once Isadora's internal actor
	// information is initialized. You can feel free to call
	// any of Isadora's callback functions that reference
	// the actor or its properties.
	
typedef	void	(*PrepareActorForFirstUseProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams);

	// ActivateActorProc (Optional)
	//
	// When the ActivateActorProc is called with the inActivate
	// flag set to true, your actor is becoming active. It should
	// allocate any memory and/or resources required to get the
	// actor to run. When inActivate is false, your actor is becoming
	// inactive - all memory and resources that were allocated
	// during the activate process should be released.
	// 
	// When the ActivateActorProc is called with the inActivate
	// flag set to false, your actor is being deactivated. It should
	// dispose of any memory or other resources that were allocated
	// when the actor was activated.
	
typedef	void	(*ActivateActorProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams,
					Boolean						inActivate);
	
	// GetActorHelpStringProc (Optional, but strongly recommended)
	//		
	// the user has clicked requested help for this actor or one of
	// its properties.
	//
	// If inPropertyType is kPropertyTypeInvalid then you should
	// return help for the whole actor. otherwise, examine the
	// values of inPropertyType and inPropertyIndex to determine
	// the property for which the user is requesting help.
	// Return the resulting help string in buffer pointed to
	// by the outParamterString buffer. (The size of this output
	// buffer is specified by the inMaxCharacters parameter.)

typedef	void	(*GetActorHelpStringProc)(
					struct IsadoraParameters*	inIzzyParams,		
					struct ActorInfo*			inActorParams,
					PropertyType				inPropertyType,
					PropertyIndex				inPropertyIndex,
					char*						outParamaterString,
					UInt32						inMaxCharacters);

	// GetActorDefinedAreaProc (Optional)
	//
	// When called, this function should return the size of
	// area that will be drawn by the actor.
	//
	// There are two areas you may define: an area to be placed 
	// between the input and output values (the top area) and/or
	// an area that will be added on to the bottom of the actor.
	// You return non-zero values for the parameters if you 
	// would like to draw your own interface element inside
	// of the actor. If you define an actor defined area, the
	// DrawActorDefinedAreaProc will be called when it is time
	// to draw your area.
	//
	// For the top area, you may specified the width you desire
	// (which will always be given to you) and a minimum height.
	// The top area will always be at least as big as the width
	// and minimum height, though it may be taller than that if
	// the actor has many inputs or outputs.
	// 
	// For the bottom area, you specify a height and mimimum
	// width. The height will be added on to the actor, below
	// the input and output values. It is preferred to use
	// the top area when possible, as it keeps the screen 
	// dimensions of the actor smaller when it is collapsed.
	// However, certain interface elements want to be wide,
	// (see the Movie Player Actor) so it is reasonable to 
	// use the bottom area in this case.
	//
typedef	Boolean	(*GetActorDefinedAreaProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams,
					SInt16*						outTopAreaWidth,
					SInt16*						outTopAreaMinHeight,
					SInt16*						outBotAreaHeight,
					SInt16*						outBotAreaMinWidth);

	// ActorResizedProc (Optional)
	//
	// This function is called if an actor is resized. This allows the
	// actor to adjust the size of its Actor Defined Area if needed.

typedef	void	(*ActorResizedProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams,
					SInt16						inWidthDelta,
					SInt16						inHeightDelta);

	// DrawActorDefinedAreaProc (Optional)
	//
	// Draw the user interface element defined by your actor.
	// 
	// The inActorDefinedAreaPart will be set either to kActorDefinedAreaTop
	// or kActorDefinedAreaBottom, indicating which area needs to be drawn.
	//
	// The inAreaDrawFlags is a bitfield used to indicate which part of the
	// actor should be drawn. When Isadora is processing an update event to
	// draw all actors in a window, this value will be 0xFFFF, all parts 
	// need to be drawn. Otherwise, it will be a value that you passed back
	// in the mDrawActorAreaFlags member of the ActorInfo struct. (See
	// Getting Your Actor Area To Draw, below).
	//
	// The inADAArea is the rectangle that defines the part to be drawn.
	//
	// The inUpdateArea is the portion of inADAArea that needs to be drawn
	// in response to an update event.
	//
	// The inDrawingContext pointer is not used at this time, but may be
	// used in future cross-platform versions of Isadora.
	//
	// Getting Your Actor Area To Draw:
	// --------------------------------
	// 
	// To get a portion of your actor area to draw, you set one or more
	// bits in the mDrawActorAreaFlags member of the ActorInfo struct. This
	// can be done at any time. On a periodic basis, Isadora scans all actors
	// that are currently visible to see if their mDrawActorAreaFlags have been
	// set to a non-zero value. When it finds one, your DrawActorDefinedAreaProc
	// will be called, with the inAreaDrawFlags set to the value you stored
	// in mDrawActorAreaFlags. The idea is that mDrawActorAreaFlags (and
	// inAreaDrawFlags) are a bitfield, with each bit indicating one part of
	// your user interface. The hope here is that by only drawing the user
	// interface element that needs to be drawn, the amount of processing 
	// time required to update your user interface is minimized.
	//
	// As an example, the Sound Player actor has both the progress/playback
	// bar at the bottom, and the volume/pan indicator at the top. Each of
	// these has their own bit to indicate which item needs to be drawn. When
	// the volume or panning changes, the bit for that UI element is set. When
	// the sound position or playback selection changes, then the other bit 
	// is set.

typedef	void	(*DrawActorDefinedAreaProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams,
					void*						inDrawingContext,
					ActorDefinedAreaPart		inActorDefinedAreaPart,
					ActorAreaDrawFlagsT			inAreaDrawFlags,
					Rect*						inADAArea,
					Rect*						inUpdateArea,
					Boolean						inSelected);

	// MouseTrackInActorDefinedAreaProc (Optional)
	//
	// The user has clicked in your user interface area. Respond
	// to the click as appropriate.
	//
	// See IzzyMouseMessage for a list of messages that can be
	// passed into this function
	
typedef	Boolean	(*MouseTrackInActorDefinedAreaProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams,
					IzzyMouseMessage			inMouseMessage,
					void*						inDrawingContext,
					Rect*						inArea,
					Point						inMouse);

	// HandlePropertyChangeTypeProc (Optional)
	//
	// The user would like to display one of your properties
	// in an alternate format. 
	
typedef void	(*HandlePropertyChangeTypeProc)(
					struct IsadoraParameters*			inIzzyParams,
					struct ActorInfo*					inActorParams,
					const PropertyDefinedValueSource*	inPropDefSrc, 			// CAN BE NULL
					PropertyType						inPropertyType,
					PropertyIndex						inPropertyIndex1,
					PropertyDispFormat					inFormats,
					PropertyDispFormat					inCurFormat,
					ValuePtr							inMinimum,
					ValuePtr							inMaximum,
					ValuePtr							inValue,
					ValuePtr							inScaleMin,
					ValuePtr							inScaleMax);

	// HandlePropertyConnectProc (Optional)
	//
	// One of your input properties, identified by inPropertyIndex1,
	// has been connected or disconnected from an output to which it was previously
	// connected. Handling this callback allows you to take action when
	// a connection is lost
	
typedef void	(*HandlePropertyConnectProc)(
					struct IsadoraParameters*	inIzzyParams,
					struct ActorInfo*			outActorParams,
					PropertyType				inPropertyType,
					PropertyIndex				inPropertyIndex1,
					Boolean						inConnect);

	// HandlePropertyChangeProc (Required)
	//
	// One of your input properties, identified by inPropertyIndex1,
	// has changed value. The current value of the property is the
	// new value, but both the old and new values are passed into
	// this function for your examination.
	
typedef void	(*HandlePropertyChangeProc)(
					struct IsadoraParameters*	inIzzyParams,
					struct ActorInfo*			outActorParams,
					PropertyIndex				inPropertyIndex1,
					ValuePtr					oldValue,
					ValuePtr					newValue,
					Boolean						initializing);

	// PropertyValueToStringProc (Optional)
	//
	// Your parameter string indicated that you would handle converting
	// the internal value of a properties into a text reprsentation by
	// specifying the "PROPDEF" as one of its types in the parameter string
	//
	// When PropertyValueToStringProc is called, examine the inPropertyType and
	// inPropertyIndex1 parameters to determine which parameter to convert.
	// The current value of that paramter is also provided. Output the resulting
	// text into the buffer pointed to by outString, which has a maximum
	// length of 256 characters.
	
typedef Boolean	(*PropertyValueToStringProc)(
					struct IsadoraParameters*	ip,
					struct ActorInfo*			inActorInfo,
					PropertyType				inPropertyType,
					PropertyIndex				inPropertyIndex1,
					const ValuePtr				inValue,
					char*						outString);
	
	// PropertyStringToValueProc (Optional)
	//
	// Your parameter string indicated that you would handle converting
	// the internal value of a properties into a text reprsentation by
	// specifying the "PROPDEF" as one of its types in the parameter string
	//
	// When PropertyStringToValueProc is called, examine the inPropertyType and
	// inPropertyIndex1 parameters to determine which parameter to convert.
	// The text that the user typed is in the buffer pointed to by inString.
	// Your job is to convert that text representation into a value, which
	// is returned in the Value struct pointed to by outValue.
	
typedef Boolean	(*PropertyStringToValueProc)(
					struct IsadoraParameters*	ip,
					struct ActorInfo*			inActorInfo,
					PropertyType				inPropertyType,
					PropertyIndex				inPropertyIndex1,
					const char*					inString,
					ValuePtr					outValue);


	// PropertyGetPopupMenuInfo (Optional)
	//
	// If your parameter string includes the "POPUP" type it 
	// indicates that both the PropertyGetPopupMenuInfo and the
	// PropertytPopupMenuSelect functions will be implemented.
	//
	// When PropertyGetPopupMenuInfo is called, the actor should provide
	// a list of text menu items separated by a carriage return character
	// i.e., ITEM1/rITEM2/rITEM3/r. The size of this bufer will be large,
	// the specific number of characters indicated by the inMenuItemsStringBufferSize
	// input parameter.
	//
	// Once this function returns, Isadora will show this menu to the 
	// user and allow them to make a selection. Subsequently, if the
	// user makes a choice from this menu, the PropertyPopupMenuSelect
	// function will be called, with the one-based index of the selection,
	// as well as the menu item text, allowing the actor to react to
	// the user selection
	
typedef void	(*PropertyGetPopupMenuInfo)(
					struct IsadoraParameters*	ip,
					struct ActorInfo*			inActorInfo,
					PropertyType				inPropertyType,
					PropertyIndex				inPropertyIndex1,
					char*						outMenuItemsString,
					UInt32						inMenuItemsStringBufferSize,
					UInt32*						outCurrentlySelectedItem);


	// PropertytPopupMenuSelect (Optional)
	//
	// If your parameter string includes the "POPUP" type it 
	// indicates that both the PropertyGetPopupMenuInfo and the
	// PropertytPopupMenuSelect functions will be implemented.
	//
	// If the user successfully chooses an item from the popup menu,
	// this function receives both the one-based index of the selected
	// item as well as the text shown in the popup menu.
	
typedef void	(*PropertytPopupMenuSelect)(
					struct IsadoraParameters*	ip,
					struct ActorInfo*			inActorInfo,
					PropertyType				inPropertyType,
					PropertyIndex				inPropertyIndex1,
					SInt32						inMenuIndex1,
					const char*					inSelectedItem);

	// SetActorPrivateDataProc (Optional)
	//
	// This actor is about to be activated - this function is called
	// with data that was saved the last time the actor's
	// GetActorPrivateDataProc function was called.
	
typedef	void	(*SetActorPrivateDataProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams,
					const void*					inActorData,
					UInt32						inDataDataLength);

	// GetActorPrivateDataLengthProc (Optional)
	//
	// Return the size of the private data that to be restored when
	// the actor is next loaded from disk.
	
typedef	UInt32	(*GetActorPrivateDataLengthProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams);

	// GetActorPrivateDataProc (Optional)
	//
	// Return the private data that to be restored when this actor 
	// is next loaded from disk.
	// 
	// Store the actor's private data into the memory location pointed
	// to by inActorDataMaxLen. Do not exceed the length specified by
	// inActorDataMaxLen. This function should return the actual length of
	// the data copied. (This shouldn't be different than the value returned
	// by the actor's GetActorPrivateDataLengthProc function, but it is there
	// for absolute certainty.)
	
typedef	UInt32	(*GetActorPrivateDataProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams,
					UInt32						inActorDataMaxLen,
					void*						outActorData);

	// ActorDoubleClickProc (Optional)
	//
	// Called when an actor has been double-clicked by the user
	// This function should return true if the double-click was
	// handled in some way.
	//
	// the function should set the outDirty flag if the actor "dirtied"
	// the document (i.e., changed data such a way that the document
	// needs to be saved) as a result of handling the double click.
	
typedef	Boolean	(*ActorDoubleClickProc)(
					struct IsadoraParameters*	inIzzyParams,			
					struct ActorInfo*			inActorParams,
					Boolean*					outDirty);


	// HandleActorMessage (Optional)
	//
	// Allows Isadora to send messages to the actor. Currently this
	// is only used to notify the actor that an Undo/Redo actions
	// after a actor has called SetActorUndoAction_(). If the
	// user specifies a user data parameter to that function, it
	// will be passed back to the caller via the inUserData parameter.
	// 
	// This list of IzzyActorMessage types is defined in IsadoraTypes.h
	// under the heading ACTOR MESSAGE DEFINES
	//
	// *NOTE*: when this function is called with the kIzzyActorDisposeUndoRedo
	// message, the plugin _must_ be able to dispose the data even when the
	// actor info (inActorParams) pointer is invalid
		
typedef	void		(*HandleActorMessageProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ActorInfo*			inActorParams,
						IzzyActorMessage			inActorMessage,
						void*						inUserData);
						
////////////////////////////////////////////////////////////////////////////
// Actor Flags
////////////////////////////////////////////////////////////////////////////
//
// Flags set in the mActorFlags member of the ActofInfo struct to indicate
// properties of this this actor. Actors who requires these flags should
// set them when they initializes the ActorInfo struct

enum {
	kActorFlags_IsVideoOutput	= 0x00000001,		// indicates this actor is a video output
	kActorFlags_IsDataOutput	= 0x00000002		// indicates this actor outputs data of some kind,
													// e.g., MIDI, serial, OSC, etc.
};

#define	kActorFlags_IsOuptut	(kActorFlags_IsVideoOutput | kActorFlags_IsDataOutput)

typedef UInt32	ActorFlags;

////////////////////////////////////////////////////////////////////////////
// Actor Info struct
////////////////////////////////////////////////////////////////////////////
//
// Members marked REQ must be filled in by your actor when its
// GetActorInfo function is called.
//
// Members marked OPT can be set to NULL if your actor does not
// need/support the specified function.
//
// Members marked ISADORA are filled in by Isadora and should not
// be changed.
	
struct ActorInfo {

	ActorClassIDT						mClass;								// REQ: actor class ID
	ActorClassIDT						mSubClass;							// OPT: actor sub-class within class
	ActorIDT							mID;								// REQ: actor unique ID
	UInt32								mCompatibleWithVersion;				// REQ: version of Isadora Callbacks for which this
																			//      this plugin was built
																			//      ms byte = major revision, ls byte = minor revision
	ActorFlags							mActorFlags;						// OPT: flags giving information about this actor; importantly if it is an output or not
	UInt32								mRequiredFeatureFlags;				// OPT: if this version requires special features, set those flags here
																			//      (e.g., CoreVideo; see FeatureFlags.h)
	const char*							mActorName;							// REQ: pointer to c-string with actor's name
	
	CONST_ Boolean						mActive;							// ISADORA: true if this actor is active now
	CONST_ ThingPtr						mThing;								// ISADORA: opaque ptr to the thing object that owns this actor
	CONST_ long							mThingOptions;						// ISADORA: options for this thing that owns actor
	CONST_ FSSpec						mPluginFileSpec;					// ISADORA: file spec for this plugin
	void*								mActorDataPtr;						// ISADORA: private data pointer
	void*								mPluginUserDefinedPtr;				// ISADORA: private
	MessageReceiverRef					mVideoTickMessageReceiver;			// ISADORA: video tick mesage receiver for this actor
	MessageReceiverRef					mIzzyTickMessageReceiver;			// ISADORA: isadora tick mesage receiver for this actor

	// CREATION/INITIALIZATION/DESTRUCTION
	GetActorParameterStringProc			mGetActorParameterStringProc;		// REQ: returns the parameter string used to create actor
	GetActorHelpStringProc				mGetActorHelpStringProc;			// OPT (but highly desireable): returns the help string for a particular property
	CreateActorProc						mCreateActorProc;					// REQ: called to create the actor
	DisposeActorProc					mDisposeActorProc;					// REQ: called to dispose the actor
	ActivateActorProc					mActivateActorProc;					// OPT: called to activate/deactivate the actor (MFC: made this optional on 01/24/08)
	ActivateActorProc					mNotifyPostActivateProc;			// OPT: called after all actors in a scene are activated or deactivated

	// PROPERTIES
	HandlePropertyChangeProc			mHandlePropertyChangeValueProc;		// REQ: called when a property changes value
	HandlePropertyChangeTypeProc		mHandlePropertyChangeTypeProc;		// OPT: called when a property changes type
	HandlePropertyConnectProc			mHandlePropertyConnectProc;			// OPT: called when an input is connected or disconnected
	PropertyValueToStringProc			mPropertyValueToStringProc;			// OPT: called to allow actor to convert a property's value to a string
	PropertyStringToValueProc			mPropertyStringToValueProc;			// OPT: called to allow actor to convert string into a property's value
	
	// ACTOR DEFINED AREA
	ActorAreaDrawFlagsT					mDrawActorAreaFlags;				// OPT: actor should set non-zero when its draw area needs updating
	GetActorDefinedAreaProc				mGetActorDefinedAreaProc;			// OPT: allows actor to define a area for display/interaction
	DrawActorDefinedAreaProc			mDrawActorDefinedAreaProc;			// OPT: called when actor defined area needs to be drawn
	MouseTrackInActorDefinedAreaProc	mMouseTrackInActorDefinedAreaProc;	// OPT: called when there is mouse activity inside actor defined area

	// ACTOR PRIVATE DATA
	SetActorPrivateDataProc				mSetActorPrivateDataProc;			// OPT: called prior to activating to restore private actor data
	GetActorPrivateDataLengthProc		mGetActorPrivateDataLengthProc;		// OPT: called prior to calling GetActorPrivateDataProc to get the length of the data
	GetActorPrivateDataProc				mGetActorPrivateDataProc;			// OPT: called after deactivating to store private actor data

	// MOUSE TRACKING
	ActorDoubleClickProc				mActorDoubleClickProc;				// OPT: called when an actor has been double-clicked by user
	PrepareActorForFirstUseProc			mPrepareActorForFirstUseProc;		// OPT: called the first time any actor is dragged from toolbox to the Scene Editor

	// DRAWING & SIZINGS
	ActorResizedProc					mActorResizedProc;					// OPT: called when actor has been resized
	
	// ACTOR EXECUTION
	ExecuteActorOutputProc				mExecuteActorOutputProc;			// OPT: called when actor receives an Execute() message

	// IZZY MESSAGE NOTIFICATION
	HandleIzzyStatusMessageProc			mHandleIzzyStatusMessageProc;		// OPT: called when an IzzyStatusMessage is available

	// ACTOR MESSAGE NOTIFICATION
	HandleActorMessageProc				mHandleActorMessageProc;			// OPT: called when an IzzyActorMessage is available
																			// *NOTE*: when this function is called with the kIzzyActorDisposeUndoRedo
																			// selector, the plugin must be able to dispose the data without the
																			// actor info pointer being valid

	// POPUP MENU HANDLERS
	PropertyGetPopupMenuInfo			mPropertyGetPopupMenuInfo;			// OPT: called if the user clicks in an input marked with the "POPUP" in the parameter string
	PropertytPopupMenuSelect			mPropertytPopupMenuSelect;			// OPT: called if the user chooses an item from a popup menu generated by mPropertyGetPopupMenuInfo
};
typedef struct ActorInfo ActorInfo;


////////////////////////////////////////////////////////////////////////////
// ISADORA CONTROLS FUNCTIONS
////////////////////////////////////////////////////////////////////////////
// Below you will find typedefs for the functions that Isadora will call
// inside of your control. Some of them are required, others are optional.
// Descriptions of what your control should do when these functions are 
// called are listed below.

	// GetControlInfoProc (Required)
	// 
	// When your control's GetControlInfoProc is called, it must fill in the
	// ControlInfo structure with the appropriate values. See the ControlInfo
	// definition below for details

typedef	void		(*GetControlInfoProc)(
						const void*				inGetActorInfoParam,
						struct ControlInfo*		outControlParams);

	// CreateControlProc (Required)
	//
	// Create a new instance of the control.
typedef	void		(*CreateControlProc)(
						struct IsadoraParameters*	inIzzyParams,		
						struct ControlInfo*			outControlParams);		// and array of availble control numbers
	
	// DisposeControlProc (Required)
	//
	// Dispose an existing instance of the control created by
	// the CreateControlProc.
typedef	void		(*DisposeControlProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams);

	// UpdateControllerNumbers (Required)
	//
	// Requests that the control update it's controller numbers
	// picking from the list provided
typedef	UInt32		(*UpdateControllerNumbersProc)(								// return number of controller numbers used
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						UInt32						inAvailControlNumberCount,	// count of avaialable control numbers
						const UInt32*				inAvailControlNumbers);		// and array of availble control numbers

	// GetIndexedControllerNumber (Required)
	//
	// This function allows Isadora to ascertain which controller
	// numbers your controller is using. If inIndex1 is 0, then
	// the function should return the count of controller numbers
	// used by your contro. When inIndex1 > 0, you should return
	// the controller number for that index.
	
typedef	IzzyCtlNum	(*GetIndexedControllerNumberProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						UInt32						inIndex1);				// set to 0 to get number of controllers, > 0 to get controller number
					
	// SetIndexedControllerNumber (Required)
	//
	// This function allows Isadora to set the controller
	// numbers your controller is using. When inIndex1 > 0,
	// you should set controller number for that index.
	
typedef	void	(*SetIndexedControllerNumberProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						UInt32						inIndex1,
						IzzyCtlNum					inNewCtlNumber);
					

	// GetIndexedControllerParamInfo (Optional)
	//
	// This function allows the host to get information about the
	// parameters of the control, i.e., associated with a particular
	// controller number
	
typedef	Boolean	(*GetIndexedControllerParamInfo)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						UInt32						inIndex1,
						ValueType*					outValueType,
						DataType*					outDataType);		// only valid if outValueType is kData
					
	// SetControllerFloatValueProc (Required)
	//
	// When a controller is linked to an Actor property, and the property
	// changes value, then this function is called to inform the control
	// of the new value
	//
	// The function should return true if the controller value actually
	// changed.
	// 

typedef UInt8	CtlUpdateCode;

// any of the SetControllerXXXValueProc functions should return
// one of these values
enum {
	kControlUpdateCode_DontUpdate = 0,		// no need to update
	kControlUpdateCode_DrawControl = 1,		// just draw the control -- don't update underlying layers (faster - use if possible)
	kControlUpdateCode_UpdateControl = 2	// invalidate the area under the control, and redraw using a full update of underlying layers
};
			

typedef	CtlUpdateCode	(*SetControllerFloatValueProc)(
							struct IsadoraParameters*	inIzzyParams,			
							struct ControlInfo*			inCtlInfo,
							IzzyCtlNum					inCtlNum,
							const Rect*					inCtlRect,
							float						inMin,
							float						inMax,
							float						inValue,
							Boolean						inDirect);
					
typedef	CtlUpdateCode	(*SetControllerDataValueProc)(
							struct IsadoraParameters*	inIzzyParams,			
							struct ControlInfo*			inCtlInfo,
							IzzyCtlNum					inCtlNum,
							const Rect*					inCtlRect,
							void*						inValue);
					
typedef	CtlUpdateCode	(*SetControllerStringValueProc)(
							struct IsadoraParameters*	inIzzyParams,			
							struct ControlInfo*			inCtlInfo,
							IzzyCtlNum					inCtlNum,
							const Rect*					inCtlRect,
							const String*				inValue);
					

	// ControlDrawProc (Required)
	//
	// Draw the user interface for your Control
typedef	void		(*ControlDrawProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inCtlInfo,
						const Rect*					inControlRect,
						const Rect*					inUpdateArea,
						const char*					inControlName,
						Boolean						inLocked,
						Boolean						inSelected);

	// ControlMouseMessageProc (Optional)
	//
	// The user has clicked in your user interface area. Respond
	// to the click as appropriate.
typedef	Boolean		(*ControlMouseMessageProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inCtlInfo,
						const Rect*					inControlRect,
						const char*					inControlName,
						Boolean						inSelected,
						IzzyMouseMessage			inMouseMessage,
						Point						inMouse,
						UInt32*						outSleepTime);
					
	// ControlEditOptionsProc (Optional)
	//
	// Supply this procedure if you have private options that the
	// user needs to edit via a dialog. It is this function's
	// responsibility to implement the user interface.
typedef	Boolean		(*ControlEditOptionsProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inCtlInfo,
						const Rect*					inControlRect,
						const char*					inControlName);
										
	// ControlEditMessageProc (Required)
	//
	// When values for this control are edited by the user
	// the new data is passed to the control via this
	// callback.

enum {
	kControlParamType_Int,
	kControlParamType_Float,
	kControlParamType_Text,
	kControlParamType_BinList,
	kControlParamType_TextBox
};
typedef UInt16	ControlParamType;

// structure definition for the data contained in a bin list
#define kBinListMaxUniqueIDs	256

struct IzzyBinList
{
	MediaCode	mMediaType;
	UInt16		mShowAllOfType;
	UInt32		mBinCount;
	UInt32		mUniqueIDs[kBinListMaxUniqueIDs];
};
typedef IzzyBinList		IzzyBinList;


typedef	Boolean		(*ControlSetParamValueProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inCtlInfo,
						Boolean						inTestOnly,				// test only, don't store: return true if value is OK
						OSType						inParamID,				// four char code - param ID
						ControlParamType			inParamType,			// param type: int, float, text
						const void*					inParamData,			// pointer to param data - determined by param type
						UInt32						inParamDataLen);		// length of param type
					
typedef	void		(*ControlGetParamValueProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inCtlInfo,
						OSType						inParamID,				// four char code - param ID
						ControlParamType*			outParamType,			// param type: int, float, text
						const void**				outParamData,			// pointer to param data - determined by param type
						UInt32*						outParamDataLen);		// length of param type

	// GetControlPrivateDataLenProc
	//
	// Called when the control data needs to be stored by Isadora
	//
	// When called, this function should return the number of bytes
	// required to store the Control's private data in a cross-platform
	// compatible way.
typedef	UInt32		(*GetControlPrivateDataLenProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams);		

	// GetControlPrivateDataProc
	//
	// Called when the control data needs to be stored by Isadora
	//
	// On entry, outDataBuffer points to a block of data, and 
	// inDataBufferSize gives the size of that buffer. It is the
	// responsibility of the Control to copy the data into the 
	// buffer in a platform independent manner (i.e., the same 
	// for both Mac and PC) and then return the size of the actual
	// data that was copied. Return 0 if the data could not be copied
	// for some reason.
	// 
typedef	UInt32		(*GetControlPrivateDataProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						void*						outDataBuffer,			// pointer to a buffer to hold the data
						UInt32						inDataBufferSize);		// size of the data buffer

	// SetControlPrivateDataProc
	//
	// Called when the control data is being restored by Isadora
	//
	// On entry, inDataBuffer points to a block of data, and 
	// inDataBufferSize gives the size of that buffer. Since the
	// data was fetched by Isadora using GetControlPrivateDataProc,
	// the data should be in a platform independent format. It is
	// the responsibility to make a copy of the buffer, convert it
	// to the native format for this machine, and use it to restore
	// its state.
typedef	void		(*SetControlPrivateDataProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						const void*					inDataBuffer,
						UInt32						inDataBufferSize);


	// ActivateDeactivateControlProc (Optional)
	//
	// Activate or deactivate the control
typedef	void		(*ActivateDeactivateControlProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						Boolean						inActivate);

	// ControlSetFrameSizeProc (Optional)
	//
	// This function is called whenever the control is moved or resized
typedef	void		(*ControlSetFrameSizeProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inCtlInfo,
						const Rect*					inControlRect);


	// GetIndexedMediaIndex (Optional)
	//
	// This function allows Isadora to ascertain which media indexes
	// your controller is using. If inIndex1 is 0, then
	// the function should return the count of media indexes
	// used by your control in outMediaIndex1. When inIndex1 > 0, you should return
	// the media type and media index for that index, as well
	// as a pointer to any private data you would like to use
	// when the SetIndexedMediaIndexProc function is called
	// to actually remap the data
	
typedef	Boolean		(*GetIndexedMediaIndexProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						UInt32						inIndex1,				// set to 0 to get number of controllers, > 0 to get controller number
						MediaCode*					outMediaType,
						UInt32*						outMediaIndex1,
						void**						outUserData);

	// SetIndexedMediaIndex (Optional)
	//
	// Allows the media index used by this control to be remapped to
	// another value
	
typedef	void		(*SetIndexedMediaIndexProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						UInt32						inIndex1,
						MediaCode					inMediaType,
						UInt32						inOldMediaIndex1,
						UInt32						inNewMediaIndex1,
						void*						inUserData);
					

	// HandleControlMessage (Optional)
	//
	// Allows Isadora to send messages to the control. Currently this
	// is only used to notify the control that an Undo/Redo actions
	// after a control has called SetControlUndoAction_(). If the
	// user specifies a user data parameter to that function, it
	// will be passed back to the caller via the inUserData parameter.
	// 
	// This list of IzzyControlMessage types is defined in IsadoraTypes.h
	// under the heading CONTROL MESSAGE DEFINES
		
typedef	void		(*HandleControlMessageProc)(
						struct IsadoraParameters*	inIzzyParams,			
						struct ControlInfo*			inControlParams,
						IzzyControlMessage			inControlMessage,
						void*						inUserData);
					

////////////////////////////////////////////////////////////////////////////
// Control Flags
////////////////////////////////////////////////////////////////////////////
//
// Flags set in the mControlFlags member of the ActofInfo struct to indicate
// properties of this this Control. Controls who requires these flags should
// set them when they initializes the ControlInfo struct

enum {
	kControlFlags_IsVideoOutput	= 0x00000001,		// indicates this control is a video output
	kControlFlags_IsDataOutput	= 0x00000002		// indicates this control outputs data of some kind
};

#define	kControlFlags_IsOuptut	(kControlFlags_IsVideoOutput | kControlFlags_IsDataOutput)

typedef UInt32	ControlFlags;

////////////////////////////////////////////////////////////////////////////
// Control Info struct
////////////////////////////////////////////////////////////////////////////
//
// Members marked REQ must be filled in by your actor when its
// GetControlInfo function is called.
//
// Members marked OPT can be set to zero or NULL if your actor does not
// need/support the specified function.
//
// Members marked ISADORA are filled in by Isadora and should not
// be changed.


struct ControlInfo {
	ControlIDT							mID;								// REQ:		actor unique ID
	UInt32								mCompatibleWithVersion;				// REQ:		version of Isadora Callbacks for which this this plugin was built
																			//     		ms byte = major revision, ls byte = minor revision
	ControlFlags						mControlFlags;						// OPT:		flags giving information about this control; importantly if it is an output or not

	CONST_ FSSpec						mPluginFileSpec;					// ISADORA:	file spec for this plugin
	CONST_ IzzyCtlPtr					mControl;							// ISADORA: thing that owns this actor
	
	SInt16								mDefaultWidth;						// OPT:		default control width when inserting new control - if set to 0, defaults to 32
	SInt16								mDefaultHeight;						// OPT: 	default control height when inserting new control - if set to 0, defaults to 32 
	
	const char*							mControlName;						// REQ:		pointer to c-string with control's name
	const char*							mEditDialogDefs;					// OPT:		pointer to string that defines the fields in your control's edit settings dialog
	void*								mControlDataPtr;					// OPT:		pointer to control's private data
	
	CreateControlProc					mCreateControlProc;					// REQ:		function called when control is to be created
	DisposeControlProc					mDisposeControlProc;				// REQ:		function called when control is to be disposed
	UpdateControllerNumbersProc			mUpdateControllerNumbersProc;		// REQ:		update controller numbers
	GetIndexedControllerParamInfo		mGetIndexedControllerParamInfo;		// OPT:		returns information about a specific parameter
	GetIndexedControllerNumberProc		mGetIndexedControllerNumberProc;	// REQ:		function called get number of/which controller numbers used by the control
	SetIndexedControllerNumberProc		mSetIndexedControllerNumberProc;	// REQ:		function called set controller numbers used by the control
	SetControllerFloatValueProc			mSetControllerFloatValueProc;		// REQ/OPT:	Required if this control can be linked to numeric properties and track their value
	SetControllerDataValueProc			mSetControllerDataValueProc;		// OPT/OPT:	Required if this control can be linked to data properties and track their value
	SetControllerStringValueProc		mSetControllerStringValueProc;		// OPT/OPT:	Required if this control can be linked to string properties and track their value
	ControlDrawProc						mControlDrawProc;					// REQ:		function called to draw your control
	ControlMouseMessageProc				mControlMouseMessageProc;			// OPT:		function called when mouse is clicked, moved, or released
	
	ControlGetParamValueProc			mControlGetParamValueProc;			// OPT:		function called when control params are edited
	ControlSetParamValueProc			mControlSetParamValueProc;			// OPT:		function called when control params are edited
	GetControlPrivateDataLenProc		mGetControlPrivateDataLenProc;		// OPT:		return the length of the control's private data
	GetControlPrivateDataProc			mGetControlPrivateDataProc;			// OPT:		get a pointer to the control's private data
	SetControlPrivateDataProc			mSetControlPrivateDataProc;			// OPT:		get a pointer to the control's private data
	
	ControlEditOptionsProc				mControlEditOptionsProc;			// OPT:		edit private options/settings
	
	ActivateDeactivateControlProc		mActivateDeactivateControlProc;		// OPT:		activate / deactivate the control

	ControlSetFrameSizeProc				mControlSetFrameSizeProc;			// OPT:		called when control moved or resized
	
	GetIndexedMediaIndexProc			mGetIndexedMediaIndexProc;			// OPT:		function called to get a number of/which media indexes used by this control
	SetIndexedMediaIndexProc			mSetIndexedMediaIndexProc;			// OPT:		function called to set a media indexes used by this control

	HandleControlMessageProc			mHandleControlMessageProc;			// OPT:		handles a message being sent to the control
	
	CONST_ Boolean						mActive;							// ISADORA: true if this control is active now
};
typedef struct ControlInfo ControlInfo;


////////////////////////////////////////////////////////////////////////////
// ISADORA CALLBACK FUNCTIONS
////////////////////////////////////////////////////////////////////////////

#define	kCurrentIsadoraCallbackVersion	0x0100		// version 1.0

// -----------------------------------------------------------------------------
// Callback Error Return Codes
// -----------------------------------------------------------------------------
enum {
	kIzzyNoError = 0,
	kIzzyErr_UnexpectedError,				// unexpected internal error
	kIzzyErr_InvalidParam,					// a parameter passed to a callback was invalid
	kIzzyErr_BadThingPtr,					// the thing ptr passed to a callback is invalid
	kIzzyErr_InvalidPropertyType,			// the property type passed to a callback is not kInputProperty or kOutputProperty
	kIzzyErr_PropertyIndexOutOfRange,		// the property index is not between 1 and the number of properties of the specified type
	kIzzyErr_ValueTypesDontMatch,			// when setting or changing min/max/value of a property, the types in the ValuePtrs don't all match
	kIzzyErr_PropertyTypeOrIndexInvalid,	// a bad property type code or index that is out of range
	kIzzyErr_InvalidJumpMode,				// caller passed an invalid jump mode
	kIzzyErr_SceneDoesntExist,				// scene in jump or get scene name call does not exist,
	kIzzyErr_ResourceNotFound,				// requested resource cannot be found
	kIzzyErr_SnapshotDoesntExist,			// requested snapshot cannot does not exist
	kIzzyErr_LastError
};
typedef	SInt16		IzzyError;			// the isadora error code type


enum {
	kIzzyPluginFailed_BadValue = 1
};
typedef	SInt16		PluginFailureError;	// the plugin error code type

void
ShowIsadoraPluginFailedAlert(
	ActorInfo*			inActorInfo,
	PluginFailureError	inError);


// All of the callbacks below come with a macro that allows you to call
// the callback without explicity using the indirection operator (->) to
// access the function. The first parameter to the macro is always the
// IsadoraParameters pointer that is passed to your actor.

// -----------------------------------------------------------------------------
// ShowAlert_
// -----------------------------------------------------------------------------

#define	ShowAlert_(izzyParamsPtr, str) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mShowAlertProc)( (str) )

typedef	void			(*ShowAlertProc)(
								const char*	inErrorString,
								const char*	inExplinationString);
					
// -----------------------------------------------------------------------------
// CreateMessageReceiver_
// -----------------------------------------------------------------------------
// Watchers typically need to be notified about events coming in from the real
// world. These include, mouse, keyboard, MIDI, video frames, etc. To be notified
// of these events, your actor will need to register a message receiver function
// using the CreateMessageReceiver_ macro.
// 
// The second parameter (msgReceiverFunc) to the macro is the address of your
// message receiver function.
// 
// The third parameter (desiredPort) is only valid for MIDI messages - it
// specifies which port (i.e., cable) the message must arrive on for your message
// receiver function to be notified. Other messages ignore this parameter.
// 
// The fourth parameter (messageMask) is a mask that defines which messages will
// be delivered to your message receiver function. For example, if you wanted to
// be notified whenever the user pressed a key on the computer keyboard, you
// would use kWantKeyDown. Since this parameter is a mask, you can use any
// combination of the mask flags to receive messages of the various types. See
// MessageReceiverCommon.h for all of the possible masks. Note that you may call
// CreateMessageReceiver_ with a mask of kWantNone to register your message
// receiver function but not receive any messages yet. You can then later call
// UpdateMessageReceiver or UpdateMessageReceiverMask to change the mask and
// start receiving messages.
// 
// The fifth parameter (refCon) is a reference constant that will be passed to
// your message receiver proc whenever it is called.
// 
// The result of this function is a MessageReceiverRef. You need to keep track of
// this value as it is used to dispose the message receiver and to update the
// MessageMask, if so desired.
// 
// INTERRUPTS: This can be called at interrupt time

// this version can be called by non-actor items -- the actor info pointer is set to NULL
#define	CreateMessageReceiver_(izzyParamsPtr, msgReceiverFunc, desiredPort, messageMask, refCon) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCreateMessageReceiverProc)( (msgReceiverFunc), (desiredPort), (messageMask), nil, nil, (refCon) )

// this version should be called by actors -- the actor info pointer is required
#define	CreateActorMessageReceiver_(izzyParamsPtr, inActorInfo, msgReceiverFunc, desiredPort, messageMask, refCon) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCreateMessageReceiverProc)( (msgReceiverFunc), (desiredPort), (messageMask), (inActorInfo), (nil), (refCon) )

// this version should be called by controls -- the control info pointer is required
#define	CreateControlMessageReceiver_(izzyParamsPtr, inControlInfo, msgReceiverFunc, desiredPort, messageMask, refCon) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCreateMessageReceiverProc)( (msgReceiverFunc), (desiredPort), (messageMask), (nil), (inControlInfo), (refCon) )

typedef	MessageReceiverRef	(*CreateMessageReceiverProc)(						// returns MessageReceiverRef opaque reference
								MessageReceiveFunction*	inReceiveFunction,		// address of receive function
								PortIndex				inDesiredPortIndex,		// for MIDI, desired input port index
								MessageMask				inMessageMask,			// mask specifies what messages, upon reception, will
																				// cause the receive function to be called
								ActorInfo*				inActorInfo,			// called by an actor, this should be the ActorInfo struct for this actor
																				// this parameter can be NULL if caller is not an actor
								ControlInfo*			inCtlInfo,				// called by aa control, this should be the ControlInfo struct for this control
																				// this parameter can be NULL if caller is not an actor
								SInt32					inRefCon);				// ref con passed to receive function
					

// -----------------------------------------------------------------------------
// DisposeMessageReceiver_
// -----------------------------------------------------------------------------
// Removes your message receiver fucntion from the Message Receiver list. This
// function should always be called to balance a call to the
// CreateMessageReceiver_ macro.
// 
// INTERRUPTS: This can be called at interrupt time

#define	DisposeMessageReceiver_(izzyParamsPtr, msgReceiver) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDisposeMessageReceiverProc)( (msgReceiver) )

typedef void				(*DisposeMessageReceiverProc)(						// disconnect a message receiver proc	
								MessageReceiverRef			inMessageReceiver);	// address of receive function
					

// -----------------------------------------------------------------------------
// UpdateMessageReceiver_
// -----------------------------------------------------------------------------
// If you want to change what messages will cause your message receiver fucntion
// to be called, then call the UpdateMessageReceiver macro to update the settings
// for your proc in the message receiver queue.
// 
// INTERRUPTS: This can be called at interrupt time

#define	UpdateMessageReceiver_(izzyParamsPtr, msgReceiver, msgRcvFunc, portIndex, msgMask, refCon) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mUpdateMessageReceiverProc)((msgReceiver), (msgRcvFunc), (portIndex), (msgMask), 0, 0, (refCon))

#define	UpdateActorMessageReceiver_(izzyParamsPtr, msgReceiver, inActorInfo, msgRcvFunc, portIndex, msgMask, refCon) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mUpdateMessageReceiverProc)((msgReceiver), (msgRcvFunc), (portIndex), (msgMask), (inActorInfo), 0, (refCon))

#define	UpdateControlMessageReceiver_(izzyParamsPtr, msgReceiver, inControlInfo, msgRcvFunc, portIndex, msgMask, refCon) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mUpdateMessageReceiverProc)((msgReceiver), (msgRcvFunc), (portIndex), (msgMask), 0, (inControlInfo), (refCon))

typedef	void				(*UpdateMessageReceiverProc)(						// update parameters of message receiver proc
								MessageReceiverRef		inMessageReceiver,		// message receiver opaque reference
								MessageReceiveFunction*	inReceiveFunction,		// address of receive function
								PortIndex				inDesiredPortIndex,		// for MIDI, desired input port index
								MessageMask				inMessageMask,			// mask specifies what messages will be received
								ActorInfo*				inActorInfo,			// called by an actor, this should be the ActorInfo struct for this actor
																				// this parameter can be NULL if caller is not an actor
								ControlInfo*			inCtlInfo,				// called by aa control, this should be the ControlInfo struct for this control
																				// this parameter can be NULL if caller is not an actor
								SInt32					inRefCon);				// ref con passed when receiver is called
					

// -----------------------------------------------------------------------------
// UpdateMessageReceiverMask_
// -----------------------------------------------------------------------------
// Allows you to change what messages will cause your message receiver function
// be called by changing the MessageMask for your function.
// 
// INTERRUPTS: This can be called at interrupt time

#define	UpdateMessageReceiverMask_(izzyParamsPtr, msgReceiver, msgMask) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mUpdateMessageReceiverMaskProc)((msgReceiver), (msgMask))

typedef	void				(*UpdateMessageReceiverMaskProc)(					
								MessageReceiverRef		inMessageReceiver,		// message receiver opaque reference
								MessageMask				inMessageMask);			// mask specifies what messages will be received
					
// -----------------------------------------------------------------------------
// SetActorFlags_
// -----------------------------------------------------------------------------
// Allows the caller to set the actor flags, which determines if the actor
// is an output. Some modules may need to change these flags if they start
// outputing based on user input, or if they want to change from being
// a video output to a data output
// 
#define	SetActorFlags_(izzyParamsPtr, actorInfo, flags) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetActorFlagsProc)((actorInfo)->mThing, (flags))

typedef void				(*SetActorFlagsProc)(								
								ThingPtr					inThing,				// thing that owns the property
								ActorFlags					inActorFlags);			// the new flags setting


// -----------------------------------------------------------------------------
// ChangePropertyType_
// -----------------------------------------------------------------------------
// ChangePropertyType allows you to reconfigure the type of one of your input or
// output properties. This macro is usually called in response to your
// HandlePropertyChangeType function being called for a muteable input or output
// property.
// 
// You must supply the Property Type (kInputProperty or kOutputProperty), and the
// one-based property index to identify which property will be changed. You then
// supply the possible display formats and default display formats (see
// IsadoraTypes.h for the possible values) and the value's new minimum, maximum,
// and current value.
// 
// INTERRUPTS: This should not be called at interrupt time

#define	ChangePropertyType_(izzyParamsPtr, actorInfo, propDefSrc, propMode, propIndex1, dispFmts, curDispFmt, min, max, scaleMin, scaleMax, value) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mChangePropertyTypeProc)((actorInfo)->mThing, (propDefSrc), (propMode), (propIndex1), (dispFmts), (curDispFmt), (min), (max), (scaleMin), (scaleMax), (value))

#define	ChangePropertyTypeAndName_(izzyParamsPtr, actorInfo, propDefSrc, propType, propIndex1, name, dispFmts, curDispFmt, min, max, scaleMin, scaleMax, value) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mChangePropertyTypeAndNameProc)((actorInfo)->mThing, (propDefSrc), (propType), (propIndex1), (name), (dispFmts), (curDispFmt), (min), (max), (scaleMin), (scaleMax), (value))

typedef IzzyError			(*ChangePropertyTypeProc)(								
								ThingPtr							inThing,				// thing that owns the property
								const PropertyDefinedValueSource*	inPropDefSrc,			// CAN BE NULL
								PropertyType						inPropertyType,			// the property type
								PropertyIndex						inPropertyIndex1,		// index of the property
								PropertyDispFormat					inDisplayFormats,
								PropertyDispFormat					inCurFormat,
								const ValuePtr						inMin,
								const ValuePtr						inMax,
								const ValuePtr						inScaleMin,
								const ValuePtr						inScaleMax,
								const ValuePtr						inValue);


typedef IzzyError			(*ChangePropertyTypeAndNameProc)(								
								ThingPtr							inThing,				// thing that owns the property
								const PropertyDefinedValueSource*	inPropDefSrc,			// CAN BE NULL
								PropertyType						inPropertyType,			// the property type
								PropertyIndex						inPropertyIndex1,		// index of the property
								const char*							inName,					// NULL = no change
								PropertyDispFormat					inDisplayFormats,
								PropertyDispFormat					inCurFormat,
								const ValuePtr						inMin,
								const ValuePtr						inMax,
								const ValuePtr						inScaleMin,
								const ValuePtr						inScaleMax,
								const ValuePtr						inValue);


// -----------------------------------------------------------------------------
// CopyPropDefValueSource_
// -----------------------------------------------------------------------------
#define	CopyPropDefValueSource_(izzyParamsPtr, actorInfo, srcPropType, srcPropIndex, dstPropType, dstPropIndex) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCopyPropDefValueSourceProc)((actorInfo)->mThing, (srcPropType), (srcPropIndex), (dstPropType), (dstPropIndex))

typedef IzzyError			(*CopyPropDefValueSourceProc)(								
								ThingPtr					inThing,				// thing that owns the property
								PropertyType				inSrcPropType,			// the source property type
								PropertyIndex				inSrcPropertyIndex1,	// index of the source property
								PropertyType				inDstPropType,			// the destination property type
								PropertyIndex				inDstPropertyIndex1);	// index of the destination property

// -----------------------------------------------------------------------------
// GetPropertyDisplayFormats_
// -----------------------------------------------------------------------------
// Gets the available and current property display formats for a property
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	GetPropertyDisplayFormats_(izzyParamsPtr, actorInfo, propType, propIndex1, minValue, maxValue) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetPropertyDisplayFormatsProc)((actorInfo)->mThing, (propType), (propIndex1), (minValue), (maxValue))

typedef IzzyError			(*GetPropertyDisplayFormatsProc)(					// returns the value of the input property
								ThingPtr				inThing,				// thing that owns the property
								PropertyType			inPropertyType,			// the property type, either input or output
								SInt32					inPropertyIndex1,		// index of the input property
								PropertyDispFormat*		outAvailFormats,		// the property's available display formats
								PropertyDispFormat*		outCurFormat);			// the property's current display format

// -----------------------------------------------------------------------------
// GetPropertyID
// -----------------------------------------------------------------------------
// Gets the property ID for a property
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	GetPropertyDisplayFormats_(izzyParamsPtr, actorInfo, propType, propIndex1, minValue, maxValue) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetPropertyDisplayFormatsProc)((actorInfo)->mThing, (propType), (propIndex1), (minValue), (maxValue))

typedef IzzyError			(*GetPropertyDisplayFormatsProc)(					// returns the value of the input property
								ThingPtr				inThing,				// thing that owns the property
								PropertyType			inPropertyType,			// the property type, either input or output
								SInt32					inPropertyIndex1,		// index of the input property
								PropertyDispFormat*		outAvailFormats,		// the property's available display formats
								PropertyDispFormat*		outCurFormat);			// the property's current display format

// -----------------------------------------------------------------------------
// GetPropertyID_
// -----------------------------------------------------------------------------
// Gets the minimum and maximum value for the specified property
// 

#define	GetPropertyID_(izzyParamsPtr, actorInfo, propType, propIndex1, outPropID) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetPropertyIDProc)((actorInfo)->mThing, (propType), (propIndex1), (outPropID))

typedef IzzyError			(*GetPropertyIDProc)(								// returns the value of the input property
								ThingPtr				inThing,				// thing that owns the property
								PropertyType			inPropertyType,			// the property type, either input or output
								SInt32					inPropertyIndex1,		// index of the input property
								PropIDT*				outPropertyID);			// resulting property ID

// -----------------------------------------------------------------------------
// GetPropertyMinMax_
// -----------------------------------------------------------------------------
// Gets the minimum and maximum value for the specified property
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	GetPropertyMinMax_(izzyParamsPtr, actorInfo, propType, propIndex1, minValue, maxValue, initValue) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetPropertyMinMaxProc)((actorInfo)->mThing, (propType), (propIndex1), (minValue), (maxValue), (initValue))

typedef IzzyError			(*GetPropertyMinMaxProc)(							// returns the value of the input property
								ThingPtr				inThing,				// thing that owns the property
								PropertyType			inPropertyType,			// the property type, either input or output
								SInt32					inPropertyIndex1,		// index of the input property
								ValuePtr				outPropertyMinimum,		// the property's mimumum value
								ValuePtr				outPropertyMaximum,		// the property's maximum value
								ValuePtr				outPropertyInitValue);

// -----------------------------------------------------------------------------
// SetPropertyMinMax_
// -----------------------------------------------------------------------------
// Sets the minimum and maximum value for the specified property.
//
// *** SIDE EFFECTS ***
//
// If the value is not within the range specified by the min and max, then the
// value will be forced in range.
//
// The Scale To Minimum and Scale To Maximum are set to match the new Minimum
// and Maximum.
//
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	SetPropertyMinMax_(izzyParamsPtr, actorInfo, propType, propIndex1, minValue, maxValue, initValue) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetPropertyMinMaxProc)((actorInfo)->mThing, (propType), (propIndex1), (minValue), (maxValue), (initValue))

typedef IzzyError			(*SetPropertyMinMaxProc)(							// returns the value of the input property
								ThingPtr				inThing,				// thing that owns the property
								PropertyType			inPropertyType,			// the property type, either input or output
								SInt32					inPropertyIndex1,		// index of the input property
								const ValuePtr			inPropertyMinimum,		// the new mimumum value
								const ValuePtr			inPropertyMaximum,		// the new maximum value
								const ValuePtr			inPropertyInit);		// the new init value -- can be nil

// -----------------------------------------------------------------------------
// SmartSetPropertyMinMax_
// -----------------------------------------------------------------------------
// Has the same function as SetPropertyMinMax_, except that this version looks
// at the current minimum and maximum values in relation to the current scale
// min and scale max values. The idea is to preserve inverse relationships if
// they have specified them as such, or to warn them that the scale min and
// scale max values will be reset.
//
// 1) If the old Min == old Scale Min, and old Max = old Scale Max, then set
// Min and Scale Min to the new minimum, and Max and Scale Max to the new
// maximum.
// 
// 2) If the old Min == old Scale Max, and old Max = old Scale Min, then set
// Min and Scale _Max_ to the new minimum, and Max and Scale _Min_ to the new
// maximum.
//
// 3) Otherwise, let the user know that the value of the specified property
// has been reset, indicating that this was because of a change in the 
// property identified by inChangedPropType and inChangedPropIndex1.
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	SmartSetPropertyMinMax_(izzyParamsPtr, actorInfo, propType, propIndex1, minValue, maxValue, notifyOfChangedValue, changedPropType, changedPropIndex) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSmartSetPropertyMinMaxProc)((actorInfo)->mThing, (propType), (propIndex1), (minValue), (maxValue), (notifyOfChangedValue), (changedPropType), (changedPropIndex))

typedef IzzyError			(*SmartSetPropertyMinMaxProc)(						// returns the value of the input property
								ThingPtr				inThing,				// thing that owns the property
								PropertyType			inPropertyType,			// the property type, either input or output
								SInt32					inPropertyIndex1,		// index of the input property
								const ValuePtr			inPropertyMinimum,		// the new mimumum value
								const ValuePtr			inPropertyMaximum,		// the new maximum value
								Boolean					inNotifyOfLimitedValue,	// if true and the actual value of the property changes, notify the input via HandlePropertyChangeValue
								PropertyType			inChangedPropType,		// type of property that caused this change - used when warning the user in case 3
								SInt32					inChangedPropIndex1);	// index of property that caused this change - used when warning the user in case 3

// -----------------------------------------------------------------------------
// GetInputPropertyValue_
// -----------------------------------------------------------------------------
// Retrieves the value of one of your actor's input properties. The result is
// returned as a ValuePtr struct, which gives the value's type and actual value.
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	GetInputPropertyValue_(izzyParamsPtr, actorInfo, propIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetInputPropertyValueProc)((actorInfo)->mThing, (propIndex1))

typedef const ValuePtr		(*GetInputPropertyValueProc)(						// returns the value of the input property
								ThingPtr				inThing,				// thing that owns the property
								SInt32					inPropertyIndex1);		// index of the input property

// -----------------------------------------------------------------------------
// SetInputPropertyValue_
// -----------------------------------------------------------------------------
// Sets the value of one of your input properties. Note that the value will be
// limited to the property's minimum and maximum values.
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	SetInputPropertyValue_(izzyParamsPtr, actorInfo, propIndex1, value) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetInputPropertyValueProc)((actorInfo)->mThing, (propIndex1), (value))

typedef IzzyError			(*SetInputPropertyValueProc)(
								ThingPtr				inThing,				// thing that owns the property
								PropertyIndex			inPropertyIndex1,		// index of the input property
								const ValuePtr			inValue);				// new property value

// -----------------------------------------------------------------------------
// SetOutputPropertyValue_
// -----------------------------------------------------------------------------
// Sets the value of one of your output properties. Note that the value will be
// limited to the property's minimum and maximum values.
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	SetOutputPropertyValue_(izzyParamsPtr, actorInfo, propIndex1, value) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetOutputPropertyValueProc)((actorInfo)->mThing, (propIndex1), (value))

typedef IzzyError			(*SetOutputPropertyValueProc)(
								ThingPtr				inThing,				// thing that owns the property
								PropertyIndex			inPropertyIndex1,		// index of the output property
								const ValuePtr			inValue);				// new property value

// -----------------------------------------------------------------------------
// GetInputPropertyScaleToMinMax_
// -----------------------------------------------------------------------------
// Gets the minimum and maximum scale to value for the specified input property
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	GetInputPropertyScaleToMinMax_(izzyParamsPtr, actorInfo, propIndex1, minValue, maxValue) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetInputPropertyScaleToMinMaxProc)((actorInfo)->mThing, (propType), (propIndex1), (minValue), (maxValue))

typedef IzzyError			(*GetInputPropertyScaleToMinMaxProc)(				// returns the value of the input property
								ThingPtr				inThing,				// thing that owns the property
								SInt32					inPropertyIndex1,		// index of the input property
								ValuePtr				inPropertyMinimum,		// the property's mimumum value
								ValuePtr				inPropertyMaximum);		// the property's maximum value

// -----------------------------------------------------------------------------
// SetInputPropertyScaleToMinMax_
// -----------------------------------------------------------------------------
// Sets the scale to minimum and scale to maximum for the specified input
// property. Note that if the current input value of this property is not
// in range when this function is called, it will be forced into range
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	SetInputPropertyScaleToMinMax_(izzyParamsPtr, actorInfo, propIndex1, minValue, maxValue) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetInputPropertyScaleToMinMaxProc)((actorInfo)->mThing, (propIndex1), (minValue), (maxValue))

typedef IzzyError			(*SetInputPropertyScaleToMinMaxProc)(				// returns any error
								ThingPtr				inThing,				// thing that owns the property
								SInt32					inPropertyIndex1,		// index of the input property
								const ValuePtr			inPropertyMinimum,		// the new mimumum value
								const ValuePtr			inPropertyMaximum);		// the new maximum value

// -----------------------------------------------------------------------------
// SetOutputPropertyLimitMinMax_
// -----------------------------------------------------------------------------
// Sets the limit minimum and limit maximum for the specified output
// property. Note that if the current output value of this property is not
// in range when this function is called, it will be forced into range
/// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	SetOutputPropertyLimitMinMax_(izzyParamsPtr, actorInfo, propIndex1, minValue, maxValue) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetOutputPropertyLimitMinMaxProc)((actorInfo)->mThing, (propIndex1), (minValue), (maxValue))

typedef IzzyError			(*SetOutputPropertyLimitMinMaxProc)(					// returns any error
								ThingPtr				inThing,				// thing that owns the property
								SInt32					inPropertyIndex1,		// index of the input property
								const ValuePtr			inPropertyMinimum,		// the new mimumum value
								const ValuePtr			inPropertyMaximum);		// the new maximum value

// -----------------------------------------------------------------------------
// GetOutputPropertyValue_
// -----------------------------------------------------------------------------
// Retrieves the current value of one of your actor's input properties. The
// result is returned as a ValuePtr struct, which gives the value's type and
// actual value.
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	GetOutputPropertyValue_(izzyParamsPtr, actorInfo, propIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetOutputPropertyValueProc)((actorInfo)->mThing, (propIndex1))

typedef const ValuePtr		(*GetOutputPropertyValueProc)(						// returns the value of the output property
								ThingPtr				inThing,				// thing that owns the property
								SInt32					inPropertyIndex1);		// index of the input property


// -----------------------------------------------------------------------------
// GetOutputPropertyMinimumValue_
// -----------------------------------------------------------------------------
// Returns the minimum value for an output property as a ValuePtr struct.
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	GetOutputPropertyMinimumValue_(izzyParamsPtr, actorInfo, propIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetOutputPropertyMinimumValueProc)((actorInfo)->mThing, (propIndex1))

typedef const ValuePtr		(*GetOutputPropertyMinimumValueProc)(				// returns the minimum value of the output property
								ThingPtr				inThing,				// thing that owns the property
								SInt32					inPropertyIndex1);		// index of the input property

// -----------------------------------------------------------------------------
// GetOutputPropertyMaximumValue_
// -----------------------------------------------------------------------------
// Returns the maximum value for an output property as a ValuePtr struct.
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	GetOutputPropertyMaximumValue_(izzyParamsPtr, actorInfo, propIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetOutputPropertyMaximumValueProc)((actorInfo)->mThing, (propIndex1))

typedef const ValuePtr		(*GetOutputPropertyMaximumValueProc)(				// returns the minimum value of the output property
								ThingPtr				inThing,				// thing that owns the property
								SInt32					inPropertyIndex1);		// index of the input property

// -----------------------------------------------------------------------------
// QueueMediaCommand_
// -----------------------------------------------------------------------------
// Adds a command to the Media Command Queue. At the next system task time (i.e.,
// when Isadora is running not at interrupt time) all pending commands in the
// queue will be processed. This allows you to queue non-interrupt time friendly
// commands for later processing. The result is a 32-bit integer that uniquely
// identifies the command in the queue.
// 
// Parameters to the marco and their meanings are as follows.
// 
// izzyParamsPtr: pointer to the IsadoraParameter struct passed to your actor
// 
// actorInfo: a pointer to your actors ActorInfo struct.
// 
// completionProc: a procedure that will be called upon the completion of your
// request.
// 
// id: a unique ID that will identify the caller who made the request. If you
// wish, you may use the mThing member of the ActorInfo struct - this is
// guaranteed to be unique.
// 
// cmd: the media command. See IsadoraTypes.h for a list of all of the possible
// media commands.
// 
// type: the type of media that will be affected by this command. This should be
// one of kMediaTypeMovie, kMediaTypeAudio, or kMediaTypeMidi file. These three
// constants refer to the three sections of the Media Window into which the user
// imports these media.
// 
// num: the one-based index of the media that will be affected by this command,
// again, refering to the index listed in the Media Window for the specified
// media type.
// 
// p1 & p2: these are parameters that are specific to the media command being
// issued. The parameters and their meanings are documented in the list of Media
// Commands given in IsadoraTypes.h
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	QueueMediaCommand_(izzyParamsPtr, actorInfo, completionProc, id, cmd, type, num, p1, p2) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mQueueMediaCommandProc)((actorInfo)->mThing, (completionProc), false, (id), (cmd), (type), (num), (p1), (p2) )

typedef	UInt32				(*QueueMediaCommandProc)(
								ThingPtr					inThing,				// opaque thing reference
								MediaCommandCompleteProc	inActionCompleteProc,	// completion proc called upon completion of the command
								Boolean						inReplace,				// if true, replace existing commands in queue with matching command & identifier
								SInt32						inIdentifier,			// unique identifier of caller who issued the command
								MediaCommand				inMediaCommand,			// media command to execute
								MediaCode					inMediaType,			// media type
								UInt32						inMediaObjectNumber,	// media object number
								SInt32						inParam1,				// first parameter
								SInt32						inParam2);				// second parameter

// -----------------------------------------------------------------------------
// QueueMediaCommandReplace_
// -----------------------------------------------------------------------------
// Same as DoQueueMediaCommand except that if there is already a Media Command in
// the queue with the same command and Identifier as the one passed to this
// macro, then the old media command and its parameters are replaced by the new
// command.
// 
// This function is useful for plugins that may receive data at interrupt time
// but queue media commands that must wait for the next system task time to
// execute. In this situations, there will be instances where you will want to
// replace the older, existing command with the newer command.
// 
// For example, at interrupt time, you queue a command to start playing Movie 2.
// A subsequent event causes the plugin to play Movie 3 instead. By calling
// QueueMediaCommandReplace_, if the movie 2 had already started playing, then
// the command would already be removed from the queue and movie 3 would start
// playing as expected. If movie 2 had not yet started playing, then the command
// would be replaced and movie 3 would start playing instead of movie 2. If you
// didn't do this, Movie 2 would play for an instant and then Movie 3 would start
// playing right afterward. This would cause a visible glitch.
// 
// The other place that this is useful is to prevent the overflow of System Task
// time commands in the queue. For instance, a MIDI continuous controller is
// connected to the Play Start parameter of Movie. When the controller is moved,
// a dense stream of MIDI messages comes in. If you didn't use
// QueueMediaCommandReplace_, your plugin would queue a Media Command for each
// control change message received, possibly overflowing the queue.
// 
// INTERRUPT SAFE: This function can be called at interrupt time.

#define	QueueMediaCommandReplace_(izzyParamsPtr, actorInfo, completionProc, id, cmd, type, num, p1, p2) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mQueueMediaCommandProc)((actorInfo)->mThing, (completionProc), true, (id), (cmd), (type), (num), (p1), (p2) )

// -----------------------------------------------------------------------------
// ExecuteMediaCommand_
// -----------------------------------------------------------------------------
// Immediately executes the specified media command if possible. If Isadora is at
// System Task time, then the command will be executed immediately. If Isadora is
// at Interrupt Time, then the command will be executed if the specified media
// command can legally be processed at interrupt time. In both of these cases,
// the result of the function call will be non-zero to indicate success. If,
// however, you attempt to exceute a non-interrupt-friendly command at interrupt
// time, the result will be zero and the command will not be executed.
// 
// INTERRUPTS: see description immediately above.
// 
// See DoQueueMediaCommand above for a list of the parameters and their meanings.

#define	ExecuteMediaCommand_(izzyParamsPtr, actorInfo, id, cmd, type, num, p1, p2) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mExecuteMediaCommandProc)((actorInfo)->mThing, (id), (cmd), (type), (num), (p1), (p2) )

typedef	Boolean				(*ExecuteMediaCommandProc)(
								ThingPtr					inThing,				// opaque thing reference
								SInt32						inIdentifier,			// unique identifier of caller who issued the command
								MediaCommand				inMediaCommand,			// media command to execute
								MediaCode					inMediaType,			// media type
								UInt32						inMediaObjectNumber,	// media object number
								SInt32						inParam1,				// first parameter
								SInt32						inParam2);				// second parameter

// -----------------------------------------------------------------------------
// SendMidiMessage_
// -----------------------------------------------------------------------------
// Sends a midi message of arbitrary length to the specified MIDI output port
// 
// INTERRUPTS: This callback can be executed at interrupt time.

#define	SendMidiMessage_(izzyParamsPtr, inPortIndex1, len, data) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSendMidiMessageProc)((inPortIndex1), (len), (data))

typedef void				(*SendMidiMessageProc)(
								PortIndex					inPortIndex1,			// one-based port index - as shown in the MIDI setup window
								UInt8						inLen,					// length of the message
								const void*					inData);				// pointer to midi data to send

// -----------------------------------------------------------------------------
// TestMTCTimecodeTime
// -----------------------------------------------------------------------------
// Compares the specified timecode time (in HHMMSSFF format) to the current
// MTC time received by Isadora. Result is 0 if the two times are equal,
// < 0 if the specified time is less than the current time, or > 0 if the
// specified time is greater than the current MTC time.

#define	TestMTCTimecodeTime_(izzyParamsPtr, inPortIndex1, inHMSF) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mTestMTCTimecodeTimeProc)((inPortIndex1), (inHMSF))

typedef SInt32				(*TestMTCTimecodeTimeProc)(
								PortIndex					inPortIndex1,			// one-based port index - as shown in the MIDI setup window
								UInt32						inTestTimeHMSF);

// -----------------------------------------------------------------------------
// AllNotesOff_
// -----------------------------------------------------------------------------
// Turns off all notes that are currently sounding. All MIDI messages sent
// using the SendMidiMessage_ function are logged. Note ons and note offs are
// kept track of, so that Isadora knows how many sound notes there are at any
// given time. When this command is executed, all of the sounding notes are
// sent a corresponding note off message.
// 
// INTERRUPTS: This callback can be executed at interrupt time.

#define	AllNotesOff_(izzyParamsPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mAllNotesOffProc)()

typedef void				(*AllNotesOffProc)();				// pointer to midi data to send

// -----------------------------------------------------------------------------
// CopyImageBufferToStage_
// -----------------------------------------------------------------------------
// Copies an Image Buffer to the stage. The destination rect specifies the
// location and dimensions of where the to which the contents of the image buffer
// will be copied. The dimensions of this rectangle can be different than the
// dimensions of the image buffer source image. Optionally, you may also specify
// a region that will be erased to the background color of the stage after the
// copy is complete.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

/*
enum {
	kCopyToStageCopyBits,					// use CopyBits to copy to stage
	kCopyToStageDirectFullScreenOnly		// use direct blitting if full stage
};
typedef SInt16	CopyToStageMode;

typedef struct {
	Rect		mDstRect;		// destination rect before zoom
	float		mZoom;			// zoom factor -- from 0 to 100%
	RgnHandle	mUpdateRgn;		// if not nil, this should be a valid RgnHandle.
								// In the latter case, erase the specified area
								// to black after copying the image buf to the
								// stage
} CopyToStageInfo;

#define	CopyImageBufferToStage_(izzyParamsPtr, actorInfo, inBlitMode, destStageIndex, imgBuffer, inCopyToStageInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCopyImageBufferToStageProc)((actorInfo)->mThing, (inBlitMode), (destStageIndex), (imgBuffer), (inCopyToStageInfo))

typedef	void				(*CopyImageBufferToStageProc)(
								ThingPtr					inThing,				// opaque thing reference
								CopyToStageMode				inBlitMode,				// if true, blit directly to stage
								StageIndexT					inDestStageIndex0,		// destination stage index
								ImageBufferPtr				inImageBufferPtr,		// pointer to an image buffer struct
								CopyToStageInfo*			inCopyToStageInfo);		// information used to copy to the stage
*/

// -----------------------------------------------------------------------------
// CopyImageBufferToRecordedOutput_
// -----------------------------------------------------------------------------
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	CopyImageBufferToRecordedOutput_(izzyParamsPtr, actorInfo, destStageIndex, imgBuffer, inTop, inLeft, inWidth, inHeight, inZoom) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCopyImageBufferToRecordedOutputProc)((actorInfo)->mThing, (destStageIndex), (imgBuffer), (inTop), (inLeft), (inWidth), (inHeight), (inZoom))

typedef	void				(*CopyImageBufferToRecordedOutputProc)(
								ThingPtr					inThing,				// opaque thing reference
								StageIndexT					inDestStageIndex0,		// destination stage index
								ImageBufferPtr				inImageBufferPtr,		// pointer to an image buffer struct
								float						inTop,
								float						inLeft,
								float						inWidth,
								float						inHeight,
								float						inZoom);

// -----------------------------------------------------------------------------
// GetStageBounds_
// -----------------------------------------------------------------------------
// Returns a rectangle that gives the bounds of the stage.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	GetStageBounds_(izzyParamsPtr, actorInfo, destStageIndex, inDrawableAreaOnly, outRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetStageBoundsProc)((actorInfo)->mThing, (destStageIndex), (inDrawableAreaOnly), (outRect))

typedef	Boolean	(*GetStageBoundsProc)(
					ThingPtr					inThing,
					StageIndexT					inDestStageIndex0,
					Boolean						inDrawableAreaOnly,
					Rect*						inInvalRect);

// -----------------------------------------------------------------------------
// GetStageWindow_
// -----------------------------------------------------------------------------
// Returns a WindowRef for the specified stage that gives the bounds of the stage.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	GetStageWindow_(izzyParamsPtr, actorInfo, destStageIndex, outWindowRef) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetStageWindowProc)((actorInfo)->mThing, (destStageIndex), (outWindowRef))

typedef	void	(*GetStageWindowProc)(
					ThingPtr					inThing,
					StageIndexT					inDestStageIndex0, 
					WindowRef*					outWindowRef);

// -----------------------------------------------------------------------------
// InvalAreaOnStage_
// -----------------------------------------------------------------------------
// Invalidates the specified area on the stage so that it will be erased to the
// background color the next time an update event can be processed.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	InvalAreaOnStage_(izzyParamsPtr, actorInfo, destStageIndex, inInvalRect, inInvalRgn) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mInvalAreaOnStageProc)((actorInfo)->mThing, (destStageIndex), (inInvalRect), (inInvalRgn))

typedef	void	(*InvalAreaOnStageProc)(
					ThingPtr					inThing,
					StageIndexT					inDestStageIndex0, 
					const Rect*					inInvalRect,
					RgnHandle					inInvalRgn);

// -----------------------------------------------------------------------------
// ValidAreaOnStage_
// -----------------------------------------------------------------------------
// Validates the specified area on the stage so that it will be erased to the
// background color the next time an update event can be processed.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	ValidAreaOnStage_(izzyParamsPtr, actorInfo, destStageIndex, inValidRect, inValidRgn) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mValidAreaOnStageProc)((actorInfo)->mThing, (destStageIndex), (inValidRect), (inValidRgn))

typedef	void	(*ValidAreaOnStageProc)(
					ThingPtr					inThing,
					StageIndexT					inDestStageIndex0, 
					const Rect*					inValidRect,
					RgnHandle					inValidRgn);

// -----------------------------------------------------------------------------
// GetStagePreviewFromControl_
// -----------------------------------------------------------------------------
// Returns an image buffer containing the a preview of the specified stage. If
// no preview is available, returns NULL
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	GetStagePreviewFromControl_(izzyParamsPtr, controlInfo, stageIndex0) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetStagePreviewFromControlProc)((controlInfo), (stageIndex0))

typedef	ImageBufferPtr	(*GetStagePreviewFromControlProc)(
							ControlInfo*				inControl,
							StageIndexT					inDestStageIndex0);

// -----------------------------------------------------------------------------
// EnableStagePreviewFromControlProc_
// -----------------------------------------------------------------------------
// Enables or disables the stage preview for the specified stage
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	EnableStagePreviewFromControlProc_(izzyParamsPtr, controlInfo, stageIndex0, inEnable) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mEnableStagePreviewFromControlProc)((controlInfo), (stageIndex0), (inEnable))

typedef	void		(*EnableStagePreviewFromControlProc)(
						ControlInfo*		inControl,
						StageIndexT			inDestStageIndex0,
						Boolean				inEnable);

// -----------------------------------------------------------------------------
// GetAggregateBufferFormat_
// -----------------------------------------------------------------------------
// Given an array of image buffer ptrs, uses the user preferences to determine
// the common  size and format at which those images should be processed.
//
// INTERRUPTS: This function may be called at interrupt time.

#define	GetAggregateBufferFormat_(izzyParamsPtr, inProcessingModeOverride, inDefaultPixelFormat, inImageBufferCount, inImageBufferPtrs, outIBFormatInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetAggregateBufferFormatProc)((inProcessingModeOverride), (inDefaultPixelFormat), (inImageBufferCount), (inImageBufferPtrs), (outIBFormatInfo))

typedef	Boolean	(*GetAggregateBufferFormatProc)(
					ImageProcessingMode			inProcessingModeOverride,		// see notes above regarding this parameter
					OSType						inDefaultPixelFormat,			// default pixel format
					SInt32						inImageBufferCount,				// number of image buffer ptrs in the array inImageBufferPtrs[]
					ImageBufferPtr*				inImageBufferPtrs,				// the array of image buffer ptrs to test
					IBFormatInfoPtr				outIBFormatInfo);				// the resulting common format and size

// -----------------------------------------------------------------------------
// IsIntermediaryImageBufferRequired_
// -----------------------------------------------------------------------------
// Given a list of image buffer ptrs, uses the user preferences to determine the
// size at which those images should be processed. if the image buffer indicated
// by inImageBufferToTestIndex does not match the size or bit depth of calculated
// based on the user prefs and image buffers to be combined, then the result will
// be true.
// 
// Normally you should always pass kImageProcessUseDefault as the image processing
// mode to respect the settings made by the User in the preferences. But, since
// it may be useful to force a particular setting in certain situations, you can
// optionally set this parameter to one of kImageProcessLargerSlower,
// kImageProcessSmallerFaster or kImageProcessFixedSize
// 
// INTERRUPTS: This function may be called at interrupt time.

#define	IsIntermediaryImageBufferRequired_(izzyParamsPtr, inProcessingModeOverride, inDefaultPixelFormat, inImageBufferCount, inImageBufferPtrs, inImageBufferToTestIndex) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mIsIntermediaryImageBufferRequiredProc)((inProcessingModeOverride), (inDefaultPixelFormat),(inImageBufferCount), (inImageBufferPtrs), (inImageBufferToTestIndex))

typedef	Boolean	(*IsIntermediaryImageBufferRequired)(
					ImageProcessingMode			inProcessingModeOverride,		// see notes above regarding this parameter
					OSType						inDefaultPixelFormat,			// default pixel format
					SInt32						inImageBufferCount,				// number of image buffer ptrs pointed to by inImageBuffers
					ImageBufferPtr*				inImageBufferPtrs,				// an array of image buffer ptrs
					SInt32						inImageBufferToTestIndex);		// the image buffer to test, to see if an intermediary
																				// buffer is required

// -----------------------------------------------------------------------------
// CreateComplimentaryImageBuffer_
// -----------------------------------------------------------------------------
// Creates a new image buffer that matches the size and depth of the specified
// image buffers. It is up to the caller to dispose the resulting image buffer
// with the DisposeImageBuffer_ function/macro.
// 
// Note that this function respects the mImgProcPrefs preference settings given
// in the IsadoraParams struct. Thus the resulting ImageBuffer may not be of the
// same dimensions as the source. Your function should take this possibility into
// account. You can test to see whether or not you need to make an intediary
// buffer with the DoIntermediaryImageBufferRequired macro.
// 
// Normally you should always pass kImageProcessUseDefault as the image processing
// mode to respect the settings made by the User in the preferences. But, since
// it may be useful to force a particular setting in certain situations, you can
// optionally set this parameter to one of kImageProcessLargerSlower,
// kImageProcessSmallerFaster or kImageProcessFixedSize, or kImageProcessMatchFormatOnly
//
// If there is not enough memory available to create the image buffer, the
// resulting value is nil.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	CreateComplimentaryImageBuffer_(izzyParamsPtr, inProcessingModeOverride, inDefaultPixelFormat, inImageBufferCount, inImageBufferPtrs) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCreateComplimentaryImageBufferProc)((inProcessingModeOverride), (inDefaultPixelFormat), (inImageBufferCount), (inImageBufferPtrs))

typedef	ImageBufferPtr		(*CreateComplimentaryImageBufferProc)(
								ImageProcessingMode			inProcessingModeOverride,	// see notes above regarding this parameter
								OSType						inDefaultPixelFormat,		// default pixel format
								SInt32						inImageBufferCount,			// number of image buffer ptrs pointed to by inImageBuffers
								ImageBufferPtr*				inImageBufferPtrs);			// an array of image buffer ptrs

// -----------------------------------------------------------------------------
// DisposeImageBuffer_
// -----------------------------------------------------------------------------
// Dispose an ImageBuffer created with the CreateComplimentaryImageBufferProc
// function or DoCreateComplimentaryImageBuffer macro.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	DisposeImageBuffer_(izzyParamsPtr, inImageBuffer) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDisposeImageBufferProc)((inImageBuffer))

typedef	void			(*DisposeImageBufferProc)(
								ImageBufferPtr	inImageBuffer);			// the image buffer to dispose

// -----------------------------------------------------------------------------
// CopyImageBuffer_
// -----------------------------------------------------------------------------
// Copy one all or part of one image buffer to another. You may optionally
// provide a pointer to source and/or destination rectangle. If either of these
// parameters are set to nil, then the respective bounds rectangle for the source
// and/or destination buffer is used. If the source and destination rectangles
// differ in size, then the source image will be scaled to fit the destination
// bounds.
//
// The CopyImageBufferBlend_ accepts a blend color that is used to determine
// the percentage of the source and destination images. As the color of each
// of the blend color's components (r, g, b) approach 65535, you will see less
// of the source image and more of the destination image.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	CopyImageBuffer_(izzyParamsPtr, inImageBufferSrc, inImageBufferDst, inSrcRect, inDstRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCopyImageBufferProc)((inImageBufferSrc), (inImageBufferDst), (inSrcRect), (inDstRect), srcCopy, nil)

#define	CopyImageBufferBlend_(izzyParamsPtr, inImageBufferSrc, inImageBufferDst, inSrcRect, inDstRect, inRGBBlendColor) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCopyImageBufferProc)((inImageBufferSrc), (inImageBufferDst), (inSrcRect), (inDstRect), blend, (inRGBBlendColor))

#define	CopyImageBufferMode_(izzyParamsPtr, inImageBufferSrc, inImageBufferDst, inSrcRect, inDstRect, inMode, inRGBBlendColor) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCopyImageBufferProc)((inImageBufferSrc), (inImageBufferDst), (inSrcRect), (inDstRect), (inMode), (inRGBBlendColor))

typedef	void			(*CopyImageBufferProc)(
								ImageBufferPtr	inImageBufferSrc,
								ImageBufferPtr	inImageBufferDst,
								const Rect*		inSrcRect,			// can be nil for entire buffer
								const Rect*		inDstRect,			// can be nil for entire buffer
								SInt16			inMode,				// transfer mode (srcCopy, srcOr, etc.)
								RGBColor*		inBlendColor);		// blend color when transfer mode is blend

// -----------------------------------------------------------------------------
// CreateBlackImageBuffer_
// -----------------------------------------------------------------------------
// Creates a black image buffer with the specified format and size
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	CreateBlackImageBuffer_(izzyParamsPtr, inFormat, inWidth, inHeight) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCreateBlackImageBufferProc)((inFormat), (inWidth), (inHeight))

typedef	ImageBufferPtr		(*CreateBlackImageBufferProc)(OSType inFormat, SInt32 inWidth, SInt32 inHeight);

// -----------------------------------------------------------------------------
// GetSceneName_
// -----------------------------------------------------------------------------
// Returns the name of the specified scene
// 
// The scene to be whose name will be returend is determined by the jump mode
// and scene index parameters -- see the JumpToScene_ command for more info.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	GetSceneName_(izzyParamsPtr, actorInfo, mode, sceneIndex, maxLen, sceneNamePtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetSceneNameProc)((actorInfo)->mThing, (mode), (sceneIndex), (maxLen), (sceneNamePtr))

typedef	IzzyError			(*GetSceneNameProc)(
								ThingPtr					inThing,
								JumpMode					inMode,
								SInt32						inSceneIndex,
								UInt32						inSceneNameMaxLen,
								char*						outSceneName);

// -----------------------------------------------------------------------------
// JumpToScene_
// -----------------------------------------------------------------------------
// Jumps from the current scene to the scene specified by the combination of the
// JumpMode specified by inMode and the value of inJumpValue.
// 
// If the JumpMode is kJumpRelative, then inJumpValue will will be added to the
// current Scene Index to determine the new scene to be activated. Thus a value
// of +1 would jump to the next scene, a value of -1 would jump to the previous
// scene.
// 
// If the JumpMode is kJumpAbsolute, then Isadora will attempt to set the Scene
// Index to the value given by inJumpValue. Note that the Scene Index is one-
// based.
// 
// In either case, if the calculated Scene Index is out of bounds (i.e., less
// than 1 or greater than the number of scenes in the current Isadora document)
// then Isadora does nothing.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	JumpToScene_(izzyParamsPtr, actorInfo, mode, sceneIndex) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mJumpToSceneProc)((actorInfo)->mThing, (mode), (sceneIndex), 0, 0)

#define	JumpToSceneWithFade_(izzyParamsPtr, actorInfo, mode, sceneIndex, duration_mS) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mJumpToSceneProc)((actorInfo)->mThing, (mode), (sceneIndex), (duration_mS), (duration_mS))

#define	JumpToSceneWithFadeInFadeOut_(izzyParamsPtr, actorInfo, mode, sceneIndex, fadeInDuration_mS, fadeOutDuration_mS) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mJumpToSceneProc)((actorInfo)->mThing, (mode), (sceneIndex), (fadeInDuration_mS), (fadeOutDuration_mS))

typedef	IzzyError			(*JumpToSceneProc)(
								ThingPtr					inThing,
								JumpMode					inMode,
								SInt32						inSceneIndex,
								UInt32						inFadeOutDuration_mS,
								UInt32						inFadeInDuration_mS);

// -----------------------------------------------------------------------------
// ActivateDeactivateScene_
// -----------------------------------------------------------------------------
// Activates or deactivates a scene other than the current scene, referred to 
// here as a 'secondary' scene. Once a secondary scene is activated, it will
// remain active until 1) it becomes the primary scene (via a jump) and is
// subsequently deactivated by jumping to another scene or 2) it is deactivated
// by this calling this function with the activate parameter set to false.
// 
// The scene to be activated or deactivated is determined by the jump mode
// and scene index parameters -- see the JumpToScene_ command for more info.
// 
// ** NOT ** INTERRUPT SAFE: This function must not be called at interrupt time.

#define	ActivateDeactivateScene_(izzyParamsPtr, actorInfo, mode, sceneIndex, activate) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mActivateDeactivateSceneProc)((actorInfo)->mThing, (mode), (sceneIndex), (activate), 0)

#define	ActivateDeactivateSceneWithFade_(izzyParamsPtr, actorInfo, mode, sceneIndex, activate, duration_mS) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mActivateDeactivateSceneProc)((actorInfo)->mThing, (mode), (sceneIndex), (activate), (duration_mS))

typedef	IzzyError			(*ActivateDeactivateSceneProc)(
								ThingPtr					inThing,
								JumpMode					inMode,
								SInt32						inSceneIndex,
								Boolean						inActivate,
								UInt32						inFadeDuration_mS);

// -----------------------------------------------------------------------------
// SetSceneIntensity_
// -----------------------------------------------------------------------------
// Allows you to set the intensity of a secondary scene. The intensity controls
// the visual brightness and aural volume of a secondary scene

#define	SetSceneIntensity_(izzyParamsPtr, actorInfo, mode, sceneIndex, intensity) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetSceneIntensityProc)((actorInfo)->mThing, (mode), (sceneIndex), (intensity))

typedef	IzzyError			(*SetSceneIntensityProc)(
								ThingPtr					inThing,
								JumpMode					inMode,
								SInt32						inSceneIndex,
								float						inIntensity);

// -----------------------------------------------------------------------------
// GetSceneIntensity_
// -----------------------------------------------------------------------------
// Returns the master intensity for the scene that owns this actor

#define	GetSceneIntensity_(izzyParamsPtr, actorInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetSceneIntensityProc)((actorInfo)->mThing)

typedef	float			(*GetSceneIntensityProc)(
								ThingPtr					inThing);

// -----------------------------------------------------------------------------
// GetSceneIndex_
// -----------------------------------------------------------------------------
// Returns the one-based index of the current scene

#define	GetSceneIndex_(izzyParamsPtr, actorInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetSceneIndexProc)((actorInfo)->mThing)

typedef	SceneIndexT		(*GetSceneIndexProc)(
								ThingPtr					inThing);

// -----------------------------------------------------------------------------
// PropertyTypeAndIndexToHelpIndex_
// -----------------------------------------------------------------------------
// Converts a Property Type and Property Index into a zero-based index that can
// be used to access a help string.
// 
// This utility function is usually called from within the GetHelpString function
// inside of your plugin to determine which help string to return to the caller.
// If you organize your help strings as follows
// 
// Index 0 = General Help String Index 1 = Input Property 1 Help String Index 2 =
// Input Property 2 Help String ... Index n = Input Property n Help String Index
// n+1 = Output Property 1 Help String Index n+2 = Output Property 2 Help String
// ...
// 
// Then you can use this function to convert the Property Type and Property Index
// passed to GetHelpString to generate the proper index into that array.

#define	PropertyTypeAndIndexToHelpIndex_(izzyParamsPtr, actorInfo, propMode, propIndex) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPropertyTypeAndIndexToHelpIndexProc)((actorInfo)->mThing, (propMode), (propIndex))

typedef UInt32				(*PropertyTypeAndIndexToHelpIndexProc)(
								ThingPtr					inThing,
								PropertyType				inPropertyType,
								PropertyIndex				inPropertyIndex1);

// -----------------------------------------------------------------------------
// GetPropertyCount_
// -----------------------------------------------------------------------------
// Returns the number of input or output properties for a given actor.

#define GetPropertyCount_(izzyParamsPtr, actorInfo, propType, intPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetPropertyCount)((actorInfo)->mThing, (propType), (intPtr))

typedef IzzyError	(*GetPropertyCountProc)(
						ThingPtr					inThingPtr,
						PropertyType				inPropertyType,
						UInt32*						outCount);


// -----------------------------------------------------------------------------
// AddProperty_
// -----------------------------------------------------------------------------
// Adds a new property to the actor.
//
#define	AddProperty_(izzyParamsPtr, actorInfo, propType, propID, conformPropID, name, dispFmts, initFmt, inMutableGroup, min, max, value) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mAddPropertyProc)((actorInfo)->mThing, (propType), (propID), (conformPropID), (name), 0, (dispFmts), (initFmt), (inMutableGroup), (min), (max), (value), 0, 0, 0)

#define	AddPropertyOptions_(izzyParamsPtr, actorInfo, propType, propID, conformPropID, name, dispFmts, initFmt, inMutableGroup, min, max, value, options) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mAddPropertyProc)((actorInfo)->mThing, (propType), (propID), (conformPropID), (name), 0, (dispFmts), (initFmt), (inMutableGroup), (min), (max), (value), 0, 0, (options))

#define	AddPropertyCustomDataType_(izzyParamsPtr, actorInfo, propType, propID, conformPropID, name, customDataType, dispFmts, initFmt, inMutableGroup, min, max, value) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mAddPropertyProc)((actorInfo)->mThing, (propType), (propID), (conformPropID), (name), (customDataType), (dispFmts), (initFmt), (inMutableGroup), (min), (max), (value), 0, 0, 0)

#define	AddPropertyCustomDataTypeAfterIndex_(izzyParamsPtr, actorInfo, propType, propID, conformPropID, name, customDataType, dispFmts, initFmt, inMutableGroup, min, max, value, inAfterIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mAddPropertyProc)((actorInfo)->mThing, (propType), (propID), (conformPropID), (name), (customDataType), (dispFmts), (initFmt), (inMutableGroup), (min), (max), (value), (inAfterIndex1), 0, 0)

#define	AddPropertyCustomDataTypeAfterPropWithID_(izzyParamsPtr, actorInfo, propType, propID, conformPropID, name, customDataType, dispFmts, initFmt, inMutableGroup, min, max, value, inAfterPropID) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mAddPropertyProc)((actorInfo)->mThing, (propType), (propID), (conformPropID), (name), (customDataType), (dispFmts), (initFmt), (inMutableGroup), (min), (max), (value), 0, (inAfterPropID), 0)

typedef IzzyError	(*AddPropertyProc)(
						ThingPtr					inThingPtr,
						PropertyType				inPropertyType,
						PropIDT						inPropID,
						PropIDT						inConformToPropID,
						const char*					inName,
						PropIDT						inCustomDataType,
						PropertyDispFormat			inDisplayFormats,
						PropertyDispFormat			inInitialDisplayFormat,
						MutableGroup				inMutableGroup,
						const ValuePtr				inMinimum,
						const ValuePtr				inMaximum,
						const ValuePtr				inInitialValue,
						SInt32						inAfterIndex1,
						PropIDT						inAfterPropID,
						PropertyOptions				inOptionFlags);


// -----------------------------------------------------------------------------
// RemovePropertyProc_
// -----------------------------------------------------------------------------
// Removes an existing property from an actor. If there are any connections to
// that property ad the time that this call is made, they are disconnected
// prior to the property being removed.

#define	RemovePropertyProc_(izzyParamsPtr, actorInfo, propType, propIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mRemovePropertyProc)((actorInfo)->mThing, (propType), (propIndex1))

typedef IzzyError	(*RemovePropertyProc)(
						ThingPtr					inThingPtr,
						PropertyType				inPropertyType,
						PropertyIndex				inPropertyIndex1);


// -----------------------------------------------------------------------------
// SetPropertyName_
// -----------------------------------------------------------------------------
// Sets the title of the specified property

#define	SetPropertyName_(izzyParamsPtr, actorInfo, propType, propIndex1, propName) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetPropertyNameProc)((actorInfo)->mThing, (propType), (propIndex1), (propName))

typedef IzzyError	(*SetPropertyNameProc)(
						ThingPtr					inThingPtr,
						PropertyType				inPropertyType,
						PropertyIndex				inPropertyIndex1,
						const char*					inPropertyName);


// -----------------------------------------------------------------------------
// GetPropertyConnectionInfo_
// -----------------------------------------------------------------------------
// Retrieves information about a specific connection to/from a particular property.
// The caller supplies the Property Type, Property Index and Connection Index; if
// there is a valid connection to/from this property with the specified connection
// index, the result will be true and outConenctedThing and outConnectedPropertyIndex1
// will identify the connection. The caller may use this information to create
// or restore a connection using the LinkProperties_ macro below

#define	GetPropertyConnectionInfo_(izzyParamsPtr, actorInfo, propType, propIndex1, connectionIndex1, outConnectedThing, outConnectedPropertyIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetPropertyConnectionInfoProc)((actorInfo)->mThing, (propType), (propIndex1), (connectionIndex1), (outConnectedThing), (outConnectedPropertyIndex1))

typedef Boolean	(*GetPropertyConnectionInfoProc)(
						ThingPtr					inThingPtr,
						PropertyType				inPropertyType,
						PropertyIndex				inPropertyIndex1,
						UInt32						inConnectionIndex1,
						ThingPtr*					outConenctedThing,
						PropertyIndex*				outConnectedPropertyIndex1);


// -----------------------------------------------------------------------------
// LinkProperties_
// -----------------------------------------------------------------------------
// Creates a link between two propereties. One property is specified using the
// actorInfo, property type, and property index parameters. The other is specified
// by the "other thing" and "other property index" parameters. If the first property
// is an input, then the other property is assumed to be an output and vice-versa

#define	LinkProperties_(izzyParamsPtr, actorInfo, propType, propIndex1, inOtherThing, inOtherPropertyIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mLinkPropertiesProc)((actorInfo)->mThing, (propType), (propIndex1), (inOtherThing), (inOtherPropertyIndex1))

typedef IzzyError	(*LinkPropertiesProc)(
						ThingPtr					inThingPtr,
						PropertyType				inPropertyType,
						PropertyIndex				inPropertyIndex1,
						ThingPtr					inOtherThing,
						PropertyIndex				inOtherPropertyIndex1);


// -----------------------------------------------------------------------------
// GetActorResourcePICT_
// -----------------------------------------------------------------------------
// Gets a Macintosh PICT resource from an actor and converts it to an AlphaBitmap.
// This function will open the resource fork of the plugin and return a rendered
// version of the PICT in an AlphaBitmap struct if it is found. If the resource
// is not found, the AlphaBitmap struct passed into the function will remain
// unchanged.
//
// WIN32 NOTE:
//
// Note that if Isadora is running under windows, and the plugin file itself
// does not have a resource fork, Izzy will attempt to open a companion resource
// file with the .qtr extension. So, if a plugin called "myplugin.dll",
// calls GetActorMacResource_ and it has not been RezWack'd to include the
// Mac style resources as part of the plugin, then Isadora will look for
// a file in the same folder as myplugin.dll called "myplugin.qtr" and attempt
// to open it. If it can find the file it will attempt to read the resources
// from there.
//
// It is up to the plugin to insure that the returned ImageBuffer is disposed!

#define	GetActorResourcePICT_(izzyParamsPtr, actorInfo, resID, outRenderedImage) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetActorResourcePICTProc)(&((actorInfo)->mPluginFileSpec), (resID), (outRenderedImage))

#define	GetControlResourcePICT_(izzyParamsPtr, controlInfo, resID, outRenderedImage) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetActorResourcePICTProc)(&((controlInfo)->mPluginFileSpec), (resID), (outRenderedImage))

typedef IzzyError	(*GetActorResourcePICTProc)(
						const FSSpec*				inFSSpec,
						SInt16						inResID,
						struct AlphaBitmap*			outRenderedImage);


// -----------------------------------------------------------------------------
// IzzyMalloc_ & IzzyMallocClear_
// -----------------------------------------------------------------------------
// Asks Isadora to allocate memory, a la malloc from the standard library. The
// IzzyMallocClear_ clears the memory to 0 after the allocation is complete.

#define	IzzyMalloc_(izzyParamsPtr, inSize) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mIzzyMallocProc)((inSize), 0)

#define	IzzyMallocClear_(izzyParamsPtr, inSize) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mIzzyMallocProc)((inSize), 1)

typedef void*	(*IzzyMallocProc)(
						UInt32						inSize,
						Boolean						inClear);


// -----------------------------------------------------------------------------
// IzzyFree_
// -----------------------------------------------------------------------------
// Frees memory allocated with IzzyMalloc_

#define	IzzyFree_(izzyParamsPtr, inPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mIzzyFreeProc)((inPtr))

typedef void	(*IzzyFreeProc)(
						void*						inPtr);

// -----------------------------------------------------------------------------
// LookupPartialStringInList_
// -----------------------------------------------------------------------------
// Searches a list of c-strings for the one whose starting characters
// most closely match the specified match string. Returns -1 if no matches
// could be made, otherwise, returns the zero based index of the matched 
// string in the list.
//
// This is useful if you have a property defined value string with a several
// options (e.g., the wave generator, which has the sine, square, triangle, etc.
// wave options). You can use this function to determine which item to
// chose based as the user types by calling it from your PropertyStringToValue
// function.

#define	LookupPartialStringInList_(izzyParamsPtr, inMaxStrings, inStringList, inStringToMatch) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mLookupPartialStringInListProc)((inMaxStrings), (inStringList), (inStringToMatch))

typedef SInt32									// returns -1 if no matches, otherwize zero-based index to match
(*LookupPartialStringInListProc)(
	UInt32				inMaxStrings,			// max strings to match
	const char* const *	inStringList,			// an array of c-strings (e.g., char* str[3] = { "one", "two", "three" }
	const char*			inStringToMatch);		// the string you are attempting to match
	

// -----------------------------------------------------------------------------
// ModKeyStringToMaskProc_
// -----------------------------------------------------------------------------
// Converts a c-string of one or more modifer keys string codes into a mod 
// key mask. Illegal or unknown codes are ignored and stop the parsing of
// the string.
//
// The valid codes are:
//
// "cmd" or "d" for command		mask = 0x0001
// "ctl" or "c" for control		mask = 0x0002
// "opt" or "o" for option		mask = 0x0004
// "shf" or "s" for shift		mask = 0x0010
//
// Example: string = "cmd opt" - result mask = 0x0005

#define	ModKeyStringToMask_(izzyParamsPtr, inString, outMask) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mModKeyStringToMaskProc)((inString), (outMask))

typedef void	(*ModKeyStringToMaskProc)(
						const char* 				inString,
						UInt32*						outMask);

// -----------------------------------------------------------------------------
// ModKeyMaskToString_
// -----------------------------------------------------------------------------
// Converts a modifer keys mask into a c-string representation. See above for
// mask codes and their string equivalents.

#define	ModKeyMaskToString_(izzyParamsPtr, inModKeyMask, outString) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mModKeyMaskToStringProc)((inModKeyMask), (outString))

typedef void	(*ModKeyMaskToStringProc)(
						UInt32						inModKeyMask,
						char*						outString);
	
// -----------------------------------------------------------------------------
// GetScreenBounds_
// -----------------------------------------------------------------------------
// Returns the bounding rectangle of any screen currently available on the
// the system. Screen 1 is always the primary start up screen. The numbering
// of other external monitors varies from system to system.
//
// The return value is true if the requested screen exists. Otherwise it is
// false.

#define	GetScreenBounds_(izzyParamsPtr, inScreenIndex1, outRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetScreenBoundsProc)((inScreenIndex1), (outRect))

typedef Boolean	(*GetScreenBoundsProc)(
						UInt32						inScreenIndex1,
						Rect*						outScreenBounds);
	
// -----------------------------------------------------------------------------
// SendMidiNoteOnOff_
// -----------------------------------------------------------------------------
// Sends a midi note on message. The note is sent out of the specified port,
// with the channel, pitch, and velocity values given. If the inNoteOffMs
// parameter is greater than 0, then a corresponding Note Off message will
// be sent after the specified number of milliseconds has elapsed.

#define	SendMidiNoteOnOff_(izzyParamsPtr, inScreenIndex1, inPort, inChan, inPitch, inVel, inNoteOffMs, inRefCon) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSendMidiNoteOnOffProc)((inPort), (inChan), (inPitch), (inVel), (inNoteOffMs), (inRefCon))

typedef void	(*SendMidiNoteOnOffProc)(
						UInt16					inPortIndex1,							// one-based midi port - as shown in the MIDI setup window
						UInt8					inChannel1,								// one-based midi channel
						UInt8					inPitch,								// midi pitch, 0-127
						UInt8					inVelocity,								// midi velocity, 0-127
						UInt32					inMatchingNoteOffMilliseconds,			// send note off message after this duration in milliseconds
						SInt32					inRefCon);								// ref con, used with CancelMidiNoteOff_
	
// -----------------------------------------------------------------------------
// CancelMidiNoteOff_
// -----------------------------------------------------------------------------
// Cancels a note off command scheduled with the SendMidiNoteOnOff_ function
// The port, channel, pitch, and refcon specified in the SendMidiNoteOnOff_
// must all match for the note off to be properly cancelled.

#define	CancelMidiNoteOff_(izzyParamsPtr, inScreenIndex1, inPort, inChan, inPitch, inRefCon) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCancelMidiNoteOffProc)((inPort), (inChan), (inPitch), (inRefCon))

typedef void	(*CancelMidiNoteOffProc)(
						UInt16					inPortIndex1,
						UInt8					inChannel1,
						UInt8					inPitch,
						SInt32					inRefCon);
	
// -----------------------------------------------------------------------------
// GetMidiDancerEnergy_
// -----------------------------------------------------------------------------
// Returns the overall energy of any MidiDancer activity seen on the specified
// MIDI input port.

#define	GetMidiDancerEnergy_(izzyParamsPtr, inPort, inLimbCount) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetMidiDancerEnergyProc)((inPort), (inLimbCount))

typedef float	(*GetMidiDancerEnergyProc)(
					UInt16						inPortIndex1,
					SInt32						inLimbCount);
	
// -----------------------------------------------------------------------------
// GetMidiDancerEnergyPlus_
// -----------------------------------------------------------------------------
// Returns the overall energy of any MidiDancer activity seen on the specified
// MIDI input port.

#define	GetMidiDancerEnergyPlus_(izzyParamsPtr, inPort, inMaxMilliseconds, inMinLimbIndex1, inMaxLimbIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetMidiDancerEnergyPlusProc)((inPort), (inMaxMilliseconds), (inMinLimbIndex1), (inMaxLimbIndex1))

typedef float	(*GetMidiDancerEnergyPlusProc)(
					UInt16						inPortIndex1,
					SInt32						inMaxMilliseconds,
					SInt32						inMinLimbIndex1,
					SInt32						inMaxLimbIndex1);
	
// -----------------------------------------------------------------------------
// GetTextFromDialog_
// -----------------------------------------------------------------------------
// Shows a dialog with the intial text provided by ioTextHandle. After the user
// is done editing, returns the new text in the same handle. The result is true
// if the user clicked OK in the dialog, false otherwise.

#define	GetTextFromDialog_(izzyParamsPtr, actorInfo, ioTextHandle, inDialogTitle, inValidateTextProc, inMakeDocDirtyOnSuccess, helpURL) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetTextFromDialogProc)((actorInfo)->mThing, (ioTextHandle), (inDialogTitle), (inValidateTextProc), (inMakeDocDirtyOnSuccess), (helpURL))

typedef Boolean (*ValidateTextProc)(							// return true if text is valid, false if there is an error
					Handle		inTextHandle,					// text handle to validate
					SInt32*		outSelStart,					// when returning FALSE, selection start in dialog
					SInt32*		outErrSelEnd,					// when returning FALSE, selection end in dialog
					char* 		outErrorMessageBuffer,			// when returning FALSE, copy an error message here. set to null string for no error message
					SInt32		inErrorMessageBufferMaxLen);	// maximum length for the error message buffer

typedef Boolean	(*GetTextFromDialogProc)(
					ThingPtr					inThingPtr,
					Handle						ioTextHandle,
					const char*					inDialogTitle,				// can be NULL
					ValidateTextProc			inValidateTextProc,			// can be NULL
					Boolean						inMakeDocDirtyOnSuccess,
					const char*					inHelpURL);					// if non NULL, the URL to a help file for this dialog
	
// -----------------------------------------------------------------------------
// GetChannelMapDialog_
// -----------------------------------------------------------------------------
// Shows a dialog that allows the user to edit a list of single input channels
// that are linked with corresponding multiple output channels

#define	GetChannelMapDialog_(izzyParamsPtr, actorInfo, ioTextHandle, inDialogTitle, inMakeDocDirtyOnSuccess, helpURL) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetChannelMapDialogProc)((actorInfo)->mThing, (ioTextHandle), (inDialogTitle), (inMakeDocDirtyOnSuccess), (helpURL))

typedef Boolean	(*GetChannelMapDialogProc)(
					ThingPtr					inThingPtr,
					Handle						ioTextHandle,
					const char*					inDialogTitle,				// can be NULL
					Boolean						inMakeDocDirtyOnSuccess,
					const char*					inHelpURL);					// if non NULL, the URL to a help file for this dialog
	
// -----------------------------------------------------------------------------
// EditLanBoxRectListDialog_
// -----------------------------------------------------------------------------
// Shows a dialog where the user may edit a list of rectangles that determine
// where the LanBox RGB actor will acquire it's RGB values.

typedef struct {
	Rect	mRect;				// coordinates of rect in raw pixels
	UInt8	mAverage;			// 1 = true, 0 = false
	UInt8	mSelected;			// 1 = selected, 0 = not
	UInt16	mBaseDMXChannel;	// base dmx channel - based on previous rects in list
} LanBoxRect, *LanBoxRectPtr;

typedef struct {
	SInt32		mVersion;		// version number
	SInt32		mHRes;			// resolution used when rect list
	SInt32		mVRes;			// was last edited
	SInt32		mMagnification;	// code for magnification
	LanBoxRect	mRectList[1];
} LanBoxRectList, *LanBoxRectListPtr, **LanBoxRectListHandle;

#define	kLanBoxRectListHeaderSizeBytes	(sizeof(SInt32) * 4)

#define	EditLanBoxRectListDialog_(izzyParamsPtr, actorInfo, ioRectHandle, inMakeDocDirtyOnSuccess, inDialogTitle, inSourceImageBuffer, inChannelOffset, inRectChangedCallback) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mEditLanBoxRectListDialogProc)((actorInfo)->mThing, (ioRectHandle), (inMakeDocDirtyOnSuccess), (inDialogTitle), (inSourceImageBuffer), (inChannelOffset), (inRectChangedCallback))

typedef void (*LBRectanglesChangedCallback)(				// return true if text is valid, false if there is an error
					Handle		inRectListHandle);			// text handle to validate

typedef Boolean	(*EditLanBoxRectListDialog)(
					ThingPtr					inThingPtr,
					LanBoxRectListHandle		ioRectHandle,
					Boolean						inMakeDocDirtyOnSuccess,
					const char*					inDialogTitle,					// can be NULL
					ImageBufferPtr				inSourceImageBuffer,			// can be NULL
					SInt32						inChannelOffset,				// can be 0
					LBRectanglesChangedCallback	inRectChangedCallbackProc);		// can be NULL
	
// -----------------------------------------------------------------------------
// PrepareActorDefinedAreaPict_
// -----------------------------------------------------------------------------
// Takes the specified picture and makes two GWorlds from it: one drawn an 
// unselected background and one on a selected background. This makes it 
// possible to draw the actor's icon pict as fast as possible, without needed
// to convert the picture into a GWorld each time.

typedef struct {
	Boolean		mInitialized;
	GWorldPtr	mUnselGWorld;
	GWorldPtr	mSelGWorld;
	SInt16		mWidth;
	SInt16		mHeight;
} ActorPictInfo;

#define	PrepareActorDefinedAreaPict_(izzyParamsPtr, actorInfo, pictID, outActorPictInfoPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPrepareActorDefinedAreaPictProc)((actorInfo), (pictID), (outActorPictInfoPtr))

typedef void	(*PrepareActorDefinedAreaPictProc)(
					ActorInfo*					inActorInfo,
					SInt16						inPictID,
					ActorPictInfo*				inActorPictInfo);
	
// -----------------------------------------------------------------------------
// DrawActorDefinedAreaPict_
// -----------------------------------------------------------------------------
// Draws a picture referenced by an ActorPictInfo struct, which is created
// by the PrepareActorDefinedAreaPict_ function.

#define	DrawActorDefinedAreaPict_(izzyParamsPtr, actorInfo, inSelected, inAreaRect, inActorPictInfoPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawActorDefinedAreaPictProc)((actorInfo), (inSelected), (inAreaRect), (inActorPictInfoPtr))

typedef void	(*DrawActorDefinedAreaPictProc)(
					ActorInfo*					inActorInfo,
					Boolean						inSelected,
					const Rect*					inAreaRect,
					ActorPictInfo*				inActorPictInfo);
	
// -----------------------------------------------------------------------------
// DrawTextInRect_
// -----------------------------------------------------------------------------
// Draws the specified text inside a rectangle without erasing the background.

#define	DrawTextInRect_(izzyParamsPtr, inGraphics, inText, inLength, inRect, inHorzJustification, inVertJustification, inOneLineOnly) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawTextInRectProc)((inGraphics), (inText), (inLength), (inRect), (inHorzJustification), (inVertJustification), (inOneLineOnly))

typedef void	(*DrawTextInRectProc)(
					UGraphics*		inGraphics, 
					const void*		inText,
					SInt32			inLength,
					const Rect*		inRect,
					SInt16			inHorzJustification,
					SInt16			inVertJustification,
					Boolean			inOneLineOnly);

//-----------------------------------------------------------------------------
// More Plugin Debugging

#define	kForcePluginAssert				1

#if DEBUG || kForcePluginAssert
	#define	PluginAssert_(ip, cond) 	if (!(cond)) (*((ip)->kConditionalCallbacks->mPluginAssertProc))( #cond, kActorName, __LINE__)
#else
	#define	PluginAssert_(ip, cond)		0
#endif

typedef void	(*PluginAssertProc)(
					const char*			inCond,
					const char*			inFile,
					int					inLine);


#if DEBUG
	#define	PluginLogInfo_(ip) 	(*((ip)->kConditionalCallbacks->mPluginLogInfoProc))
#else
	#define	PluginLogInfo_(ip)	0,
#endif

typedef void	(*PluginLogInfoProc)(
					const char*			inString,
					...);

// -----------------------------------------------------------------------------
// WarnPluginLowMemory_
// -----------------------------------------------------------------------------
// Shows a dialog that indicates a plugin ran out of memory

#define	WarnPluginLowMemory_(izzyParamsPtr, inPluginName) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mWarnLowMemoryProc)((inPluginName))

typedef void	(*WarnLowMemoryProc)(
					const char*		inPluginName);
	
// -----------------------------------------------------------------------------
// RecallSnapshot_
// -----------------------------------------------------------------------------
// Recals the specified snapshot, restoring the input values of all actors
// in the specified scene

#define	RecallSnapshot_(izzyParamsPtr, actorInfo, inSnapshotID) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mRecallSnapshotProc)((actorInfo)->mThing, (inSnapshotID))

typedef IzzyError	(*RecallSnapshotProc)(
						ThingPtr					inActorInfo,
						SInt32						inSnapshotID);
	
// -----------------------------------------------------------------------------
// TakeSnapshot_
// -----------------------------------------------------------------------------
// Takes a new snapshot of the current input values of all actors in the
// associated scene

#define	TakeSnapshot_(izzyParamsPtr, actorInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mTakeSnapshotProc)((actorInfo)->mThing)

typedef IzzyError	(*TakeSnapshotProc)(
						ThingPtr					inActorInfo);
	
// -----------------------------------------------------------------------------
// UpdateSnapshot_
// -----------------------------------------------------------------------------
// Updates the specified snapshot so that it reflects the current input values
// for all actors in the associated scene.

#define	UpdateSnapshot_(izzyParamsPtr, actorInfo, inSnapshotID) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mUpdateSnapshotProc)((actorInfo)->mThing, (inSnapshotID))

typedef IzzyError	(*UpdateSnapshotProc)(
						ThingPtr					inActorInfo,
						SInt32						inSnapshotID);
	
// -----------------------------------------------------------------------------
// CreateImageBuffer_
// -----------------------------------------------------------------------------
// Creates an image buffer with the specified width, height, and depth

#define	CreateImageBuffer_(izzyParamsPtr, inWidth, inHeight, inFormat) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCreateImageBufferProc)((inWidth), (inHeight), (inFormat))

typedef	ImageBufferPtr		(*CreateImageBufferProc)(
								SInt32			inWidth,			// width of image buffer
								SInt32			inHeight,			// height of image buffer
								OSType			inFormat);			// buffer format

// -----------------------------------------------------------------------------
// EnterVideoProcessing_
// -----------------------------------------------------------------------------
// Called just prior to entering code that does video processing - returns an
// integer that is passed back to ExitVideoProcessing so that we can determine
// how much time is spent on video processing.

#define	EnterVideoProcessing_(izzyParamsPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mEnterVideoProcessingProc)()

typedef UInt64				(*EnterVideoProcessingProc)();
	
// -----------------------------------------------------------------------------
// ExitVideoProcessing_
// -----------------------------------------------------------------------------
// Called just after to exiting code that does video processing - you should
// pass in the 64 bit integer returned by EnterVideoProcessing_

#define	ExitVideoProcessing_(izzyParamsPtr, time) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mExitVideoProcessingProc)((time))

typedef void				(*ExitVideoProcessingProc)(
								UInt64 inStartTime);
	
// -----------------------------------------------------------------------------
// BroadcastControlValue_
// -----------------------------------------------------------------------------
// Broadcasts the the control value for the controller number specfied. The
// min, max, and current value are given, so that listeners can scale the
// values appropriately.

#define	BroadcastControlValue_(izzyParamsPtr, inControlInfo, inControlNum, inMin, inMax, inValue, inRedrawControl) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mBroadcastControlValueProc)((inControlInfo), (inControlNum), (inMin), (inMax), (inValue), (inRedrawControl))

typedef void	(*BroadcastControlValueProc)(
					ControlInfo*	inControlInfo,
					UInt32			inControlNum,
					float			inMin,
					float			inMax,
					float			inValue,
					Boolean			inRedrawControl);
	
// -----------------------------------------------------------------------------
// BroadcastControlValueDirect_
// -----------------------------------------------------------------------------
// Broadcasts the the control value for the controller number specfied. The
// "direct" version of this call prevents any scaling -- controls that are
// listenting to these messages will either limit or ignore the values to 
// the available range, depending on the setting of the inIgnoreOutOfRange
// parameter

#define	BroadcastControlValueDirect_(izzyParamsPtr, inControlInfo, inControlNum, inValue, inIgnoreOutOfRange, inRedrawControl)  \
	(*(izzyParamsPtr)->kConditionalCallbacks->mBroadcastControlValueDirectProc)((inControlInfo), (inControlNum), (inValue), (inIgnoreOutOfRange), (inRedrawControl))

typedef void	(*BroadcastControlValueDirectProc)(
					ControlInfo*	inControlInfo,
					UInt32			inControlNum,
					float			inValue,
					Boolean			inIgnoreOutOfRange,
					Boolean			inRedrawControl);
	
// -----------------------------------------------------------------------------
// GetMediaListFromDialog_
// -----------------------------------------------------------------------------
// Shows/edits a list of indexes into a media array -- allows the caller to 
// keep a list of items chosen from within the media types supported by the
// media window.
//
// The function returns true if the data was changed

#define	GetMediaIndexListFromDialog_(izzyParamsPtr, inControlInfo, mediaType, ioIndexHandle) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetMediaIndexListFromDialogProc)((inControlInfo), (mediaType), (ioIndexHandle))

typedef Boolean	(*GetMediaIndexListFromDialogProc)(
					ControlInfo* 				inControlInfo,
					MediaCode					inMediaType,
					Handle						ioIndexHandle);
	
// -----------------------------------------------------------------------------
// GetMediaObjectInfo_
// -----------------------------------------------------------------------------
// Gets information about a particular media object, including its name and 
// its thumbnail, if one exists.
//
//	The function returns true if the media object actually exists

#define	GetMediaObjectInfo_(izzyParamsPtr, inControlInfo, mediaType, index1, outName, inFullSizeImage, outImage, outDuration_mS) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetMediaObjectInfoProc)((inControlInfo), (mediaType), (index1), (outName), (inFullSizeImage), (outImage), (outDuration_mS))

typedef Boolean	(*GetMediaObjectInfoProc)(
					ControlInfo* 	inControlInfo,
					MediaCode		inMediaType,
					SInt32			inMediaObjectIndex1,	// the one-based media object index
					char*			outName,				// buffer should be at least 256 characters
					Boolean			outFullSizeImage,		// set to true to get image in full resolution
					ImageBufferPtr*	outImage,				// the object's thumbnail or full size image if it exists - otherwise outThumbnail is set to nil
					UInt32*			outDuration_mS);		// Can be NULL -- Duration of the object in mS
	
// -----------------------------------------------------------------------------
// TrackPopupMenu_
// -----------------------------------------------------------------------------
// Given a c-string with a list of carriage return terminated items,
// shows a popup menu with those items and let's the user choose.
// The return value is the one-based index of the item that the user
// chose. If the user doesn't choose an item, then the return value
// is zero.

#define	TrackPopupMenu_(izzyParamsPtr, inControlInfo, inItemString, inCurrentItemIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mTrackPopupMenuProc)((inControlInfo), (inItemString), (inCurrentItemIndex1))

typedef SInt32	(*TrackPopupMenuProc)(
					ControlInfo* 				inControlInfo,
					const char*					inItemString,
					SInt32						inCurrentItemIndex1);
	
// -----------------------------------------------------------------------------
// GetRandomSeed_
// -----------------------------------------------------------------------------
// Returns a number to be used when seeding a random number generator
//

#define	GetRandomSeed_(izzyParamsPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetRandomSeed)()

typedef UInt32	(*GetRandomSeedProc)();
	
// -----------------------------------------------------------------------------
// SetCaptureMode_
// -----------------------------------------------------------------------------
// Starts video and/or audio capture
//
// To enable, use the following flags when calling SetCaptureMode
//
//	Audio + Video	kIzzyCaptureVideo | kIzzyCaptureAudio
//	Vido			kIzzyCaptureVideo
//	Audio			kIzzyCaptureAudio
//
// To disable capture use kIzzyCaptureNone
//
// Returns true if successful, false otherwise
//

#define	SetCaptureMode_(izzyParamsPtr, inCaptureChannelIndex1, inMode, inVideoInputSelect, inAudioInputSelect, inSize) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetCaptureModeProc)((inCaptureChannelIndex1), (inMode), (inVideoInputSelect), (inAudioInputSelect), (inSize), false, 0,0,0,0,0,0)

#define	SetCaptureModeCustom_(izzyParamsPtr, inCaptureChannelIndex1, inMode, inVideoInputSelect, inAudioInputSelect, inSize, inUseCustom, inTop, inLeft, inWidth, inHeight, inDstWidth, inDstHeight) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetCaptureModeProc)((inCaptureChannelIndex1), (inMode), (inVideoInputSelect), (inAudioInputSelect), (inSize), (inUseCustom), (inTop), (inLeft), (inWidth), (inHeight), (inDstWidth), (inDstHeight))

typedef void	(*SetCaptureModeProc)(
					CaptureChannelIndex		inCaptureChannelIndex1,				// one based capture channel
					CaptureModeFlags		inMode,
					VideoCaptureInputType	inVideoInputSelect,
					AudioCaptureInputType	inAudioInputSelect,
					CaptureSize				inCaptureSize,
					Boolean					inUseCustomSize,
					float					inTop,				// 0 - 100 %
					float					inLeft,				// 0 - 100 %
					float					inWidth,			// 0 - 100 %
					float					inHeight,			// 0 - 100 %
					SInt32					inDstWidthPixels,
					SInt32					inDstHeightPixels);
	
// -----------------------------------------------------------------------------
// StartRecordCaptureInputToDisk_
// -----------------------------------------------------------------------------
// Starts recording video and/or audio to disk via the capture mechanism
//
// Before calling this function, you must have already enabled capturing for
// the specified channel by calling SetCaptureMode_
//
// inDestinationMovieIndex1 is the one based index where the new movie
// will be stored in the media window. An item must already exist in that
// location, or it must be the next available slot for this function to
// succeed. (i.e., if you specifiy index 5, there must already be an item
// in that index or you must have 4 items already in the list.)
//

#define	StartRecordCaptureInputToDisk_(izzyParamsPtr, actorInfo, inCaptureChannelIndex1, inDestinationMovieIndex1, inFileNameString) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mStartRecordCaptureInputToDiskProc)((actorInfo)->mThing, (inCaptureChannelIndex1), (inDestinationMovieIndex1), (inFileNameString))

typedef void	(*StartRecordCaptureInputToDiskProc)(
					ThingPtr				inThingPtr,
					CaptureChannelIndex		inCaptureChannelIndex,
					VideoMediaIndex			inDestinationMovieIndex,
					const char*				inFileNameString);			// can be NULL or empty string
	
// -----------------------------------------------------------------------------
// StopRecordCaptureInputToDisk_
// -----------------------------------------------------------------------------
// Starts recording video and/or audio to disk via the capture mechanism
//
// Before calling this function, you must have already enabled capturing for
// the specified channel by calling SetCaptureMode_
//
// inDestinationMovieIndex1 is the one based index where the new movie
// will be stored in the media window. An item must already exist in that
// location, or it must be the next available slot for this function to
// succeed. (i.e., if you specifiy index 5, there must already be an item
// in that index or you must have 4 items already in the list.)
//

#define	StopRecordCaptureInputToDisk_(izzyParamsPtr, actorInfo, inCaptureChannelIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mStopRecordCaptureInputToDiskProc)((actorInfo)->mThing, (inCaptureChannelIndex1))

typedef void	(*StopRecordCaptureInputToDiskProc)(
					ThingPtr				inThing,
					CaptureChannelIndex		inCaptureChannelIndex1);
	
// -----------------------------------------------------------------------------
// EncodeString
// -----------------------------------------------------------------------------
// Takes a 32 bit number and encodes it in a particular manner - so that 
// plugins can check to see if they are running in the isadora environment
//

#define	EncodeString_(izzyParamsPtr, inStr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mEncodeStringProc)(inStr)

typedef UInt32	(*EncodeStringProc)(
					const char*		inStr);
	
// -----------------------------------------------------------------------------
// GetUserName
// -----------------------------------------------------------------------------
// Returns the name of the user of this copy of Isadora.
//

#define	GetUserName_(izzyParamsPtr, outStr, inStrMaxLen) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetUserNameProc)((outStr), (inStrMaxLen))

typedef void	(*GetUserNameProc)(
					char*			outStr,
					UInt32			inStrMaxLen);
	
// -----------------------------------------------------------------------------
// BroadcastValue_
// -----------------------------------------------------------------------------
// Broadcasts the a pointer to a Value struct on the specified channel.

#define	BroadcastValue_(izzyParamsPtr, inActorInfo, inOverLan, inMachineID, inChannel, inValuePtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mBroadcastValueProc)((inActorInfo)->mThing, false, 0, (inChannel), (inValuePtr))

typedef void	(*BroadcastValueProc)(
					ThingPtr		inThingPtr,
					Boolean			inOverLAN,
					UInt32			inMachineID,
					UInt32			inChannel,
					const ValuePtr	inValuePtr);
	
// -----------------------------------------------------------------------------
// BroadcastValueFromControl_
// -----------------------------------------------------------------------------
// Broadcasts the a pointer to a Value struct on the specified channel.

#define	BroadcastValueFromControl_(izzyParamsPtr, inControlInfo, inOverLan, inMachineID, inChannel, inValuePtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mBroadcastValueFromControlProc)((inControlInfo), (inOverLan), (inMachineID), (inChannel), (inValuePtr))

typedef void	(*BroadcastValueFromControlProc)(
					ControlInfo*	inControlInfo,
					Boolean			inOverLAN,
					UInt32			inMachineID,
					UInt32			inChannel,
					const ValuePtr	inValuePtr);
	
// -----------------------------------------------------------------------------
// BroadcastValueOverLAN_
// -----------------------------------------------------------------------------
// Broadcasts the a pointer to a Value struct on the specified channel.

#define	BroadcastValueOverLAN_(izzyParamsPtr, inActorInfo, inMachineID, inChannel, inValuePtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mBroadcastValueProc)((inActorInfo)->mThing, true, (inMachineID), (inChannel), (inValuePtr))

// -----------------------------------------------------------------------------
// IPAddressStringToInetAddress
// -----------------------------------------------------------------------------
// Sends a single integer value via Open Sound Countrol

#define	IPAddressStringToInetAddress_(izzyParamsPtr, inIPAddr, outInetAddr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mIPAddressStringToInetAddressProc)((inIPAddr), (outInetAddr))

typedef Boolean	(*IPAddressStringToInetAddressProc)(
					const char*		inIPAddress,
					UInt32*			outInetAddress);
	
// -----------------------------------------------------------------------------
// SendOSCValues_
// -----------------------------------------------------------------------------
// Sends a multiple integers or floats, as contained in an array of Value structs
// via open Sound Control

#define	SendOSCValues_(izzyParamsPtr, inIPAddr, inPort, inOSCAddr, inSendType, inValueCount, inValueArray) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSendOSCValuesProc)((inIPAddr), (inPort), (inOSCAddr), (inSendType), (inValueCount), (inValueArray))

typedef void	(*SendOSCValuesProc)(
					UInt32			inIPAddr,
					UInt16			inPort,
					const char*		inOSCAddr,
					Boolean			inSendType,
					SInt32			inValueCount,
					const Value*	inValueArray);
	
// -----------------------------------------------------------------------------
// SendLanBoxValues_
// -----------------------------------------------------------------------------
// Sends multiple channels of data to a LanBox LC at the specified UDP address
// and port

#define	SendLanBoxValues_(izzyParamsPtr, inIPAddr, inPort, inMsgType, inTargetBufferID, inChannelOffset, inChannelCount, inChannelArray) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSendLanBoxProc)((inIPAddr), (inPort), (inMsgType), (inTargetBufferID), (inChannelOffset), (inChannelCount), (inChannelArray))

typedef void	(*SendLanBoxProc)(
					UInt32			inIPAddr,
					UInt16			inPort,
					UInt16			inMsgType,
					UInt16			inTargetBufferID,
					UInt16			inChannelOffset,
					SInt32			inChannelCount,
					const UInt8*	inChannelArray);
	
// -----------------------------------------------------------------------------
// BroadcastStreamingData
// -----------------------------------------------------------------------------
// Broadcasts streaming data of the specified type to the specified port.

#define	BroadcastStreamingData_(izzyParamsPtr, inStreamingDataTypeAndPort, inData, inDataLength) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mBroadcastStreamingDataProc)((inStreamingDataTypeAndPort), (inData), (inDataLength))

typedef void	(*BroadcastStreamingDataProc)(
					UInt32			inStreamingDataTypeAndPort,
					const void*		inData,
					UInt32			inDataLen);
	
// -----------------------------------------------------------------------------
// StartRecordingLanBoxDMXInput
// StopRecordingLanBoxDMXInput
// -----------------------------------------------------------------------------
// Starts or stops recording the LanBox DMX Input channels

#define	StartRecordingLanBoxDMXInput(izzyParamsPtr, inAddress, inPort, inBufferID, inPassword, inChannelOffset, inChannelCount, inFileName) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mStartStopRecordingLanBoxDMXInputProc)(TRUE, (inAddress), (inPort), (inBufferID), (inPassword), (inChannelOffset), (inChannelCount), (inFileName))

#define	StopRecordingLanBoxDMXInput(izzyParamsPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mStartStopRecordingLanBoxDMXInputProc)(FALSE, NULL, 0, 0, NULL, 0, 0, NULL)

typedef void	(*StartStopRecordingLanBoxDMXInputProc)(
					Boolean			inRecord,
					const char*		inServerAddress,
					UInt16			inServerPort,
					UInt16			inBufferID,
					const char*		inPassword,
					UInt32			inChannelOffset,
					SInt32			inChannelCount,
					const char*		inFileName);
	
// -----------------------------------------------------------------------------
// EnableDisableMIDI_
// -----------------------------------------------------------------------------
// Enables or disables MIDI input and output for the specified port.

#define	EnableDisableMIDI_(izzyParamsPtr, inActorInfo, inEnable, inPort) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mEnableDisableMIDIProc)((inActorInfo)->mThing, (inEnable), (inPort))

typedef void	(*EnableDisableMIDIProc)(
					ThingPtr		inThingPtr,
					Boolean			inEnable,
					PortIndex		inPortIndex1);		// 0 = all ports, > 0 = enable/disable the specified port
	
// -----------------------------------------------------------------------------
// ShowNumericValueEditBox_
// -----------------------------------------------------------------------------
// Shows a value edit box that allows the user to edit a number

typedef void (*NumericValueChangedProc)(IsadoraParameters* ip, ValuePtr inNewValue, SInt32 inRefCon);

#define	ShowNumericValueEditBox_(izzyParamsPtr, inCtlInfo, inDispRect, inCallback, inRefCon, inMin, inMax, inValue, inAlignment, inMaxDecimalPoints) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mShowNumericValueEditBoxProc)((inCtlInfo), (inDispRect), (inCallback), (inRefCon), (inMin), (inMax), (inValue), (inAlignment), (inMaxDecimalPoints))

typedef void	(*ShowNumericValueEditBoxProc)(
					ControlInfo* 				inControlInfo,
					const Rect*					inDisplayRect,
					NumericValueChangedProc		inProc,
					SInt32						inRefCon,
					ValuePtr					inMin,
					ValuePtr					inMax,
					ValuePtr					inValue,
					SInt32						inAlignment, 			// -1 = left, 0 = center, +1 = right
					SInt32						inMaxDecimalPoints);
	
// -----------------------------------------------------------------------------
// DrawPascalString_
// -----------------------------------------------------------------------------
//	Draws a pascal string into the current graphics port.
//
//	Because of issues with QuickTime under windows, you should use this function
//	to draw text in your actor's Actor Defined Area or in your Custom Control.

/*
DEPRECATED! Use DrawTextAt_
#define	DrawPascalString_(izzyParamsPtr, inPString) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawPascalStringProc)((inPString))

typedef void	(*DrawPascalStringProc)(
					StringPtr			inString);
*/

// -----------------------------------------------------------------------------
// DrawCString_
// -----------------------------------------------------------------------------
//	Draws a c-string into the current graphics port
//
//	Because of issues with QuickTime under windows, you should use this function
//	to draw text in your actor's Actor Defined Area or in your Custom Control.

/*
DEPRECATED! Use DrawTextAt_
#define	DrawCString_(izzyParamsPtr, inCString) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawCStringProc)((inCString))

typedef void	(*DrawCStringProc)(
					const char*			inString);
*/

// -----------------------------------------------------------------------------
// DrawText_
// -----------------------------------------------------------------------------
//	Draws the specified text into the current graphics port
//
//	Because of issues with QuickTime under windows, you should use this function
//	to draw text in your actor's Actor Defined Area or in your Custom Control.

/*
DEPRECATED! Use DrawTextAt_
#define	DrawText_(izzyParamsPtr, inCString, inLength) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawTextProc)((inCString), (inLength))

typedef void	(*DrawTextProc)(
					const char*			inString,
					SInt32				inLength);
*/

// -----------------------------------------------------------------------------
// TextWidthPascalString_
// -----------------------------------------------------------------------------
//	Returns the width in pixels of a pascal string for the current graphics port.
//
//	Because of issues with QuickTime under Windows, you should use this function
//	to measure text in your actor's Actor Defined Area or in your Custom Control.

/*

DEPRECATED! Use GetTextWidth_

#define	TextWidthPascalString_(izzyParamsPtr, inPString) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mTextWidthPascalStringProc)((inPString))

typedef SInt16	(*TextWidthPascalStringProc)(
					StringPtr			inString);
*/

// -----------------------------------------------------------------------------
// TextWidthCString_
// -----------------------------------------------------------------------------
//	Returns the width in pixels of a c string for the current graphics port.
//
//	Because of issues with QuickTime under windows, you should use this function
//	to draw text in your actor's Actor Defined Area or in your Custom Control.

/*

DEPRECATED! Use GetTextWidth_

#define	TextWidthCString_(izzyParamsPtr, inCString) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mTextWidthCStringProc)((inCString))

typedef SInt16	(*TextWidthCStringProc)(
					const char*			inString);
#endif
*/

// -----------------------------------------------------------------------------
// TextWidth_
// -----------------------------------------------------------------------------
//	Returns the width in pixels of a a block of text for the current graphics port.
//
//	Because of issues with QuickTime under windows, you should use this function
//	to draw text in your actor's Actor Defined Area or in your Custom Control.

/*

DEPRECATED! Use GetTextWidth_

#define	TextWidth_(izzyParamsPtr, inCString, inLength) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mTextWidthProc)((inCString), (inLength))

typedef SInt16	(*TextWidthProc)(
					const char*			inString,
					SInt32				inLength);
*/

// -----------------------------------------------------------------------------
// GetBinNameAndUniqueIDFromIndex_
// -----------------------------------------------------------------------------
// Returns the name of the bin at the specified one based index. If no bin
// exists at that location, the result is false

#define	GetBinNameAndUniqueIDFromIndex_(izzyParamsPtr, inControlInfo, inMediaType, inBinIndex1, outBinName, outBinUniqueID) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetBinNameAndUniqueIDFromIndexProc)((inControlInfo), (inMediaType), (inBinIndex1), (outBinName), (outBinUniqueID))

typedef Boolean	(*GetBinNameAndUniqueIDFromIndexProc)(
					ControlInfo* 	inControlInfo,
					MediaCode		inMediaType,
					UInt32			inBinIndex1,		// one-based index
					char*			outBinName,			// buffer should be at least 256 characters
					UInt32*			outBinUniqueID);
	
// -----------------------------------------------------------------------------
// GetBinNameAndTypeFromUniqueID_
// -----------------------------------------------------------------------------
// Returns the name of the bin with the specified unique ID

#define	GetBinNameAndTypeFromUniqueID_(izzyParamsPtr, inControlInfo, inBinUniqueID, outBinName, outMediaType) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetBinNameAndTypeFromUniqueIDProc)((inControlInfo), (inBinUniqueID), (outBinName), (outMediaType))

typedef Boolean	(*GetBinNameAndTypeFromUniqueIDProc)(
					ControlInfo* 	inControlInfo,
					UInt32			inBinUniqueID,		// one-based index
					char*			outBinName,			// buffer should be at least 256 characters
					MediaCode*		outMediaType);
	
// -----------------------------------------------------------------------------
// CountMediaObjectsInBin_
// -----------------------------------------------------------------------------
// Returns the number of objects within a named bin in the Media Window.
//
// The function searches the Media Window for a bin that matches the specified
// name and media type. If one is found, the result is the number of objects
// in that bin. If the bin can't be found, the result is 0.

#define	CountMediaObjectsInBin_(izzyParamsPtr, inControlInfo, inBinUniqueID) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mCountMediaObjectsInBinProc)((inControlInfo), (inBinUniqueID))

typedef SInt32	(*CountMediaObjectsInBinProc)(
					ControlInfo* 	inControlInfo,
					UInt32			inBinUniqueID);
	
// -----------------------------------------------------------------------------
// GetInfoForIndexedBin_
// -----------------------------------------------------------------------------
// Returns the name, unique ID, and media object count for a specified bin

#define	GetInfoForIndexedBin_(izzyParamsPtr, inActorInfo, inMediaType, inBinIndex1, outBinName, outBinUniqueID, outMediaObjectCount) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetInfoForIndexedBinProc)((inActorInfo), (inMediaType), (inBinIndex1), (outBinName), (outBinUniqueID), (outMediaObjectCount))

typedef Boolean	(*GetInfoForIndexedBinProc)(
					ActorInfo* 		inActorInfo,
					MediaCode		inMediaType,
					UInt32			inBinIndex1,		// one-based index
					char*			outBinName,			// buffer should be at least 256 characters
					UInt32*			outBinUniqueID,
					UInt32*			outMediaObjectCount);
	
// -----------------------------------------------------------------------------
// GetMediaObjectInfoFromBin_
// -----------------------------------------------------------------------------
// Gets information about a media object within a specified bin.
//
// If a bin with the specified unique ID exists, and if a media object
// at the specified index exists within that bin, then the result is true and
// the media object index, name and thumbnail for the requested object are
// returned.
//
// If the bin or media object do not exist, the result is false.

#define	GetMediaObjectInfoFromBin_(izzyParamsPtr, inControlInfo, inBinUniqueID, inObjectIndexWithinBin1, outMediaObjectIndex1, outMediaName, outThumbnail) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetMediaObjectInfoFromBinProc)((inControlInfo), (inBinUniqueID), (inObjectIndexWithinBin1), (outMediaObjectIndex1), (outMediaName), (outThumbnail))

typedef Boolean	(*GetMediaObjectInfoFromBinProc)(
					ControlInfo* 	inControlInfo,
					UInt32			inBinUniqueID,				// the unique ID of the bin
					UInt32			inObjectIndexWithinBin1,	// one based index of item within its bin
					UInt32*			outMediaObjectIndex1,		// the media object index for this item
					char*			outName,					// buffer to hold name should be 256 characters
					ImageBufferPtr*	outThumbnail);				// the object's thumbnail if it exists - otherwise outThumbnail is set to nil
	
// -----------------------------------------------------------------------------
// GetMediaIndexForItemInBin
// -----------------------------------------------------------------------------
// Returns the absolute media index of the specified media object within the
// specified bin

#define	GetMediaIndexForItemInBin_(izzyParamsPtr, inActorInfo, inMediaType, inBinIndex1, inMediaIndex1, outMediaIndex1) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetMediaIndexForItemInBinProc)((inActorInfo), (inMediaType), (inBinIndex1), (inMediaIndex1), (outMediaIndex1))

typedef Boolean	(*GetMediaIndexForItemInBinProc)(
					ActorInfo* 		inActorInfo,
					MediaCode		inMediaType,
					UInt32			inBinIndex1,		// one-based index
					UInt32			inMediaIndex1,
					UInt32*			outMediaIndex1);
	
// -----------------------------------------------------------------------------
// ConvertTimeToPercentForMediaObject
// -----------------------------------------------------------------------------
// Receives an array of millisecond values, and converts them to a percentage
// of the objects duration.

#define	ConvertTimeToPercentForMediaObject_(izzyParamsPtr, inActorInfo, inMediaType, inBinIndex1, inMediaIndex1, inTimeArraySize, inMillisecondTimeArray, outPercentages) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mConvertTimeToPercentForMediaObject)((inActorInfo), (inMediaType), (inBinIndex1), (inMediaIndex1), (inTimeArraySize), (inMillisecondTimeArray), (outPercentages))

typedef Boolean	(*ConvertTimeToPercentForMediaObjectProc)(
					ActorInfo* 		inActorInfo,
					MediaCode		inMediaType,
					UInt32			inBinIndex1,			// one-based index: 0 = use inMediaIndex only - ignore bin
					UInt32			inMediaIndex1,			// one-based index of the media within the bin, or absolute index if inBinIndex1 is 0
					UInt32			inTimeArraySize,		// size of array of mS times
					const UInt32*	inMillisecondTimeArray,	// array of mS times
					float*			outPercentages);		// array of percentages from 0 to 100%
	
// -----------------------------------------------------------------------------
// ConvertPercentToTimeForMediaObject
// -----------------------------------------------------------------------------
// Receives an array of millisecond values, and converts them to a percentage
// of the objects duration.

#define	ConvertPercentToTimeForMediaObject_(izzyParamsPtr, inActorInfo, inMediaType, inBinIndex1, inMediaIndex1, inTimeArraySize, inPercentages, outMillisecondTimeArray) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mConvertPercentToTimeForMediaObject)((inActorInfo), (inMediaType), (inBinIndex1), (inMediaIndex1), (inTimeArraySize), (inPercentages), (outMillisecondTimeArray))

typedef Boolean	(*ConvertPercentToTimeForMediaObjectProc)(
					ActorInfo* 		inActorInfo,
					MediaCode		inMediaType,
					UInt32			inBinIndex1,			// one-based index: 0 = use inMediaIndex only - ignore bin
					UInt32			inMediaIndex1,			// one-based index of the media within the bin, or absolute index if inBinIndex1 is 0
					UInt32			inTimeArraySize,		// size of array of mS times
					const float*	outPercentages,			// array of percentages from 0 to 100%
					UInt32*			inMillisecondTimeArray);// array of mS times
	
// -----------------------------------------------------------------------------
// EnableDisableActors_
// -----------------------------------------------------------------------------
// Gets information about a media object within a specified bin.
//
// If a bin with the specified unique ID exists, and if a media object
// at the specified index exists within that bin, then the result is true and
// the media object index, name and thumbnail for the requested object are
// returned.
//
// If the bin or media object do not exist, the result is false.

#define	EnableDisableActors_(izzyParamsPtr, inActorInfo, inEnable) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mEnableDisableActorsProc)((inActorInfo), (inEnable))

typedef void	(*EnableDisableActorsProc)(
					ActorInfo* 		inActorInfo,
					Boolean			inEnable);
	
// -----------------------------------------------------------------------------
// OutputTextFile_
// -----------------------------------------------------------------------------
//	Writes the data pointed to by dataPtr to a text file. If you want the file
//	to appear on the desktop, then pass a file name in 'fileName'. Otherwise,
//	pass nil for fileName, and a standard dialog will appear allowing the user
//	to choose the save location
//
#define	OutputTextFile_(izzyParamsPtr, fileName, dataPtr, dataLen, deleteExisting, appendDate, appendTime) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mOutputTextFileProc)((fileName), (dataPtr), (dataLen), (deleteExisting), (appendDate), (appendTime))

typedef Boolean	(*OutputTextFileProc)(
					const char*		inFileName,
					const void*		inDataPtr,
					SInt32			inLen,
					Boolean			inDeleteExistingFile,
					Boolean			inAppendDate,
					Boolean			inAppendTime);
	
// -----------------------------------------------------------------------------
// WriteToLogFile_
// -----------------------------------------------------------------------------
//	Writes the c-string to the log file associated with the document that 
//	owns the actor making this request. The log file will be writtent to
//	the same folder as the document.
//
#define	WriteToLogFile_(izzyParamsPtr, inActorInfo, inEventID, inCString) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mWriteToLogFileProc)(((inActorInfo)->mThing), (inEventID), (inCString))

typedef void	(*WriteToLogFileProc)(
					ThingPtr						inThingPtr,
					SInt32							inEventID,
					const char*						inCString);
	
// -----------------------------------------------------------------------------
// WriteToSerialPort_
// -----------------------------------------------------------------------------
//	Write data to the serial port

#define	WriteToSerialPort_(izzyParamsPtr, inActorInfo, inPortIndex1, inDataLen, inData) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mWriteToSerialPortProc)((inActorInfo), (inPortIndex1), (inDataLen), (inData))

typedef UInt32	(*WriteToSerialPortProc)(				// returns bytes written
					ActorInfo*			inActorInfo,
					SerialStreamIndexT	inPortIndex1,	// one based serial port index
					UInt32				inDataLen,		// length of data in bytes
					const void*			inData);		// pointer to the data
	
// -----------------------------------------------------------------------------
// ReadFromSerialPort_
// -----------------------------------------------------------------------------
//	Read data from the serial port

#define	ReadFromSerialPort_(izzyParamsPtr, inActorInfo, inPortIndex1, inDataLen, inData) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mReadFromSerialPortProc)((inActorInfo), (inPortIndex1), (inDataLen), (inData))

typedef UInt32	(*ReadFromSerialPortProc)(				// returns bytes read
					ActorInfo*			inActorInfo,
					SerialStreamIndexT	inPortIndex1,	// one based serial port index
					UInt32				inDataLen,		// size of data buffer to receive data
					void*				inData);		// pointer to the data buffer
	
// -----------------------------------------------------------------------------
// SetTCPStreamInfo_
// -----------------------------------------------------------------------------
//	Sets the TCP address, port, and buffer size for the specified TCP stream

#define	SetTCPStreamInfo_(izzyParamsPtr, inActorInfo, inStreamIndex1, inTCPAddressStr, inTCPPort, inBufferSize) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetTCPStreamInfoProc)((inActorInfo), (inStreamIndex1), (inTCPAddressStr), (inTCPPort), (inBufferSize))

typedef Boolean	(*SetTCPStreamInfoProc)(					// returns TRUE if successful
					ActorInfo*		inActorInfo,
					TCPStreamIndexT	inStreamIndex1,			// one based TCP stream Index
					const char*		inTCPAddressStr,		// tcp address (string)
					UInt16			inTCPPort,				// tcp port number
					UInt32			inBufferSize);			// tcp buffer size
	
// -----------------------------------------------------------------------------
// EnableDisableTCPStream
// -----------------------------------------------------------------------------
//	Enables or disables a TCP port; enabling opens the connection; disabling
//	closes it

#define	EnableDisableTCPStream_(izzyParamsPtr, inActorInfo, inStreamIndex1, inEnable) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mEnableDisableTCPStreamProc)((inActorInfo), (inStreamIndex1), (inEnable))

typedef Boolean	(*EnableDisableTCPStreamProc)(				// returns TRUE if successful
					ActorInfo*		inActorInfo,
					TCPStreamIndexT	inStreamIndex1,			// one based TCP stream Index
					Boolean			inEnable);				// pointer to the data
	
// -----------------------------------------------------------------------------
// WriteToTCPStream_
// -----------------------------------------------------------------------------
//	Write data to the TCP port

#define	WriteToTCPStream_(izzyParamsPtr, inActorInfo, inStreamIndex1, inDataLen, inData) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mWriteToTCPStreamProc)((inActorInfo), (inStreamIndex1), (inDataLen), (inData))

typedef UInt32	(*WriteToTCPStreamProc)(					// returns bytes written
					ActorInfo*		inActorInfo,
					TCPStreamIndexT	inStreamIndex1,			// one based TCP stream Index
					UInt32			inDataLen,				// length of data in bytes
					const void*		inData);				// pointer to the data
	
// -----------------------------------------------------------------------------
// ReadFromTCPStream_
// -----------------------------------------------------------------------------
//	Read data from the TCP port

#define	ReadFromTCPStream_(izzyParamsPtr, inActorInfo, inStreamIndex1, inDataLen, inData, inTimeSeconds) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mReadFromTCPStreamProc)((inActorInfo), (inStreamIndex1), (inDataLen), (inData), (inTimeSeconds))

typedef UInt32	(*ReadFromTCPStreamProc)(					// returns bytes actually read
					ActorInfo*		inActorInfo,
					TCPStreamIndexT	inStreamIndex1,			// one based TCP stream Index
					UInt32			inDataLen,				// size of data buffer to receive data
					void*			inData,					// pointer to the data buffer
					float			inTimeout);				// number of seconds to wait before returning
	
// -----------------------------------------------------------------------------
// SetLocalTranslationAndRotation_
// -----------------------------------------------------------------------------
//	Sets the translation and rotation for OpenGL rendering for the currently
//	active scene. This setting does not affect other scenes.

#define	SetLocalTranslationAndRotation_(izzyParamsPtr, inActorInfo, inChannel1, transX, transY, transZ, rotX, rotY, rotZ) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetLocalTranslationAndRotationProc)(((inActorInfo)->mThing), (inChannel1), (transX), (transY), (transZ), (rotX), (rotY), (rotZ))

typedef void	(*SetLocalTranslationAndRotationProc)(
					ThingPtr	inActorInfo,
					SInt32		inChannelIndex1,
					float		inTranslateX,
					float		inTranslateY,
					float		inTranslateZ,
					float		inRotateX,
					float		inRotateY,
					float		inRotateZ);
	
// -----------------------------------------------------------------------------
// SetLocalLighting_
// -----------------------------------------------------------------------------
//	Sets the 3D Lighting orientation for the scene that owns the calling actor.

#define	SetLocalLighting_(izzyParamsPtr, inActorInfo, inChannel1, lightingInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetLocalLightingProc)(((inActorInfo)->mThing), (inChannel1), (lightingInfo))

typedef void	(*SetLocalLightingProc)(
					ThingPtr					inActorInfo,
					SInt32						inChannelIndex1,
					const LightingInfo*			inInfo);
	
// -----------------------------------------------------------------------------
// SetLocalScalingOptions
// -----------------------------------------------------------------------------
//	Sets the Stage Scaling Options for the scene that owns the calling actor.

#define	SetLocalScalingOptions_(izzyParamsPtr, inActorInfo, inChannel1, scalingOptions) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetLocalScalingOptionsProc)(((inActorInfo)->mThing), (inChannel1), (scalingOptions))

typedef void	(*SetLocalScalingOptionsProc)(
					ThingPtr						inThingPtr,
					SInt32							inChannelIndex1,
					const StageScalingOptions*		inInfo);
	
// -----------------------------------------------------------------------------
// SetLocalStageBackgroundOptions
// -----------------------------------------------------------------------------
//	Sets the Stage Background Options for the scene that owns the calling actor.

/*
#define	SetLocalStageBackgroundOptions_(izzyParamsPtr, inActorInfo, inChannel1, stageBackgroundOptions) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetLocalStageBackgroundOptionsProc)(((inActorInfo)->mThing), (inChannel1), (stageBackgroundOptions))

typedef void	(*SetLocalStageBackgroundOptionsProc)(
					ThingPtr						inThingPtr,
					SInt32							inChannelIndex1,
					const StageBackgroundOptions*	inInfo);
*/

// -----------------------------------------------------------------------------
// AllocateValueString_
// -----------------------------------------------------------------------------
//	Allocates a Value object that points to a string. It is the caller's
//	responsibiltiy to release the string using the ReleaseValueString_
//	function.

#define	AllocateValueString_(izzyParamsPtr, inString, outValue) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mAllocateValueStringProc)((inString), (outValue))

typedef void	(*AllocateValueStringProc)(	
					const char*		inString,	// pointer to a null terminated string
					ValuePtr		outValue);
	
// -----------------------------------------------------------------------------
// ReleaseValueString_
// -----------------------------------------------------------------------------
//	Releases a string allocated with the AllocateValueString_ function

#define	ReleaseValueString_(izzyParamsPtr, inValue) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mReleaseValueStringProc)(inValue)

typedef void	(*ReleaseValueStringProc)(	
					ValuePtr		outValue);
	
// -----------------------------------------------------------------------------
// ControlAcquireImage_
// -----------------------------------------------------------------------------
//	Allows a control to acquire a media object in the media window, and returns
//	a GWorldPtr to the bitmap associated with that piece of media. Currently
//	this should only be used for pictures.

#define	ControlAcquireImage_(izzyParamsPtr, inCtlInfo, inMediaType, inMediaIndex1, inID, outImageBuffer) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mControlAcquireImageProc)((inCtlInfo), (inMediaType), (inMediaIndex1), (inID), (outImageBuffer))

typedef void	(*ControlAcquireImageProc)(
					ControlInfo*	inCtlInfo,
					MediaCode		inMediaType,		// should always be kMediaTypePicture
					UInt32			inMediaIndex1,		// media index of the piece of media
					AcquireIDT		inID,				// unique acquire ID -- often the pointer to the control object
					ImageBufferPtr*	outImageBuffer);	// resulting GWorldPtr with the image data
	
// -----------------------------------------------------------------------------
// ControlReleaseImage_
// -----------------------------------------------------------------------------
//	Releases the media acquired using the ControlAcquireImage_ function above

#define	ControlReleaseImage_(izzyParamsPtr, inCtlInfo, inMediaType, inMediaIndex1, inID) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mControlReleaseImageProc)((inCtlInfo), (inMediaType), (inMediaIndex1), (inID))

typedef void	(*ControlReleaseImageProc)(
					ControlInfo*	inCtlInfo,
					MediaCode		inMediaType,
					UInt32			inMediaIndex1,
					AcquireIDT		inID);
	
// -----------------------------------------------------------------------------
// GetActorDefaultPixelFormat_
// -----------------------------------------------------------------------------
//	Get the default pixel format for the specified Actor.

#define	GetActorDefaultPixelFormat_(izzyParamsPtr, inActorInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetActorDefaultPixelFormatProc)((inActorInfo))

typedef OSType	(*GetActorDefaultPixelFormatProc)(
					ActorInfo*		inActorInfo);
	
// -----------------------------------------------------------------------------
// GetActorGraphics_
// -----------------------------------------------------------------------------
//	Get the Graphics Context for the specified Actor. This object is required
//	when using the graphics routine that follow

#define	GetActorGraphics_(izzyParamsPtr, inActorInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetActorGraphicsProc)((inActorInfo))

typedef UGraphics*	(*GetActorGraphicsProc)(
						ActorInfo*		inActorInfo);
	
// -----------------------------------------------------------------------------
// GetControlGraphics_
// -----------------------------------------------------------------------------
//	Get the Graphics Context for the specified Control so that it can draw
//	into the current window. Callers should use GetControlGraphics_ when
//	responding to an update event via the ControlDraw callback, and use
//	GetControlGraphicsForceRefocus_ when drawing from within a Message Receiver
//	callback. (This is because the graphics context may not be focused properly
//	during such a callback.)

#define	GetControlGraphics_(izzyParamsPtr, inControlInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetControlGraphicsProc)((inControlInfo), false)

#define	GetControlGraphicsForceRefocus_(izzyParamsPtr, inControlInfo) \
(*(izzyParamsPtr)->kConditionalCallbacks->mGetControlGraphicsProc)((inControlInfo), true)
	
typedef UGraphics*	(*GetControlGraphicsProc)(
						ControlInfo*		inControlInfo,
						bool				inForceFocus);
	
// -----------------------------------------------------------------------------
// SetForegroundColorRef_
// -----------------------------------------------------------------------------
//	Sets the current foreground color
#define	SetForegroundColorRef_(izzyParamsPtr, inGraphics, inColorRef) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetForegroundColorRefProc)((inGraphics), (inColorRef))

typedef void	(*SetForegroundColorRefProc)(
					UGraphics*	inGraphics,
					ColorRef	inColorRef);
	
// -----------------------------------------------------------------------------
// DrawLine_
// -----------------------------------------------------------------------------
//	Draws a line from x1/y1 to x2/y2 in the current foreground color

#define	DrawLine_(izzyParamsPtr, inGraphics, x1, y1, x2, y2, inWidth) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawLineProc)((inGraphics), (x1), (y1), (x2), (y2), (inWidth))

typedef void	(*DrawLineProc)(
					UGraphics*	inGraphics,
					SInt16		x1,
					SInt16		y1,
					SInt16		x2,
					SInt16		y2,
					SInt16		inWidth);
	
// -----------------------------------------------------------------------------
// PaintRect_/FrameRect_
// -----------------------------------------------------------------------------
//	Each of these variations allows an Actor or Control to draw the specified
//	shape

// fill a rectangle with the current foreground color
#define	PaintRect_(izzyParamsPtr, inGraphics, inRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPaintRectProc)((inGraphics), (inRect), 0, 0, 0)

// draw the outline of a rectangle with the current foreground color
#define	FrameRect_(izzyParamsPtr, inGraphics, inRect, inSize) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPaintRectProc)((inGraphics), (inRect), (inSize), 0, 0)

// fill a rounded rectangle with the current foreground color
#define	PaintRoundRect_(izzyParamsPtr, inGraphics, inRect, inRoundRectCornerH, inRoundRectCornerV) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPaintRectProc)((inGraphics), (inRect), 0, (inRoundRectCornerH), (inRoundRectCornerV))

// draw the outline of a rounded rectangle with the current foreground color
#define	FrameRoundRect_(izzyParamsPtr, inGraphics, inRect, inSize, inRoundRectCornerH, inRoundRectCornerV) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPaintRectProc)((inGraphics), (inRect), (inSize), (inRoundRectCornerH), (inRoundRectCornerV))

// fill an oval with the current foreground color
#define	PaintOval_(izzyParamsPtr, inGraphics, inRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPaintRectProc)((inGraphics), (inRect), 0, -1, -1)

// draw the outline of an oval with the current foreground color
#define	FrameOval_(izzyParamsPtr, inGraphics, inRect, inSize) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPaintRectProc)((inGraphics), (inRect), (inSize), -1, -1)

typedef void	(*PaintRectProc)(
					UGraphics*	inGraphics,
					const Rect*	inRect,
					SInt32		inFrameSize,
					SInt32		inRoundRectCornerH,		// set these last two to 0 for a normal rect, -1 for an oval, > 0 for round rect
					SInt32		inRoundRectCornerV);
					
// -----------------------------------------------------------------------------
// DrawGWorld
// -----------------------------------------------------------------------------
//	Draws a GWorld into the specified graphics context

#define GetGWorldBounds_(izzyParamsPtr, inGraphics, inGWorld, outRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetGWorldBoundsProc)((inGraphics), (inGWorld), (outRect))

typedef void	(*GetGWorldBoundsProc)(
					UGraphics*	inGraphics,
					GWorldPtr	inGWorld,
					Rect*		outRec);
					
#define	DrawGWorld_(izzyParamsPtr, inGraphics, inGWorld, dstRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawGWorldProc)((inGraphics), (inGWorld), nil, (dstRect), false)

#define	DrawAlphaGWorld_(izzyParamsPtr, inGraphics, inGWorld, dstRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawGWorldProc)((inGraphics), (inGWorld), nil, (dstRect), true)

#define	DrawGWorldSection_(izzyParamsPtr, inGraphics, inGWorld, srcRect, dstRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawGWorldProc)((inGraphics), (inGWorld), (srcRect), (dstRect), false)

#define	DrawAlphaGWorldSection_(izzyParamsPtr, inGraphics, inGWorld, srcRect, dstRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawGWorldProc)((inGraphics), (inGWorld), (srcRect), (dstRect), true)

typedef void	(*DrawGWorldProc)(
					UGraphics*	inGraphics,
					GWorldPtr	inGWorld,
					const Rect*	inSrcRect,
					const Rect*	inDstRect,
					Boolean		inHasAlpha);
					
#define	DrawImageBuffer_(izzyParamsPtr, inGraphics, inImageBufferPtr, dstRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawImageBufferProc)((inGraphics), (inImageBufferPtr), nil, (dstRect), false)

#define	DrawAlphaImageBuffer_(izzyParamsPtr, inGraphics, inImageBufferPtr, dstRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawImageBufferProc)((inGraphics), (inImageBufferPtr), nil, (dstRect), true)

#define	DrawImageBufferSection_(izzyParamsPtr, inGraphics, inImageBufferPtr, srcRect, dstRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawImageBufferProc)((inGraphics), (inImageBufferPtr), (srcRect), (dstRect), false)

#define	DrawAlphaImageBufferSection_(izzyParamsPtr, inGraphics, inImageBufferPtr, srcRect, dstRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawImageBufferProc)((inGraphics), (inImageBufferPtr), (srcRect), (dstRect), true)

typedef void	(*DrawImageBufferProc)(
					UGraphics*		inGraphics,
					ImageBufferPtr	inImageBufferPtr,
					const Rect*		inSrcRect,
					const Rect*		inDstRect,
					Boolean			inHasAlpha);
					
#define	DrawPicture_(izzyParamsPtr, inGraphics, inPicture, inRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawPictureProc)((inGraphics), (inPicture), (inRect), false)

typedef void	(*DrawPictureProc)(
					UGraphics*	inGraphics,
					PicHandle	inPicture,
					const Rect*	inRect,
					Boolean		inHasAlpha);
					

#define	PushAndSetRectClip_(izzyParamsPtr, inGraphics, inRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPushPopSetClipProc)((inGraphics), true, (inRect))

#define	PopClip_(izzyParamsPtr, inGraphics) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPushPopSetClipProc)((inGraphics), false, nil)

typedef void	(*PushPopSetClipProc)(
					UGraphics*	inGraphics,
					Boolean		inPush,
					const Rect*	inRect);		// can be nil if poping clip
					

// -----------------------------------------------------------------------------
// PaintPoly/FramePoly
// -----------------------------------------------------------------------------
//	Given an array of points, fills, frames or fills and frames the specified
//	region.

// fill a poly with the current foreground color
#define	PaintPoly_(izzyParamsPtr, inGraphics, inPointCount, inPointList, inFillColor) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPaintFramePolyProc)((inGraphics), (inPointCount), (inPointList), (inFillColor), nil, 0)

// fill a poly with the current foreground color
#define	FramePoly_(izzyParamsPtr, inGraphics, inPointCount, inPointList, inFrameColor, inFrameSize) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPaintFramePolyProc)((inGraphics), (inPointCount), (inPointList), nil, (inFrameColor), (inFrameSize))

// fill a poly with the current foreground color
#define	PaintAndFramePoly_(izzyParamsPtr, inGraphics, inPointCount, inPointList, inFillColor, inFrameColor, inFrameSize) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPaintFramePolyProc)((inGraphics), (inPointCount), (inPointList), (inFillColor), (inFrameColor), (inFrameSize))

typedef void	(*PaintFramePolyProc)(
					UGraphics*		inGraphics,
					SInt32			inPointCount,
					const Point*	inPointList,
					const RGBColor*	inFillColor,
					const RGBColor*	inFrameColor,
					SInt32			inFrameSize);
					
// -----------------------------------------------------------------------------
// EraseRect_
// -----------------------------------------------------------------------------
//	Fills the specified rectangle with the background color

#define	EraseRect_(izzyParamsPtr, inGraphics, inRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mEraseRectProc)((inGraphics), (inRect))

typedef void	(*EraseRectProc)(
					UGraphics*	inGraphics,
					const Rect*	inRect);
					
// -----------------------------------------------------------------------------
// DrawShadedBoxPercent_
// -----------------------------------------------------------------------------
//	Draws a rectangled with shaded edges. 

#define	DrawShadedBoxPercent_(izzyParamsPtr, inGraphics, inRect, inColorRef, inTopPercent, inBottomPercent, inOutline) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawShadedBoxPercentProc)((inGraphics), (inRect), (inColorRef), (inTopPercent), (inBottomPercent), (inOutline))

typedef void	(*DrawShadedBoxPercentProc)(
					UGraphics*	inGraphics,
					const Rect*	inRect,				// rectangle to be drawn
					ColorRef	inColorRef,			// color ref used to draw the rectangle
					float		inTopPecent,		// how much lighter the top edges will be, 0-100%
					float		inBottomPercent,	// how much darker the bottom edges will be, 0-100%
					Boolean		inOutline);			// set to true to draw the outline only

// -----------------------------------------------------------------------------
// Draw3DRect_
// -----------------------------------------------------------------------------
//
#define	Draw3DRect_(izzyParamsPtr, inGraphics, inRect, inFillColor, inLightenPercent, inDarkenPercent, inBorderWidth, inBorderFlags, inInset) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDraw3DRectProc)((inGraphics), (inRect), (inFillColor), (inLightenPercent), (inDarkenPercent), (inBorderWidth), (inBorderFlags), (inInset))

typedef void	(*Draw3DRectProc)(
					UGraphics*	inGraphics,
					const Rect*	inRect,
					ColorRef	inFillColor,
					double		inLightenPercent,
					double		inDarkenPercent,
					SInt16		inBorderWidth,
					UInt8		inBorderFlags,
					Boolean		inInset);

// -----------------------------------------------------------------------------
// DrawTextAt_
// -----------------------------------------------------------------------------
//	Draws the specified block of text at the specified horizontal and vertical
//	location. If inClipRect is not NULL, then that rectangle is used to clip
//	the text when drawn

#define	DrawTextAt_(izzyParamsPtr, inGraphics, h, v, inString, inStringLen, inClipRect) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mDrawTextAtProc)((inGraphics), (h), (v), (inString), (inStringLen), (inClipRect))

typedef void	(*DrawTextAtProc)(
					UGraphics*			inGraphics,
					SInt16				h,
					SInt16				v,
					const void*			inString,
					SInt32				inStringLen,
					const Rect*			inClipRect);		// can be NULL

// -----------------------------------------------------------------------------
// GetTextWidth_
// -----------------------------------------------------------------------------
//	Returns the width, in pixels, of the specified block of test
#define	GetTextWidth_(izzyParamsPtr, inGraphics, inString, inStringLen) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetTextWidthProc)((inGraphics), (inString), (inStringLen))

typedef SInt32	(*GetTextWidthProc)(
					UGraphics*			inGraphics,
					const void*			inString,		// pointer to the text
					SInt32				inStringLen);	// length of the text

// -----------------------------------------------------------------------------
// GetFontInfo_
// -----------------------------------------------------------------------------
//
#define	GetFontInfo_(izzyParamsPtr, inGraphics, outFontInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetFontInfoProc)((inGraphics), (outFontInfo))

typedef void	(*GetFontInfoProc)(
					UGraphics*			inGraphics,
					FontInfo*			outFontInfo);

// -----------------------------------------------------------------------------
// SetupFont_
// -----------------------------------------------------------------------------
//	Prepares to draw with the specified font

#define	SetupFont_(izzyParamsPtr, inGraphics, inFontName, inFontSize, inFontStyle, inFontMode) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetupFontProc)((inGraphics), (inFontName), (inFontSize), (inFontStyle), (inFontMode) )

typedef void	(*SetupFontProc)(
					UGraphics*			inGraphics,
					const char*			inFontName,
					SInt16				inFontSize,
					SInt16				inFontStyle,
					SInt16				inFontMode);

// -----------------------------------------------------------------------------
// InvalidateControl_
// -----------------------------------------------------------------------------
//	For the inInvalidateFlags parameter, caller should pass
//		kControlUpdateCode_DrawControl to draw just the control
//		kControlUpdateCode_UpdateControl to draw the control and all controls underlying it 
//	kControlUpdateCode_DrawControl assumes that the control draws its own background
//	and thus the controls beneath it do not need to be redrawn. Use this option if possible
//	as it is much faster in complicated control panels

#define	InvalidateControl_(izzyParamsPtr, inControlInfo, inInvalidateFlags) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mInvalidateControlProc)((inControlInfo), (inInvalidateFlags))

typedef void	(*InvalidateControlProc)(
					ControlInfo*		inControlInfo,
					UInt32				inInvalidateFlags);

// -----------------------------------------------------------------------------
// IsControlVisible_
// -----------------------------------------------------------------------------
//	Returns true if the specified control is visible

#define	IsControlVisible_(izzyParamsPtr, inControlInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mIsControlVisibleProc)(inControlInfo)

typedef bool	(*IsControlVisibleProc)(
					ControlInfo*		inControlInfo);

// -----------------------------------------------------------------------------
// GetFPS_
// -----------------------------------------------------------------------------
// Returns the current Frames Per Second at which Isadora is rendering

#define	GetFPS_(izzyParamsPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetFPSProc)()

typedef float	(*GetFPSProc)();
	
// -----------------------------------------------------------------------------
// GetPerformanceData_
// -----------------------------------------------------------------------------
// Returns the current Video Processing Overhead value

#define	GetPerformanceData_(izzyParamsPtr, outTargetFPS, outFPS, outCycles, outVPO) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetPerformanceDataProc)((outTargetFPS), (outFPS), (outCycles), (outVPO))

typedef void	(*GetPerformanceDataProc)(
					float*				outTargetFPS,
					float*				outFPS,
					float*				outCycles,
					float*				outVPO);
	
// -----------------------------------------------------------------------------
// MarkActorDocumentModified_
// -----------------------------------------------------------------------------
// Makes the curret document dirty so the user will be asked to save

#define	MarkActorDocumentModified_(izzyParamsPtr, inThingPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mMarkActorDocumentModifiedProc)(inThingPtr)

typedef void	(*MarkActorDocumentModifiedProc)(
					ThingPtr			inThing);
	
// -----------------------------------------------------------------------------
// MarkControlDocumentModified_
// -----------------------------------------------------------------------------
// Makes the curret document dirty so the user will be asked to save

#define	MarkControlDocumentModified_(izzyParamsPtr, inControlInfo) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mMarkControlDocumentModifiedProc)(inControlInfo)

typedef void	(*MarkControlDocumentModifiedProc)(
					ControlInfo*		inControlInfo);
	
// -----------------------------------------------------------------------------
// SetActorUndoAction_
// -----------------------------------------------------------------------------
// Allows a Actor to handle undo/redo operations.
//
// The calling function passes in a pointer to a block of data. This data
// will be passed back to the caller via the HandleActorMessage. The
// data may be allocated using any method desired (IzzyMalloc_() or new)
// as the caller is repsonsible for disposing the data when the 
// kIzzyDisposeUndoRedo message is passed to the callback. Typically this
// data will include the data required to both undo _and_ redo the
// user action taken.
//
// An optional c-string may also be provided to specify the
// text that will appear next to the undo/redo menu item.
//
// if the user chooses undo or redo, a message will be sent to the Actor's
// HandleActorMessage function, with a mesasge of kIzzyUndoChosen, kIzzyRedoChosen, 
// and will also pass back the user data given to this function. The 
// HandleActorMessage should take the respective action.
//
// When the undo/redo data is no longer needed, the message will be
// kIzzyDisposeUndoRedo. At this point the callback should dispose and allocated
// data.

#define	SetActorUndoAction_(izzyParamsPtr, inActorInfo, inUserData, inText) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetActorUndoActionProc)((inActorInfo), (inUserData), (inText))

typedef void	(*SetActorUndoActionProc)(
					ActorInfo*			inActorInfo,
					void*				inUserData,
					const char*			inUndoText);	// can be NULL -- shows next to Undo/Redo items in Edit Menu
	
// -----------------------------------------------------------------------------
// Set	UndoAction_
// -----------------------------------------------------------------------------
// Allows a control to handle undo/redo operations.
//
// The calling function passes in a pointer to a block of data. This data
// will be passed back to the caller via the HandleControlMessage. The
// data may be allocated using any method desired (IzzyMalloc_() or new)
// as the caller is repsonsible for disposing the data when the 
// kIzzyDisposeUndoRedo message is passed to the callback. Typically this
// data will include the data required to both undo _and_ redo the
// user action taken.
//
// An optional c-string may also be provided to specify the
// text that will appear next to the undo/redo menu item.
//
// if the user chooses undo or redo, a message will be sent to the Control's
// HandleControlMessage function, with a mesasge of kIzzyUndoChosen, kIzzyRedoChosen, 
// and will also pass back the user data given to this function. The 
// HandleControlMessage should take the respective action.
//
// When the undo/redo data is no longer needed, the message will be
// kIzzyDisposeUndoRedo. At this point the callback should dispose and allocated
// data.

#define	SetControlUndoAction_(izzyParamsPtr, inControlInfo, inUserData, inText) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mSetControlUndoActionProc)((inControlInfo), (inUserData), (inText))

typedef void	(*SetControlUndoActionProc)(
					ControlInfo*		inControlInfo,
					void*				inUserData,
					const char*			inUndoText);	// can be NULL -- shows next to Undo/Redo items in Edit Menu
	
// -----------------------------------------------------------------------------
// PostEditActorPrivateDataAction
// -----------------------------------------------------------------------------
// Allows a control to handle undo/redo operations.
//
// The calling function passes in a pointers and lengths to the old and
// new data. These data blocks _must_ be allocated with IzzyMalloc_(),
// as Isadora disposes the data using IzzyFree_() when the data is no
// longer needed.
//
// An optional c-string may also be provided to specify the
// text that will appear next to the undo/redo menu item.
//
// if the user chooses undo or redo, the actor's SetPrivateData function
// wil be called with the old or new data, as appropriate.
//
// The data blocks are disposed using IzzyFree_() when the data is no
// longer needed.


#define	PostEditActorPrivateDataAction_(izzyParamsPtr, inActorInfo, inOldData, inOldLength, inNewData, inNewLength, inUndoText) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mPostEditActorPrivateDataActionProc)((inActorInfo), (inOldData), (inOldLength), (inNewData), (inNewLength), (inUndoText))

typedef void	(*PostEditActorPrivateDataActionProc)(
					ActorInfo*			inActorInfo,
					void*				inOldData,
					UInt32				inOldLength,
					void*				inNewData,
					UInt32				inNewLength,
					const char*			inUndoText);	// can be NULL -- shows next to Undo/Redo items in Edit Menu
	
// -----------------------------------------------------------------------------
// ShowHideStage_
// -----------------------------------------------------------------------------
// Allows a control or an actor to show or hide the stage.

#define	ShowHideStage_(izzyParamsPtr, inThing, inControlInfo, inShow, inDontWarn) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mShowHideStageProc)((inThing), (inControlInfo), (inShow), (inDontWarn))

typedef void	(*ShowHideStageProc)(
					ThingPtr			inThing,		// only one of either inThing or inControl info should be non-nulll
					ControlInfo*		inControlInfo,
					Boolean				inShow,
					Boolean				inDontWarn);
	
// -----------------------------------------------------------------------------
// FileSavedOnBigEndianMachine_
// -----------------------------------------------------------------------------
// Returns 1 if current document saved on Big Endian machine; 0 if saved on
// a little endian machine, and 0xFF the endianness of the file is unknown

#define	FileSavedOnBigEndianMachine_(izzyParamsPtr, inThingPtr) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mFileSavedOnBigEndianMachineProc)(inThingPtr)

typedef UInt8	(*FileSavedOnBigEndianMachineProc)(
					ThingPtr			inThing);
	
// -----------------------------------------------------------------------------
// GetUTF8PathToCurrentDocument_
// -----------------------------------------------------------------------------
// Returns a path in UTF8 format to the current document. If the document has
// not yet saved, then the path to root level drive is given. This function
// returns true if a valid path was returned; false otherwise.

#define	GetUTF8PathToCurrentDocument_(izzyParamsPtr, inThingPtr, inMaxLen, outString) \
	(*(izzyParamsPtr)->kConditionalCallbacks->mGetUTF8PathToCurrentDocumentProc)((inThingPtr), (inMaxLen), (outString))

typedef Boolean	(*GetUTF8PathToCurrentDocumentProc)(
					ThingPtr			inThing,
					UInt32				inMaxLen,
					char*				outBuffer);
	
// -----------------------------------------------------------------------------
// callback function structure
struct IsadoraCallbacks {

	ShowAlertProc						mShowAlertProc;
	
	CreateMessageReceiverProc			mCreateMessageReceiverProc;
	DisposeMessageReceiverProc			mDisposeMessageReceiverProc;
	UpdateMessageReceiverProc			mUpdateMessageReceiverProc;
	UpdateMessageReceiverMaskProc		mUpdateMessageReceiverMaskProc;

	ChangePropertyTypeProc				mChangePropertyTypeProc;
	GetInputPropertyValueProc			mGetInputPropertyValueProc;
	SetOutputPropertyValueProc			mSetOutputPropertyValueProc;
	GetOutputPropertyValueProc			mGetOutputPropertyValueProc;
	GetOutputPropertyMinimumValueProc	mGetOutputPropertyMinimumValueProc;
	GetOutputPropertyMaximumValueProc	mGetOutputPropertyMaximumValueProc;

	QueueMediaCommandProc				mQueueMediaCommandProc;
	ExecuteMediaCommandProc				mExecuteMediaCommandProc;
	SendMidiMessageProc					mSendMidiMessageProc;

	void*								mDummyCopyImageBufferToStageProc;
	GetStageBoundsProc					mGetStageBoundsProc;
	InvalAreaOnStageProc				mInvalAreaOnStageProc;

	IsIntermediaryImageBufferRequired	mIsIntermediaryImageBufferRequiredProc;
	CreateComplimentaryImageBufferProc	mCreateComplimentaryImageBufferProc;
	DisposeImageBufferProc				mDisposeImageBufferProc;
	CopyImageBufferProc					mCopyImageBufferProc;
	ProcPtr								mNotUsed1;
	
	JumpToSceneProc						mJumpToSceneProc;
	
	PropertyTypeAndIndexToHelpIndexProc	mPropertyTypeAndIndexToHelpIndexProc;
	
	AddPropertyProc						mAddPropertyProc;
	RemovePropertyProc					mRemovePropertyProc;
	GetPropertyCountProc				mGetPropertyCount;
	
	GetPropertyMinMaxProc				mGetPropertyMinMaxProc;
	SetPropertyMinMaxProc				mSetPropertyMinMaxProc;

	GetInputPropertyScaleToMinMaxProc	mGetInputPropertyScaleToMinMaxProc;
	SetInputPropertyScaleToMinMaxProc	mSetInputPropertyScaleToMinMaxProc;

	AllNotesOffProc						mAllNotesOffProc;
	
	GetSceneNameProc					mGetSceneNameProc;
	ActivateDeactivateSceneProc			mActivateDeactivateSceneProc;
	
	GetActorResourcePICTProc			mGetActorResourcePICTProc;
	
	IzzyMallocProc						mIzzyMallocProc;
	
	IzzyFreeProc						mIzzyFreeProc;
	
	LookupPartialStringInListProc		mLookupPartialStringInListProc;
	
	ModKeyStringToMaskProc				mModKeyStringToMaskProc;
	
	ModKeyMaskToStringProc				mModKeyMaskToStringProc;
	
	GetScreenBoundsProc					mGetScreenBoundsProc;
	
	SendMidiNoteOnOffProc				mSendMidiNoteOnOffProc;
	
	GetMidiDancerEnergyProc				mGetMidiDancerEnergyProc;
	
	GetTextFromDialogProc				mGetTextFromDialogProc;
	
	PluginAssertProc					mPluginAssertProc;
	
	PrepareActorDefinedAreaPictProc		mPrepareActorDefinedAreaPictProc;
	
	DrawActorDefinedAreaPictProc		mDrawActorDefinedAreaPictProc;

	WarnLowMemoryProc					mWarnLowMemoryProc;

	RecallSnapshotProc					mRecallSnapshotProc;
	
	CreateImageBufferProc				mCreateImageBufferProc;
	
	EnterVideoProcessingProc			mEnterVideoProcessingProc;
	
	ExitVideoProcessingProc				mExitVideoProcessingProc;
	
	BroadcastControlValueProc			mBroadcastControlValueProc;
	
	DrawTextInRectProc					mDrawTextInRectProc;

	GetMediaIndexListFromDialogProc		mGetMediaIndexListFromDialogProc;
	
	GetMediaObjectInfoProc				mGetMediaObjectInfoProc;
	
	BroadcastControlValueDirectProc		mBroadcastControlValueDirectProc;
	
	GetPropertyDisplayFormatsProc		mGetPropertyDisplayFormatsProc;

	SetInputPropertyValueProc			mSetInputPropertyValueProc;
	
	CancelMidiNoteOffProc				mCancelMidiNoteOffProc;
	
	GetRandomSeedProc					mGetRandomSeed;
	
	SetCaptureModeProc					mSetCaptureModeProc;
	
	GetStageWindowProc					mGetStageWindowProc;
	
	EncodeStringProc					mEncodeStringProc;

	BroadcastValueProc					mBroadcastValueProc;
	
	CopyImageBufferToRecordedOutputProc	mCopyImageBufferToRecordedOutputProc;

	PluginLogInfoProc					mPluginLogInfoProc;
	
	StartRecordCaptureInputToDiskProc	mStartRecordCaptureInputToDiskProc;
	
	StopRecordCaptureInputToDiskProc	mStopRecordCaptureInputToDiskProc;
	
	GetMidiDancerEnergyPlusProc			mGetMidiDancerEnergyPlusProc;
	
	EnableDisableMIDIProc				mEnableDisableMIDIProc;
	
	ShowNumericValueEditBoxProc			mShowNumericValueEditBoxProc;
	
	void*								mDummy2;
	void*								mDummy3;
	void*								mDummy4;
	void*								mDummy5;
	void*								mDummy6;
	void*								mDummy7;
	
	// DrawPascalStringProc				mDrawPascalStringProc;
	// DrawCStringProc						mDrawCStringProc;
	// DrawTextProc						mDrawTextProc;
	
	// TextWidthPascalStringProc			mTextWidthPascalStringProc;
	// TextWidthCStringProc				mTextWidthCStringProc;
	// TextWidthProc						mTextWidthProc;
	
	GetBinNameAndTypeFromUniqueIDProc	mGetBinNameAndTypeFromUniqueIDProc;
	CountMediaObjectsInBinProc			mCountMediaObjectsInBinProc;
	GetMediaObjectInfoFromBinProc		mGetMediaObjectInfoFromBinProc;
	GetBinNameAndUniqueIDFromIndexProc	mGetBinNameAndUniqueIDFromIndexProc;

	ValidAreaOnStageProc				mValidAreaOnStageProc;
	
	SmartSetPropertyMinMaxProc			mSmartSetPropertyMinMaxProc;
	
	EnableDisableActorsProc				mEnableDisableActorsProc;
	
	GetInfoForIndexedBinProc			mGetInfoForIndexedBinProc;
	
	TrackPopupMenuProc					mTrackPopupMenuProc;
	
	GetAggregateBufferFormatProc		mGetAggregateBufferFormatProc;
	
	CreateBlackImageBufferProc			mCreateBlackImageBufferProc;
	
	IPAddressStringToInetAddressProc	mIPAddressStringToInetAddressProc;
	SendOSCValuesProc					mSendOSCValuesProc;
	
	SetSceneIntensityProc				mSetSceneIntensityProc;

	GetSceneIntensityProc				mGetSceneIntensityProc;
	
	OutputTextFileProc					mOutputTextFileProc;
	
	GetMediaIndexForItemInBinProc		mGetMediaIndexForItemInBinProc;
	
	WriteToSerialPortProc				mWriteToSerialPortProc;
	
	ReadFromSerialPortProc				mReadFromSerialPortProc;
	
	SetLocalTranslationAndRotationProc	mSetLocalTranslationAndRotationProc;
	
	AllocateValueStringProc				mAllocateValueStringProc;

	ReleaseValueStringProc				mReleaseValueStringProc;
	
	ControlAcquireImageProc				mControlAcquireImageProc;
	ControlReleaseImageProc				mControlReleaseImageProc;
	
	GetUserNameProc						mGetUserNameProc;
	
	GetActorDefaultPixelFormatProc		mGetActorDefaultPixelFormatProc;
	
	GetActorGraphicsProc				mGetActorGraphicsProc;
	
	GetControlGraphicsProc				mGetControlGraphicsProc;
	
	SetForegroundColorRefProc			mSetForegroundColorRefProc;
	DrawLineProc						mDrawLineProc;
	PaintRectProc						mPaintRectProc;
	DrawShadedBoxPercentProc			mDrawShadedBoxPercentProc;
	InvalidateControlProc				mInvalidateControlProc;
	Draw3DRectProc						mDraw3DRectProc;
	DrawTextAtProc						mDrawTextAtProc;
	GetTextWidthProc					mGetTextWidthProc;
	GetFontInfoProc						mGetFontInfoProc;
	SetupFontProc						mSetupFontProc;
	
	TestMTCTimecodeTimeProc				mTestMTCTimecodeTimeProc;
	
	SetOutputPropertyLimitMinMaxProc	mSetOutputPropertyLimitMinMaxProc;
	
	SetLocalLightingProc				mSetLocalLightingProc;
	
	void*								mDummy1;

	SendLanBoxProc						mSendLanBoxProc;
	
	EditLanBoxRectListDialog			mEditLanBoxRectListDialogProc;

	GetPropertyIDProc					mGetPropertyIDProc;

	ConvertTimeToPercentForMediaObjectProc		mConvertTimeToPercentForMediaObject;
	
	ConvertPercentToTimeForMediaObjectProc		mConvertPercentToTimeForMediaObject;
	
	EraseRectProc						mEraseRectProc;
	
	GetFPSProc							mGetFPSProc;
	
	MarkControlDocumentModifiedProc		mMarkControlDocumentModifiedProc;
	MarkActorDocumentModifiedProc		mMarkActorDocumentModifiedProc;

	BroadcastValueFromControlProc		mBroadcastValueFromControlProc;
	
	SetActorUndoActionProc				mSetActorUndoActionProc;
	SetControlUndoActionProc			mSetControlUndoActionProc;
	
	ShowHideStageProc					mShowHideStageProc;
	
	PaintFramePolyProc					mPaintFramePolyProc;
	
	DrawGWorldProc						mDrawGWorldProc;
	
	DrawPictureProc						mDrawPictureProc;
	
	PushPopSetClipProc					mPushPopSetClipProc;
	
	SetLocalScalingOptionsProc			mSetLocalScalingOptionsProc;
	
	void*								mDummy8;
	//SetLocalStageBackgroundOptionsProc	mSetLocalStageBackgroundOptionsProc;
	
	GetPerformanceDataProc				mGetPerformanceDataProc;

	FileSavedOnBigEndianMachineProc		mFileSavedOnBigEndianMachineProc;
	
	PostEditActorPrivateDataActionProc	mPostEditActorPrivateDataActionProc;

	ChangePropertyTypeAndNameProc		mChangePropertyTypeAndNameProc;
	
	GetGWorldBoundsProc					mGetGWorldBoundsProc;
	
	DrawImageBufferProc					mDrawImageBufferProc;
	
	GetPropertyConnectionInfoProc		mGetPropertyConnectionInfoProc;
	
	LinkPropertiesProc					mLinkPropertiesProc;
	
	TakeSnapshotProc					mTakeSnapshotProc;
	
	UpdateSnapshotProc					mUpdateSnapshotProc;
	
	SetActorFlagsProc					mSetActorFlagsProc;
	
	WriteToLogFileProc					mWriteToLogFileProc;
	
	GetUTF8PathToCurrentDocumentProc	mGetUTF8PathToCurrentDocumentProc;

	SetTCPStreamInfoProc				mSetTCPStreamInfoProc;
	
	EnableDisableTCPStreamProc			mEnableDisableTCPStreamProc;
	
	WriteToTCPStreamProc				mWriteToTCPStreamProc;
	
	ReadFromTCPStreamProc				mReadFromTCPStreamProc;
	
	GetStagePreviewFromControlProc		mGetStagePreviewFromControlProc;
	
	EnableStagePreviewFromControlProc	mEnableStagePreviewFromControlProc;
	
	StartStopRecordingLanBoxDMXInputProc	mStartStopRecordingLanBoxDMXInputProc;
	
	GetSceneIndexProc					mGetSceneIndexProc;
	
	BroadcastStreamingDataProc			mBroadcastStreamingDataProc;

	GetChannelMapDialogProc				mGetChannelMapDialogProc;
	
	SetPropertyNameProc					mSetPropertyNameProc;
	
	CopyPropDefValueSourceProc			mCopyPropDefValueSourceProc;
	
	IsControlVisibleProc				mIsControlVisibleProc;
	
};
typedef struct IsadoraCallbacks IsadoraCallbacks;

// ---------------------------------------------------------------
// Isadora parameters

#define	kConditionalCallbacks	mCallbacks

struct IsadoraParameters {

	UInt32						mIsadoraMagic;
	
	ImageProcessingPrefs		mImgProcPrefs;
	
	IsadoraCallbacks*			mCallbacks;				// isadora callbacks
	
	UInt32						mMillisecondCount;		// current time in milliseconds

	Boolean						mHasFastMath;			// MacOS: true for Altivec / PC: true for MMX processor
};
typedef struct IsadoraParameters IsadoraParameters;

// ---------------------------------------------------------------
// Private Functions -- do not call from plugins

void	SetImageProcessingPreferences(const ImageProcessingPrefs* inImageProcPrefs);
Boolean	GetProcessVideoAtFixedSizeRect(Rect* outRect);
void	InitIsadoraCallbacks();
void	DisposeIsadoraCallbacks();
double	GetVideoProcessingOverhead();

#ifdef __cplusplus
}
#endif

extern	IsadoraParameters	gIzzyParams;

#endif
