/***********************************************************************************************
�ļ���	Menu_00.c
����	2014.02.28
����	��ʫ��	�¶���
�ļ�˵��	�˵�ģʽ00 ���������ģʽ
�޶�˵��:

2013.02.16  ��ʫ�� ��ʼ�汾
***********************************************************************************************/

/*ͷ�ļ�--------------------------------------------------------------------------------------*/
#include "Menu.h"
#include "Seg7Font.h"
//#include ""

#if SMENU_MODE_SETUP==0

/*�ӿں���------------------------------------------------------------------------------------*/

//�˵�������� ���ڹ���˵���ת
void CMenuObject_Proc(void)
{
	switch(CMsgPr_GetMessage())	
	{
		//��ʼ����Ϣ
		case WM_INIT:
			/*���㵱ǰ�˵���ָ�� �Է������*/
			menuDC.pMenuItemPtr=menuDC.theMenu.m_pMenuList+menuDC.theMenu.m_menuIndex;
			//�ж��ǲ˵������
			if (0 == CMenu_GetMenuItemPtr()->m_pfun){CMenu_Quit();break;}
			#if SMENU_01_CURSOR==1
			CMenu_CursorTurnOff();					/*�رչ�궨ʱ��*/
			#endif
			CMsgPr_PollMessage(WM_PAINT);			/*Ͷ����ʾ��Ϣ*/
			break;
		//�¸��˵���
		case WM_MENU_NEXT:	
			++menuDC.theMenu.m_menuIndex;
			CMsgPr_PollMessage(WM_INIT);			/*Ͷ�ݳ�ʼ����Ϣ*/
			break;
		//������Ϣ
		case WM_PAINT:
			CMenu_DrawMenuText();					/*���Ʋ˵�����*/
			break;
		//���ü�
		case WM_KEY_SET:
			//����SET��������ֵ�����ؼ�
			CMenu_SetFocus(CMenu_GetMenuItemPtr()->m_pfun);	//��ǰ���뽹��
			CMsgPr_PollMessage(WM_INIT);					//INIT��Ϣ
			break;
		default:					/*Ĭ����Ϣ������Ϣ����*/
			CMsgPr_EndMessage();
			break;
	}
}
//�˵���ֵ�������� ������ֵ�����ĸ���
void CValueObject_Proc(void)
{
	switch(CMsgPr_GetMessage())
	{
		case WM_INIT:		/*��ʼ���˵��ؼ���Ϣ*/
			CMsgPr_PollMessage(WM_PAINT);			/*Ͷ����ʾ��Ϣ*/
			#if SMENU_01_CURSOR==1
			CMenu_CursorTurnOn();					/*�򿪹�궨ʱ��*/
			#endif
			break;
		case WM_KEY_SET:	/*���ü���Ϣ*/
			//SET�����½���˵�����ؼ�
			CMenu_SetFocus(CMenuObject_Proc);		//�������뽹��
			CMsgPr_PollMessage(WM_MENU_NEXT);		/*���¸��˵���ͳ�ʼ����Ϣ*/
			break;
		default:									/*Ĭ����Ϣ������Ϣ����*/
			CMsgPr_EndMessage();
			break;
	}
}
//��ֵ�˵���Ϣ����
void CValueMenu_Proc(void)
{
	unsigned char amsg;
	//��Ϣ����
	switch(amsg=CMsgPr_GetMessage())
	{
		case WM_INIT:			/*��ʼ����Ϣ*/
			//����
			if (CMenu_GetMenuItemPtr()->attri|MENU_VALUTYPE_U16)	/*˫�ֽ�*/
				menuDC.cValue = *(unsigned short*)CMenu_GetMenuItemPtr()->m_pdata;
			else							/*���ֽ�*/
				menuDC.cValue = *(unsigned char*)CMenu_GetMenuItemPtr()->m_pdata;
			
			CMenu_SetValueLimit();		//�޷�����
			CMsgPr_ParentMessage(CValueObject_Proc,WM_INIT);/*�����ʼ����Ϣ*/
			break;
		case WM_PAINT:				/*��ʾ��Ϣ*/
			CMenu_DrawMenuValue(SMENU_VALUE_POSIT,VALUE_ZERO_MIN,menuDC.cValue);
			break;
		case WM_KEY_SET:			/*���ü���Ϣ*/
			//�޷�����
			CMenu_SetValueLimit();		//�޷�����
			//������ֵ
			if (CMenu_GetMenuItemPtr()->attri|MENU_VALUTYPE_U16)	/*˫�ֽ�*/
				*(unsigned short*)CMenu_GetMenuItemPtr()->m_pdata = menuDC.cValue;
			else							/*���ֽ�*/
				*(unsigned char*)CMenu_GetMenuItemPtr()->m_pdata = menuDC.cValue;
			CMsgPr_ParentMessage(CValueObject_Proc,WM_KEY_SET);	/*����SET��Ϣ*/
			break;
		case WM_KEY_ADD:	/*�Ӱ�����Ϣ*/
			#if SMENU_01_CURSOR==1
			menuDC.cValue+=CursorIncr[menuDC.cursorIndex];
			#else
			++menuDC.cValue;
			#endif
			CMenu_SetValueLimit();				//�޷�����
			CMsgPr_PollMessage(WM_PAINT);	/*Ͷ����ʾ��Ϣ*/
			break;
		case WM_KEY_SUB:	/*��������Ϣ*/
			#if SMENU_01_CURSOR==1
			menuDC.cValue-=CursorIncr[menuDC.cursorIndex];
			#else
			--menuDC.cValue;
			#endif
			CMenu_SetValueLimit();				//�޷�����
			CMsgPr_PollMessage(WM_PAINT);	/*Ͷ����ʾ��Ϣ*/
			break;
		//left 
		#if SMENU_01_CURSOR==1
		case WM_KEY_LEFT:
			//����λѭ��
			#if SMENU_01_DIG_LOOP==0	//�򿪵���λѭ��
			if (menuDC.cursorIndex<(CURSOR_NUMBER-1)) {++menuDC.cursorIndex;}
			#else						//�رյ���λѭ��
			if (++menuDC.cursorIndex>(CURSOR_NUMBER-1)) {menuDC.cursorIndex = 0;}
			#endif
			break;
		case WM_KEY_RIGHT:
			//����λѭ��
			#if SMENU_01_DIG_LOOP==0	//�򿪵���λѭ��
			if (menuDC.cursorIndex>0){--menuDC.cursorIndex;}
			#else						//�رյ���λѭ��
			if (!menuDC.cursorIndex){menuDC.cursorIndex=(CURSOR_NUMBER-1);}
			else {--menuDC.cursorIndex;}
			#endif
			break;
		//��ʱ����Ϣ
		case WM_TIMER:
			if (MENU_CURSOR_SHOW==CMenu_GetCursorState())
			{
				CMenu_DrawMenuValue(SMENU_VALUE_POSIT,menuDC.cursorIndex+1,menuDC.cValue);
				menuDC.cursorTimer=MENU_CURSOR_HIDE;
			}
			else
			{
				segDisBuff[SMENU_VALUE_POSIT+CURSOR_NUMBER-1-menuDC.cursorIndex]=CURSOR_CHARACTER;
				menuDC.cursorTimer=MENU_CURSOR_SHOW;
			}
			break;
		#endif
		default:
			CMsgPr_ParentMessage(CValueObject_Proc,amsg);/*δ������Ϣ��������*/
			break;
	}
}
#endif
