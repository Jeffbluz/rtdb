#ifdef SIMFES_POI_DEBUG
#include "Simulate_Fes.hxx"
#include "Simulate_Fes_queue.hxx"
#include <lib_msg.hxx>
#include <message_channel.h>
#include <message_type.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

extern SIMFES_QUEUE* SimFes_poi_fifo;
extern int SimFes_poi_num_per_packet;

//����: 	��SCADA����ң����
//����: 	��
//����ֵ:	��
void *SimFes_poi_send(void *arg)
{
    SCADA_MES    scd_mes;
    SGDPS_CALGCHG *chg_poi;
    SGDPS_CALGCHG analog;
    int i;
    int send_msg_count;

    char * d5000_home;
    if((d5000_home=getenv("D5000_HOME"))==NULL)
    {
        fprintf(stdin,"getenv $D5000_HOME failed!");
        return NULL;
    }
    /**
    	char logFile[128];
    	bzero(logFile,sizeof(logFile));
    	time_t now;
    	struct tm *tm;
    	now=time(NULL);
    	tm=localtime(&now);
    	sprintf(logFile,"%s/src/scada/sca_analog/log/%d-%d-%d.log",d5000_home,tm->tm_year+1900,tm->tm_mon,tm->tm_mday);

    	FILE *fp;
    	if((fp=fopen(logFile,"w"))==NULL)
    	{
    		fprintf(stdin,"%s:%d:%s        Fail when open log file %s!\n",logFile,__FILE__,__LINE__,__FUNCTION__);
    		return NULL;
    	}
    */
    if((chg_poi=(SGDPS_CALGCHG *)malloc(sizeof(SGDPS_CALGCHG)*SimFes_poi_num_per_packet))==NULL)
    {
        fprintf(stdin,"%s:%d:%s	malloc for chg_poi array failed!\n",__FILE__,__LINE__,__FUNCTION__);
        return NULL;
    }

    send_msg_count=0;
    while(1)
    {
        analog=(SGDPS_CALGCHG *)SimFes_queue_del(SimFes_poi_fifo);
#ifdef SIMFES_DEBUG
        struct timeval tv;
        gettimeofday (&tv,NULL);
        struct tm *tm;
        tm=localtime(&tv.tv_sec);
        printf("%d-%d-%d %d:%d:%d:%d	<<<psid=%ld\tvalue=%f\n",tm->tm_year+1900,tm->tm_mon,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec,tv.tv_usec/1000,analog.keyid,analog.value);
#endif
        memcpy(chg_ana[i],analog,sizeof(SGDPS_CALGCHG));
        i++;

        if(i==SimFes_poi_num_per_packet)
        {
            if((sizeof(SGDPS_CALGCHG)*SimFes_poi_num_per_packet) < sizeof(scd_mes.mes))
            {
                int ret_val;
                struct timeval tp;
                struct   timezone tz;

                gettimeofday(&tp,&tz);
                scd_mes.head.data_num = SimFes_poi_num_per_packet;
                scd_mes.head.second = tp.tv_sec;
                scd_mes.head.msecond = tp.tv_usec/1000;

                bzero(scd_mes.mes,sizeof(scd_mes.mes));
                memcpy(scd_mes.mes,(char *)chg_ana,sizeof(SGDPS_CALGCHG)*SimFes_poi_num_per_packet);
                if((ret_val = MsgSend(MT_YC_CHANGE,CH_UP_REAL_DATA,(char *)&scd_mes,(sizeof(SCADA_MES_HEAD)+sizeof(SGDPS_CALGCHG)*SimFes_poi_num_per_packet)))<0)
                {
                    fprintf(stdin,"%s:%d:%s	Fail when sending message!\n",__FILE__,__LINE__,__FUNCTION__);
                }

                send_msg_count++;
#ifdef SIMFES_DEBUG
                printf("%s:%d:%s	send_msg_count=%d\n",__FILE__,__LINE__,__FUNCTION__,send_msg_count);
#endif
            }
            else
            {
                fprintf(stdin,"Message is out of range!");
            }

            i=0;
        }
    }
}
#endif
