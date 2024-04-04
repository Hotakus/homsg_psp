/**
  ******************************************************************************
  * @file           : homsg_sp_conf.h
  * @author         : Hotakus (hotakus@foxmail.com)
  * @brief          : None
  * @date           : 2024/4/4
  ******************************************************************************
  */

#ifndef HOMSG_SP_HOMSG_PSP_CONF_H
#define HOMSG_SP_HOMSG_PSP_CONF_H

#define HOMSG_SP_DIR_ONE 1
#define HOMSG_SP_DIR_TWO 2
#define HOMSG_SP_DIR HOMSG_SP_DIR_TWO

#define HOMSG_SP_SUBJECT_MAX_CNT  (256u)
#define HOMSG_SP_SUBSCRIBER_MAX_CNT (256u)

#define HOMSG_USE_CUSTOM_MEM_FUNC 0
#if (HOMSG_USE_CUSTOM_MEM_FUNC == 1)
#include "xxx.h"
#define HOMSG_SP_MALLOC(x)  your_malloc(x)
#define HOMSG_SP_FREE(x)    your_free(x)
#else
#include <stdlib.h>
#define HOMSG_SP_MALLOC(x)  malloc(x)
#define HOMSG_SP_FREE(x)    free(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif // __cplusplus

#endif  //HOMSG_SP_HOMSG_PSP_CONF_H
