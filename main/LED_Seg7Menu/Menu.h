/***********************************************************************************************
�ļ���	Menu.h
����	2014.02.16
����	��ʫ��	�¶���
�ļ�˵��	�����˵�֧��
�޶�˵��:

2013.02.16  ��ʫ�� ��ʼ�汾
2013.03.28	��ʫ�� ��������λ�������� �Ͳ˵��ֲ�֧��
***********************************************************************************************/

#ifndef _LIB_SMENU_MENU_
#define _LIB_SMENU_MENU_

/*ͷ�ļ�--------------------------------------------------------------------------------------*/
#include "Message.h"
#include "Key.h"
#include "SegDynamicScan.h"
#include "Seg7Font.h"

//#include ""

/*�û�����------------------------------------------------------------------------------------*/
//���˵�����
//#define SMENU_PLIE_MAX		4		/*���˵�����*/

//��������ַ�
#define CURSOR_CHARACTER SEG7_CODE_LL	/*�������ʱ��ʾ���ַ�*/
//�����ַ���ʾ��ʼλ��
#define SMENU_TITLE_POSIT	0			/*��ߵ�һ�������Ϊ0 ������*/
//�����ַ���
#define SMENU_TITLE_LEN		4			/*��ʾ����ʱ��ʾ���ַ���Ŀ*/
//��ֵ��ʾ��ʼλ��
#define SMENU_VALUE_POSIT	4			/*��ߵ�һ�������Ϊ0*/
//����������λ ��ֵ�ַ���
#define CURSOR_NUMBER		4			/*���λ���ֵ*/
//��ֵ������ʾλ��
#define VALUE_ZERO_MIN 		1			/*������ʾλ�� ������0����*/
//
/*--------------------------------�˵�ģʽ--------------------------------
0	����ģʽ
1	���������ģʽ
2	ȫ��ģʽ
3	ȫ�Դ����ģʽ
*/
#define SMENU_MODE_SETUP	3		/*�˵�ģʽ*/
//���֧�ֿ���
#define SMENU_01_CURSOR		1		/*0��֧�ֹ�� 1֧�ֹ��*/
//���ֵ���ѭ���ƶ�����
#define SMENU_01_DIG_LOOP	1		/*0��겻��ѭ����λ 1���ѭ����λ*/

//��ֲ��������

//�˵����� �û�����ú������˵���ʼ��ʱ���ص�
void CMenu_Start(void);
//�˵����� �û�����ú������˵�����ʱ���ص�
void CMenu_Quit(void);
//��λ��궨ʱ�� ��������ʱ�ص� ʹ����ʾһ��ʱ������˸
#define CMenu_ResetCursorTimer() (cursorTimer=0)

/*��������------------------------------------------------------------------------------------*/

//�˵�����ֵ����
#define MENU_VALUTYPE_U8	0x00	/*u8һ�ֽ�����*/	
#define MENU_VALUTYPE_U16 	0x01	/*u16˫�ֽ�����*/

//�����Ƴ���
#define MENU_CURSOR_OFF		0x00	/*��궨ʱ���ر�*/
#define MENU_CURSOR_SHOW	0x01	/*�����ʾ*/
#define MENU_CURSOR_HIDE	0x02	/*�������*/

/*������------------------------------------------------------------------------------------*/
#if CURSOR_NUMBER==0
	#error "SMENU_CURSOR_MAX must be greater than 0!"
#endif

#if SMENU_TITLE_LEN>SEG_DIGIT_NUM
	#error "SMENU_TITLE_LEN value is set too large"
#endif

#if CURSOR_NUMBER>SEG_DIGIT_NUM
	#error "CURSOR_NUMBER value is set too large"
#endif

#if SMENU_VALUE_POSIT+CURSOR_NUMBER>SEG_DIGIT_NUM
	#error "SMENU_VALUE_POSIT+CURSOR_NUMBER value is set too large"
#endif

#if CURSOR_NUMBER>5
	#error "CURSOR_NUMBER must be less than 6!"
#endif
/*���Ͷ���------------------------------------------------------------------------------------*/

//�˵���������
typedef struct
{
	unsigned char str[SMENU_TITLE_LEN];	/*��ʾ�ַ���*/
	unsigned char attri;	/*����*/
	int min;				/*��ֵ��Сֵ*/
	int max;				/*��ֵ���ֵ*/
	void* m_pdata;			/*����ָ��*/
	void* m_pfun;			/*�ؼ����̺���*/
}CMenuItem;

//�˵�����
typedef struct
{
	unsigned char m_menuIndex;	/*��ǰ�˵�������*/
	CMenuItem* m_pMenuList;		/*��ǰ�˵��б�ָ��*/
}
CMenuI;

//ȫ�ֲ˵�����
typedef struct
{
	CMenuI theMenu;				/*��ǰ�˵�*/
	CMenuItem* pMenuItemPtr;	/*��ǰ�˵���ָ��*/
	MsgPrce pFocus;				/*��ǰ���뽹��*/

	signed int cValue;			/*��ǰֵ*/

	#if SMENU_01_CURSOR==1
	unsigned char cursorIndex;	/*���ֵ���λ*/
	unsigned char cursorTimer;	/*��궨ʱ��*/
	#endif
}
CMenuDC;						/*ȫ�ֲ˵�������*/

/*�ⲿ����------------------------------------------------------------------------------------*/

extern CMenuDC menuDC;			/*ȫ�ֲ˵�����*/
extern unsigned char cursorTimer;
extern const unsigned short CursorIncr[];
extern const unsigned char SegDigCode[];
/*�꺯��--------------------------------------------------------------------------------------*/

//��ȡ��ǰ�˵���ָ��
#define CMenu_GetMenuItemPtr() ((CMenuItem*const)menuDC.pMenuItemPtr)
//��ȡ��ǰ�˵�������
#define CMenu_GetMenuIndex() ((const unsigned char)(menuDC.theMenu.m_menuIndex))
//�������뽹��
//#define CMenu_SetFocus(X) (menuDC.pFocus=(X),CMsgPr_SetMsgObject(X))
void CMenu_SetFocus(MsgPrce aFocus);
//��ȡ��ǰ���뽹��
#define CMenu_GetFocus() ((const MsgPrce)menuDC.pFocus)
//�ɷ��˵��¼�
#define CMenu_MenuEvent(X) (CMsgPr_SendMessage(CMenu_GetFocus(),X))

/*�û��ӿ�------------------------------------------------------------------------------------*/

//��ʼ���˵� �û�ʹ�ò˵�ǰ����
void CMenu_MenuInit(CMenuItem* pRootMenu);
//�����¼� ���û���ʱ����
void CMenu_KeyEvent(unsigned char aKey);

#if SMENU_01_CURSOR==1
//��궨ʱ��
void CMenu_CursorEvent(void);
//�򿪹�궨ʱ��
//#define CMenu_CursorTurnOn() (menuDC.cursorTimer=MENU_CURSOR_SHOW,menuDC.cursorIndex=0)
void CMenu_CursorTurnOn(void);
//�رչ�궨ʱ��
#define CMenu_CursorTurnOff() (menuDC.cursorTimer=MENU_CURSOR_OFF)
//��ȡ��궨ʱ��
#define CMenu_GetCursorState() ((const unsigned char)menuDC.cursorTimer)
#else
#define CMenu_CursorEvent()
#endif

//�˵�������� ���ڹ���˵���ת
void CMenuObject_Proc(void);
//�˵���ֵ��������
void CValueObject_Proc(void);
//ͨ����ֵ�����˵���Ϣ����
void CValueMenu_Proc(void);
//�޷�����
void CMenu_SetValueLimit(void);
//��ʾ�˵����� �ص�����
void CMenu_DrawMenuText(void);
//��ʾ�˵���ֵ �ص�����
void CMenu_DrawMenuValue(unsigned char x,unsigned char width, unsigned short value);

#endif
