#ifndef RES_FS_H
#define RES_FS_H

#ifdef __cplusplus
extern "C" {
#endif
#include "res.h"

/* 注册资源文件系统驱动. */
void res_fs_init(void);

const jl_resource_info_t *find_res_by_path(const char *path);

#ifdef __cplusplus
}
#endif

#endif
