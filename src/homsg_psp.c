/**
  ******************************************************************************
  * @file           : homsg_sp.c
  * @author         : Hotakus (hotakus@foxmail.com)
  * @brief          : None
  * @date           : 2024/4/4
  ******************************************************************************
  */
#include <stdio.h>
#include "homsg_psp.h"

homsg_subject_t *homsg_create_subject(char *desc);
void homsg_destroy_subject(homsg_subject_t *subject);

#if (HOMSG_SP_USE_SS_MANAGER == 1)
//static homsg_res_t homsg_find_subject(homsg_psp_t *psp, char *desc);
static homsg_res_t homsg_publish(homsg_psp_t *psp, homsg_subject_t *subject);
static homsg_res_t homsg_revoke(homsg_psp_t *psp, homsg_subject_t *subject);
#endif

static homsg_res_t homsg_subscribe(homsg_psp_t *psp, homsg_subject_t *subject, char *subscriber_name, homsg_subscriber_update_callback_t update);
static homsg_res_t homsg_unsubscribe(homsg_psp_t *psp, homsg_subject_t *subject, char *subscriber_name);
static homsg_res_t homsg_notify(homsg_subject_t *subject, void *msg);


homsg_psp_t *homsg_psp_create() {
    homsg_psp_t *psp = HOMSG_SP_MALLOC(sizeof(homsg_psp_t));
    if (psp == NULL) {
        return NULL;
    }

    // TODO
#if (HOMSG_SP_USE_SS_MANAGER == 1)
    psp->all_subjects = chain_create("all_subjects");
    if (psp->all_subjects == NULL) {
        return NULL;
    }

    psp->publish = homsg_publish;
    psp->revoke = homsg_revoke;
#endif

    psp->create_subject = homsg_create_subject;
    psp->destroy_subject = homsg_destroy_subject;

    // basic
    psp->notify = homsg_notify;
    psp->subscribe = homsg_subscribe;
    psp->unsubscribe = homsg_unsubscribe;

    return psp;
}

void homsg_psp_destroy(homsg_psp_t *psp) {
    if (psp == NULL) {
        return;
    }

 

#if (HOMSG_SP_USE_SS_MANAGER == 1)
    // free all subscribers in subjects
    chain_node_t *node = psp->all_subjects->head->next_node;
    while (node != psp->all_subjects->tail) {
        homsg_subject_t *subject = (homsg_subject_t *)node->data;
        if (subject != NULL) {
            homsg_destroy_subject(subject);
        }
        node = node->next_node;
    }

    chain_destroy(psp->all_subjects);
#endif

    HOMSG_SP_FREE(psp);
}


homsg_subject_t *homsg_create_subject(char *desc) {
    homsg_subject_t *subject = HOMSG_SP_MALLOC(sizeof(homsg_subject_t));
    if (subject == NULL) {
        return NULL;
    }

    subject->subscribers = chain_create(desc);
    if (subject->subscribers == NULL) {
        return NULL;
    }
    return subject;
}

void homsg_destroy_subject(homsg_subject_t *subject) {
    chain_destroy(subject->subscribers);
    HOMSG_SP_FREE(subject);
}


#if (HOMSG_SP_USE_SS_MANAGER == 1)

/**
 * @brief  find a subject in all_subjects
 * @param psp  instance
 * @param subject  subject
 * @return  homsg_res_t
 */
homsg_res_t homsg_publish(homsg_psp_t *psp, homsg_subject_t *subject) {
    if (psp == NULL || subject == NULL) {
        return HOMSG_RES_ERROR;
    }

 

    if (chain_find_node_by_name(psp->all_subjects, subject->subscribers->desc, true) != NULL) {
        printf("subject %s already published\n\r", subject->subscribers->desc);
        return HOMSG_RES_ALREADY_PUBLISHED;
    }

    chain_node_t *_subject = chain_node_create(subject->subscribers->desc, subject);
    if (_subject == NULL) {
        return HOMSG_RES_ERROR;
    }

    chain_append(psp->all_subjects, _subject);

  

    return HOMSG_RES_OK;
}

/**
 * @brief revoke a subject from all_subjects
 * @param psp instance
 * @param subject subject
 * @return homsg_res_t
 */
homsg_res_t homsg_revoke(homsg_psp_t *psp, homsg_subject_t *subject) {
    if (psp == NULL || subject == NULL) {
        return HOMSG_RES_ERROR;
    }
 
    chain_remove_node(psp->all_subjects, subject->subscribers->desc, NULL, false);
  

    return HOMSG_RES_OK;
}

#endif

homsg_res_t homsg_notify(homsg_subject_t *subject, void *msg) {
    if (subject == NULL) {
        return HOMSG_RES_ERROR;
    }

    chain_node_t *node = subject->subscribers->head->next_node;
    if (node == subject->subscribers->tail) {
        return HOMSG_RES_NO_SUBSCRIBER;
    }
    while (node != subject->subscribers->tail) {
        homsg_subscriber_update_callback_t update = (homsg_subscriber_update_callback_t)node->data;
        update(msg);
        node = node->next_node;
    }

    return HOMSG_RES_OK;
}

homsg_res_t homsg_subscribe(homsg_psp_t *psp, homsg_subject_t *subject, char *subscriber_name, homsg_subscriber_update_callback_t update) {
    if (subject == NULL || update == NULL) {
        return HOMSG_RES_ERROR;
    }

 

    chain_node_t *subscriber = NULL;
#if (HOMSG_SP_USE_SS_MANAGER == 1)
    chain_node_t *subject_node = chain_find_node_by_name(psp->all_subjects, subject->subscribers->desc, true);
    if (subject_node == NULL) {
        printf("subject: \"%s\" not published. \n\r", subject->subscribers->desc);
        return HOMSG_RES_NOT_PUBLISHED;
    }
    subject = (homsg_subject_t *)subject_node->data;
    if (subject == NULL) {
        printf("subject: \"%s\" not published. \n\r", subject->subscribers->desc);
        return HOMSG_RES_NOT_PUBLISHED;
    }
#endif

    subscriber = chain_find_node_by_name(subject->subscribers, subscriber_name, true);
    if (subscriber != NULL) {
        printf("subscriber \"%s\" already subscribed subject \"%s\". \n\r", subscriber_name, subject->subscribers->desc);
        return HOMSG_RES_ALREADY_SUBSCRIBED;
    } else {
        subscriber = chain_node_create(subscriber_name, update);
        if (subscriber == NULL) {
            return HOMSG_RES_ERROR;
        }
    }

    subject->subscribers->head->id += 1;    // How many subscribers in this subject
    subscriber->id += 1;                    // How many subjects this subscriber is subscribed
    chain_append(subject->subscribers, subscriber);

  
    return HOMSG_RES_OK;
}

homsg_res_t homsg_unsubscribe(homsg_psp_t *psp, homsg_subject_t *subject, char *subscriber_name) {
    if (psp == NULL || subject == NULL) {
        return HOMSG_RES_ERROR;
    }

 

    chain_node_t *subscriber = NULL;
#if (HOMSG_SP_USE_SS_MANAGER == 1)
    chain_node_t *subject_node = chain_find_node_by_name(psp->all_subjects, subject->subscribers->desc, true);
    if (subject_node == NULL) {
        printf("subject: \"%s\" not published. \n\r", subject->subscribers->desc);
        return HOMSG_RES_NOT_PUBLISHED;
    }
    subject = (homsg_subject_t *)subject_node->data;
    if (subject == NULL) {
        printf("subject: \"%s\" not published. \n\r", subject->subscribers->desc);
        return HOMSG_RES_NOT_PUBLISHED;
    }
#endif

    subscriber = chain_find_node_by_name(subject->subscribers, subscriber_name, true);
    if (subscriber == NULL) {
        printf("subscriber \"%s\" not subscribed subject \"%s\". \n\r", subscriber_name, subject->subscribers->desc);
        return HOMSG_RES_NO_SUBSCRIBER;
    }

    subject->subscribers->head->id -= 1;    // How many subscribers in this subject
    subscriber->id -= 1;                    // How many subjects this subscriber is subscribed
    chain_remove_node(subject->subscribers, NULL, subscriber, false);

  
    return HOMSG_RES_OK;
}

#if (HOMSG_SP_TEST == 1)

void test_update(void *msg) {
    char *msg_str = (char *)msg;
    printf("test_update got msg: %s\n\r", msg_str ? msg_str : "NULL");
}

void homsg_test() {
    homsg_psp_t *psp = homsg_psp_create();

    printf("----------------------- publish\n\r");
    homsg_subject_t *subject = psp->create_subject("test");
#if (HOMSG_SP_USE_SS_MANAGER == 1)
    psp->publish(psp, subject);
#endif

    printf("----------------------- subscribe\n\r");
    char subscriber[5][5] = {"sub1", "sub2", "sub3", "sub4", "sub5"};
    psp->subscribe(psp, subject, subscriber[0], test_update);
    psp->subscribe(psp, subject, subscriber[1], test_update);
    psp->subscribe(psp, subject, subscriber[2], test_update);
    psp->subscribe(psp, subject, subscriber[3], test_update);
    psp->subscribe(psp, subject, subscriber[4], test_update);

    printf("----------------------- notify 1\n\r");
    psp->notify(subject, "hello world");

    printf("----------------------- unsubscribe\n\r");
    psp->unsubscribe(psp, subject, subscriber[2]);
    psp->unsubscribe(psp, subject, subscriber[3]);
    psp->unsubscribe(psp, subject, subscriber[4]);

    printf("----------------------- notify 2\n\r");
    psp->notify(subject, "My name is Hotakus");

#if (HOMSG_SP_USE_SS_MANAGER == 0)
    psp->destroy_subject(subject);
#endif
    homsg_psp_destroy(psp);
    return;
}

#endif