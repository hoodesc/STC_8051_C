#ifndef _GSM_GTM900b_H_
#define _GSM_GTM900b_H_
enum GSM_function{GSM_TSIM,GSM_CSQ,GSM_CMGF,GSM_CMGS,GSM_CNMI,GSM_CMGR,GSM_CSMP,GSM_COPS};
//				 测试SIM卡  信号  文本/PDU  号码 前  短信提醒  短信读取 sms格式	运营商查询
enum GSM_SMS{warning_gas,warning_t,open_relay,close_relay};
//			 气体警告 	 温度警告  开继电器   关继电器

extern bit GSM_tsim_ok;		// SIM卡是否在位标识
extern bit GSM_cmgf_ok;		// 文本设置完成标识
extern bit GSM_csmp_ok;		// SMS格式设置完成标识
extern bit GSM_csq_ok;		// 信号强度获取标识
extern bit GSM_cmgs_ok;   	// 电话号码发送完成标识
extern bit GSM_sms_ready_ok;
extern bit GSM_receive_ok;	// “接收到短信”标识
extern bit GSM_cops_ok;		// 运营商获取成功标识
extern bit GSM_cnmi_ok;		// 短信提示设置完成标识


extern bit GSM_CMCC;
extern bit GSM_CUCC;
extern uchar code GSM_TAB[];
extern uchar TEMP[];
extern uchar CSQ[];	// 存放信号质量值
extern uchar gsm_delay_time; // GSM模块等待初始化时间
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