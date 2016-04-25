 #ifdef __cplusplus
 #define EXTERNC extern "C"
 #else
 #define EXTERNC
 #endif

 #include "../dynamic_libs/gx2_types.h"

 EXTERNC void draw_Cursor_at(f32 x, f32 y);
 EXTERNC void draw_Cursor_destroy();

 #undef EXTERNC
