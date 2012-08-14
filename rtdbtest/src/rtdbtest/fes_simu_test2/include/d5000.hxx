/*************************************************
 * Copyright (C),
 * File name:      d5000.hxx // �ļ���
 * Author:       Version: 2.0        Date: // ���ߡ��汾���������
 * Description:    // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
 *                 // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
 *                 // �ơ�˳�򡢶����������ȹ�ϵ
 * Others:         // �������ݵ�˵��
 * Function List:  // ��Ҫ�����б���ÿ����¼Ӧ���������������ܼ�Ҫ˵��
 *   1. ....
 *      History:        // �޸���ʷ��¼�б���ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
 *                 // �߼��޸����ݼ���  
 *   1. Date:
 *      Author:
 *      Modification:
 *   2. ...
 **************************************************/




#ifndef    _D5000_HXX_
#define    _D5000_HXX_




#define    MAX_MES_LEN    512    
//#define    MAX_DATA_LEN    500
#define    MAX_DATA_LEN    40000
#define    MAX_CTRL_LEN    512    



enum CHANNEL_DEF
{
    CH_UP_REAL_DATA   = 30,        //����ʵʱ����ͨ����fes--->scada
    CH_DOWN_REAL_DATA = 31,        //����ʵʱ����ͨ����scada--->fes

    CH_CHANGE_DATA    = 34,        //���б仯����ͨ����scada--->fes

    CH_CTRL_REQ       = 11,        //��������ͨ����scada--->fes
    CH_CTRL_REPLY     = 12         //����Ӧ��ͨ����fes--->scada
};


#define MT_YX_CHANGE          1000    //�仯ң��
#define MT_YX_UPDATE          1001    //ȫң��
#define MT_YX_SOE             1002    //SOE
#define MT_YC_CHANGE          1010    //�仯ң��
#define MT_YC_UPDATE          1011    //ȫң��
#define MT_YM_UPDATE          1020    //ȫң��
#define MT_DATA_REQUEST       1030    //�����ٻ�

//ң��
#define MT_YK_PREV            1050            //ң��Ԥ��
#define MT_YK_EXEC            1051            //ң��ִ��
#define MT_YK_CANCEL          1052            //ң�س���
#define MT_YK_DIRECT          1053            //ֱ��ִ��,��ǰ���Զ����Ԥ��,ִ�е���������
#define MT_YK_REPLY           1054            //ң��Ӧ��
//��λ����
#define MT_TAP_PREV           1055            //Ԥ��
#define MT_TAP_EXEC           1056            //ִ��
#define MT_TAP_CANCEL         1057            //����
#define MT_TAP_DIRECT         1058            //ֱ��ִ��,��ǰ���Զ����Ԥ��,ִ�е���������
#define MT_TAP_REPLY          1059            //Ӧ��
//���ң��
#define MT_SET_EXEC           1060            //ִ��
#define MT_SET_REPLY          1061            //Ӧ��
//��������
#define MT_PULSE_EXEC         1062            //ִ��
#define MT_PULSE_REPLY        1063            //Ӧ��

typedef struct SCADA_MES_HEAD
{
    int    package_type;
    int    data_num;
    time_t second;
    short  msecond;
    int    para;
}SCADA_MES_HEAD;

typedef struct SCADA_MES
{
    SCADA_MES_HEAD    head;
    char    mes[MAX_DATA_LEN-sizeof(SCADA_MES_HEAD)];
}SCADA_MES;

//ң��ȫ���ݽṹ
typedef struct SGDPS_CFALG
{
    long  keyid;
    float value;
    int   status;
}SGDPS_CFALG;

//ң��ȫ���ݽṹ
typedef struct SGDPS_CFPNT
{
    long keyid;
    char value;
    int  status;
}SGDPS_CFPNT;

//�仯ң�ⱨ��
typedef struct SGDPS_CALGCHG
{
    long  keyid;
    float value;
    int   status;
}SGDPS_CALGCHG;

//�仯ң���ʱ�걨��
typedef struct SGDPS_CALGCHGWithTm
{
    long  keyid;
    float value;
    int   status;
    int   second;
    short msecond;
}SGDPS_CALGCHGWithTm;

//�仯ң�ű���
typedef struct SGDPS_CPNTCHG
{
    long keyid;
    char value;
    int  status;
}SGDPS_CPNTCHG;

//�仯ң�Ŵ�ʱ�걨��
typedef struct SGDPS_CPNTCHGWithTm
{
    long keyid;
    char value;
    int  status;
    time_t second;
    short  msecond;
}SGDPS_CPNTCHGWithTm;

//SOE����
typedef struct SGDPS_PNTSOE
{
    long  keyid;
    char  value;
    int   status;
    time_t   second;
    short msecond;
}SGDPS_PNTSOE;

//ȫң��
typedef struct SGDPS_PCOUNT
{
    long  keyid;
    float value;
    int   status;
}SGDPS_PCOUNT;

//�����ٻ�
typedef struct SGDPS_DATAREQUEST
{
    char type;        //1:analog, 2:point, 3:pcount, -1:all
    long st_id;       //��վID
}SGDPS_DATAREQUEST;

//ң�����б��Ľṹ
typedef struct SGDPS_DCREQUEST
{
    long keyid;
    char value;
}SGDPS_DCREQUEST;

//ң�ط�У
typedef struct SGDPS_DCREPLY
{
    long keyid;
    int  result;
}SGDPS_DCREPLY;


typedef struct SGDPS_ALARM
{
    char  almmsg[128];    //��������
    char  sourceid[20];   //���ͱ����ĳ�����
    int   gtime_s;        //ʱ�䣨�룩
    short gtime_m;        //ʱ�䣨���룩
}SGDPS_ALARM;

#define    MAX_ANA_CHG     30*8
#define    MAX_POI_CHG     30*8    //MAX_DATA_LEN/sizeof(SGDPS_CPNTCHG)
#define    MAX_ANA_CHG_TM  20*6    //MAX_DATA_LEN/sizeof(SGDPS_CALGCHGWithTm)
#define    MAX_POI_CHG_TM  5*2    //MAX_DATA_LEN/sizeof(SGDPS_CPNTCHGWithTm)
#define    MAX_ANA_FULL    30*8
#define    MAX_POI_FULL    30*8    //MAX_DATA_LEN/sizeof(SGDPS_CFPNT)
#define    MAX_MOD_EVT     10      //Receive link moidify max event

#define    MAX_SOE        MAX_DATA_LEN/sizeof(SGDPS_PNTSOE)

/*
#define    MAX_ANA_CHG    MAX_MES_LEN/sizeof(SGDPS_CALGCHG)
#define    MAX_POI_CHG    MAX_MES_LEN/sizeof(SGDPS_CPNTCHG)
#define    MAX_ANA_CHG_TM    MAX_MES_LEN/sizeof(SGDPS_CALGCHGWithTm)
#define    MAX_POI_CHG_TM    MAX_MES_LEN/sizeof(SGDPS_CPNTCHGWithTm)
#define    MAX_ANA_FULL    MAX_MES_LEN/sizeof(SGDPS_CFALG)    
#define    MAX_POI_FULL    MAX_MES_LEN/sizeof(SGDPS_CFPNT)

#define    MAX_SOE        MAX_MES_LEN/sizeof(SGDPS_PNTSOE)

typedef	struct LINK_MODIFY_QUE
{
	int			input_cur_pos;//��ǰ�������ݿ�仯�¼��ڶ��е��е�λ��
	int			deal_cur_pos  //��ǰ�������ݿ�仯�ڶ����е�λ��
	LINK_MODIFY_STRUCT	link_notify_que[MAX_MOD_EVT];//�����Ի���MAX_MOD_EVT�������ݿ�仯�¼�
}LINK_MODIFY_QUE;
*/

extern    int    full_ana_count;
extern    int    full_poi_count;
extern    int    full_soe_count;
extern    int    chg_ana_count;
extern    int    chg_poi_count;
extern    int    chg_soe_count;

extern    SGDPS_CFALG        full_ana[MAX_ANA_FULL];
extern    SGDPS_CFPNT        full_poi[MAX_POI_FULL];
extern    SGDPS_CALGCHG        chg_ana[MAX_ANA_CHG];
extern    SGDPS_CPNTCHG        chg_poi[MAX_POI_CHG];
extern    SGDPS_CALGCHGWithTm    chg_ana_tm[MAX_ANA_CHG_TM];
extern    SGDPS_CPNTCHGWithTm    chg_poi_tm[MAX_POI_CHG_TM];

extern    SGDPS_PNTSOE        full_soe[MAX_SOE];
extern    SGDPS_PNTSOE        chg_soe[MAX_SOE];

extern    SGDPS_CFALG        old_ana[MAX_ANA_FULL];
extern    SGDPS_CFPNT        old_poi[MAX_POI_FULL];





/*******************************************************************************************
���ǵ����ƶ��󡢿��Ʋ��������أ����Ʒ����ࣺ
ң�أ�һ���豸��/�ϣ��翪�ص�բ�������źŶ���/����
��λ���ڣ���ѹ���Զ����ֶ�����
���ң������Ҫ���ڻ����Զ����ֶ����ڣ�AVCҲ����
�������ƣ���Ҫ���ڻ����Զ����ֶ�����

���ṹ���ԭ��
����Ŀ��Ʋ�����ͨ������š���ֵ��ϵ���ȣ���ǰ�����ݿ���¼�룬����ṹ��ǰ�ö���
���ڿ��ƿ����漰���ñ����߼�����ң�أ���ʱ���п��ƶ�����ص���������״̬�����ɺ�̨���������ں�̨���ݿ���¼�롣

����ͨѶ���ԭ��
ÿ�����������ֱ��Ľṹ������ṹ��Ӧ��ṹ��
ÿ����Ƹ��ݿ����ص㣬�������ɸ��������͡�
���п��ƹ���������Ϣͨ������������ͨ������̨->ǰ�ã��������ķ�������ͨ��������Ӧ��ͨ����ǰ��->��̨����Ӧ���ķ�������ͨ��
���ĺ����ƶ��󡢿���Ŀ�ꡢ���Ʒ�ʽ�ȣ�ǰ���յ�����ݱ������ݲ�����ؿ��Ʋ����·������ʵ�ͨ���������·�������ɹ���ʧ�ܣ�����̨������Ӧ��Ӧ���ġ�
���ں�̨���͵Ŀ�������������Ԥ�á�ִ�С�ֱ��ִ�У�Ϊ�˱��ں�̨�жϽ����ǰ���Ƿ����Ӧ��

��Ϣͨ�����壺
    ��������ͨ����     CH_CTRL_REQ
    ����Ӧ��ͨ����    CH_CTRL_REPLY

�������Ͷ��壺
    ң�أ�
            Ԥ�ã�            MT_YK_PREV
            ִ�У�            MT_YK_EXEC
            ����:            MT_YK_CANCEL
            ֱ��ִ�У�        MT_YK_DIRECT            //��ǰ���Զ����Ԥ�á�ִ�е���������
            Ӧ��            MT_YK_REPLY
    ��λ����:
            Ԥ�ã�            MT_TAP_PREV
            ִ�У�            MT_TAP_EXEC
            ����:            MT_YK_CANCEL
            ֱ��ִ�У�        MT_TAP_DIRECT            //��ǰ���Զ����Ԥ�á�ִ�е���������
            Ӧ��            MT_TAP_REPLY
    ���ң��:
            ִ�У�            MT_SET_EXEC
            Ӧ��            MT_SET_REPLY
    ��������:
            ִ�У�            MT_PULSE_EXEC
            Ӧ��            MT_PULSE_REPLY
*******************************************************************************************/

/*
���Ľṹ��
*/

typedef long KEY_ID_STRU;

struct CtrlPkgHead
{
    int    package_type;    //������
    int    data_num;        //����
    int    para1;           //����
    int    para2;           //����
    int    para3;           //����
};

struct TYkReq                 //ң������,��̨->ǰ��
{
    KEY_ID_STRU     key_id;        
    char            ctrl_type;         //��������:1==��ͨң��;2==��ͬ��ң��;3==����ѹң��
    char            ctrl_value;        //����ֵ:0==�ط�;1==�غ�
};

struct TTapReq                //��λ��������,��̨->ǰ��
{
    KEY_ID_STRU     key_id;
    char            ctrl_value;        //����ֵ:0==����;1==�ؽ�;2==��ͣ
};

struct TSetReq                //���ң������,��̨->ǰ��
{
    KEY_ID_STRU      key_id;
    char             ctrl_type;         //��������:0==��ͨ;1==����;2==����
    float            set_value;        //���ֵ
}; 

struct TYkReply               //ң��Ӧ��,ǰ��->��̨
{
    KEY_ID_STRU     key_id;        
    char            ctrl_type;
    char            ctrl_value;
    //�����������Ӧ��������һ��
    char            ctrl_result;       //���ƽ����0==�ɹ�������ֵ��ʾ����
    char            err_msg[64];       //����ԭ��
};

struct TTapReply              //��λ����Ӧ��,ǰ��->��̨
{
    KEY_ID_STRU     key_id;
    char            ctrl_value;
    //�����������Ӧ��������һ��
    char            ctrl_result;       //���ƽ����0==�ɹ�������ֵ��ʾ����
    char            err_msg[64];       //����ԭ��
};
struct TSetReply              //���ң��Ӧ��,ǰ��->��̨
{
    KEY_ID_STRU     key_id;
    char            ctrl_type;
    float           set_value;
    //�����������Ӧ��������һ��
    char            ctrl_result;       //���ƽ����0==�ɹ�������ֵ��ʾ����
    char            err_msg[64];       //����ԭ��
};  

struct CtrlPkg                //�������ݰ�,ͬһ��ֻ��һ�������Ϣ                       
{                                                                 
    CtrlPkgHead     package_head;                             
    char            mes[MAX_CTRL_LEN-sizeof(CtrlPkgHead)];  
};  

struct DataRequest     //�����ٻ�
{
    char   type;       //1:yc, 2:yx, 3:ym, -1:all
    long   st_id;      //��վID
    //DEVICE_ID   st_id;      //��վID
};



#endif