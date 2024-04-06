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
#include "chain.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum homsg_res_t homsg_res_t;
typedef enum homsg_res_t {
    HOMSG_RES_OK = 0,
    HOMSG_RES_ERROR,
    HOMSG_RES_NO_SUBSCRIBER,
    HOMSG_RES_ALREADY_SUBSCRIBED,
    HOMSG_RES_ALREADY_REGISTERED,
    HOMSG_RES_ALREADY_PUBLISHED,
    HOMSG_RES_NOT_REGISTERED,
    HOMSG_RES_NOT_PUBLISHED,
} homsg_res_t;


typedef struct homsg_subject_t homsg_subject_t;
typedef struct homsg_subject_t {
    chain_t *subscribers;
} homsg_subject_t;

// update callback
typedef void (*homsg_subscriber_update_callback_t)(void *msg);

typedef struct homsg_psp_t homsg_psp_t;
typedef struct homsg_psp_t {

    //create subject
    homsg_subject_t *(*create_subject)(char *desc);
    void (*destroy_subject)(homsg_subject_t *subject);

    // TODO: whether use functions of subjects and subscribers manager
#if (HOMSG_SP_USE_SS_MANAGER == 1)
    chain_t *all_subjects;

    struct {
        homsg_res_t (*find_subject)(homsg_psp_t *psp, char *desc);

        homsg_res_t (*publish)(homsg_psp_t *psp, homsg_subject_t *subject);
        homsg_res_t (*revoke)(homsg_psp_t *psp, homsg_subject_t *subject);
    };

#endif

    // publisher
    struct {
        homsg_res_t (*notify)(homsg_subject_t *subject, void *msg);
    };

    // subscriber
    struct {
#if (HOMSG_SP_USE_SS_MANAGER == 1)
        homsg_res_t (*subscribe)(homsg_psp_t *psp, homsg_subject_t *subject, char *subscriber_name, homsg_subscriber_update_callback_t update);
        homsg_res_t (*unsubscribe)(homsg_psp_t *psp, homsg_subject_t *subject, char *subscriber_name);
#else
        homsg_res_t (*subscribe)(homsg_subject_t *subject, char *subscriber_name, homsg_subscriber_update_callback_t update);
        homsg_res_t (*unsubscribe)(homsg_subject_t *subject, char *subscriber_name);
#endif
    };
} homsg_psp_t;

homsg_psp_t *homsg_psp_create();
void homsg_psp_destroy(homsg_psp_t *psp);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  //HOMSG_SP_HOMSG_PSP_H
