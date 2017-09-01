#ifndef _GSM_GTM900b_H_
#define _GSM_GTM900b_H_
enum GSM_function{GSM_TSIM,GSM_CSQ,GSM_CMGF,GSM_CMGS,GSM_CNMI,GSM_CMGR,GSM_CSMP,GSM_COPS};
//				 ����SIM��  �ź�  �ı�/PDU  ���� ǰ  ��������  ���Ŷ�ȡ sms��ʽ	��Ӫ�̲�ѯ
enum GSM_SMS{warning_gas,warning_t,open_relay,close_relay};
//			 ���徯�� 	 �¶Ⱦ���  ���̵���   �ؼ̵���

extern bit GSM_tsim_ok;		// SIM���Ƿ���λ��ʶ
extern bit GSM_cmgf_ok;		// �ı�������ɱ�ʶ
extern bit GSM_csmp_ok;		// SMS��ʽ������ɱ�ʶ
extern bit GSM_csq_ok;		// �ź�ǿ�Ȼ�ȡ��ʶ
extern bit GSM_cmgs_ok;   	// �绰���뷢����ɱ�ʶ
extern bit GSM_sms_ready_ok;
extern bit GSM_receive_ok;	// �����յ����š���ʶ
extern bit GSM_cops_ok;		// ��Ӫ�̻�ȡ�ɹ���ʶ
extern bit GSM_cnmi_ok;		// ������ʾ������ɱ�ʶ


extern bit GSM_CMCC;
extern bit GSM_CUCC;
extern uchar code GSM_TAB[];
extern uchar TEMP[];
extern uchar CSQ[];	// ����ź�����ֵ
extern uchar gsm_delay_time; // GSMģ��ȴ���ʼ��ʱ��
extern void GSM_TX(uchar value);
extern void GSM_RX(uchar value);
extern void GSM_RX_clear(void);
extern void GSM_CSQ_manage(void);
extern void GSM_COPS_manage(void);
extern void GSM_TSIM_manage(void);
extern void GSM_sms_ready(uchar value);
extern void GSM_Init(void);
extern void Uart_Init(void);




#endif