#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


#ifndef NULL
	#define NULL 0
#endif

// ��ʱ��unknown����knownʱ��������
#define time_after(unknown, known) ((int)(known) - (int)(unknown) < 0)
// ��ʱ��unknownû����knownʱ��������
#define time_before(unknown, known) ((int)(unknown) - (int)(known) < 0)
#define time_after_eq(unknown, known) ((int)(unknown) - (int)(known) >= 0)
#define time_before_eq(unknown, known) ((int)(known) - (int)(unknown) >= 0)

/*********************** �����������в��� **************************/
// ѭ�����л������ṹ
struct queue_buf{
	int len;				// �����ѭ�����л���������
	volatile int head;		// ������ͷָ��
	volatile int tail;		// ������βָ��
	char * buf;    			// ������
};
/*
** �������ܣ���ʼ��ѭ�����л�����
** ���������queue_buf=����ʼ����ѭ�����нṹ��ָ�� len=ѭ�����г���
** �����������
** ����ֵ��0=�ɹ� -1=ʧ��
** ��ע��len����Ϊ128/256/512/1024/2048/4096
*/
int queue_buf_init(struct queue_buf * queue_buf, char * buf, unsigned int len);

/*
** �������ܣ�ɾ��ѭ�����л�����
** ���������queue_buf=��ɾ����ѭ�����нṹ��ָ�� 
** �����������
** ����ֵ��0=�ɹ� -1=ʧ��
*/
void * queue_buf_exit(void * arg);

/*
** �������ܣ���ȡѭ�����л�������Ч���ݳ���
** �������: queue_buf=ѭ�����нṹ��ָ��
** �����������
** ����ֵ����Ч���ݳ���
*/
int get_queue_buf_len(struct queue_buf * queue_buf);

/*
** �������ܣ�ѭ�����л�������C
** �������: queue_buf=ѭ�����нṹ��ָ�� buf=����C������ָ�� len=����C���ݳ���
** �����������
** ����ֵ��0=�ɹ� -1=ʧ��
*/
int push_queue_buf(struct queue_buf * queue_buf, char * buf, unsigned int len);

/*
** �������ܣ�ѭ�����л��������C
** �������: queue_buf=ѭ�����нṹ��ָ�� buf=�����C������ָ�� len=�����C���ݳ���
** �����������
** ����ֵ��0=�ɹ� -1=ʧ��
*/
int pop_queue_buf(struct queue_buf * queue_buf, char * buf, unsigned int len);

/*
** �������ܣ���HEXתΪunsigned short int �ͣ���������
** �������: p=hex����ָ��
** �����������
** ����ֵ��ת��ֵ
*/
unsigned short int hex2uint16(unsigned char * p);

/*
** �������ܣ���HEXתΪASCII
** �������: p=hex���� len=���ݳ���
** ���������out=ascii����
** ����ֵ��ת���������
*/
int hex2ascii(unsigned char * p, int len, unsigned char * out);
int hex2ascii_ex(unsigned char * p, int len);

// asciiתΪHEX
unsigned char _ascii2hex(unsigned char p);
/*
** �������ܣ���ASCIIתΪHEX
** �������: p=ascii���� len=���ݳ���
** ���������out=hex����
** ����ֵ��ת���������
*/
int ascii2hex(unsigned char * p, int len, unsigned char * out);
int ascii2hex_ex(unsigned char * p, int len);
/*
** �������ܣ������ֽ��з�����תΪ4�ֽ��з�����
** �������: val=���ֽ��з�����
** �����������
** ����ֵ��4�ֽ��з�����
*/
int signed_1to4(char val);

/*
** �������ܣ���2�ֽ��з�����תΪ4�ֽ��з�����
** �������: val=���ֽ��з�����
** �����������
** ����ֵ��4�ֽ��з�����
*/
int signed_2to4(short val);

// �ֽڽ���
void swap(char *p, char *q);

// ��������
// str=����ָ��  len=������Ч���ݳ���  cnt=���Ƴ���
void shift_left(char * str, int len, int cnt);

// ����CRC
unsigned short crc16(unsigned char *ptr, unsigned short len);

void my_printf(const char * format, ...);

int float2int(float f);

#ifdef __cplusplus
}
#endif

#endif 


