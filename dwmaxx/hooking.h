#pragma once

#define DEFINE_HOOK(result, name, ...) \
    typedef result ( WINAPI * ##name##_PROC )( __VA_ARGS__ ); \
    result WINAPI My_##name( __VA_ARGS__ );

#define HOOK(result, name, ...) \
    name##_PROC __##name = NULL; \
    result WINAPI My_##name( __VA_ARGS__ )

#define IAT_HOOK(imageName, importImageName, importName) \
    extern importName##_PROC __##importName; \
    __##importName = (importName##_PROC)IATPatchSub(imageName, importImageName, #importName, (FARPROC)My_##importName);

//#define EASY_HOOK(InEntryPoint, InHookProc, InCallback) \
//    extern importName##_PROC __##importName; \
//    __##importName = (importName##_PROC)IATPatchSub(imageName, importImageName, #importName, (FARPROC)My_##importName);


#define DEFINE_METHOD_HOOK(result, type, name, ...) \
    typedef result ( WINAPI * ##type##_##name##_PROC )( type *This, __VA_ARGS__ ); \
    result WINAPI My_##type##_##name( type *This, __VA_ARGS__ );

#define METHOD_HOOK(result, type, name, ...) \
    type##_##name##_PROC __##type##_##name = NULL; \
    result WINAPI My_##type##_##name( type *This, __VA_ARGS__ )

#define DX_METHOD_HOOK(pObject, type, name) \
    extern type##_##name##_PROC __##type##_##name; \
    __##type##_##name = (type##_##name##_PROC)HookVTableProc((FARPROC *)&((pObject)->lpVtbl->##name), (FARPROC)My_##type##_##name);

#define DX_HOOK_GET_ORIGINAL_PROC(type, name) \
    extern type##_##name##_PROC __##type##_##name;

#define HOOK_GET_ORIGINAL_PROC(name) \
    extern name##_PROC __##name;

#define IAT_UNHOOK(imageName, importImageName, importName) \
    IATPatchSub(imageName, importImageName, #importName, (FARPROC)__##importName); \
    __##importName = NULL;

#define DX_METHOD_UNHOOK(pObject, type, name) \
    HookVTableProc((FARPROC *)&((pObject)->lpVtbl->##name), (FARPROC)__##type##_##name); \
    __##type##_##name = NULL;

FARPROC HookVTableProc(FARPROC *vtableproc, FARPROC newfunc);
FARPROC IATPatchSub(char *imageName, char *importImageName, char *importName, FARPROC newProc);
