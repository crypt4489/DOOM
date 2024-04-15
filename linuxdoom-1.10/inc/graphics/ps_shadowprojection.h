#ifndef PS_SHADOWSPROJECTION_H
#define PS_SHADOWSPROJECTION_H
#include "ps_global.h"


#define SHADOW_PROJ_PIPE "PROJSHADOW"

void createShadowMatrix(VECTOR plane, VECTOR light_pos, MATRIX matrix);

void create_draw_pipeline_shadow_projection_obj(GameObject *obj, u32 programNumber, u32 qwSize, int context);

qword_t* create_shadow_projection_obj_headers(qword_t* q, GameObject *obj);

void update_shadow_projection_draw(GameObject *obj, VECTOR lightPos, VECTOR plane, VECTOR bottom, VECTOR top, qword_t *q);

int plane_collide_with_light_vector(VECTOR pointOnPoly, VECTOR lightPos, VECTOR planeEqu);

int check_shadow_collides_with_plane(GameObject *currObj, VECTOR lightPos, VECTOR outPlane, VECTOR top, VECTOR bottom, int index);

//int find_plane_to_cast_shadow_on(GameObject *currObj, int indexOfFloorPoly, VECTOR lightPos, VECTOR outPlane, VECTOR top, VECTOR bottom);

qword_t* create_shadow_obj_headers(qword_t* q, GameObject *obj, Color color);

void shadow_proj_call_routine(GameObject *obj);

#endif