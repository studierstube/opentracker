#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>

// *** PI constant
#define PI 3.141592653589793

// *** constants for the facial topology
// *** must be maintained when adding new vertices or triangles!!!
#define MAX_VERTEX          825     // *** max vertex NUMBER
#define MAX_TRIANGLE        1386    // *** max triangle NUMBER
#define MAX_FACE_VERTEX     787     // *** max face vertex NUMBER (for smoothing)
#define MAX_FACE_TRIANGLE   1358    // *** max face triangle NUMBER (for smoothing)
#define MAX_FID             37      // *** max vertex group INDEX
#define MAX_ACTION_UNIT     23      // *** max action unit number
#define MAX_VISEME          22      // *** max viseme number (INCLUDING "silence")
#define MAX_COLOR           18      // *** max color code number
#define MAX_RND_ACTION      8       // *** max random behavior action type

// *** for speech and message bubble
#define MAX_TEXT                255     // *** max message length
#define MAX_SPEEK_TEXT_LENGTH   30      // *** max displayed message length

enum {
    RND_BLINK,RND_SWALLOW,RND_MOUTH,RND_EYEBROW,RND_EYE,RND_HEAD,RND_ACTION
};

// *** constant values we should get rid of!
#define IDLE_TIME 0.0f // 2.5f
#define DEFAULT_ZOOM 0.07f

// *** type:   old -> new
// ***  exp:     0 -> 9
// *** head:   1-3 -> 11-13
// ***  eye:   4-7 -> 14-17 
// ***  col:  8-15 -> 18-35 
// ***   au: 26-48 -> 36-58
enum AnimationType {    
    ANIM_NULL=0,
    ANIM_PLAY,ANIM_SPEAK,ANIM_SND,ANIM_SHUT,ANIM_THINK,
    ANIM_EXP,ANIM_VIS,
    ANIM_HEAD_VER,ANIM_HEAD_HOR,ANIM_HEAD_ZOOM,
    ANIM_EYE_L_VER,ANIM_EYE_L_HOR,ANIM_EYE_R_VER,ANIM_EYE_R_HOR,
    ANIM_COL_FACE,ANIM_COL_LIP,ANIM_COL_NOSE,ANIM_COL_EYELID,ANIM_COL_EYEBROW,ANIM_COL_CHEEK,
        ANIM_COL_CHIN,ANIM_COL_MOUSTACHE,ANIM_COL_IRIS,ANIM_COL_EYEBALL,ANIM_COL_TEETH,
        ANIM_COL_GUM,ANIM_COL_HAIR,ANIM_COL_EYELASH,ANIM_COL_PUPIL,ANIM_COL_EAR,ANIM_COL_TONGUE,
        ANIM_COL_GLASSES, // ANIM_COL_FACE ... ANIM_COLOR_FACE+MAX_COLOR
    ANIM_AU1,ANIM_AU2,ANIM_AU3,ANIM_AU4,ANIM_AU5,ANIM_AU6,ANIM_AU7,ANIM_AU8,ANIM_AU9,
    ANIM_AU10,ANIM_AU11,ANIM_AU12,ANIM_AU13,ANIM_AU14,ANIM_AU15,ANIM_AU16,ANIM_AU17,ANIM_AU18,
    ANIM_AU19,ANIM_AU20,ANIM_AU21,ANIM_AU22,ANIM_AU23 // ANIM_AU ... ANIM_AU+MAX_ACTION_UNIT
};
#define ANIM_HEAD ANIM_HEAD_VER
#define ANIM_EYE ANIM_EYE_L_VER
#define ANIM_COL ANIM_COL_FACE
#define ANIM_AU ANIM_AU1
#define MAX_ANIM 55 // *** max type of requests

// *** grouping parts of the face (e.g, nose , eyebrows, eye etc.) --> face index
enum {
    FID_LIP_LEFT=0,         FID_LIP_RIGHT,
    FID_MOUTHAREA_LEFT,     FID_MOUTHAREA_RIGHT,
    FID_NOSE,   
    FID_EYEAREA_LEFT,       FID_EYEAREA_RIGHT,
    FID_EYELID_LEFT,        FID_EYELID_RIGHT,   
    FID_FOREHEAD_LEFT,      FID_FOREHEAD_RIGHT,
    FID_EYEBROW_LEFT,       FID_EYEBROW_RIGHT,
    FID_CHEEK_LEFT,         FID_CHEEK_RIGHT,    
    FID_CHIN_LEFT,          FID_CHIN_RIGHT,
    FID_NECK_LEFT,          FID_NECK_RIGHT,
    FID_IRIS_LEFT,          FID_IRIS_RIGHT,
    FID_EYEBALL_LEFT,       FID_EYEBALL_RIGHT,  
    FID_TEETH_UPPER,        FID_TEETH_LOWER,    
    FID_GUM_UPPER,          FID_GUM_LOWER,
    FID_HAIR_LEFT,          FID_HAIR_RIGHT,
    FID_EYELASH_LEFT,       FID_EYELASH_RIGHT,
    FID_PUPIL_LEFT,         FID_PUPIL_RIGHT,
    FID_EAR_LEFT,           FID_EAR_RIGHT,
    FID_TONGUE,
    FID_GLASSES
};

// *** color codes
enum {
    COL_FACE=0,
    COL_LIP,
    COL_NOSE,   
    COL_EYELID,
    COL_EYEBROW,
    COL_CHEEK,
    COL_CHIN,
    COL_MOUSTACHE,
    COL_IRIS,
    COL_EYEBALL,
    COL_TEETH,
    COL_GUM,
    COL_HAIR,
    COL_EYELASH,
    COL_PUPIL,
    COL_EAR,
    COL_TONGUE,
    COL_GLASSES
};

static char colorGroupName[MAX_COLOR][255]={
    "face",
    "lip",
    "nose",
    "eyelid",
    "eyebrow",
    "cheek",
    "chin",
    "moustache",
    "iris",
    "eyeball",
    "teeth",
    "gum",
    "hair",
    "eyelash",
    "pupil",
    "ear",
    "tongue",
    "glasses"
};

// *** for surface smoothing: index of vertices where smooth normals shouldn't be calculated
static int disableNormalIndex[]={
    75,77,81,84,85,89,90,94,95,97,98,99,            // nose
    371,372,373,374,375,376,377,378,379,380,381,389,
    391,392,393,394,395,396,397,398,399,400,401,    // head contour
    129,130,
    164,165,                                        // eye area
    27,28,29,67,68,69                               // lips

};

// *** constants for eye and head movement
#define MAX_EYE_ANGLE   30
#define MAX_HEAD_ANGLE  180
#define MIN_HEAD_ZOOM   0
#define MAX_HEAD_ZOOM   500

// *** special face topology vertex and triangle IDs for auxiliary facial elements and rendering
#define VERTEX_ID_EYE_TOP_LEFT      426
#define VERTEX_ID_EYE_BOTTOM_LEFT   430
#define VERTEX_ID_EYE_TOP_RIGHT     451
#define VERTEX_ID_EYE_BOTTOM_RIGHT  455
#define VERTEX_ID_EYELASH_LEFT1     116
#define VERTEX_ID_EYELASH_LEFT2     122
#define VERTEX_ID_EYELASH_RIGHT1    151
#define VERTEX_ID_EYELASH_RIGHT2    157
#define VERTEX_ID_PUPIL_LEFT        409
#define VERTEX_ID_PUPIL_RIGHT       434
#define VERTEX_ID_GLASSES           787
#define TRIANGLE_ID_IRIS_LEFT       738
#define TRIANGLE_ID_IRIS_RIGHT      778
#define TRIANGLE_ID_EYE_LEFT_LAST   777
#define TRIANGLE_ID_EYE_RIGHT_LAST  817

// *** filename constants
#define FILENAME_VERTEX             "model\\vertex.dat"
#define FILENAME_TRIANGLE           "model\\triangle.dat"
#define FILENAME_COLOR              "color\\color.dat"
#define FILENAME_BASE_ACTIONUNIT    "actionunit\\au"
#define FILENAME_BASE_VISEME        "viseme\\vis"
#define FILENAME_BASE_EXPRESSION    "expression\\"
#define FILENAME_BASE_ACTION        "action\\"
#define FILENAME_BASE_VOICE         "voice\\"
#define FILENAME_BASE_CONFORMATION  "conformation\\"
#define FILENAME_BASE_STB           "bin\\apps\\arpuppet\\characters\\facepuppet"

enum {
    ERR_INIT_BEHAVIOR,
    ERR_INIT_RENDERER,
    ERR_RENDER_SCENE,
    ERR_SETUP_RC,
    ERR_SETPIXELFORMAT,
    ERR_INIT_FACEMODEL_VERTEX,
    ERR_INIT_FACEMODEL_TRIANGLE,
    ERR_INIT_FACEMODEL_CONFORMATION,
    ERR_INIT_DEFORMATION_AU,
    ERR_INIT_DEFORMATION_VIS,
    ERR_INIT_DEFORMATION_EXP,
    ERR_INIT_DEFORMATION_COLOR,
    ERR_INIT_BEHAVIOR_ACT,
    ERR_INIT_VOICE,
    ERR_INIT_COM
};

// *** strings
static char errorString[][255]={
    "Failed to initialize behavior engine",
    "Failed to initialize OpenGL engine",
    "Error while rendering scene",
    "Failed to set up rendering context",
    "Failed to set pixel format",
    "Unable to initialize face model - vertex file error",
    "Unable to initialize face model - triangle file error",
    "Unable to initialize face model - conformation file error",
    "Unable to initialize action unit data - action unit file error",
    "Unable to initialize viseme data - viseme file error",
    "Unable to load expression files",
    "Unable to load color file",
    "Unable to load action files",
    "Unable to initialize TTS",
    "Unable to initialize COM"
};

static void ErrorMessage(char errorMessage){
    
    printf("AGENT ERROR: %s\n",errorString[errorMessage]);
}

static char visemeString[MAX_VISEME][15]={
        "silence",
        "ae,ax,ah",
        "aa",
        "ao",
        "ey,eh,uh",
        "er",
        "y,iy,ih,ix",
        "w,uw",
        "ow",
        "aw",
        "oy",
        "ay",
        "h",
        "r",
        "l",
        "s,z",
        "sh,ch,jh,zh",
        "th,dh",
        "f,v",
        "d,t,n",
        "k,g,ng",
        "p,b,m"
};


// *** typedef for "animation command finished" callback function
typedef void SoFinishCB(void* userData, int id);

#endif