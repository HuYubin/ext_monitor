#include "protocol.h"
#include "common.h"
#include "uart.h"
#include <string.h>
#include "flash.h"
#include "ds1302.h"
#include "stm32f10x_it.h"
#include "adc.h"


unsigned char report_start_flag = 1; // PE5
unsigned char report_poll_flag = 1;  // PE4
unsigned char report_repair_flag = 1;  // PE3
unsigned char report_change_flga = 1;  // PE2
__IO unsigned char g_alarm_send_recv_flag = 0;
unsigned int g_alarm_send_time = 0;
unsigned char g_realarm_cnt = 0;

__IO unsigned char write_flash_flag = 0;

struct ltv_st para_all[];
int fun_comm(void * p_ltv, int idx, int cmd)
{
 	unsigned char * p_char = (unsigned char *)p_ltv; 
	struct ltv_st * p_st = (struct ltv_st * )p_ltv; // ����ʹ��len��tag
	int ret = 0;

	if(cmd == 0x01){   // �����ϱ�Ӧ��
		if((*(para_all[idx].value)) == 1){
			*(para_all[idx].value) = 0x81;
		}else if((*(para_all[idx].value)) == 0x80){ 
			*(para_all[idx].value) = 0;
		}	
		g_alarm_send_recv_flag = 0;   // �����澯ȷ�Ϻ����㣡����
		g_realarm_cnt = 0;
	}else if(cmd == 0x02){ // ��ѯ
		if((para_all[idx].len+3) !=  p_st->len){
			ret = MCP_FAIL_ERR;
		}else{
			memcpy(p_char+3, para_all[idx].value, para_all[idx].len);
		}
		if((para_all[idx].tag & 0x0f00) == 0x0300){ // ��ѯ�澯״̬
			if((*(para_all[idx].value)) == 1){
				*(para_all[idx].value) = 0x81;
			}else if((*(para_all[idx].value)) == 0x80){ 
				*(para_all[idx].value) = 0;
			}
			g_alarm_send_recv_flag = 0;   // �����澯ȷ�Ϻ����㣡����
			g_realarm_cnt = 0;
		}
	}else if(cmd == 0x03){ // ����
		if((para_all[idx].len+3) !=  p_st->len){
			ret = MCP_FAIL_ERR;
		}else{
			if(memcmp(para_all[idx].value, p_char+3, para_all[idx].len) != 0){
				memcpy(para_all[idx].value, p_char+3, para_all[idx].len);
				if(para_all[idx].eep_addr != 0){
					write_flash_flag = 1;
				}
			}
		}
	}else{

	}
	return ret;
}
// ��ǰʱ�䣬��ѯ����
int fun_0150(void * p_ltv, int idx, int cmd)
{
	unsigned char * p_char = (unsigned char *)p_ltv; 
	struct ltv_st * p_st = (struct ltv_st * )p_ltv; // ����ʹ��len��tag
	int ret = 0;
	
	if(cmd == 0x02){ // ��ѯ
		if((para_all[idx].len+3) !=  p_st->len){
			ret = MCP_FAIL_ERR;
		}else{
			DS1302_GetTime(para_all[idx].value+1);
			para_all[idx].value[0] = 0x20;
			memcpy(p_char+3, para_all[idx].value, para_all[idx].len);
		}
	}else if(cmd == 0x03){ // ����
		if((para_all[idx].len+3) !=  p_st->len){
			ret = MCP_FAIL_ERR;
		}else{
			DS1302_SetTime(p_char+4);
		}
	}else{

	}
	
	return ret;
}
unsigned char vendor_id = 251;   // ���̴���
unsigned char type_id = 155;   // �豸���
unsigned char dev_type_id[20]={"EM-GF-I"};	// �豸�ͺ�
unsigned char serial_id[20]={"2015040100"};	// �豸�������к�
unsigned char longitude[20]={"E12345"};		// ����
unsigned char latitude[20]={"N23.12345678"};// γ��
unsigned char fm_sf_ver[20]={"V1.1"};	   // ��ǰ�������汾
unsigned char fm_work_mode=0;			   // ����������ģʽ
unsigned char dev_num[4]={0,0,0,0};			// վ����
unsigned char sub_num=255;					// �豸���
unsigned char query_tel1[20]={"13312345678"}; // ��ѯ/���õ绰����1
unsigned char query_tel2[20]={"13312345678"}; // ��ѯ/���õ绰����2
unsigned char query_tel3[20]={"13312345678"}; // ��ѯ/���õ绰����3
unsigned char query_tel4[20]={"13312345678"}; // ��ѯ/���õ绰����4
unsigned char query_tel5[20]={"13312345678"}; // ��ѯ/���õ绰����5
unsigned char notify_tel[20]={"13312345678"}; // �ϱ�����
unsigned char comm_type=1;		// ͨ�ŷ�ʽ
unsigned char curr_time[7]={0};	 // �豸��ǰʱ��
unsigned char modem_type=20;	// Զ������ͨ��ģ��
signed char dl_output = 0;	   // ����������ʵ�ƽ
signed char dl_input = 0;   // �������빦�ʵ�ƽ
signed char ul_rated_gain=20;	// ������������
signed char dl_actual_gain=20; // ����ʵ������
unsigned char dl_swr=10;		 // ����פ����ֵ
unsigned char dl_swr_thr=10;	// ����פ��������
unsigned char dl_output_min=25;	 // �����������Ƿ��������
unsigned char pw_fail_alarm_enable=1; // ��Դ����澯ʹ��
unsigned char pw_fault_alarm_enable=1;	 // ��Դ���ϸ澯ʹ��
unsigned char bat_fault_alarm_enable=1;	 // ���ģ���ع��ϸ澯ʹ��
unsigned char pos_cha_alarm_enable=1;	// λ�ø澯ʹ��
unsigned char dl_low_output_alarm_enable=1;	// �������Ƿ���ʸ澯ʹ��
unsigned char dl_swr_alarm_enable=1;	 // ����פ���ȸ澯ʹ��
unsigned char ext1_alarm_enable=1;	 // �ⲿ�澯ʹ��1
unsigned char ext2_alarm_enable=1;	 // �ⲿ�澯ʹ��2
unsigned char ext3_alarm_enable=1;	 // �ⲿ�澯ʹ��3
unsigned char ext4_alarm_enable=1;	 // �ⲿ�澯ʹ��4
unsigned char ext5_alarm_enable=1;	 // �ⲿ�澯ʹ��5
unsigned char ext6_alarm_enable=1;	 // �ⲿ�澯ʹ��6
unsigned char pw_fail_alarm=0; // ��Դ����澯
unsigned char pw_fault_alarm=0;	 // ��Դ���ϸ澯
unsigned char bat_fault_alarm=0;	 // ���ģ���ع��ϸ澯
unsigned char pos_cha_alarm=0;	// λ�ø澯
unsigned char dl_low_output_alarm=0;	// �������Ƿ���ʸ澯
unsigned char dl_swr_alarm=0;	 // ����פ���ȸ澯
unsigned char ext1_alarm=0;	 // �ⲿ�澯1
unsigned char ext2_alarm=0;	 // �ⲿ�澯2
unsigned char ext3_alarm=0;	 // �ⲿ�澯3
unsigned char ext4_alarm=0;	 // �ⲿ�澯4
unsigned char ext5_alarm=0;	 // �ⲿ�澯5
unsigned char ext6_alarm=0;	 // �ⲿ�澯6
// �Զ������
signed short rf_ref_a = 380;
signed short rf_ref_b = 0;
signed short rf_in_a = 380;
signed short rf_in_b = 0;
signed short rf_out_a = 380;
signed short rf_out_b = 0;
signed short dl_output_ref = 0;  // ���з��书��

struct ltv_st para_all[] = {
	{.eep_addr = 1, .fun = fun_comm, .len = 1, .tag = 0x0002, .value = &vendor_id}, 		// ���̴���
	{.eep_addr = 2, .fun = fun_comm, .len = 1,	.tag = 0x0003, 	.value = &type_id},	 		// �豸���
	{.eep_addr = 3, .fun = fun_comm, .len = 20, .tag = 0x0004,	.value = dev_type_id,},		// �豸�ͺ�
	{.eep_addr = 23, .fun = fun_comm, .len = 20, .tag = 0x0005,	.value = serial_id,},		// �豸�������к�		
	{.eep_addr = 43, .fun = fun_comm, .len = 20, .tag = 0x0007,	.value = longitude,},		 // ����
	{.eep_addr = 63, .fun = fun_comm, .len = 20, .tag = 0x0008,	.value = latitude,},		// γ��
	{.eep_addr = 0, .fun = fun_comm, .len = 20, .tag = 0x000A,	.value = fm_sf_ver,},		// ��ǰ�������汾
	{.eep_addr = 0, .fun = fun_comm, .len = 1, .tag = 0x0010, .value = &fm_work_mode,},	// ����������ģʽ
	{.eep_addr = 83, .fun = fun_comm, .len = 4, .tag = 0x0101, .value = dev_num,},		   // վ����
	{.eep_addr = 84, .fun = fun_comm, .len = 1, .tag = 0x0102, .value = &sub_num,},		   // �豸���
	{.eep_addr = 85, .fun = fun_comm, .len = 20, .tag = 0x0111, .value = query_tel1,},	  // ��ѯ/���õ绰����1
	{.eep_addr = 105, .fun = fun_comm, .len = 20, .tag = 0x0112, .value = query_tel2,},	  // ��ѯ/���õ绰����2
	{.eep_addr = 125, .fun = fun_comm, .len = 20, .tag = 0x0113, .value = query_tel3,},	  // ��ѯ/���õ绰����3
	{.eep_addr = 145, .fun = fun_comm, .len = 20, .tag = 0x0114, .value = query_tel4,},	  // ��ѯ/���õ绰����4
	{.eep_addr = 165, .fun = fun_comm, .len = 20, .tag = 0x0115, .value = query_tel5,},	  // ��ѯ/���õ绰����5
	{.eep_addr = 185, .fun = fun_comm, .len = 20, .tag = 0x0120,	.value = notify_tel,},	  // �ϱ�����
	{.eep_addr = 0, .fun = fun_comm, .len = 1, .tag = 0x0142, .value = &comm_type,},	 // ͨ�ŷ�ʽ
	{.eep_addr = 0, .fun = fun_0150, .len = 7, .tag = 0x0150, .value = curr_time,},	 // �豸��ǰʱ��
	{.eep_addr = 205, .fun = fun_comm, .len = 1, .tag = 0x000B, .value = &modem_type,},	 // Զ������ͨ��ģ��
	{.eep_addr = 0, .fun = fun_comm, .len = 1, .tag = 0x0502, .value = (unsigned char *)&dl_input,},	 // �������빦��
	{.eep_addr = 0, .fun = fun_comm, .len = 1, .tag = 0x0503, .value = (unsigned char *)&dl_output,},	 // ����������ʵ�ƽ
	{.eep_addr = 0, .fun = fun_comm, .len = 1, .tag = 0x0504, .value = (unsigned char *)&ul_rated_gain,}, // ������������
	{.eep_addr = 0, .fun = fun_comm, .len = 1, .tag = 0x0505, .value = (unsigned char *)&dl_actual_gain,},// ����ʵ������
	{.eep_addr = 0, .fun = fun_comm, .len = 1, .tag = 0x0506, .value = &dl_swr,},		  // ����פ����ֵ
	{.eep_addr = 206, .fun = fun_comm, .len = 1, .tag = 0x0450, .value = &dl_swr_thr,},	  // ����פ��������
	{.eep_addr = 207, .fun = fun_comm, .len = 1, .tag = 0x0455, .value = &dl_output_min,}, // �����������Ƿ��������
	{.eep_addr = 208, .fun = fun_comm, .len = 1, .tag = 0x0201, .value = &pw_fail_alarm_enable,}, // ��Դ����澯ʹ��
	{.eep_addr = 209, .fun = fun_comm, .len = 1, .tag = 0x0202, .value = &pw_fault_alarm_enable,}, // ��Դ���ϸ澯ʹ��
	{.eep_addr = 210, .fun = fun_comm, .len = 1, .tag = 0x0204, .value = &bat_fault_alarm_enable,},// ���ģ���ع��ϸ澯ʹ��
	{.eep_addr = 211, .fun = fun_comm, .len = 1, .tag = 0x0205, .value = &pos_cha_alarm_enable,},  // λ�ø澯ʹ��
	{.eep_addr = 212, .fun = fun_comm, .len = 1, .tag = 0x0213, .value = &dl_low_output_alarm_enable,},// �������Ƿ���ʸ澯ʹ��
	{.eep_addr = 213, .fun = fun_comm, .len = 1, .tag = 0x0214, .value = &dl_swr_alarm_enable,},		// ����פ���ȸ澯ʹ��
	{.eep_addr = 214, .fun = fun_comm, .len = 1, .tag = 0x0220, .value = &ext1_alarm_enable,},	 // �ⲿ�澯ʹ��1
	{.eep_addr = 215, .fun = fun_comm, .len = 1, .tag = 0x0221, .value = &ext2_alarm_enable,},	 // �ⲿ�澯ʹ��2
	{.eep_addr = 216, .fun = fun_comm, .len = 1, .tag = 0x0222, .value = &ext3_alarm_enable,},	 // �ⲿ�澯ʹ��3
	{.eep_addr = 217, .fun = fun_comm, .len = 1, .tag = 0x0223, .value = &ext4_alarm_enable,},	 // �ⲿ�澯ʹ��4
	{.eep_addr = 218, .fun = fun_comm, .len = 1, .tag = 0x0224, .value = &ext5_alarm_enable,},	 // �ⲿ�澯ʹ��5
	{.eep_addr = 219, .fun = fun_comm, .len = 1, .tag = 0x0225, .value = &ext6_alarm_enable,},	 // �ⲿ�澯ʹ��6
	{.eep_addr = 220, .fun = fun_comm, .len = 1, .tag = 0x0301, .value = &pw_fail_alarm,}, // ��Դ����澯
	{.eep_addr = 221, .fun = fun_comm, .len = 1, .tag = 0x0302, .value = &pw_fault_alarm,}, // ��Դ���ϸ澯
	{.eep_addr = 222, .fun = fun_comm, .len = 1, .tag = 0x0304, .value = &bat_fault_alarm,},// ���ģ���ع��ϸ澯
	{.eep_addr = 224, .fun = fun_comm, .len = 1, .tag = 0x0313, .value = &dl_low_output_alarm,},// �������Ƿ���ʸ澯
	{.eep_addr = 225, .fun = fun_comm, .len = 1, .tag = 0x0314, .value = &dl_swr_alarm,},		// ����פ���ȸ澯
	{.eep_addr = 226, .fun = fun_comm, .len = 1, .tag = 0x0320, .value = &ext1_alarm,},	 // �ⲿ�澯1
	{.eep_addr = 227, .fun = fun_comm, .len = 1, .tag = 0x0321, .value = &ext2_alarm,},	 // �ⲿ�澯2
	{.eep_addr = 228, .fun = fun_comm, .len = 1, .tag = 0x0322, .value = &ext3_alarm,},	 // �ⲿ�澯3
	{.eep_addr = 229, .fun = fun_comm, .len = 1, .tag = 0x0323, .value = &ext4_alarm,},	 // �ⲿ�澯4
	{.eep_addr = 230, .fun = fun_comm, .len = 1, .tag = 0x0324, .value = &ext5_alarm,},	 // �ⲿ�澯5
	{.eep_addr = 231, .fun = fun_comm, .len = 1, .tag = 0x0325, .value = &ext6_alarm,},	 // �ⲿ�澯6
	{.eep_addr = 223, .fun = fun_comm, .len = 1, .tag = 0x0305, .value = &pos_cha_alarm,},  // λ�ø澯
	// �Զ������
	{.eep_addr = 232, .fun = fun_comm, .len = 2, .tag = 0x04BF, .value = (unsigned char *)&rf_ref_a,},	 // ���书��a
	{.eep_addr = 234, .fun = fun_comm, .len = 2, .tag = 0x04C0, .value = (unsigned char *)&rf_ref_b,},	 // ���书��b
	{.eep_addr = 236, .fun = fun_comm, .len = 2, .tag = 0x04B8, .value = (unsigned char *)&rf_in_a,},	 // ���빦��a
	{.eep_addr = 238, .fun = fun_comm, .len = 2, .tag = 0x04B9, .value = (unsigned char *)&rf_in_b,},	 // ���빦��b
	{.eep_addr = 240, .fun = fun_comm, .len = 2, .tag = 0x04BD, .value = (unsigned char *)&rf_out_a,},	 // �������a
	{.eep_addr = 242, .fun = fun_comm, .len = 2, .tag = 0x04BE, .value = (unsigned char *)&rf_out_b,},	 // �������b
	{.eep_addr = 244, .fun = fun_comm, .len = 2, .tag = 0x05A4, .value = (unsigned char *)&dl_output_ref,},	 // ����������ʵ�ƽ
};


unsigned char get_ext1(void);	 // �ⲿ�澯1~6
unsigned char get_ext2(void);
unsigned char get_ext3(void);
unsigned char get_ext4(void);
unsigned char get_ext5(void);
unsigned char get_ext6(void);
unsigned char get_ac(void);		// ��Դ����澯
unsigned char get_dc(void);	 // ��Դ���ϸ澯
unsigned char get_bat(void);		// ��ع��ϸ澯
unsigned char get_location(void);	 // λ�ø澯
unsigned char get_dl_pw_under(void); // �������Ƿ���ʸ澯
unsigned char get_swr(void);    // ����פ���ȸ澯
// ������������
#define PARA_CNT (sizeof(para_all)/sizeof(struct ltv_st))

struct _alarm_st alarm_st[] = {
	{.fun = get_ac, .p_value = &pw_fail_alarm, .p_en = &pw_fail_alarm_enable}, // ��Դ����澯
	{.fun = get_dc, .p_value = &pw_fault_alarm, .p_en = &pw_fault_alarm_enable}, // ��Դ���ϸ澯
	{.fun = get_bat, .p_value = &bat_fault_alarm, .p_en = &bat_fault_alarm_enable},// ���ģ���ع��ϸ澯
	{.fun = get_dl_pw_under, .p_value = &dl_low_output_alarm, .p_en = &dl_low_output_alarm_enable},// �������Ƿ���ʸ澯
	{.fun = get_swr, .p_value = &dl_swr_alarm, .p_en = &dl_swr_alarm_enable},		// ����פ���ȸ澯
	{.fun = get_ext1, .p_value = &ext1_alarm, .p_en = &ext1_alarm_enable},	 // �ⲿ�澯1
	{.fun = get_ext2, .p_value = &ext2_alarm, .p_en = &ext2_alarm_enable},	 // �ⲿ�澯2
	{.fun = get_ext3, .p_value = &ext3_alarm, .p_en = &ext3_alarm_enable},	 // �ⲿ�澯3
	{.fun = get_ext4, .p_value = &ext4_alarm, .p_en = &ext4_alarm_enable},	 // �ⲿ�澯4
	{.fun = get_ext5, .p_value = &ext5_alarm, .p_en = &ext5_alarm_enable},	 // �ⲿ�澯5
	{.fun = get_ext6, .p_value = &ext6_alarm, .p_en = &ext6_alarm_enable},	 // �ⲿ�澯6
	{.fun = get_location, .p_value = &pos_cha_alarm, .p_en = &pos_cha_alarm_enable},  // λ�ø澯
};
// �����澯��������
#define ALARM_PARA_CNT (sizeof(alarm_st)/sizeof(struct _alarm_st))


int g_send_pack_num = 0x8000;
unsigned char protocol_buf[512];
__IO unsigned int protocol_cnt = 0;
unsigned char protocol_sta = 0;

unsigned char apa_buf[512];
volatile unsigned int apa_buf_cnt = 0;
unsigned char apb_buf[512];
volatile unsigned int apb_buf_cnt = 0;

// ���干����, FLASH����ʹ��
union {
	unsigned int para_buf32bit[512];
	unsigned char para_buf8bit[2048];
}para_buf;

// ����������FLASH����
int write_to_buf32bit(void)
{
	int i,j;
	int addr = 0;
	
	j = PARA_CNT;
	for(i = 0; i < j; i++){
		memcpy(para_buf.para_buf8bit + addr, para_all[i].value, para_all[i].len);
		addr += para_all[i].len;
	}
	return 0;
}

// �����е�����д���������
int read_from_buf32bit(void)
{
	int i,j;
	int addr = 0;
	
	j = PARA_CNT;
	for(i = 0; i < j; i++){
		if(para_all[i].tag != 0x000A){
			memcpy(para_all[i].value, para_buf.para_buf8bit + addr, para_all[i].len);
		}		
		addr += para_all[i].len;
	}
	return 0;
}
// ����������FLASH���棬�ڽ������е�����д��FLASH
int write_to_flash(void)
{
	write_to_buf32bit();
	flash_write(para_buf.para_buf32bit, 512);
	return 0;
}
// ��FLASH�е����ݵ����棬�ڽ������е�����д���������
int read_from_flash(void)
{
	flash_read(para_buf.para_buf32bit, 512);
	read_from_buf32bit();
	return 0;
}
// CRCУ��
int check_crc(unsigned char * buf, int len)
{
	unsigned short tmp1 = 0;
	unsigned short tmp2 = 0;

	tmp1 = crc16(buf+1, len-4);
	tmp2 = buf[len-3] + buf[len-2]*0x100;
	if(tmp1 == tmp2){
		return 1;
	}else{
		return 0;
	}
}
/*
** �������ܣ�����PC��������������
** ����������� 
** �����������
** ����ֵ����
*/
void recv_local_omc(void)
{
	int len = 0;

	len = get_queue_buf_len(&uart3);
	if(len != 0){
		pop_queue_buf(&uart3, (char *)(protocol_buf+protocol_cnt), 1);
		if(0 == protocol_sta){
			// ����ͷ0x7E
			if(protocol_buf[0] == 0x7E){
				protocol_sta = 1;
				protocol_cnt += 1;
			}else{
				protocol_cnt = 0;
				protocol_sta = 0;
			}
		}else if(1 == protocol_sta){
			if(protocol_buf[protocol_cnt] == 0x7E){
				if((protocol_cnt+1) < 17){
					protocol_buf[0] = 0x7E;
					protocol_cnt = 1;
					protocol_sta = 1;
				}else{
					memcpy(apa_buf, protocol_buf, protocol_cnt+1);
					apa_buf_cnt = protocol_cnt+1;
					protocol_sta = 0;
					protocol_cnt = 0;
				}
			}else{
			  protocol_cnt += 1;
				if(protocol_cnt > 300){
					protocol_cnt = 0;
					protocol_sta = 0;
				}
			}
		}
		
	}
}
// ����ת�崦��
int ap_esc_process(unsigned char * buf, int len)
{
	int i = 0;
	int cnt = 0;
	
	while(i < len){
		if(buf[i] == 0x5E){
			if(buf[i+1] == 0x5D){
				buf[i] = 0x5E;
				cnt += 1;
				i += 1;
			}else if(buf[i+1] == 0x7D){
				buf[i] = 0x7E;
				cnt += 1;
				i += 1;
			}else{
				return MSG_AP_ERR;
			}
		}
		i += 1;
		buf[i-cnt] = buf[i];		
	}
	return cnt;
}
// ��ѯ�����б�
#define PARA_PACK_CNT 20
int query_idlist(unsigned char * buf, int len)
{
	struct ltv_st * p_tmp_st;
	int pack_idx = 0;
	int i = 0;
	int j = 0;
	int idlist_cnt = PARA_CNT;
	
	p_tmp_st = (struct ltv_st *)buf;
	if((0x01 == buf[3]) && (0x01 == buf[4])){ // ��һ�β�ѯ�����б�
		buf[3] = (idlist_cnt+PARA_PACK_CNT-1)/PARA_PACK_CNT;
	}
	pack_idx = buf[4];
	for(i = (pack_idx-1)*PARA_PACK_CNT; i < idlist_cnt; i++){
		buf[5+(j<<1)] = (unsigned char)(para_all[i].tag&0xff);
		buf[6+(j<<1)] = (unsigned char)((para_all[i].tag>>8)&0xff);
		p_tmp_st->len += 2;
		j += 1;
		if(j >= PARA_PACK_CNT){
			break;
		}
	} 
	return p_tmp_st->len;	
}
// ��������ltv
// ע����ltv��������ֻ����len,tag,value.��������func
int _ltv_deal(struct ltv_st * st, int cmd)
{
	int cnt, i;
	int ret = 0;

	cnt = PARA_CNT;
	for(i = 0; i < cnt; i++){
		if(st->tag == para_all[i].tag){
			ret = para_all[i].fun((void*)st, i, cmd);
			break;
		}
	}
	if(i == cnt){
		ret = MCP_NOID_ERR;
		st->tag |= MCP_NOID_ERR; // ��������޷�ʶ��
	}
	return ret;
}
// ����ltv�ṹ,���ltv���
int ltv_deal(unsigned char * buf, int * plen, int cmd)
{
	struct ltv_st * p_st;
	int i = 0;
	int ret = 0;
	int cnt = 0;

	p_st = (struct ltv_st *)&buf[0];
	if((p_st->tag == 0x0009) && (cmd == 0x02)){	// ��ѯ�����б�
		cnt = query_idlist(buf, *plen);
		*plen = cnt;
	}else{
		for(i = 0; i < *plen; ){
			p_st = (struct ltv_st *)&buf[i];
			ret = _ltv_deal(p_st, cmd);
			i += p_st->len;
		}
		if(cmd == 0x01){
			pw_fail_alarm &= 0x7f; 
			pw_fault_alarm &= 0x7f;
			bat_fault_alarm &= 0x7f;
			pos_cha_alarm &= 0x7f;
			dl_low_output_alarm &= 0x7f;
			dl_swr_alarm &= 0x7f;
			ext1_alarm &= 0x7f;
			ext2_alarm &= 0x7f;
			ext3_alarm &= 0x7f;
			ext4_alarm &= 0x7f;
			ext5_alarm &= 0x7f;
			ext6_alarm &= 0x7f;
		}
	}

	return ret;
}

// ����APA����
int ap_deal(unsigned char * buf, int * plen, unsigned char ap)
{
	struct ap_st * p_ap;
	int i = 0;
	int ret = 0;
	int cnt = 0;

	p_ap = (struct ap_st *)buf;
	if(p_ap->ap_type != ap){
		return MSG_AP_ERR;	 // apЭ�����ʹ�
	}
	if(p_ap->vp_type != VP_A){
		return MSG_VP_ERR;	 // VP��Э���
	}
	if(0 != memcmp(p_ap->dev_num, dev_num, 4)){
			if((p_ap->dev_num[0] == 0)&&(p_ap->dev_num[0] == 0)&&(p_ap->dev_num[0] == 0)&&(p_ap->dev_num[0] == 0)){
				
			}else{
				return MSG_STATION_ERR;	 // վ���Ŵ�
			}	
	}
	if(sub_num != p_ap->sub_num){
		return MSG_DEV_ERR;	   // �豸��Ŵ�
	}
	if((p_ap->pack_num > 0x8000) && (p_ap->pack_num != g_send_pack_num)){
		return MSG_PACK_NUM_ERR;  // ͨ�Ű���ʶ�Ŵ�
	}
	if((p_ap->vp_iflag != 0x80) && (p_ap->vp_iflag != 0x00)){
		return MSG_VP_INTERACT_ERR;	  // VP�������ʶ�Ŵ�
	}
	if(p_ap->mcp_flag != 0x01){
	 	return MSG_MCP_ERR;  // ��MCP:A�� 
	}
	if((p_ap->cmd_flag != 0x01) && (p_ap->rep_flag != 0xFF)){
		return MSG_MCP_RESPONSE_ERR;  // MCPӦ���ʶ��
	}
	for(i = 0; i < (*plen-17); ){
		i = p_ap->buf[i] + i;
	}
	if(i != (*plen-17)){
		p_ap->rep_flag = 0x03;
		return MCP_LEN_ERR;		 // MCPӦ�����ݳ��ȴ�
	}
	if((p_ap->cmd_flag != 0x01) && (p_ap->cmd_flag != 0x02) && (p_ap->cmd_flag != 0x03)){
		p_ap->rep_flag = 0x02;
		return MCP_CMD_ERR;		 // MCPӦ�������Ŵ�
	}
	p_ap->vp_iflag = 0x00; // 0x00:ͨ�������ִ�н������ʾִ������
	p_ap->rep_flag = 0x00;
	cnt = *plen-17;
	ret = ltv_deal(p_ap->buf, &cnt, p_ap->cmd_flag);
	if(ret < 0){// ���������ִ��
		p_ap->rep_flag = 0x01;
	}
	*plen = cnt+17;
	if(p_ap->cmd_flag == 0x01){
		return -1;
	}
	return 0;
}
int get_esc_cnt(unsigned char * buf, int len)
{
	int i = 0;
	int cnt = 0;

	for(i = 1; i < len-1; i++){
	 	if((buf[i] == 0x7E) || (buf[i] == 0x5E)){
			cnt++;
		}
	}
	return cnt;
}
// ׼����Ҫ���͵�����
// CRCУ���ת��
int prepare_send(unsigned char * buf, int len)
{
	unsigned short tmp1 = 0;
	int cnt = 0;
	int i, j;

	// ����CRC
	tmp1 = crc16(buf+1, len-4);
	buf[len-3] = (unsigned char)(tmp1&0xff);
	buf[len-2] = (unsigned char)((tmp1>>8)&0xff);
	buf[len-1] = 0x7E;
	// ת��
	cnt = get_esc_cnt(buf, len);
	if(cnt > 0){
		i = len-1;
		j = len+cnt-1;
		buf[j--] = buf[i--];
		while(i > 0){
			if(buf[i] == 0x7E){
				buf[j--] = 0x7D;
				buf[j--] = 0x5E;
				i--; 	
			}else if(buf[i] == 0x5E){ 
			 	buf[j--] = 0x5D;
				buf[j--] = 0x5E;
				i--;
			}else{
				buf[j--] = buf[i--];
			}
		}
	}
	
	return (len+cnt);	
}
/*
** �������ܣ�����������������
** ����������� 
** �����������
** ����ֵ����
*/
void local_deal(void)
{
	int len = 0;
	int tmp = 0;
	int ret = 0;
	int send_cnt = 0;

	if(apa_buf_cnt != 0){
		// ת�崦��
		tmp = ap_esc_process(apa_buf, apa_buf_cnt);
		if(tmp < 0){
			apa_buf_cnt = 0;// ת���
		}else{
			len = apa_buf_cnt - tmp; 
			apa_buf_cnt = 0;
			if(1 == check_crc(apa_buf, len)){
				ret = ap_deal(apa_buf, &len, AP_A);
				if(ret >= 0){ // 
					send_cnt = prepare_send(apa_buf, len);
					uart_send(USART3, apa_buf, send_cnt);
				}
			}else{
				apa_buf_cnt = 0;// crc У���
			}
		}
		
	}
	if(write_flash_flag == 1){
		write_to_flash();
		write_flash_flag = 0;
	}
}
extern __IO unsigned char read_current_value_flag;
extern __IO unsigned char check_alarm_flag;
void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  // AC_ON SW1 SW2 SW3 SW4
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  // ext1 2 3 4 5 6 λ�ø澯
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4  | GPIO_Pin_5 | GPIO_Pin_15;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}
// �ⲿ�澯1
unsigned char get_ext1(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0) == 0x0){
		return 1; // alarm
	}else{
		return 0;
	}
}
// �ⲿ�澯2
unsigned char get_ext2(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1) == 0x0){
		return 1; // alarm
	}else{
		return 0;
	}
}
// �ⲿ�澯3
unsigned char get_ext3(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) == 0x0){
		return 1; // alarm
	}else{
		return 0;
	}
}
// �ⲿ�澯4 
unsigned char get_ext4(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) == 0x0){
		return 1; // alarm
	}else{
		return 0;
	}
}
// �ⲿ�澯5 
unsigned char get_ext5(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) == 0x0){
		return 1; // alarm
	}else{
		return 0;
	}
}
// �ⲿ�澯6  
unsigned char get_ext6(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) == 0x0){
		return 1; // alarm
	}else{
		return 0;
	}
}
// ��Դ����澯
unsigned char get_ac(void)
{
	//if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 0x0){
	if(get_voltage(VDC_DET) < 1500){ // ��DC����1.5Vʱ����ΪΪ��Դ����
		return 1; // ac alarm
	}else{
		return 0;
	}
}
// ��Դ���ϸ澯
unsigned char get_dc(void)
{
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 0x1){
		if(get_voltage(VDC_DET) < 1500){
			//return 1;
		}else{
			//return 0;
		}
	}else{
		//return 0;
	}
	return 0;
}
// ��ع��ϸ澯
unsigned char get_bat(void)
{
	if(get_voltage(VDC_DET) > 1500){
		if(get_voltage(VBAT_DET) < 1000){
		//	return 1;
		}else{
		//	return 0;
		}
	}else{
		//return 0;
	}
	return 0;
}
// λ�ø澯
unsigned char get_location(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15) == 0x0){
		return 0; 
	}else{
		return 1; // alarm
	}
}	
// �������Ƿ���ʸ澯
unsigned char get_dl_pw_under(void)
{
	if(dl_output < dl_output_min){
		return 1;
	}
	return  0;
} 
// ����פ���ȸ澯
unsigned char get_swr(void)
{
	if(dl_swr > dl_swr_thr){
		return 1;
	}
	return 0;
}    
		
void read_swr(void)
{
	if((dl_output >= 20)&&(dl_output <= 40)){
		if((dl_output-dl_output_ref/10) >= 20){
			dl_swr = 12;
		}else if((dl_output-dl_output_ref/10) >= 17){
			dl_swr = 13;
		}else if((dl_output-dl_output_ref/10) >= 15){
			dl_swr = 14;
		}else if((dl_output-dl_output_ref/10) >= 14){
			dl_swr = 15;
		}else if((dl_output-dl_output_ref/10) >= 12){
			dl_swr = 16;
		}else if((dl_output-dl_output_ref/10) >= 11){
			dl_swr = 18;
		}else if((dl_output-dl_output_ref/10) >= 9){
			dl_swr = 20;
		}else if((dl_output-dl_output_ref/10) >= 8){
			dl_swr = 23;
		}else if((dl_output-dl_output_ref/10) >= 7){
			dl_swr = 26;
		}else{
			dl_swr = 30;
		} 
	}else{
		dl_swr = 12;
	}
}
// �澯���
#define ALARM_CNT 36
#define RECOVER_CNT 81
unsigned char new_alarm_flag = 1;
void check_alarm(void)
{
	int i = ALARM_PARA_CNT;
	int v_tmp;
	int alarm_cnt, recover_cnt;
	float f_tmp;

	if(check_alarm_flag == 1){	// ÿ2��ִ��һ��
		check_alarm_flag = 0;
		v_tmp = get_voltage(RF_REF_DET);	// ���书��
		f_tmp = 10*(((float)rf_ref_a/10000)*v_tmp+(float)rf_ref_b);
		dl_output_ref = float2int(f_tmp);
		v_tmp = get_voltage(RF_OUT_DET);	// �����������
		f_tmp = ((float)rf_out_a/10000)*v_tmp+(float)rf_out_b;
		dl_output = float2int(f_tmp);
		v_tmp = get_voltage(RD_IN_DET);		// �������빦��
		f_tmp = ((float)rf_in_a/10000)*v_tmp+(float)rf_in_b;	
		dl_input = float2int(f_tmp);		
		dl_actual_gain = dl_output - dl_input;	// ����ʵ������
		read_swr();  // ����פ����

		// �澯���
		for(i = 0; i < 12; i++){
			if(i < 5){
				alarm_cnt = ALARM_CNT;
				recover_cnt = RECOVER_CNT;
			}else{
				alarm_cnt = 1;
				recover_cnt = 1;
			}
		 	if(1 == alarm_st[i].fun()){ // alarm
			 	alarm_st[i].alarm_cnt++;	 // �澯������1
			}
			alarm_st[i].all_cnt++;
			if(alarm_st[i].alarm_cnt >= alarm_cnt){ 
			  alarm_st[i].status = 1;		 // �����澯
				alarm_st[i].alarm_cnt = 0;
				alarm_st[i].all_cnt = 0;
				if(*(alarm_st[i].p_en) == 1){ // �澯ʹ��
					if((*(alarm_st[i].p_value))&0x1){
					}else{
						*(alarm_st[i].p_value) = 0x01;// ���¸澯����
						new_alarm_flag = 1;
						g_realarm_cnt = 0;
					}
				}else{	// ����
					*(alarm_st[i].p_value) = 0;
				}
			}else if((alarm_st[i].all_cnt - alarm_st[i].alarm_cnt) >= recover_cnt){
				alarm_st[i].status = 0;		// �澯�ָ�
				alarm_st[i].alarm_cnt = 0;
				alarm_st[i].all_cnt = 0;	
				if(*(alarm_st[i].p_en) == 1){ // �澯ʹ��
					if((*(alarm_st[i].p_value))&0x01){
						*(alarm_st[i].p_value) = 0x80;// ���µĸ澯�ָ�
						new_alarm_flag = 1;
						g_realarm_cnt = 0;
					}
				}else{	// ����
					*(alarm_st[i].p_value) = 0;
				}			
			}
		}
	}
}
extern const char ascii_tb[];
// HEX���ΪASCII��
void apb_hex2asc(unsigned char * buf, int len)
{
	int w_cnt, r_cnt;

	r_cnt = len-1;
	w_cnt = len*2-2-1;
	buf[w_cnt--] = buf[r_cnt--]; // copy !
	while(r_cnt){
		buf[w_cnt--] = ascii_tb[(buf[r_cnt])&0xF];
		buf[w_cnt--] = ascii_tb[(buf[r_cnt--]>>4)&0xF];
	}
}
int is_alarm(void)
{
	int i, cnt;
	unsigned char tmp;
	
	cnt = ALARM_PARA_CNT;
	for(i = 0; i < cnt; i++){
		tmp = *(para_all[i+38].value);
		if((tmp == 0x80) || (tmp == 0x01)){
			return 1;
		}
	}
	return 0;
}
// ��ȡ�澯����
int read_alarm(unsigned char * buf)
{
 	int len, i, cnt;
	unsigned char tmp;

	len = 0;
	cnt = ALARM_PARA_CNT;
	for(i = 0; i < cnt; i++){
		tmp = *(para_all[i+38].value);
		if((tmp == 0x80) || (tmp == 0x01)){
			buf[len++] = 4;
			buf[len++] = ((para_all[i+38].tag)&0xFF);
			buf[len++] = ((para_all[i+38].tag>>8)&0xFF);
			buf[len++] = tmp&0x1;
		}
	}
	return len;
}
#define REPORT_ALARM 0x01
#define REPORT_START 0x02
#define REPORT_POLL  0x03
#define REPORT_REPAIR 0x04
#define REPORT_CHANGE 0x05

unsigned short g_pack_num = 0x8000;
// �ϱ�����
int report(unsigned char cmd)
{
	struct ap_st * apb_st = (struct ap_st *)apb_buf;
	int len = sizeof(struct ap_st);
	int len_tmp = 0;
	unsigned short crc_tmp = 0;

	apb_st->start_flag = 0x21;		// !
	apb_st->ap_type = 0x02;  // ap:b
	apb_st->vp_type = 0x01; // vp
	memcpy(apb_st->dev_num, dev_num, 4); // dev_num[4]
	apb_st->sub_num = sub_num;
	apb_st->pack_num = g_pack_num++;
	apb_st->vp_iflag = 0x80;
	apb_st->mcp_flag = 0x01;
	apb_st->cmd_flag = 0x01;
	apb_st->rep_flag = 0xFF;
	apb_st->buf[0] = 0x04; // len=4
	apb_st->buf[1] = 0x41; // tag=0x0141
	apb_st->buf[2] = 0x01;
	apb_st->buf[3] = cmd;  // report type
	len += 4;
	if(cmd == REPORT_ALARM){
		len_tmp = read_alarm(apb_st->buf+4);
	}
	len += len_tmp;
	crc_tmp = crc16(apb_buf+1, len-1);
	apb_buf[len++] = (unsigned char)(crc_tmp&0xFF);
	apb_buf[len++] = (unsigned char)((crc_tmp>>8)&0xFF);
	apb_buf[len++] = 0x21;
	apb_hex2asc(apb_buf, len);	
	apb_buf_cnt = len*2-2;
	apb_buf[apb_buf_cnt] = 0x1A;
	apb_buf[apb_buf_cnt+1] = 0;
	return apb_buf_cnt;
}
// ����Ƿ����ϱ�����
// 0:��λ״̬ 1���ȴ������ϱ�״̬  2���ѷ�����ɣ��ȴ���λ״̬

int check_report(void)
{
	unsigned int tmp = 0;
	
	// ��վ�ϱ�
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0x0){
		report_start_flag = 0; 
	}else{
		if(report_start_flag == 0){
			report_start_flag = 1;
			report(REPORT_START);
		}
	}
	// Ѳ���ϱ�
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0x0){
		report_poll_flag = 0; 
	}else{
		if(report_poll_flag == 0){
			report_poll_flag = 1;
			report(REPORT_POLL);
		}
	}
	// �����޸��ϱ�
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0x0){
		report_repair_flag = 0; 
	}else{
		if(report_repair_flag == 0){
			report_repair_flag = 1;
			report(REPORT_REPAIR);
		}
	}
	// ���ñ���ϱ�
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0x0){
		report_change_flga = 0; 
	}else{
		if(report_change_flga == 0){
			report_change_flga = 1;
			report(REPORT_CHANGE);
		}
	}
	// �澯�ϱ�
	if(new_alarm_flag == 0x01){
		if(apb_buf_cnt == 0){
			new_alarm_flag = 0;
			if(is_alarm() == 1){
				report(REPORT_ALARM);
				g_alarm_send_recv_flag = 1;
				g_alarm_send_time = get_time6_cnt();
				g_realarm_cnt += 1;
			}
		}
	}
	if(g_alarm_send_recv_flag == 1){ // �����澯ȷ�Ϻ����㣡����
		if(g_realarm_cnt < 9){
			tmp = get_time6_cnt();
			if(g_realarm_cnt%3){ // 				
				if(time_after(tmp, g_alarm_send_time+180)){ // ��ʱ
					new_alarm_flag = 1;
				}
			}else{
				if(time_after(tmp, g_alarm_send_time+10800)){ // ��ʱ
					new_alarm_flag = 1;
				}
			}
		}
	}
	return 0;
}
