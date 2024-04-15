#ifndef PS_GLOBAL_H
#define PS_GLOBAL_H

#include <packet.h>
#include <draw_blending.h>
#include <draw_buffers.h>
#include <draw_primitives.h>
#include <draw_sampling.h>

#define DEFAULT_PIPELINE_SIZE 200

#define MAX_CHAR_TEXTURE_NAME 20
#define MAX_CHAR_PIPELINE_NAME 20
#define MAX_FILE_NAME 35
#define MAX_ANIMATION_NAME 50
#define MAX_JOINT_NAME 50

#define DRAW_DISABLE 0
#define DRAW_ENABLE 1

typedef float VECTOR[4] __attribute__((__aligned__(16)));

typedef float MATRIX[16] __attribute__((__aligned__(16)));

typedef s32 VectorInt[4] __attribute__((__aligned__(16)));

typedef union {
	u64 rgbaq;
	struct {
		u8 r;
		u8 g;
		u8 b;
		u8 a;
		float q;
	};
} __attribute__((packed,aligned(8))) Color;

typedef struct vu1_program_t
{
    union
    {
        struct
        {
            unsigned int size : 12;
            unsigned int address : 12;
            unsigned int stage : 2;
            unsigned int pad : 6;
        };
        u32 programInfo;
    };
    u32 programId;
    u32 *codeStart;
    u32 *codeEnd;

} VU1Program;

#define MAX_VU1_CODE_ADDRESS 2048

typedef struct vu1_manager_t
{
    u16 programsInVU1;
    u16 programIdentifier;
    u32 basePointer;
    u32 numPrograms;
    VU1Program **programs;
} VU1Manager;

enum VU1Programs
{
    VU1GenericStage4 = 0,
    VU1GenericLight3D = 1,
    VU1GenericMorph = 2,
    VU1GenericEnvMap,
    VU1GenericAnimTex,
    VU1GenericSpecular,
    VU1GenericClipper,
    VU1GenericSkinned
};

enum VU1PipelineLocations
{
    VU1_LOCATION_GLOBAL_MATRIX = 4,
    VU1_LOCATION_SCALE_VECTOR = 8,
    VU1_LOCATION_ANIMATION_VECTOR = 11,
    VU1_LOCATION_UV_TRANSFORM = 16,
    VU1_LOCATION_LIGHTS_BUFFER = 20,
};

enum VU1Stages
{
    VU1StageClip = 16,
    VU1Stage4 = 8,
    VU1Stage3 = 4,
    VU1Stage2 = 2,
    VU1Stage1 = 1,
};

#define GEN_PIPELINE_NAME "GEN_PIPELINE"
#define LIGHT_PIPELINE_NAME "LIGHT_PIPELINE"

#define SWAP_ENDIAN(num)          \
    ((num >> 24) & 0xff) |        \
        ((num << 8) & 0xff0000) | \
        ((num >> 8) & 0xff00) |   \
        ((num << 24) & 0xff000000)

#define DMA_DCODE(channel, qwc, tte, type)                        \
    (u32)((tte)&0x00000001) << 0 | (u32)((qwc)&0x00007FFF) << 1 | \
        (u32)((channel)&0x0000000F) << 16 | (u32)((type)&0x00000001) << 20

enum DMA_DCODE
{
    DMA_DCODE_END = 0xFF841234,
    DMA_DCODE_LOAD_OBJ_TEXTURE = 0xFF128765,
    DMA_DCODE_LOAD_ID_TEXTURE = 0xFF128766,
    DMA_DCODE_LOAD_MATERIAL = 0xFF128767,
    DMA_DCODE_CALLBACK_FUNC = 0xFF369453,
    DMA_DCODE_SKIP_QWORDS = 0xFF369454,
    DMA_DCODE_UPLOAD_MESH = 0xFF369455,
    DMA_DCODE_DRAW_FINISH = 0xFF748992
};

typedef union dma_dcode_t
{
    struct
    {
        unsigned int tte : 1;
        unsigned int qwc : 15;
        unsigned int chann : 4;
        unsigned int type : 1;
        unsigned int pad : 11;
    };

    unsigned int code;

} DMA_DCODE_STRUCT;

enum DMATAG_CODES
{
    DMA_CNT = 1,
    DMA_CNTS = 0, // destination tag only
    DMA_NEXT = 2,
    DMA_END = 7,
    DMA_CALL = 5,
    DMA_RET = 6,
    DMA_REF = 3,
    DMA_REFS = 4,
    DMA_REFE = 0
};

typedef struct linked_list_t
{
    struct linked_list_t *next;
    void *data;
} LinkedList;

enum QueueType
{
    FIFO = 1,
    LIFO = 2,
};

typedef struct queue_t
{
    u32 count;
    u32 maxCount;
    u32 type;
    LinkedList *top;
} Queue;

typedef union Bin2Float_t
{
    u32 int_x;
    float float_x;
} Bin2Float;

enum TextureReadType
{
    READ_BMP = 0,
    READ_PNG = 1
};

#define TEX_ADDRESS_CLAMP 0
#define TEX_ADDRESS_WRAP 1

#define PS_TEX_MEMORY 0
#define PS_TEX_VRAM 1

typedef struct
{
    char name[MAX_CHAR_TEXTURE_NAME];
    texbuffer_t texbuf;
    clutbuffer_t clut;
    lod_t lod;
    unsigned char *pixels;
    unsigned char *clut_buffer;
    qword_t *upload;
    u32 width;
    u32 height;
    u32 psm;
    u32 id;
    u16 mode;
    u16 type;
    u16 mipLevels;
    LinkedList *mipMaps;
} Texture;

enum ObjectBoundingTypes
{
    BBO_FIT = 0,
    BBO_FIXED = 1,
    BBO_SPHERE = 2
};

typedef struct
{
    u32 type;
    void *obb;
} ObjectBounds;

typedef struct
{
    VECTOR top;
    VECTOR bottom;
} BoundingBox;

typedef struct
{
    VECTOR center;
    float radius;
} BoundingSphere;

struct morph_target_handle_t;
typedef struct morph_target_handle_t MorphTargetBuffer;
struct interpolater_node_t;
typedef struct interpolater_node_t Interpolator;

struct material_node_t;
typedef struct material_node_t Material;
struct mesh_buffers_t;
typedef struct mesh_buffers_t MeshBuffers;
struct joint_t;
typedef struct joint_t Joint;
struct animation_node_t;
typedef struct animation_node_t AnimationNode;
struct animation_key_t;
typedef struct animation_key_t AnimationKey;
struct animation_key_holder_t;
typedef struct animation_key_holder_t AnimationKeyHolder;
struct animation_data_t;
typedef struct animation_data_t AnimationData;
struct animation_mesh_t;
typedef struct animation_mesh_t AnimationMesh;
struct animator_t;
typedef struct animator_t Animator;
struct pipelinecblist_t;
typedef struct pipelinecblist_t PipelineCallback;
struct vu_pipeline_renderpass_t;
typedef struct vu_pipeline_renderpass_t VU1PipelineRenderPass;
struct vu_pipeline_t;
typedef struct vu_pipeline_t VU1Pipeline;
struct gameobject_t;
typedef struct gameobject_t GameObject;
struct render_world_t;
typedef struct render_world_t RenderWorld;
struct worldcblist_t;
typedef struct worldcblist_t WorldCallback;

typedef void (*world_callback)(RenderWorld *, GameObject *);

struct animation_key_holder_t
{
    u32 id;
    u32 count;
    AnimationKey **keys;
};
struct animation_key_t
{
    float timeStamp;
    VECTOR key;
};

struct joint_t
{
    char name[MAX_JOINT_NAME];
    u32 id;
    MATRIX offset;
};

struct animation_node_t
{
    char name[MAX_ANIMATION_NAME];
    u32 childrenCount;
    MATRIX transformation;
    AnimationNode **children;
};

struct animation_data_t
{
    char name[MAX_ANIMATION_NAME];
    float duration;
    float ticksPerSecond;
    AnimationNode *root;
    u32 numPositionKeys;
    u32 numRotationKeys;
    u32 numScalingKeys;
    AnimationKeyHolder **keyPositions;
    AnimationKeyHolder **keyRotations;
    AnimationKeyHolder **keyScalings;
};

struct animation_mesh_t
{
    u32 jointsCount;
    u32 animationsCount;
    LinkedList *animations;
    Joint **joints;
};

struct animator_t
{
    AnimationData *animation;
    float currentTime;
    float deltaTime;
};

enum MeshBuffersType
{
    MESHVERTICES = 0,
    MESHTRIANGLES = 1
};
typedef struct mesh_vectors
{
    u32 vertexCount;
    VECTOR *vertices __attribute__((aligned(128)));
    VECTOR *normals __attribute__((aligned(128)));
    VECTOR *texCoords __attribute__((aligned(128)));
    VectorInt *bones __attribute__((aligned(128)));
    VECTOR *weights __attribute__((aligned(128)));
    VECTOR *colors __attribute__((aligned(128)));
} MeshVectors;

struct mesh_buffers_t
{
    u32 *indices;
    MeshVectors *meshData[2];
    u32 matCount;
    LinkedList *materials;
    AnimationMesh *meshAnimationData;
};

struct material_node_t
{
    u32 start;
    u32 end;
    u32 materialId;
};

enum AnimationStates
{
    ANIM_PAUSE = 0,
    ANIM_RUNNING = 1,
    ANIM_LOOP = 2,
    ANIM_RUNNING_NEXT = 3,
    ANIM_LOOP_NEXT = 4,
    ANIM_STOPPED = 5
};

typedef u32 (*interpolator_callback)(Interpolator *);
typedef void (*morph_target_callback)(MorphTargetBuffer *);

struct morph_target_handle_t
{
    MeshBuffers **morph_targets; // 0 is always base
    morph_target_callback mtCb;
    u16 meshCap;
    u16 meshCount;
    u32 currInterpNode;
    u32 interpCap;
    u32 interpCount;
    Interpolator **interpolators;
    interpolator_callback intCb;
};

struct interpolater_node_t
{
    u32 status;
    u16 begin;
    u16 end;
    float scale;
    float position;
};

struct worldcblist_t
{
    world_callback callback;
    void *args;
    GameObject *obj;
};

typedef void (*pipeline_callback)(VU1Pipeline *, GameObject *, void *, qword_t *);

struct pipelinecblist_t
{
    u32 id;
    pipeline_callback callback;
    void *args;
    qword_t *q;
    /* data */
};

typedef struct plane_t
{
    VECTOR pointInPlane;  // point in plane
    VECTOR planeEquation; // normal and d
} Plane;

struct vu_pipeline_renderpass_t
{
    MeshBuffers *target;
    qword_t programs;
};

struct vu_pipeline_t
{
    char name[MAX_CHAR_PIPELINE_NAME];
    struct vu_pipeline_t *next;
    int callBackSize;
    int numberCBS;
    PipelineCallback **cbs;
    qword_t *q;
    u32 renderPasses;
    u32 currentRenderPass;
    VU1PipelineRenderPass **passes;
};

typedef struct TessGridStruct
{
    int xDim, yDim;
    BoundingBox extent;
    int tessLevel;
} TessGrid;

typedef struct frustum_t
{
    Plane sides[6];
    float nwidth;
    float nheight;
} Frustum;

typedef struct camera_t
{
    float near;
    float far;
    float aspect;
    float angle;
    u32 height;
    u32 width;
    Frustum *frus;
    MATRIX view;
    MATRIX proj;
    ObjectBounds *obb;
    MATRIX ltm;
} Camera;

enum DrawTags
{
    DRAW_VERTICES = 0x001,
    DRAW_TEXTURE = 0x002,
    DRAW_NORMAL = 0x004,
    DRAW_COLOR = 0x008,
    DRAW_MORPH = 0x010,
    DRAW_SKINNED = 0x020,
    DRAW_ANIM_TEX = 0x040,
    DRAW_ENVMAP = 0x080,
    DRAW_SPECULAR = 0x100,
    DRAW_ALPHAMAP = 0x200
};

#define RENDER_STATE(draw, cull, alpha_enable, alpha_state,                                                                                        \
                     tex_map, color_enable, z_enable, z_type,                                                                                      \
                     light_enable, bface, clip, envmp,                                                                                             \
                     spec, animtex, morph, bones, alphamap)                                                                                \
    (u32)((draw)&0x00000001) << 0 | (u32)((cull)&0x00000001) << 1 | (u32)((alpha_enable)&0x00000001) << 3 | (u32)((alpha_state)&0x00000003) << 4 | \
        (u32)((tex_map)&0x00000001) << 6 | (u32)((color_enable)&0x00000001) << 7 | (u32)((z_enable)&0x00000001) << 8 |                             \
        (u32)((z_type)&0x00000003) << 9 | (u32)((light_enable)&0x00000001) << 11 | (u32)((bface)&0x00000001) << 12 |                               \
        (u32)((clip)&0x00000001) << 13 | (u32)((envmp)&0x00000001) << 14 | (u32)((spec)&0x00000001) << 15 | (u32)((animtex)&0x00000001) << 16 |    \
        (u32)((morph)&0x00000001) << 17 | (u32)((bones)&0x00000001) << 18 | (u32)((alphamap)&0x00000001) << 19

typedef union obj_render_state
{
    struct
    {
        unsigned int DRAWING_OPTION : 1;     // 1
        unsigned int CULLING_OPTION : 1;     // 2
        unsigned int CULLING_VISIBLE : 1;    // 3
        unsigned int ALPHA_ENABLE : 1;       // 4
        unsigned int ALPHA_STATE : 2;        // 5-6
        unsigned int TEXTURE_MAPPING : 1;    // 7
        unsigned int COLOR_ENABLE : 1;       // 8
        unsigned int Z_ENABLE : 1;           // 9
        unsigned int Z_TYPE : 2;             // 10-11
        unsigned int LIGHTING_ENABLE : 1;    // 12
        unsigned int BACKFACE_CULLING : 1;   // 13
        unsigned int CLIPPING : 1;           // 14
        unsigned int ENVIRONMENTMAP : 1;     // 15
        unsigned int SPECULAR : 1;           // 16
        unsigned int ANIMATION_TEXUTRE : 1;  // 17
        unsigned int MORPH_TARGET : 1;       // 18
        unsigned int SKELETAL_ANIMATION : 1; // 19
        unsigned int ALPHA_MAPPING : 1;      // 20
        unsigned int pad : 12;               // 21-32
    };

    unsigned int state;

} OBJ_RENDER_STATE;

struct gs_state_t
{
    int gs_reg_count;
    int gs_reg_mask;
    OBJ_RENDER_STATE render_state;
};

typedef struct render_state_t
{
    prim_t prim;
    Color color;
    struct gs_state_t state;
} ObjectRenderState;

enum Lighting
{
    PS_AMBIENT_LIGHT = 0,
    PS_DIRECTIONAL_LIGHT = 1,
    PS_POINT_LIGHT = 2,
    PS_SPOT_LIGHT = 3,
};

typedef struct lights_t
{
    MATRIX ltm;
    VECTOR color;
    float theta;
    float radius;
    u32 type;
} LightStruct;

struct render_world_t
{
    u32 objectCount;
    u32 lightCount;

    LinkedList *objList;
    LinkedList *lights;

    Camera *cam;
};

struct gameobject_t
{
    MATRIX ltm;
    MeshBuffers vertexBuffer;
    void (*update_object)(struct gameobject_t *);
    void *objData;
    ObjectBounds *obb;
    VU1Pipeline *pipelines;
    VU1Pipeline *activePipeline;
    ObjectRenderState renderState;
    MorphTargetBuffer *interpolator;
    Animator *objAnimator;
};

typedef struct RenderTarget
{
    framebuffer_t *render;
    zbuffer_t *z;
} RenderTarget;

typedef struct
{
    u32 globalIndex;  // unique identifier
    u32 count;        // global tex count
    int currIndex;    // current texture loaded into memory
    LinkedList *list; // list of textures;
} TexManager;

struct dma_buffers_t;
typedef struct dma_buffers_t DMABuffers;

struct dma_buffers_t
{
    packet_t *dma_chains[2];
    qword_t *currPointer;
    int bufferId;
};

struct timer_struct_t;
typedef struct timer_struct_t TimerStruct;

struct timer_struct_t
{
    u64 ctr;
    s32 id;
};

typedef struct
{
    RenderTarget *targetBack;
    RenderTarget *targetDisplay;
    TexManager *texManager;
    VU1Manager *vu1Manager;
    Texture *textureInVram;
    Camera *mainCam;
    DMABuffers *dmabuffers;
    u32 vu1DoneProcessing;
    u16 gs_context;
    u16 enableDoubleBuffer;
    Color bgkc;
    u32 ScreenWidth, ScreenHeight;
    u32 ScreenWHalf, ScreenHHalf;
    float lastTime, currentTime;
    u32 FPS;
    TimerStruct *timer;
} GameManager;

typedef struct waves_t
{
    float amp;
    float wavelength;
    float time;
    float velocity;
    float firstTerm;
} Waves;

typedef struct font_t
{
    Texture *fontTex;
    char *fontWidths;
    Color color;
    prim_t prim;
    u16 picHeight;
    u16 picWidth;
    u8 cellWidth;
    u8 cellHeight;
    u16 startingChar;
    u32 widthSize;
} Font;

// Global Variables

extern RenderWorld *g_DrawWorld; // active render world. PS_RenderWorld.c
extern GameManager g_Manager;    // PS2 render manager PS_Manager.c
extern Camera *g_DrawCamera;     // current drawing camra PS_Camera.c


// vu1 data address VU1Manager.c
extern volatile u32 *vu1_data_address;

extern volatile u32 *vu0_data_address;

extern volatile u32 *vif1_top;

extern volatile u32 *vif1_tops;

#endif
