// ===========================================================================
//	CValueCommon				  (C)2000 Mark F. Coniglio. All rights reserved.
// ===========================================================================
//
// Structure that encapsulates a value, embodying its type and value.
//
// Isadora passes values as a structure that consists of the value's type
// and its actual value. There are four basic types, and a fifth type that
// allows plugins to define their own data types. The Value structure 
// represents this with two members: the 'type' member holds a constant
// that designates the value's type, and the 'value' member holds the
// actual value. The value is designated inside the struct as a signed
// 32 bit integer, but the meaning of the contents of this member is
// determined by the 'type' member.
// 
// The five basic types are as follows:
//
// kBoolean: a boolean value, non-zero being true and zero being false.
//
// kInteger: a 32-bit signed integer
//
// kFloat: a 32-bit floating point value
//
// kRange: a pair of signed 16 bit numbers designating a range of values
//
// kString: a pointer to a String structure
//
// The sixth type, kData allows artibrarily sized chunks of data to be
// passed through Isadora using the Value structure. In this case, the
// 'value' parameter points to a block of data that begins with a 
// DataValueInfo struct that gives some basic information about the
// data. Generally, your data struct will look something like this:
//
//	typedef struct {
//		DataValueInfo	info;
//		MyDataType		myData;
//	} MyDataBlock;
//
// In this case, the contents of the 'value' member would be a pointer
// to MyDataBlock. There is more information about the members of the
// DataValueInfo struct below.
//
// Note that one important, built-in use of the kData type is to pass
// ImageBuffers from actor to actor. See ImageBufferCommon.h for more
// information about ImageBuffers.

#ifndef _H_ValueCommon
#define _H_ValueCommon

// -----------------------------------------------------------------------------
// The ValueType constants
enum {
	kTypeInvalid,				// invalid type (0)
	kBoolean,					// a true (non-zero) or false (zero) value
	kInteger,					// a 32-bit signed integer
	kFloat,						// a 32-bit floating point number
	kRange,						// a pair of 16-bit signed integers defining a range of values
	kData,						// application defined data type
								// mValue = ptr to block of data staring with DataValueType struct
	kString,					// str member points to String type
	
	kNumValueTypes = kString	// number of type codes
};

// special maker used when broadcasting Values over
// a network: this value indicates that a string
// length and the string data will follow after the
// end of the packet

#define	kValueDataToFollowMarker	0xFFFFFFFF

typedef UInt16						ValueType;

#define	ValueTypeIsNumber(a)		((a) == kBoolean || (a) == kInteger || (a) == kFloat)

// -----------------------------------------------------------------------------
// The Range structure, used inside of the Value struct

struct Range {
	SInt16	hi;
	SInt16	lo;
};

typedef struct	Range 	Range;

const char		kPropRangeDelimiter			= '-';

// -----------------------------------------------------------------------------
// The String structure, used inside of the Value struct

#define kStringMagic	0x12345678

struct String {
	SInt32	useCount;					// use count for this string
	#ifdef kStringMagic
	UInt32	magic;						// magic value for checking - only if kStringMagic defined
	#endif
	char	strData[1];					// the actual string data
};

typedef struct String 	String;

#ifdef kStringMagic
	#define	AssertStringMagicValid_(value)			Assert_( ((value).type == kString) && ( (value).u.str == nil || (value).u.str->magic == kStringMagic ) )
	#define	PluginAssertStringMagicValid_(value)	PluginAssert_( ((value).type == kString) && ( (value).u.str == nil || (value).u.str->magic == kStringMagic ) )
#else
	#define	AssertStringMagicValid_(valuePtr)		0
	#define	PluginAssertStringMagicValid_(valuePtr)	0
#endif

// -----------------------------------------------------------------------------
// The Value structure
struct Value {
	ValueType	type;				// type - use this to choose which member of the union 'u' to access
	union {
		SInt32		ivalue;			// type == kInteger or kBoolean
		float		fvalue;			// type == kFloat
		Range		range;			// type == kRange
		void*		data;			// type == kData
		String*		str;			// type == kString
	} u;
};

typedef struct Value			Value;
typedef Value*					ValuePtr;

// -----------------------------------------------------------------------------
// The MultiValue structure

struct MultiValue {
	UInt32	mValueCount;
	Value	mValues[1];			// array of values -- actual len determined by mValueCount
};

typedef struct MultiValue		MultiValue;
typedef MultiValue*				MultiValuePtr;

// -----------------------------------------------------------------------------
// Isadora assumes that a Value struct with a type of kData
// points to a block of memory that begins with the following
// information

typedef	OSType	DataType;

struct DataValueInfo {
	DataType	mDataType;			// set to a unique four character id for this data type

	Boolean		mDescNeverChanges;	// prevents the description in the thing from being redrawn when
									// the new data is sent out.

	UInt32		mDataChangeCount;	// should be incremented whenever the internal data changes
};
typedef struct DataValueInfo	DataValueInfo;

// -----------------------------------------------------------------------------
// the CustomDataValueInfo struct is a variant of the DataValueInfo
// struct, with name and length fields attached to the end -- this allows
// custom data types to be added by third parties

struct CustomDataValueInfo {
	DataValueInfo	mDataValueInfo;		// embedded DataValueInfo struct
	char			mDataName[16];		// name of the data - to be displayed by Isadora
	SInt32			mDataLen;			// optional -- number of bytes of data to follow
};
typedef struct CustomDataValueInfo	CustomDataValueInfo;

// macro to return the data type of a from the DataValueInfo struct
// pointed to by a Value struct with a type of kData
#define	GetDataValueDataType(inValuePtr)  ((((ValuePtr)(inValuePtr))->type == kData \
	&& ((ValuePtr)(inValuePtr))->u.data != nil) \
		? ((DataValueInfo*)((ValuePtr)(inValuePtr))->u.data)->mDataType : 0)

// macro returns true if the data type of the Value pointed to by
// inValuePtr is kData, and if the data value is not null, and if
// the data type matches inDataType
#define	ValueUsesDataType(inValuePtr, inDataType) (((ValuePtr)(inValuePtr))->type == kData \
	&& ((ValuePtr)(inValuePtr))->u.data != nil \
	&& ((DataValueInfo*)((ValuePtr)(inValuePtr))->u.data)->mDataType == (inDataType))

// macro returns the data value pointed if the data value matches the
// one specified, nil otherwise.
#define GetDataValueOfType(inValuePtr, inDataType, inDesiredType) ((inDesiredType)(ValueUsesDataType(inValuePtr, inDataType) ? (((ValuePtr)(inValuePtr))->u.data) : nil))

#define GetCustomDataName(inValuePtr) (((CustomDataValueInfo*) ((ValuePtr)(inValuePtr))->u.data)->mDataName )

// -----------------------------------------------------------------------------
// Useful Constants for the Value struct

#define	kFloatMin				((float) -FLT_MAX)
#define	kFloatMax				((float) FLT_MAX)
#define	kIntMin					((SInt32) INT_MIN)
#define	kIntMax					((SInt32) INT_MAX)
#define	kRangeMin				((SInt16) SHRT_MIN)
#define kRangeMax				((SInt16) SHRT_MAX)

#endif	// ifdef _H_ValueCommon
