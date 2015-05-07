#ifndef __MODEM_H
#define __MODEM_H

#ifdef __cplusplus
 extern "C" {
#endif
	
#define HUAWEI 1

// ���յ� OK
#define OK_FLAG 			(0x00000001)
// ���յ� ERROR
#define ERROR_FLAG		(0x00000002) 
// ���յ� CSQ
#define CSQ_FLAG			(0x00000004)
// ���յ� CGMI ��������
#define GMR_FLAG			(0x00000008)
// ���յ� > ���Է��Ͷ�������
#define SMS_READY			(0x00000010)
// SMS���ͳɹ� ^HCMGSS:9
#define SMS_OK				(0x00000020)
// SMS����ʧ�� ^HCMGSF:107
#define SMS_ERROR			(0x00000040)
// ��ѯCREG �Ƿ�ע��ɹ�  +CREG: 0,1/4
#define CREG_FLAG     (0x00000080)
// ���յ�SMS	 ^HCMGR:15369364554,2015,04,09,21,33,10,0,1,12,1,0,0,1
#define HCMGR_FLAG    (0x00000100)


// modem��Դ����IO��ʼ�� PD6
void modem_power_init(void);
// MODEM ״̬��
void modem_state_machine(void);
/*
** �������ܣ��������MODEM����
** ����������� 
** �����������
** ����ֵ����
*/
void recv_modem(void);

#ifdef __cplusplus
}
#endif

#endif 
