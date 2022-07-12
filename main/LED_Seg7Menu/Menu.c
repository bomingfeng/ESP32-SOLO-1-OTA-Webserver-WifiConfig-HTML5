/***********************************************************************************************
�ļ���	Menu.c
����	2014.02.16
����	��ʫ��	�¶���
�ļ�˵��	�����˵�֧��
�޶�˵��:

2013.02.16  ��ʫ�� ��ʼ�汾
***********************************************************************************************/

/*ͷ�ļ�--------------------------------------------------------------------------------------*/
#include "Menu.h"

//#include ""

/*ȫ�ֱ���------------------------------------------------------------------------------------*/
//ȫ�ֲ˵�
CMenuDC menuDC;				/*ȫ�ֲ˵�����*/

//����������
#if SMENU_01_CURSOR==1
const unsigned short CursorIncr[]=
{1,10,100,1000,10000};
#endif

/*�ӿں���------------------------------------------------------------------------------------*/

//�˵���ʼ��
void CMenu_MenuInit(CMenuItem* pRootMenu)
{
	//��ʼ���˵�����
	menuDC.theMenu.m_menuIndex = 0;			/*�˵�������ʼ��*/
	menuDC.theMenu.m_pMenuList = pRootMenu;	/*Ҫ��ʾ�Ĳ˵��б�*/
	menuDC.pMenuItemPtr = pRootMenu;		/*��ǰ�˵���ָ��*/

	#if SMENU_01_CURSOR==1
	menuDC.cursorIndex = 0;					/*������ֵ���λ*/
	menuDC.cursorTimer = MENU_CURSOR_OFF;	/*��궨ʱ��*/
	#endif

	CMenu_SetFocus(CMenuObject_Proc);				/*���뽹��*/
	CMsgPr_SendMessage(CMenu_GetFocus(),WM_INIT);	/*��ʼ���ؼ�*/
	CMsgPr_EndMessage();							/*������Ϣ����*/

	//�ص��˵���ʼ����
	CMenu_Start();
}
//��ֵ�޷�����
void CMenu_SetValueLimit(void)
{
	int max;				/*���ֵ*/
	int min;				/*��Сֵ*/

	max = CMenu_GetMenuItemPtr()->max;	/*���ֵ*/
	min = CMenu_GetMenuItemPtr()->min;	/*��Сֵ*/
	
	//�޷�����
	if (menuDC.cValue>max){menuDC.cValue=max;}
	else if (menuDC.cValue<min){menuDC.cValue=min;}
}
//�������뽹��
void CMenu_SetFocus(MsgPrce aFocus)
{
	menuDC.pFocus=aFocus;			/*��ǰ���뽹��*/
	CMsgPr_SetMsgObject(aFocus);	/*��ǰ��ϢĿ��*/
}

//����������
#if SMENU_01_CURSOR==1
//�򿪹�궨ʱ��
void CMenu_CursorTurnOn(void)
{
	menuDC.cursorTimer=MENU_CURSOR_SHOW;	/*�����ʾ*/
	menuDC.cursorIndex=0;					/*���������ʼ��*/
}
#endif

//�����¼����趨ʱ����
void CMenu_KeyEvent(unsigned char aKey)
{
	//����״̬������
	if (WM_KEY_NULL == aKey){CKey_PullKey();}
	else {CKey_PushKey();}

	//�����¼��ɷ�
	if (CKey_GetState() == KEY_STATE_PUSH)
	{
		CMenu_MenuEvent(aKey);		/*�����¼�*/
		CMenu_ResetCursorTimer();	/*��λ��궨ʱ��*/
	}
	else if (CKey_GetState()==KEY_STATE_M_PUSHED&& 
		aKey != WM_KEY_SET)
		CMenu_MenuEvent(aKey);
}
//��ʾ�˵�����
void CMenu_DrawMenuText(void)
{
	unsigned char i;
	//������ʾ����
	for (i=0;i<SMENU_TITLE_LEN;++i)
		segDisBuff[i+SMENU_TITLE_POSIT]=CMenu_GetMenuItemPtr()->str[i];
}
//��ʾ�˵���ֵ
void CMenu_DrawMenuValue(unsigned char x,unsigned char width, unsigned short value)
{
	unsigned char strTemp[5],i;

	for (i=0;i<5;++i)
	{
		strTemp[4-i]=SegDigCode[value%10];
		value /= 10;
	}
	//��0����
	for (i=0;i<5;++i)
	{
		if (i>=(5-width))
			break;

		if (strTemp[i]==SEG7_CODE_0)
			strTemp[i]=SEG7_CODE_NULL;
		else
			break;
	}
	//��ʾ
	for (i=0;i<CURSOR_NUMBER;++i)
		segDisBuff[CURSOR_NUMBER+x-1-i]=strTemp[4-i];
}

#if SMENU_01_CURSOR==1
//��궨ʱ��
void CMenu_CursorEvent(void)
{
	//��궨ʱ���� ��ʱ���Ͷ�ʱ��Ϣ
	if (MENU_CURSOR_OFF==menuDC.cursorTimer)
		return;

	CMenu_MenuEvent(WM_TIMER);
}
#endif
