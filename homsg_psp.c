/**
  ******************************************************************************
  * @file           : homsg_sp.c
  * @author         : Hotakus (hotakus@foxmail.com)
  * @brief          : None
  * @date           : 2024/4/4
  ******************************************************************************
  */

#include "homsg_psp.h"

static homsg_subject_node_t *homsg_create_subject(const char *desc);

homsg_sp_t *homsg_sp_create() {
    homsg_sp_t *sp = HOMSG_SP_MALLOC(sizeof(homsg_sp_t));
    if (sp == NULL) {
        return NULL;
    }

    sp->create_subject = homsg_create_subject;

    return sp;
}

void homsg_sp_destroy(homsg_sp_t *sp) {
    if (sp == NULL) {
        return;
    }
    HOMSG_SP_FREE(sp);
}

homsg_subject_node_t *homsg_create_subject(const char *desc) {
    homsg_subject_node_t *subject = HOMSG_SP_MALLOC(sizeof(homsg_subject_node_t));
    if (subject == NULL) {
        return NULL;
    }

    subject->cnt = 0;
    subject->subscribers = NULL;
    return subject;
}
