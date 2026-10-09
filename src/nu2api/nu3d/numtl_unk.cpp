// nu2api/nu3d/numtl_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nucore/common.h"

typedef struct numtl_s {
  unsigned char pad0[0x2c0];
  u16 version; // 0x2c0, bumped by NuMtlUpdate
  u16 filler5;
} NUMTL;

void NuMtlUpdatePS(NUMTL *mtl);

// STUB: LEGOBATMAN 0x006ef910
// PC body diverges from saga (DX9 shader manager thiscalls on 0x9d08d0); not
// attempted
#if 0
typedef int32_t i32;
typedef i32 NUCOLOUR32;

struct nushaderobject_s {
    NUSHADEROBJECTGLSL glsl;
    NUSHADERUSAGEMASK *usage_mask;
    i32 last_uniform_frame;   // 0x20
    void *last_light_packet;  // 0x24
    void *last_camera_packet; // 0x28
    GLSLParameter parameters[NUSHADEROBJECT_PARAMETERS_COUNT];
    // Present in both serialized objects and the original manager slot stride.
    u8 unknown_0x304[4];
};
typedef nushaderobject_s NUSHADEROBJECT;

struct nushaderobjectglsl_s {
    NUSHADEROBJECTBASE base;
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
};
typedef struct nushaderobjectglsl_s NUSHADEROBJECTGLSL;

struct GLSLParameter {
    i16 location;
    union {
        u8 element_count_and_setter;
        struct {
            u8 setter_class : 2;
            u8 element_count : 6;
        };
    };
    u8 array_size;
    u8 semantic;
    union {
        u8 type_and_flags;
        struct {
            u8 parameter_type : 4;
            u8 flags : 4;
        };
    };
    u8 reserved[2];

#ifdef __cplusplus
    void setElementsMatrix(i32 first_element, i32 count, const f32 *values) __attribute__((weak));
#endif
};

typedef struct nushadermtldesc_s {
    u32 flags; // 0x000

    i32 diffuse_map_tex_id[4];   // 0x004
    NUCOLOUR32 diffuse_color[4]; // 0x014

    f32 unknown_24;         // 0x024
    u8 unknown_28[0x0c];    // 0x028..0x033
    i32 lightmap_tex_id[2]; // 0x034
    u8 unknown_3c[0x08];    // 0x03C..0x043

    u8 blend_op2;  // 0x044 (0xff = no shader retrieval)
    u8 blend_op3;  // 0x045
    u8 blend_op4;  // 0x046
    u8 unknown_47; // 0x047

    i32 specular_map_tid;   // 0x048
    i32 normal_map_tid;     // 0x04C
    i32 envmap_cubic_tid;   // 0x050
    i32 shine_map_ps2_tid;  // 0x054
    i32 vtf_height_map_tid; // 0x058
    i32 vtf_normal_map_tid; // 0x05C

    u8 unknown_60[0x48]; // 0x060..0x0A7
    u8 unknown_a8;       // 0x0A8 (texture-count-ish, forced >= 1)
    u8 unknown_a9[0x63]; // 0x0A9..0x10B

    i32 tex_anim_data[4];       // 0x10C
    f32 tex_anim_offsets[4][2]; // 0x11C

    NUVERTEXDESCRIPTOR vtx_desc; // 0x13C

    union {
        struct {
            i16 shader_id;         // 0x140 — assigned by NuMtlUpdatePS
            i16 shader_variant_id; // 0x142 (-1 when unvarianted)
        };
        i16 shader_ids[2];
    };

    NUTEXANIMDATA tex_anim_desc[4]; // 0x144
    u8 unknown_194[4];              // 0x194..0x197
    i32 unknown_198;                // 0x198

    u8 unknown_19c[0x18]; // 0x19C..0x1B3
    u8 unknown_1b4;       // 0x1B4
    u8 filler_1b5[3];     // 0x1B5..0x1B7

    u8 flagsbits_1b8; // 0x1B8 (bitfield dword, accessed per-byte)
    u8 byte4;         // 0x1B9 (legacy name: alpha-test select bits)
    u8 flagsbits_1ba; // 0x1BA
    u8 flagsbits_1bb; // 0x1BB

    u32 field_1bc;        // 0x1BC
    u8 unknown_1c0[0x24]; // 0x1C0..0x1E3
    i32 field_1e4;        // 0x1E4
    i32 field_1e8;        // 0x1E8
    u8 unknown_1ec[0x1C]; // 0x1EC..0x207
} NUSHADERMTLDESC;

typedef struct nuvertexdescriptor_s {
    union {
        struct {
            u32 has_position : 1;

            u32 unknown_0_2 : 1;

            u32 has_normal : 1;
            u32 has_packed_normal : 1;
            u32 has_tangent : 1;
            u32 has_packed_tangent : 1;
            u32 has_binormal : 1;
            u32 has_packed_binormal : 1;

            u32 has_diffuse : 1;

            u32 unknown_1_2_4 : 2;
            u32 tex_coord_mode : 3;
            u32 unknown_1_64 : 1;
            u32 unknown_1_128 : 1;

            u32 unknown_2_1 : 1;
            u32 unknown_2_2 : 1;

            u32 has_no_transform : 1;

            u32 unknown_2_8 : 1;
            u32 unknown_2_16 : 1;
            u32 unknown_2_32 : 1;
            u32 unknown_2_64 : 1;
            u32 unknown_2_128 : 1;

            u32 unknown_3_1_2_3 : 3;
            u32 has_half_uvs : 1;
        };

        u32 flags;
    };
} NUVERTEXDESCRIPTOR;

struct nushaderobjectbase_s {
    i32 field0;
    i32 field1;
    u32 key;
    i32 field3;
};
typedef struct nushaderobjectbase_s NUSHADEROBJECTBASE;

typedef struct nutexanimdata_s {
    NUTEXANIM_MODE u_mode;
    NUTEXANIM_MODE v_mode;
    u8 padding_02[2];
    f32 u_amplitude;
    f32 v_amplitude;
    f32 u_frequency;
    f32 v_frequency;
} NUTEXANIMDATA;

enum NUTEXANIM_MODE : u8 {
    NUTEXANIM_MODE_NONE = 0,
    NUTEXANIM_MODE_LINEAR = 2,
    NUTEXANIM_MODE_SINE = 3,
    NUTEXANIM_MODE_COSINE = 4,
};

extern "C" void NuShaderManagerReleaseShader(NUSHADEROBJECT *slot);

extern "C" NUSHADEROBJECT *NuShaderManagerGetShaderById(i32 id);

extern "C" void *NuShaderManagerRetrieveShader(NUSHADERMTLDESC *desc, void *mtl);

extern "C" void *NuShaderManagerRetrieveShaderVariant(NUSHADERMTLDESC *desc, void *mtl, i32 variant);

NuVertexFormatPS *NuGetVertexDeclaration(NUVERTEXDESCRIPTOR vtx_desc);

// from saga nu2api/nu3d/numtl.cpp
void NuMtlUpdatePS(numtl_s *mtl) {
    if (0 < mtl->tex_id) {
        mtl->shader_desc.diffuse_map_tex_id[0] = mtl->tex_id;

        i32 count = mtl->shader_desc.unknown_a8;
        if (count == 0) {
            count = 1;
        }
        mtl->shader_desc.unknown_a8 = count;

        // Vtx-desc tex-unit nibble: at least one unit.
        u8 b = ((u8 *)&mtl->shader_desc.vtx_desc)[1];
        u8 units = (b >> 3) & 7;
        if (units == 0) {
            units = 1;
        }
        ((u8 *)&mtl->shader_desc.vtx_desc)[1] = (b & 199) | (units << 3);
    }

    if ((((u8 *)&mtl->shader_desc.vtx_desc)[3] & 4) != 0) {
        mtl->shader_desc.flagsbits_1bb |= 0x80;
        mtl->shader_desc.flags |= 0x200000;
    }

    if (0 < mtl->shader_desc.shader_id) {
        NuShaderManagerReleaseShader(NuShaderManagerGetShaderById(mtl->shader_desc.shader_id));
    }
    if (0 < mtl->shader_desc.shader_variant_id) {
        NuShaderManagerReleaseShader(NuShaderManagerGetShaderById(mtl->shader_desc.shader_variant_id));
    }

    if (((char *)&mtl->attribs)[6] < 0 && mtl->display_list == NULL) {
        if (mtl->shader_desc.blend_op2 != 0xff) {
            ((u8 *)&mtl->shader_desc.vtx_desc)[2] |= 4;
            void *shader = NuShaderManagerRetrieveShader(&mtl->shader_desc, mtl);
            if (shader != NULL) {
                mtl->shader_desc.shader_id = *(i16 *)shader;
            }
        }
    } else if (mtl->shader_desc.blend_op2 != 0xff) {
        void *shader = NuShaderManagerRetrieveShaderVariant(&mtl->shader_desc, mtl, 0x10);
        if (shader != NULL) {
            mtl->shader_desc.shader_variant_id = -1;
            mtl->shader_desc.shader_id = *(i16 *)shader;
        }
    }

    mtl->vertex_decl = NuGetVertexDeclaration(mtl->shader_desc.vtx_desc);

    if (mtl->tex_id == 0) {
        i32 packed = ((i32)(mtl->diffuse_color.r * 255.0f) & 0xff) | 0xff000000 |
                     (((i32)(mtl->diffuse_color.g * 255.0f) & 0xff) << 8) |
                     (((i32)(mtl->diffuse_color.b * 255.0f) & 0xff) << 16);
        mtl->shader_desc.diffuse_color[0] = (NUCOLOUR32)packed;
    }
}
#endif

// FUNCTION: LEGOBATMAN 0x00727b40
void NuMtlUpdate(NUMTL *mtl) {
  NuMtlUpdatePS(mtl);
  mtl->version++;
}

struct NuDynamicLight {
  unsigned int data[0x12f0 / 4];

  static void destroy(NuDynamicLight *light);
};

// GLOBAL: LEGOBATMAN 0x029f8ef0
extern NuDynamicLight g_dynamicLights[20];
// GLOBAL: LEGOBATMAN 0x029df970
extern unsigned char g_dynamicLightUsed[20];

// GLOBAL: LEGOBATMAN 0x029f4640
i32 numtl_renderplane;

// An empty function (0x006fceb0 is a lone `ret`), see nutexanm_gen.cpp.
static void NuErrorUnk006fceb0(...) {}

// STUB: LEGOBATMAN 0x00727f20
// matches (tested) once some other function in this TU calls the empty static
// NuErrorUnk006fceb0 with a pointer argument; until then VC8 drops the call.
extern "C" i32 NuMtlSetCurrentRenderPlane(i32 render_plane) {
  if (render_plane >= 0x18 || render_plane < 0)
    NuErrorUnk006fceb0();
  i32 previous_render_plane = numtl_renderplane;
  numtl_renderplane = render_plane;
  return previous_render_plane;
}

// FUNCTION: LEGOBATMAN 0x0072b040
void NuDynamicLight::destroy(NuDynamicLight *light) {
  for (int i = 0; i < 20; i++) {
    if (&g_dynamicLights[i] == light) {
      g_dynamicLightUsed[i] = 0;
    }
  }
}
