#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Simulate_Fes_queue.hxx>
#include <pthread.h>

#define QUEUE_LENGTH	1024
#define OFFSET(start,index,size)	((char *)start+index*size)

//����:	��ʼ������
//����:	size-������Ԫ�صĴ�С
//����ֵ:	����ָ��
SIMFES_QUEUE * SimFes_queue_init (int size)
{
    SIMFES_QUEUE *q= (SIMFES_QUEUE *)malloc (size*QUEUE_LENGTH);
    if (q == NULL)
        return (NULL);
    if((q->buf=malloc(size*QUEUE_LENGTH))==NULL)
    {
        fprintf(stdin,"%s:%d:%s	[ERROR]malloc failed\n",__FILE__,__LINE__,__FUNCTION__);
        return (NULL);
    }
    q->head = 0;
    q->tail = 0;
    q->size = size;

    q->mutex = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
    pthread_mutex_init (q->mutex, NULL);
    q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
    pthread_cond_init (q->notFull, NULL);
    q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
    pthread_cond_init (q->notEmpty, NULL);

    return (q);
}

//����:	�ڶ�β���Ԫ��
//����:	q-����ָ��
//		item-������Ԫ��ָ��
//����ֵ:	��
void SimFes_queue_add(SIMFES_QUEUE *q, void *item)
{
    pthread_mutex_lock (q->mutex);
    while((q->tail+1)%QUEUE_LENGTH==q->head)
        pthread_cond_wait (q->notEmpty, q->mutex);
    memcpy(OFFSET(q->buf,q->tail,q->size),item,q->size);
    q->tail++;
    if (q->tail == QUEUE_LENGTH)
        q->tail = 0;
    pthread_mutex_unlock (q->mutex);
    pthread_cond_signal (q->notFull);

    return;
}

//����:	ȡ��ͷԪ��
//����:	q-����ָ��
//����ֵ:	��ͷԪ��ָ��
void* SimFes_queue_del (SIMFES_QUEUE *q)
{
    void *item;
    pthread_mutex_lock (q->mutex);
    while(q->head==q->tail)
        pthread_cond_wait (q->notFull, q->mutex);
    item=OFFSET(q->buf,q->head,q->size);
    q->head++;
    if (q->head == QUEUE_LENGTH)
        q->head = 0;
    pthread_mutex_unlock (q->mutex);
    pthread_cond_broadcast (q->notEmpty);

    return item;
}

//����:	���ٶ���
//����:	q-����ָ��
//����ֵ:	��
void SimFes_queue_destroy (SIMFES_QUEUE*q)
{
    pthread_mutex_destroy (q->mutex);
    free (q->mutex);

    pthread_cond_destroy (q->notFull);
    free (q->notFull);

    pthread_cond_destroy (q->notEmpty);
    free (q->notEmpty);

    free (q);
}
