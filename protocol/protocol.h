#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include "common.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define MSG_AP_ERR			-1 //APЭ�����ʹ���
#define MSG_CRC_ERR			-2 // CRCУ�����
#define MSG_VP_ERR			-3 // VP���ز����
#define MSG_STATION_ERR		-4 // վ���Ŵ�
#define MSG_DEV_ERR			-4 // �豸��Ŵ�
#define MSG_VP_INTERACT_ERR	-5 // VP������־��
#define MSG_MCP_ERR			-6 // ��ؿ��Ʋ�Э���־��
#define MSG_AUTHORIZATION_FAILURE -7 // �绰�����Ȩ
#define MSG_MCP_RESPONSE_ERR	-8 // MCPӦ���־��
#define MSG_MCP_PDU_LEN_ERR		-8 // MCP������ݳ��ȴ�
#define MSG_MCP_COMMAND_ERR		-9 // MCP�����־��
#define MSG_MCP_DATARANGE_ERR	-10 // ������ݳ���
#define MSG_ESC_ERR				-11 // ת�����
#define MSG_PACK_NUM_ERR        -12  // ͨ�Ű���ʶ�Ŵ�

#define MCP_LEN_ERR 0x03   // MCPӦ�����ݳ��ȴ�
#define MCP_CMD_ERR 0x02   // MCPӦ�������Ŵ�

#define MCP_NOID_ERR		0x10  // 1:������ݱ�ʶ�޷�ʶ��
#define MCP_OVER_ERR		0x20  // 2:�����������ֵ������Χ
#define MCP_CODE_ERR		0x30  // 3:������ݲ�����Ҫ��
#define MCP_FAIL_ERR		0x40  // 4:������ݱ�ʶ�������ݳ��Ȳ�ƥ��
#define MCP_UNDER_ERR		0x50  // 5:������ݵļ��ֵ���ڹ�����Χ
#define MCP_UP_ERR			0x60  // 6:������ݵļ��ֵ���ڹ�����Χ
#define MCP_DATA_ERR		0x70  // 7:��������޷����

#define AP_A		0x01  // �����Э��	  ����
#define AP_B        0x02  // SMS
#define AP_C        0x03  // ��̫��
#define VP_A		0x01  // VP�㣬Ŀǰ��֧��VP:A
#pragma pack(1)
// APЭ��ṹ
struct ap_st{
	unsigned char start_flag; // ��ʼ��־ AP:A=0x7E AP:B=0x21
	unsigned char ap_type;    // ap:a  ap:b ap:c
	unsigned char vp_type;    // vp:a=0x01
	unsigned char dev_num[4]; // վ����  4
	unsigned char sub_num;    // �豸���
	unsigned short pack_num;  // ͨ�Ű���ʶ�� 2
	unsigned char vp_iflag;   // vp�㽻����־
	unsigned char mcp_flag;   // mcp���־
	unsigned char cmd_flag;   // �����־
	unsigned char rep_flag;   // Ӧ���־
	unsigned char buf[];	  // �㳤����
};
#pragma pack()

typedef int (*func)(void*, int, int);
typedef unsigned char (*func_a)(void);

#pragma pack(1)
// PDU��ʽ����
struct ltv_st{	
	unsigned char len;		  // ����
	unsigned short tag;		  // ��־
	unsigned char *value;	  // ֵ
	func fun;
	int eep_addr;			  // eeprom��ַ
};
#pragma pack()

struct _alarm_st{		// �澯����ṹ
	func_a fun;		// �ж�״̬
	int alarm_cnt;	 // ���澯����
	int all_cnt;		 // ��⼼��
	unsigned char status;	 // ��ǰ״̬
	unsigned char * p_en;	 // ʹ��״̬
	unsigned char * p_value;	// �澯״̬
};

void local_deal(void);
void recv_local_omc(void);
int read_from_flash(void);
int write_to_flash(void);
// �����豸��ǰ����ֵ
void read_current_value(void);
void check_alarm(void);
// ����APA����
int ap_deal(unsigned char * buf, int * plen, unsigned char ap);

void gpio_init(void);

int check_report(void);
#ifdef __cplusplus
}
#endif

#endif 


