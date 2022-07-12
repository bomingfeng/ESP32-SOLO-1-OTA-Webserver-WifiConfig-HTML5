#include "LED_Seg7Menu.h"

unsigned short config[10];
unsigned char aKey;

//菜单开始
void CMenu_Start(void)
{

}

//菜单结束
void CMenu_Quit(void)
{
	CMenu_MenuInit(menuDC.theMenu.m_pMenuList);
}

//菜单描述
 const CMenuItem menuList[]=
{
	{{0xd4,0,0x06,0},MENU_VALUTYPE_U16,-9999,9999,&config[0],CValueMenu_Proc},
	{{0xd4,0,0x5b,0},MENU_VALUTYPE_U16,0,100,&config[1],CValueMenu_Proc},
	{{0xd4,0,0x4f,0},MENU_VALUTYPE_U16,0,100,&config[2],CValueMenu_Proc},
	{{0xd4,0,0x66,0},MENU_VALUTYPE_U16,0,100,&config[3],CValueMenu_Proc},
	{{0xd4,0,0x6d,0},MENU_VALUTYPE_U16,0,100,&config[4],CValueMenu_Proc},
	{{0xd4,0,0x7d,0},MENU_VALUTYPE_U16,0,100,&config[5],CValueMenu_Proc},
	{{0xd4,0,0x07,0},MENU_VALUTYPE_U16,0,100,&config[6],CValueMenu_Proc},
	{{0xd4,0,0x7f,0},MENU_VALUTYPE_U16,0,100,&config[7],CValueMenu_Proc},
	{{0xd4,0,0x6f,0},MENU_VALUTYPE_U16,0,100,&config[8],CValueMenu_Proc},
	{{0,0,0,0},0,0,0,0,0}
};

unsigned char keyTimer,cursorTimer;


void LED_Seg7Menu_Task(void * parameter)
{
	CMenu_MenuInit(menuList);
	for (;;)
	{
		//扫描显示
		SegDynamicScan();

		{
			//按键扫描
			/*P2 = 0xff;

			if (!P2_0) {aKey=WM_KEY_ADD;}
			else if (!P2_1){aKey = WM_KEY_SET;}
			else if (!P2_2){aKey = WM_KEY_SUB;}
			else if (!P2_3){aKey = WM_KEY_LEFT;}
			else if (!P2_4){aKey = WM_KEY_RIGHT;}
			else {aKey = WM_KEY_NULL;}*/

			CMenu_KeyEvent(aKey);
		}

		//光标
		CMenu_CursorEvent();
	}
}

