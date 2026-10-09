// gameapi/gizmo_sys_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

// STUB: LEGOBATMAN 0x005bd2d0
// calls statics FUN_005bbf10/FUN_005bbf70 with the sys in edx (register args);
// not attempted
#if 0
typedef struct GIZMOSYS_s {
    GIZMOSET *sets;
    char *error_log;
    u8 flags;
} GIZMOSYS;

typedef struct gizmotype_s {
    char name[32];
    char prefix[8];
    GIZMOFNS fns;
    VARIPTR *buffer;
} GIZMOTYPE;

typedef struct GIZMOSET_s {
    struct gizmotype_s *type;
    i32 count;
    i32 max_count;
    GIZMO *gizmos;
    void *unknown;
} GIZMOSET;

typedef struct GIZMOFNS_s {
    i16 unknown1;
    // pretty sure these two are just padding and can be removed, but keeping them for now to be safe
    u8 unknown2;
    u8 unknown3;
    GIZMOGETMAXGIZMOSFN get_max_gizmos_fn;
    GIZMOADDGIZMOSFN add_gizmos_fn;
    GIZMOEARLYUPDATEFN early_update_fn;
    GIZMOLATEUPDATEFN late_update_fn;
    GIZMODRAWFN draw_fn;
    GIZMOPANELDRAWFN panel_draw_fn;
    void *unknown_fn;
    GIZMOGETGIZMONAMEFN get_gizmo_name_fn;
    GIZMOGETOUTPUTFN get_output_fn;
    GIZMOGETOUTPUTNAMEFN get_output_name_fn;
    GIZMOGETNUMOUTPUTSFN get_num_outputs_fn;
    GIZMOACTIVATEFN activate_fn;
    GIZMOACTIVATEREVFN activate_rev_fn;
    GIZMOSETVISIBILITYFN set_visibility_fn;
    GIZMOGETVISIBILITYFN get_visibility_fn;
    GIZMOGETPOSFN get_pos_fn;
    GIZMOUSINGSPECIALFN using_special_fn;
    GIZMOBOLTHITPLATFN bolt_hit_plat_fn;
    GIZMOGETBESTBOLTTARGETFN get_best_bolt_target_fn;
    GIZMOBOLTHITFN bolt_hit_fn;
    GIZMOALLOCATEPROGRESSDATAFN allocate_progress_data_fn;
    GIZMOCLEARPROGRESSFN clear_progress_fn;
    GIZMOSTOREPROGRESSFN store_progress_fn;
    GIZMORESETFN reset_fn;
    GIZMORESERVEBUFFERSPACEFN reserve_buffer_space_fn;
    GIZMOLOADFN load_fn;
    GIZMOPOSTLOADFN post_load_fn;
    GIZMOADDLEVELSFXFN add_level_sfx_fn;
} GIZMOFNS;

typedef enum nufilemode_e {
    NUFILE_READ = 0,
    NUFILE_WRITE = 1,
    NUFILE_APPEND = 2,
    NUFILE_READ_NOWAIT = 3,
    NUFILE_READWRITE = 4,
    NUFILE_MODE_CNT = 5,
} NUFILEMODE;

i32 gizmoerrorlogsize = 0x800;

GIZMOTYPES *gizmotypes;

void EdFileSetMedia(i32 media);

i32 EdFileOpen(char *filepath, NUFILEMODE mode);

i32 EdFileReadInt();

void EdFileRead(void *buf, i32 len);

i32 GizmoGetTypeIDByName(GIZMOSYS *gizmo_sys, char *name);

char EdFileReadChar();

i32 EdFileClose();

// the attribs byte6 bit7 set (asm has no `orb $0x80,0x46` here). The material
// is queued for dynamic display-list creation and the platform pass runs with
// is_3d=1 (no has_no_transform bit). Byte6 clear keeps NuMtlUpdatePS on the
// RetrieveShaderVariant path instead of the vtx_desc bit2 poke path.
extern "C" NUMTL *NuMtlCreate3D(i32 count);

// from saga legoapi/gizmo/base/gizmo_sys.cpp
void LoadGizmoSys(GIZMOSYS_s *gizmo_sys, void *world, char *config_file) {
    if (gizmo_sys != NULL) {
        gizmo_sys->flags |= GIZMOSYS_FLAG_LOADING;
        if (gizmo_sys->error_log != NULL) {
            memset(gizmo_sys->error_log, 0, gizmoerrorlogsize);
        }
        gizmo_sys->flags &= ~3;

        if (gizmotypes != NULL) {
            char gizmo_name[32];
            char path[256];
            sprintf(path, "%s.giz", config_file);

            EdFileSetMedia(1);
            if (EdFileOpen(path, NUFILE_READ) != 0) {
                EdFileReadInt();
                i32 name_length = EdFileReadInt();
                while (name_length != 0) {
                    memset(gizmo_name, 0, sizeof(gizmo_name));
                    EdFileRead(gizmo_name, name_length);

                    i32 data_length = EdFileReadInt();
                    i32 type_id = GizmoGetTypeIDByName(gizmo_sys, gizmo_name);
                    if (data_length > 0 && type_id >= 0 && type_id < gizmotypes->count &&
                        gizmotypes->types[type_id].fns.load_fn != NULL &&
                        gizmotypes->types[type_id].fns.load_fn(world, gizmo_sys->sets[type_id].unknown) != 0) {
                        name_length = EdFileReadInt();
                        continue;
                    }

                    while (data_length != 0) {
                        EdFileReadChar();
                        --data_length;
                    }
                    name_length = EdFileReadInt();
                }
                EdFileClose();
            }

            // File callbacks may replace or clear the registry before post-load dispatch.
            if (gizmotypes != NULL) {
                GIZMOTYPE *type = gizmotypes->types;
                GIZMOSET *set = gizmo_sys->sets;
                for (i32 type_id = 0; type_id < gizmotypes->count; ++type_id, ++type, ++set) {
                    if (type->fns.post_load_fn != NULL) {
                        type->fns.post_load_fn(world, set->unknown);
                    }
                }
            }
        }

        gizmo_sys->flags &= ~GIZMOSYS_FLAG_LOADING;
    }
}
#endif
