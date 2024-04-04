/**
  ******************************************************************************
  * @file           : homsg_sp.h
  * @author         : Hotakus (hotakus@foxmail.com)
  * @brief          : None
  * @date           : 2024/4/4
  ******************************************************************************
  */

#ifndef HOMSG_SP_HOMSG_PSP_H
#define HOMSG_SP_HOMSG_PSP_H

#include <stdint.h>
#include "homsg_psp_conf.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum homsg_res_t homsg_res_t;
typedef enum homsg_res_t {
    HOMSG_RES_OK = 0,
    HOMSG_RES_ERROR
} homsg_res_t;

// update callback
typedef void (*homsg_subscriber_update_callback_t)(void *msg);

typedef struct homsg_subscriber_node_t homsg_subscriber_node_t;
typedef struct homsg_subscriber_node_t {
    size_t index;
    homsg_subscriber_update_callback_t update;
} homsg_subscriber_node_t;

typedef struct homsg_subject_node_t homsg_subject_node_t;
typedef struct homsg_subject_node_t {
    size_t cnt;
    homsg_subscriber_node_t **subscribers;
} homsg_subject_node_t;


typedef struct homsg_sp_t homsg_sp_t;
typedef struct homsg_sp_t {
    struct {
        // publish a new subject
        homsg_subject_node_t *(*create_subject)(const char *desc);

    };
} homsg_sp_t;

homsg_sp_t *homsg_sp_create();
void homsg_sp_destroy(homsg_sp_t *sp);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  //HOMSG_SP_HOMSG_PSP_H
