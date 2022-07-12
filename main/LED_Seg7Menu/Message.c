/***********************************************************************************************
�ļ���	Message.c
����	2014.02.16
����	��ʫ��	�¶���
�ļ�˵��	������Ϣ֧��
�޶�˵��:

2014.02.16  ��ʫ�� ��ʼ�汾
2014.03.13	��ʫ��	�޶���Ϣ���ͷ��������ٺ����ݹ����
***********************************************************************************************/

/*ͷ�ļ�--------------------------------------------------------------------------------------*/
#include "Message.h"
//#include ""

/*ȫ�ֱ���------------------------------------------------------------------------------------*/
CMsgDC msgDC;		/*ȫ����Ϣ����*/

/*�ӿں���------------------------------------------------------------------------------------*/

//����Ϣ���л�ȡ��Ϣ ���Ƴ���Ϣ
unsigned char CMsgPr_GetMessage(void)
{
	unsigned char amsg = msgDC.m_msg;
	msgDC.m_msg = WM_NULL;					/*��ȡ��Ϣ���������Ϣ*/
	return amsg;
}
//�������ɷ���Ϣ �ɷ�ֹ�����ݹ����
void CMsgPr_PollMessage(Msg aMsg)
{
	msgDC.m_msgPrce = msgDC.m_theObject;	/*��ǰ��ϢĿ��*/
	//������Ϣ����
	msgDC.m_msg = aMsg;	
}
//��ؼ�ֱ�ӷ�����Ϣ ������������Ϣ���� ��ε��ý����º����ݹ�
void CMsgPr_SendMessage(MsgPrce pmsgPrce, Msg aMsg)
{
	msgDC.m_theObject = pmsgPrce;		/*��Ϣ����*/
	msgDC.m_msgPrce = pmsgPrce;			/*��ǰ��ϢĿ��*/

	//������Ϣ����
	msgDC.m_msg = aMsg;					/*��Ϣ*/
	//ѭ��������Ϣ����
	while (1)
	{
		if ((0==msgDC.m_msgPrce)||		/*��ϢĿ����Ч ������Ϣ��Ч*/
			(WM_NULL==msgDC.m_msg))
			break;
		msgDC.m_msgPrce();	/*������Ϣ����*/
	}
}
//������Ϣ����
void CMsgPr_ParentMessage(MsgPrce pmsgPrce, Msg aMsg)
{
	msgDC.m_msg = aMsg;	
	msgDC.m_msgPrce = pmsgPrce;
}
//������Ϣ����
void CMsgPr_EndMessage(void)
{
	msgDC.m_msgPrce = 0;			/*������Ϣ����*/
	msgDC.m_msg = WM_NULL;
}

