/***********************************************************************************************
�ļ���	SegDynamicScan.c
����	2014.4.13
����	��ʫ��	�¶���
�޶�	2014.4.13
�ļ�˵��	LED����ܶ�̬ɨ��֧��
�޶�˵��	��ʼ�汾
2014.04.13	��ʫ��	��ʼ�汾
***********************************************************************************************/
/*ͷ�ļ�--------------------------------------------------------------------------------------*/
#include "Seg7Font.h"
#include "SegDynamicScan.h"

//#include ""


/*ȫ�ֱ���------------------------------------------------------------------------------------*/
//��ʾ������
unsigned char segDisBuff[SEG_DIGIT_NUM];

code const unsigned char SegDigCode[]=
{SEG7_CODE_0,SEG7_CODE_1,SEG7_CODE_2,SEG7_CODE_3,SEG7_CODE_4,
SEG7_CODE_5,SEG7_CODE_6,SEG7_CODE_7,SEG7_CODE_8,SEG7_CODE_9};
/*�ӿں���------------------------------------------------------------------------------------*/

//�û��ӿ�
void SegDynamicScan(void)
{
	static unsigned char scanIndex=0;		

	//����
	SegDyn_Hidden();
	//�������
	SegDyn_OutputData(scanIndex);

	if (++scanIndex>(SEG_DIGIT_NUM-1))
		scanIndex=0;
}