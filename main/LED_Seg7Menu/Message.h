/***********************************************************************************************
�ļ���	Message.h
����	2014.02.16
����	��ʫ��	�¶���
�ļ�˵��	������Ϣ֧��
�޶�˵��:

2014.02.16  ��ʫ�� ��ʼ�汾
2014.03.13	��ʫ�� �޶���Ϣ���ͷ�ʽΪ��������
					Ϊ�˼�����Ϣ����ĵݹ���� �޸�����Ϣ������
***********************************************************************************************/

#ifndef _LIB_SMENU_MESSAGE_
#define _LIB_SMENU_MESSAGE_

/*ͷ�ļ�--------------------------------------------------------------------------------------*/
//#include ""

/*�û�����------------------------------------------------------------------------------------*/

/*��������------------------------------------------------------------------------------------*/

//��Ϣ����
#define WM_NULL			0					/*��Ϣ���ݽ���*/

//ϵͳ��Ϣ
#define WM_SYS_OFFSET 	1					/*ϵͳ��Ϣ��ʼƫ��*/

#define WM_INIT			WM_SYS_OFFSET+0		/*��ʼ����Ϣ*/
#define WM_PAINT 		WM_SYS_OFFSET+1		/*�ػ���Ϣ*/
#define WM_CLEAR		WM_SYS_OFFSET+2		/*������Ϣ*/
#define WM_DESTROY		WM_SYS_OFFSET+3		/*������Ϣ*/
#define WM_UPDATE		WM_SYS_OFFSET+4		/*��ֵ����ˢ����Ϣ*/
#define WM_TIMER		WM_SYS_OFFSET+5		/*��ʱ��Ϣ ������˸�����*/

//�˵���Ϣ
#define WM_MENU_PREV	WM_SYS_OFFSET+6		/*�ϸ��˵�*/
#define WM_MENU_NEXT	WM_SYS_OFFSET+7		/*�¸��˵�*/

//������Ϣ
#define WM_KEY_OFFSET	WM_SYS_OFFSET+32	/*������Ϣ*/

#define WM_KEY_NULL		0					/*�ް���*/

#define WM_KEY_SET		WM_KEY_OFFSET+0		/*SET����*/
#define WM_KEY_LEFT		WM_KEY_OFFSET+1		/*�󰴼�*/
#define WM_KEY_RIGHT	WM_KEY_OFFSET+2		/*�Ұ���*/
#define WM_KEY_ADD		WM_KEY_OFFSET+3		/*�Ӱ���*/
#define WM_KEY_SUB		WM_KEY_OFFSET+4		/*������*/
#define WM_KEY_MENU		WM_KEY_OFFSET+5		/*������Ϣ*/
#define WM_KEY_RETURN	WM_KEY_OFFSET+6		/*���ذ���*/

//�û���Ϣƫ��
#define WM_USER_OFFSET 	WM_SYS_OFFSET+128	/*�û���Ϣƫ��*/

/*���Ͷ���------------------------------------------------------------------------------------*/

//��Ϣ����
typedef unsigned char Msg;			/*Message����*/
typedef void (*MsgPrce)(void);		/*��Ϣ���̺���ָ��*/
//ȫ����Ϣ�ṹ
typedef struct
{
	Msg m_msg;				/*��Ϣ*/
	MsgPrce m_msgPrce;		/*��ǰ��ϢĿ������*/
	
	MsgPrce m_theObject;	/*��Ϣ����*/
}CMsgDC;

/*��������------------------------------------------------------------------------------------*/
extern CMsgDC msgDC;		/*ȫ����Ϣ����*/

/*�ӿں���------------------------------------------------------------------------------------*/

//������Ϣջ����Ϣ���̶���
#define CMsgPr_SetMsgObject(X) (msgDC.m_theObject=(MsgPrce)(X))
//����Ϣ���л�ȡ��Ϣ
unsigned char CMsgPr_GetMessage(void);
//���Լ�Ͷ��������Ϣ
void CMsgPr_PollMessage(Msg aMsg);
//ֱ�ӷ�����Ϣ
void CMsgPr_SendMessage(MsgPrce pmsgPrce, Msg aMsg);
//������Ϣ����
void CMsgPr_ParentMessage(MsgPrce pmsgPrce, Msg aMsg);
//������Ϣ����
void CMsgPr_EndMessage(void);

#endif
