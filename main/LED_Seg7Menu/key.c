/***********************************************************************************************
�ļ���	key.c
����	2013.3.1
����	��ʫ��	�¶���
�ļ�˵��	����״̬���ļ�
�޶�˵��:

2013.3.1  ��ʫ�� ��ʼ�汾
***********************************************************************************************/
/*ͷ�ļ�--------------------------------------------------------------------------------------*/
#include "key.h"


/*ȫ�ֱ���------------------------------------------------------------------------------------*/

//��������
CKey keyInput;

/*�ӿں���------------------------------------------------------------------------------------*/

//���°���
void CKey_PushKey(void)
{
	//����״̬��
	switch(CKey_GetState())
	{
		/*�����ͷ�״̬���밴������״̬*/
		case KEY_STATE_PULLED:
		case KEY_STATE_PULL:
			CKey_SetState(KEY_STATE_PUSH);		/*���밴������״̬*/
			break;
		/*��������״̬*/
		case KEY_STATE_PUSH:
			CKey_SetState(KEY_STATE_PUSHCHECK);	/*���밴������ȷ��״̬*/
			break;
		/*��������ȷ��״̬*/
		case KEY_STATE_PUSHCHECK:	
			keyInput.m_keyState = KEY_STATE_PUSHED;		/*���밴���Ѱ���״̬*/
			keyInput.m_keyCount = KEY_TIME_M_PUSH;		/*����������ʱ*/
			break;
		/*�����Ѱ���״̬����ʱ��������״̬*/
		case KEY_STATE_PUSHED:
			if (!keyInput.m_keyCount)
				keyInput.m_keyState = KEY_STATE_M_PUSHED;	/*������ʱ��������������״̬*/
			break;
		/*�����ѽ�������״̬*/
		case KEY_STATE_M_PUSHED:			/*����״̬*/
			break;
		default:
			keyInput.m_keyState = KEY_STATE_PULL;
			break;
	}

	/*��ʱ����*/
	if (keyInput.m_keyCount){--keyInput.m_keyCount;}
}

//�ɿ�����
void CKey_PullKey(void)
{
	switch(CKey_GetState())
	{
		/*���������ͷ�״̬*/
		case KEY_STATE_PULL:		/*�����ͷ�*/
			keyInput.m_keyState = KEY_STATE_PULLED;	/*���밴�����ͷ�״̬*/
			break;
		/*�������ͷ�״̬*/
		case KEY_STATE_PULLED:		/*�������ͷ�*/
			break;
		default:
			keyInput.m_keyState = KEY_STATE_PULL;	/*���밴���ͷ�״̬*/
			break;
	}
	/*��ʱ����*/
	if (keyInput.m_keyCount){--keyInput.m_keyCount;}
}

