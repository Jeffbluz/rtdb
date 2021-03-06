#include "Simulate_Fes.hxx"
#include "Simulate_Fes_queue.hxx"
#include <d5000.hxx>
#include <lib_msg.hxx>
#include <message_channel.h>
#include <message_type.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "db_api/rtdb_api.h"
extern SIMFES_QUEUE* SimFes_ana_fifo;
extern int SimFes_ana_num_per_packet;
extern int latency_time;
extern int pkgnum;
extern int rndnum;

//功能: 	向SCADA发送遥测量
//参数: 	无
//返回值:	无
void *SimFes_ana_send(void *arg)
{
	SCADA_MES    scd_mes;
	SGDPS_CALGCHG *chg_ana;
	SGDPS_CALGCHG analog;
	int i,round;
	struct timeval tp;
	struct   timezone tz;
	char * d5000_home;
	if((d5000_home=getenv("D5000_HOME"))==NULL)
	{
		fprintf(stdin,"getenv $D5000_HOME failed!");
		return NULL;
	}

	if((chg_ana=(SGDPS_CALGCHG *)malloc(sizeof(SGDPS_CALGCHG)*SimFes_ana_num_per_packet*pkgnum))==NULL)
	{
		fprintf(stdin,"%s:%d:%s	malloc for chg_ana array failed!\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	long key_no;
	int field_no, table_no;
	int tstcnt,pile_tstcnt,welltstcnt,pile_welltstcnt;
	float val;
	TB_DESCR tb_des;
	timeval tv1,tv2;
	FILE *tstresfile;
	char stime[64],code[32];
	if ((tstresfile=fopen("test_result.log","w"))==NULL){
		perror("open file(test_result.log)");
		exit(1);
	}

	pile_tstcnt=0;
	pile_welltstcnt=0;
	for (round=0;round<rndnum;round++) {
		for (i=0;i<SimFes_ana_num_per_packet*pkgnum;i++) {
			analog=*((SGDPS_CALGCHG *)SimFes_queue_del(SimFes_ana_fifo));
			struct timeval tv;
			gettimeofday (&tv,NULL);
			struct tm *tm;
			tm=localtime(&tv.tv_sec);
			printf("%4d-%02d-%02d %02d:%02d:%02d:%d	<<<<psid=%ld\tvalue=%f\n",tm->tm_year+1900,tm->tm_mon,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec,tv.tv_usec/1000,analog.keyid,analog.value);
			chg_ana[i]=analog;
		}
		gettimeofday(&tp,&tz);
		scd_mes.head.data_num = SimFes_ana_num_per_packet*pkgnum;
		scd_mes.head.second = tp.tv_sec;
		scd_mes.head.msecond = tp.tv_usec/1000;

		bzero(scd_mes.mes,sizeof(scd_mes.mes));
		memcpy(scd_mes.mes,(char *)chg_ana,sizeof(SGDPS_CALGCHG)*SimFes_ana_num_per_packet*pkgnum);
		if( MsgSend(MT_YC_CHANGE,CH_UP_REAL_DATA,(char *)&scd_mes,(sizeof(SCADA_MES_HEAD)+sizeof(SGDPS_CALGCHG)*SimFes_ana_num_per_packet*pkgnum))<0)
		{
			fprintf(tstresfile,"%s:%d:%s	Fail when sending message!\n",__FILE__,__LINE__,__FUNCTION__);
		}

		sleep(latency_time);
		bzero(stime,64);
		strftime(stime,64,"%m/%d %T",localtime(&tp.tv_sec));
		fprintf(tstresfile,"###round:%d time:%s %d ms\n",round,stime,tp.tv_usec/1000);
		gettimeofday(&tv1,NULL);
		bzero(stime,64);
		strftime(stime,64,"%m/%d %T",localtime(&tv1.tv_sec));
		fprintf(tstresfile,"###Testing's begining at:%s %d ms\n",stime,tv1.tv_usec/1000);
		fprintf(tstresfile,"###%d\n",(tv1.tv_sec-tp.tv_sec)*1000+(tv1.tv_usec-tp.tv_usec)/1000);
		tstcnt=0;
		welltstcnt=0;
		for (i=0;i<SimFes_ana_num_per_packet*pkgnum;i++){
			key_no = chg_ana[i].keyid & 0xffff0000ffffffff;
			field_no = (short)(chg_ana[i].keyid>>32);
			table_no = (int)(chg_ana[i].keyid>>48);
			//open table
			if (OpenTableByID(NULL,AC_REALTIME_NO,AP_SCADA,table_no,&tb_des)<0){
				fprintf(tstresfile,"$###ERROR:Table(%d)Can not be Opened!\n",table_no);
				continue;
			}

			if (GetFieldsByID(&tb_des,(char*)&key_no,&field_no,1,sizeof(val),(char*)&val)<0){
				fprintf(tstresfile,"$###ERROR:Value(%d,%d,%d) Can not be Gotten!\n",table_no,(int)key_no,field_no);
				CloseTable(&tb_des);
				continue;
			}else
				fprintf(tstresfile,"###Value(%d,%d,%d) has been Gotten successfully!\n",table_no,(int)key_no,field_no);

			fprintf(tstresfile,"###Value(%d,%d,%d)=%f\n",table_no,(int)key_no,field_no,val);
			if (val==chg_ana[i].value){
				pile_welltstcnt++;
				welltstcnt++;
				fprintf(tstresfile,"@%d/%d:%f=%f\n",welltstcnt,tstcnt,chg_ana[i].value,val);
			}else {

				fprintf(tstresfile,"$%f!=%f\n",chg_ana[i].value,val);
			}

			tstcnt++;
			pile_tstcnt++;
			CloseTable(&tb_des);

		}

		fprintf(tstresfile,"###ROUND=%d total:%d total success:%d round:%d round success:%d\n",round,pile_tstcnt,pile_welltstcnt,tstcnt,welltstcnt);
		gettimeofday(&tv2,NULL);
		bzero(stime,64);
		strftime(stime,64,"%m/%d %T",localtime(&tv2.tv_sec));
		fprintf(tstresfile,"###Testing's ended at:%s %d ms\n",stime,tv2.tv_usec/1000);
		fprintf(tstresfile,"###%d\n",(tv2.tv_sec-tv1.tv_sec)*1000+(tv2.tv_usec-tv1.tv_usec)/1000);
	}
	fflush(tstresfile);
	fclose(tstresfile);
	fprintf(stderr,"...OK!\n");
}

