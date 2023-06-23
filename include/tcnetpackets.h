#ifndef TCNETPACKETS_H
#define TCNETPACKETS_H



#include <stdint.h>

// Packet types
enum TCNetPacket {
    OptIn = 2,
    OptOut = 3,
    Status = 5,
    TimeSync = 10,
    ErrorNotification = 13,
    Request = 20,
    ApplicationSpecificPacket = 30,
    Control = 101,
    Text = 128,
    Keyboard = 132,
    Data = 200,
    LowResArtworkImage = 204,
    ApplicationSpecificData = 213,
    Time = 254
};

// Node Types

enum TCNetNodeTypes {
    Auto = 1,
    Master = 2,
    Slave = 3,
    Repeater = 8
};

// Length Definitions

#define TCNET_NODE_NAME_LENGTH 8
#define TCNET_VENDOR_NAME_LENGTH 16
#define TCNET_DEVICE_NAME_LENGTH 16
#define TCNET_LAYER_NAME_LENGTH 16
#define TCNET_LAYER_COUNT 8
#define TCNET_MAX_CHANNELS 6
// TC Net Data types

enum TCNetDataTypes {
    Metrics = 2,
    Metadata = 4,
    BeatGrid = 8,
    CueInfo = 12,
    SmallWaveForm = 16,
    BigWaveForm = 32,
    Mixer = 150
};

#pragma pack(push, 1)

// Helper Struct Definitions

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} CueColor;


typedef struct {
    uint8_t cueType;
    uint8_t reserved1;
    uint32_t cueInTime;
    uint32_t cueOutTime;
    uint8_t reserved2;
    CueColor cueColor;
    uint8_t reserved3[8];
} CueData;
// Packet Definitions

// TCNet Packet Header

typedef struct {
    uint16_t nodeID;                // Node ID of sending device (Little Endian) (V1-0)
    uint8_t protocolVersionMajor;   // Protocol Version (Major) of sending device (V1-0)
    uint8_t protocolVersionMinor;   // Protocol Version (Minor) of sending device (V1-0)
    char header[3];                 // TCNet Protocol Header (Must be "TCN") (V3-1)
    uint8_t messageType;            // Message type of packet (V1-0)
    char nodeName[TCNET_NODE_NAME_LENGTH];// Node Name / Signature (ASCII Text) (V3-1)
    uint8_t sequenceNumber;         // Sequence number of packet (0-255) (V3-1)
    uint8_t nodeType;               // Node Type (0-255) (V3-1)
    uint16_t nodeOptions;           // Node Options (Little Endian) (V3-1)
    uint32_t timestamp;             // Timestamp in Microseconds (Little Endian) (0-999999) (V3-1)
} TCNetPacketHeader;

// Network Participation Messages

typedef struct {
    TCNetPacketHeader header; // The common packet header
    uint16_t nodeCount; // Amount of Registered Nodes
    uint16_t nodeListenerPort; // Listener Port for Unicast Messages
    uint16_t uptime; // Uptime of Node in SEC
    uint16_t reserved1; // RESERVED
    char vendorName[TCNET_VENDOR_NAME_LENGTH]; // Vendor
    char applicationDeviceName[TCNET_DEVICE_NAME_LENGTH]; // Application / Device Name
    uint8_t applicationDeviceMajorVersion; // Application/Device Major Version
    uint8_t applicationDeviceMinorVersion; // Application/Device Minor Version
    uint8_t applicationDeviceBugVersion; // Application/Device Minor Version
    uint8_t reserved2; // RESERVED
} TCNetOptInPacket;

typedef struct {
    TCNetPacketHeader header; // The common packet header
    uint16_t nodeCount; // Amount of Registered Nodes
    uint16_t nodeListenerPort; // Listener Port for Unicast Messages
} TCNetOptOutPacket;


// Status Messages

typedef struct {
    uint16_t nodeCount; // Amount of Registered Nodes
    uint16_t nodeListenerPort; // Listener Port for Unicast Messages
    uint8_t reserved1[6]; // Reserved field
    uint8_t layerSource[TCNET_LAYER_COUNT]; // Layer Source (8 Layers: 1, 2, 3, 4, A, B, M, C)
    uint8_t layerStatus[TCNET_LAYER_COUNT]; // Layer Status (8 Layers: 1, 2, 3, 4, A, B, M, C)
    uint32_t layerTrackID[TCNET_LAYER_COUNT]; // Assigned Track ID for each layer (8 Layers: 1, 2, 3, 4, A, B, M, C)
    uint8_t reserved2; // Another reserved field
    uint8_t smpteMode; // SMPTE Mode
    uint8_t autoMasterMode; // Auto Master Mode
    uint8_t reserved3[15]; // Reserved field
    uint8_t appSpecific[72]; // APP Specific field
    char layerName[TCNET_LAYER_COUNT][TCNET_LAYER_NAME_LENGTH]; // Name for each layer (8 Layers: 1, 2, 3, 4, A, B, M, C)
} TCNetStatusPacketData;


// Time Sync Messages
typedef struct {
    TCNetPacketHeader header; // The common packet header

    // Data
    uint8_t step; // Step No
    uint8_t reserved; // Reserved field
    uint16_t nodeListenerPort; // Listener Port for Unicast Messages
    uint32_t remoteTimestamp; // Timestamp of Remote Node
} TCNetTimeSyncPacket;


// Notification Messages


typedef struct {
    TCNetPacketHeader header; // The common packet header
    
    // Data
    uint8_t dataType; // Data type of Request
    uint8_t layerId; // Layer ID of original request
    uint16_t code; // Returned Code
    uint16_t messageType; // Message type of Request
} TCNetErrorPacket;


typedef struct {
    TCNetPacketHeader header; // The common packet header
    
    // Data
    uint8_t dataType; // Data Type to request
    uint8_t layer; // Layer where data belongs to
} TCNetRequestPacket;

struct TCNetApplicationSpecificDataPacket {
    TCNetPacketHeader header;

    // Data
    uint8_t dataIdentifier1;        // Application Identifier Signature 1/2 (Defaults to 0) (0-255) (V3-0)
    uint8_t dataIdentifier2;        // Application Identifier Signature 2/2 (Defaults to 0) (0-255) (V3-0)
    uint32_t dataSize;              // Data Size of all packets (Little Endian) (V3-0)
    uint32_t totalPackets;          // Total number of packets for data (Little Endian) (V3-0)
    uint32_t packetNumber;          // Packet number of data (Little Endian) (V3-0)
    uint32_t packetSignature;       // Signature of Packet (Little Endian) (178260640) (V3-0)
    uint8_t data[];                 // Data (Variable length based on dataSize) (V3-0)
};

// Control Messages


typedef struct {
    TCNetPacketHeader header;
    
    // Data
    uint8_t step; // Step No
    uint8_t reserved1; // RESERVED
    uint32_t dataSize; // Total Data Size
    uint8_t reserved2[12]; // RESERVED
    char controlPath[]; // String with Control Path
} TCNetControlPacket;

typedef struct {
    TCNetPacketHeader header;
    
    //Data
    uint8_t step; // Step No
    uint8_t reserved1; // RESERVED
    uint32_t dataSize; // Total Data Size
    uint8_t reserved2[12]; // RESERVED
    char textData[]; // String with Text Data
} TCNetTextControlPacket;


typedef struct {
    TCNetPacketHeader header; 
    
    // Data
    uint8_t reserved1; // RESERVED
    uint8_t reserved2; // RESERVED
    uint32_t dataSize; // Total Data Size
    uint8_t reserved3[12]; // RESERVED
    uint16_t keyboardData; // Keyboard Data
} TCNetKeyboardControlPacket;


// Data Messages

// Metrics
typedef struct {
    TCNetPacketHeader header;
    
    //Data
    uint8_t dataType; // Datatype 2 = Metrics
    uint8_t layerID; // Layer Number
    uint8_t reserved1; // RESERVED
    uint8_t layerState; // Layer State
    uint8_t reserved2; // RESERVED
    uint8_t syncMaster; // Sync Master
    uint8_t reserved3; // RESERVED
    uint8_t beatMarker; // Beat Marker
    uint32_t trackLength; // Track Length in Milliseconds
    uint32_t currentPosition; // Play head Position in Milliseconds
    uint32_t speed; // Play head Speed
    uint8_t reserved4[13]; // RESERVED
    uint32_t beatNumber; // Beat Number
    uint8_t reserved5[51]; // RESERVED
    uint32_t bpm; // BPM
    uint16_t pitchBend; //\ Pitch Bend
    uint32_t trackID; // Assigned Track ID
} TCNetMetricsDataPacket;


// TCNet Data Packet - Meta
typedef struct {
    TCNetPacketHeader header; // The common packet header
    
    
    // Data
    uint8_t dataType; // Datatype 4 = Metadata
    uint8_t layerID; // Layer ID
    uint8_t reserved1; // RESERVED
    uint8_t reserved2[2]; // RESERVED
    char trackArtist[128]; // Track Artist Name
    char trackTitle[256]; // Track Title Name
    uint16_t trackKey; // Track KEY
    uint32_t trackID; // Assigned Track ID
} TCNetMetaDataPacket;

// Beat Grid Data

typedef struct {
    uint16_t beatNumber; // Beat Number
    uint8_t beatType; // Beat Type (20=Down Beat, 10=Upbeat)
    uint8_t reserved; // RESERVED
    uint32_t beatTimestamp; // Timestamp in MS
} BeatGrid;

// Beat Grid

typedef struct {
    TCNetPacketHeader header; // The common packet header
    
    // Data
    uint8_t dataType; // Datatype 8 = Beat Grid Data
    uint8_t layerID; // Layer Number
    uint32_t dataSize; // Total Data Size
    uint32_t totalPacket; // Total Packets used for data
    uint32_t packetNo; // Packet Number
    uint32_t dataClusterSize; // Data Cluster Size
    BeatGrid beatGrid[300]; // Assuming max of 2400 bytes divided by 8 (size of BeatGrid struct)
} TCNetBeatGridDataPacket;

// Cue Data 
typedef struct {
    uint8_t red;    // Red component (0-255)
    uint8_t green;  // Green component (0-255)
    uint8_t blue;   // Blue component (0-255)
} CueColor;

typedef struct {
    TCNetPacketHeader header;   // The header information for the TCNet Data Packet
    uint8_t dataType;           // Datatype of TCNet Data Packet. For Cue Data it is 12.
    uint8_t layerID;            // Layer number if layer is sending data. Example: 1=LAYER1, 2=LAYER2, etc.
    uint8_t reserved1[16];
    uint32_t loopInTime;        // Time of Loop IN
    uint32_t loopOutTime;       // Time of Loop OUT
    CueData cues[18];           // Array of Cue Data
} TCNetDataPacket;


// Waveform Data

typedef struct {
    uint8_t BLevel; // Bar Levels
    uint8_t BColor; // Bar Colors
} WaveformData;

typedef struct {
    TCNetPacketHeader header; // The common packet header
    // Data
    uint8_t dataType; // Datatype 16 = Small Waveform
    uint8_t layerID; // Layer Number
    uint32_t dataSize; // Total Data Size
    uint32_t totalPacket; // Total Packets used for data
    uint32_t packetNo; // Packet Number
    uint32_t reserved; // Reserved space
    WaveformData waveform[]; // Waveform Data (1200 pairs of BLevel / BColor) => dataSize / 2
} TCNetSmallWaveFormDataPacket;

typedef struct {
    TCNetPacketHeader header;  // Includes Node ID, Protocol Version, Header, Message Type, Node Name, SEQ, Node Type, Node Options, Timestamp
    
    // Data
    uint8_t dataType; // Data type of the TCNet Data Packet. For Big Waveform, it's 32.
    uint8_t layerID;  // Layer number if the layer is sending data. Values: 1 to 8. 
    uint32_t dataSize; // Total size of the data, represents the total data size across all packets. 
    uint32_t totalPackets; // Total number of packets used for data, read as little-endian.
    uint32_t packetNumber; // Packet number of the data, read as little-endian.
    uint32_t dataClusterSize; // Size of the data clusters. In standard cases, it's 4800.
    WaveformData waveform[]; // Waveform data, variable up to 4842 bytes. Odd bytes represent bar levels, even bytes represent bar colors. Equivilent to dataSize / 2 
} TCNetBigWaveformDataPacket;

// Mixer Data

// Mixer Helper Structs


struct MMixerInfo {
    uint8_t mixerID;
    uint8_t mixerType;
    uint8_t reserved[2];  
    char mixerName[16];
    uint8_t reserved2[12];
};

struct MMasterChannel {
    uint8_t reserved[2]; // reserved for mic levels
    uint8_t micEqHi;
    uint8_t micEqLow;
    uint8_t masterAudioLevel;
    uint8_t masterFaderLevel;
    uint8_t reserved2[4];
    uint8_t linkCueA;
    uint8_t linkCueB;
    uint8_t masterFilter;
    uint8_t reserved3;
    uint8_t masterCueA;
    uint8_t masterCueB;
    uint8_t reserved4;
    uint8_t masterIsolatorToggle;
    uint8_t masterIsolatorHigh;
    uint8_t masterIsolatorMid;
    uint8_t masterIsolatorLow;
    uint8_t reserved5;
    uint8_t filterHPF;
    uint8_t filterLPF;
    uint8_t filterResonance;
    uint8_t reserved6[2];
};

struct MSendReturnFX{
    uint8_t sendFXEffect; // Send FX Effect (0-255)
    uint8_t sendFXExt1; // Send FX Ext 1 (0-1)
    uint8_t sendFXExt2; // Send FX Ext 2 (0-1)
    uint8_t sendFXMasterMix; // Send FX Master Mix (0-1)
    uint8_t sendFXSizeFeedback; // Send FX Size Feedback (0-255)
    uint8_t sendFXTime; // Send FX Time (0-255)
    uint8_t sendFXHPF;  // Send FX HPF (0-255)
    uint8_t sendFXLevel;// Send FX Level (0-255)
    uint8_t sendReturn3Source; //Setting of Send Return 3 Source switch (0=CH1, 1=CH2, 2=CH3, 3=CH4, 4=CH5, 5=CH6, 6=MIC, 7=MASTER 8=CRF A, 9=CRF B, 255=NONE)
    uint8_t sendReturn3Type; // Setting of Send Return 3 Source switch (0=CH1, 1=CH2, 2=CH3, 3=CH4, 4=CH5, 5=CH6, 6=MIC, 7=MASTER 8=CRF A, 9=CRF B, 255=NONE)
    uint8_t sendReturn3Toggle; //Current state of Send Return 3 (0=Off, 1=On)
    uint8_t sendReturn3Level; // Value of Send Return 3 Level
    uint8_t reserved;
};

struct MCrossFader {
    uint8_t channelFaderCurve; //Setting of Channel Fader Curve (0-2) 
    uint8_t crossFaderCurve;   //Setting of Cross Fader Curve (0-2)
    uint8_t crossfaderLevel;   //Value of Cross Fader (0-255)
};

struct MBeatFX {
    uint8_t beatFXToggle; //BeatFX ON/OFF (0-1)
    uint8_t beatFXlevel;  //BeatFX Level/Depth (0-255)
    uint8_t beatFXChannelSelect; //BeatFX Channel Select (0-255)
    uint8_t beatFXSelect;   //BeatFX Select (0-255)
    uint8_t beatFXFreqHigh; //BeatFX Frequency Hi (0-255)
    uint8_t beatFXFreqMid;  //BeatFX Frequency Mid  (0-255)
    uint8_t beatFXFreqLow;  //BeatFX Frequency Low (0-255)
};

struct MHeadphones {
    uint8_t headphonesPreEq;  // Headphones Pre EQ (0-255)
    uint8_t headphonesALevel; // Headphones A Level (0-2 55)
    uint8_t headphonesAMix;   // Headphones A Mix (0-255)
    uint8_t headphonesBLevel; // Headphones B Level (0-255)
    uint8_t headphonesBMix;   // Headphones B Mix (0-255)
};

struct MBooth {
    uint8_t boothLevel; // Value of Booth Level (0-255)
    uint8_t boothEqHigh; // Booth EQ High (0-255)
    uint8_t boothEqLow;  // Booth EQ Low (0-255)
    uint8_t reserved[10];
};

struct MixerChannel {
    uint8_t sourceSelect;         // Setting of Channel Source (0=USBA, 1=USBB, 2=DIGITAL, 3=LINE 4=PHONO, 5=INT, 6=RTN1, 7=RTN2, 8=RTN3, 9=RTN_ALL)
    uint8_t audioLevel;           // Value of channel audio level
    uint8_t faderLevel;          // Value of channel fader level
    uint8_t trimLevel;
    uint8_t compLevel;
    uint8_t eqHiLevel;
    uint8_t eqHiMidLevel;
    uint8_t eqLoMidLevel;
    uint8_t eqLowLevel;
    uint8_t colorLevel;
    uint8_t send;
    uint8_t cueA;
    uint8_t cueB;
    uint8_t crossfaderAssign;
    uint8_t reserved[10];       
};


struct TCNetMixerDataPacket {
    TCNetPacketHeader header;   // Packet header

    // Data
    struct MMixerInfo info;
    struct MMasterChannel masterChannel;
    struct MSendReturnFX sendReturnFX;
    struct MCrossFader crossfader;
    struct MBeatFX beatFX;
    struct MHeadphones headphones;
    struct MBooth booth;
    struct MixerChannel channels[TCNET_MAX_CHANNELS]; // Channel array
};


// File Packets


// TCNet Data File Packet - Low Res Artwork File
struct TCNetLowResArtworkFilePacket {
    TCNetPacketHeader header;

    // Data
    uint8_t dataType;               // Datatype of TCNet Data Packet (Low Res Artwork File=128) (V3-2)
    uint8_t layerID;                // Layer number if layer sending data (1-8) (V3-2)
    uint32_t dataSize;              // Total Data size (Total data size including extra packets) (Little Endian) (V3-2)
    uint32_t totalPackets;          // Total number of packets for data (Little Endian) (0-FF) (V3-2)
    uint32_t packetNumber;          // Packet number of data (Little Endian) (V3-2)
    uint32_t dataClusterSize;       // Data Cluster Size (Amount of bytes used per cluster to split up total data) (Little Endian) (Standard value = 4800) (V3-2)
    uint8_t fileData[];         // Raw file data of JPEG file (Max 4842 bytes) (V3-2)
};



// TCNet Time Packet
struct TCNetTimePacket {
    TCNetPacketHeader header;

    // Data
    uint32_t l1Time;                // LAYER 1 Current Time in Milliseconds (Little Endian) (V3-0)
    uint32_t l2Time;                // LAYER 2 Current Time in Milliseconds (Little Endian) (V3-0)
    uint32_t l3Time;                // LAYER 3 Current Time in Milliseconds (Little Endian) (V3-0)
    uint32_t l4Time;                // LAYER 4 Current Time in Milliseconds (Little Endian) (V3-0)
    uint32_t laTime;                // LAYER A Current Time in Milliseconds (Little Endian) (V3-0)
    uint32_t lbTime;                // LAYER B Current Time in Milliseconds (Little Endian) (V3-0)
    uint32_t lmTime;                // LAYER M Current Time in Milliseconds (Little Endian) (V3-0)
    uint32_t lcTime;                // LAYER C Current Time in Milliseconds (Little Endian) (V3-0)
    uint32_t l1TotalTime;           // LAYER 1 Total Time in Milliseconds (Little Endian) (V3-0)
    uint32_t l2TotalTime;           // LAYER 2 Total Time in Milliseconds (Little Endian) (V3-0)
    uint32_t l3TotalTime;           // LAYER 3 Total Time in Milliseconds (Little Endian) (V3-0)
    uint32_t l4TotalTime;           // LAYER 4 Total Time in Milliseconds (Little Endian) (V3-0)
    uint32_t laTotalTime;           // LAYER A Total Time in Milliseconds (Little Endian) (V3-0)
    uint32_t lbTotalTime;           // LAYER B Total Time in Milliseconds (Little Endian) (V3-0)
    uint32_t lmTotalTime;           // LAYER M Total Time in Milliseconds (Little Endian) (V3-0)
    uint32_t lcTotalTime;           // LAYER C Total Time in Milliseconds (Little Endian) (V3-0)
    uint8_t l1BeatMarker;           // Layer 1 Beatmarker (0-4) (V3-0)
    uint8_t l2BeatMarker;           // Layer 2 Beatmarker (0-4) (V3-0)
    uint8_t l3BeatMarker;           // Layer 3 Beatmarker (0-4) (V3-0)
    uint8_t l4BeatMarker;           // Layer 4 Beatmarker (0-4) (V3-0)
    uint8_t laBeatMarker;           // Layer A Beatmarker (0-4) (V3-0)
    uint8_t lbBeatMarker;           // Layer B Beatmarker (0-4) (V3-0)
    uint8_t lmBeatMarker;           // Layer M Beatmarker (0-4) (V3-0)
    uint8_t lcBeatMarker;           // Layer C Beatmarker (0-4) (V3-0)
    uint8_t l1LayerState;           // Layer 1 Layer State (0-FF) (V3-0)
    uint8_t l2LayerState;           // Layer 2 Layer State (0-FF) (V3-0)
    uint8_t l3LayerState;           // Layer 3 Layer State (0-FF) (V3-0)
    uint8_t l4LayerState;           // Layer 4 Layer State (0-FF) (V3-0)
    uint8_t laLayerState;           // Layer A State (0-FF) (V3-0)
    uint8_t lbLayerState;           // Layer B State (0-FF) (V3-0)
    uint8_t lmLayerState;           // Layer M State (0-FF) (V3-0)
    uint8_t lcLayerState;           // Layer C State (0-FF) (V3-0)
};

#pragma pack(pop)
#endif TCNETPACKETS_H