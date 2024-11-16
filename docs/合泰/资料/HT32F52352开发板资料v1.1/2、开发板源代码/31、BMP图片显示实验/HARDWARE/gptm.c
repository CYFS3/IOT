
#include "HT32.h"
#include "gptm.h"



sPulseCaptureStructure CaptureCHx;




void gptm_cap_Init(void) 
{	
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  TM_CaptureInitTypeDef  CapInit;

  CKCUClock.Bit.PC         = 1;
  CKCUClock.Bit.AFIO      = 1;
  CKCUClock.Bit.GPTM1     = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
  
  AFIO_GPxConfig(GPIO_PC, GPIO_PIN_13, AFIO_FUN_MCTM_GPTM);
  
  CapInit.Channel = TM_CH_3;//key2  ͨ��3
  CapInit.Polarity = TM_CHP_INVERTED;//����Ч
  CapInit.Selection = TM_CHCCS_DIRECT;//ֱ���ź�����
  CapInit.Prescaler = TM_CHPSC_OFF;//ÿ���źŶ�����
  CapInit.Filter = 0x0;//���˲���
  TM_CaptureInit(HT_GPTM1, &CapInit);
  
  TM_IntConfig(HT_GPTM1, TM_INT_CH3CC | TM_INT_UEV, ENABLE);                                                                                      
  TM_Cmd(HT_GPTM1, ENABLE);
  NVIC_EnableIRQ(GPTM1_IRQn);
 
  

}


void CaptureProcess(sPulseCaptureStructure* cap, u16 capture_value, bool isCapBeforeUpdate)
{
  if (cap->ChannelPolarity == TM_CHP_INVERTED)//��ǰ���½��ش���
  {                        
    if (isCapBeforeUpdate)
    {
      cap->OverflowCounter = 1;
    }
    else
    {
      cap->OverflowCounter = 0;
    }
    cap->StartValue = capture_value;//��¼�¿�ʼʱ������ֵ
    cap->ChannelPolarity = TM_CHP_NONINVERTED;//�´θ�Ϊ������
  }
  else  //�����ص��󣬿�ʼ�����½��س����೤ʱ��
  {                                      
    if (isCapBeforeUpdate)
      cap->OverflowCounter--;
	//���������������ʼ����ֵ����ƥ�����ֵ������͵�ƽʱ��
    cap->CapturePulse = (cap->OverflowCounter << 16) + capture_value - cap->StartValue;
	
	//�����ϴ����ݣ���ɱ�־
    if (cap->DataValid)
      cap->DataOverwrite = TRUE;
    else
      cap->DataValid = TRUE;     
	
    cap->ChannelPolarity = TM_CHP_INVERTED;
  }
}


void GPTM1_IRQHandler(void)
{
  bool update_flag = FALSE;

  //��ȡ״̬�ͼ���ֵ
  u32 status = HT_GPTM1->INTSR;
  u32 cnt = HT_GPTM1->CNTR;
	
  //�洢�����־λ
  if ((status & TM_INT_UEV) != (HT_GPTM1->INTSR & TM_INT_UEV))
  {
    status = HT_GPTM1->INTSR;
    cnt = HT_GPTM1->CNTR;     //cnt=
  }
  HT_GPTM1->INTSR = ~status;

  //�������ж�
  if (status & TM_INT_UEV)
  {
    update_flag = TRUE;                                                
    if (CaptureCHx.OverflowCounter != 0xFFFF) CaptureCHx.OverflowCounter++;//��¼�������
  }
  //�����ж�
  if (status & TM_INT_CH3CC)
  {
    u32 cap_value = TM_GetCaptureCompare1(HT_GPTM1);//��ȡƥ�䵽��ֵ
    bool isCapBeforeUpdate = (update_flag && (cap_value > cnt))? TRUE : FALSE;//�жϱ��μ�����û�����
    CaptureProcess(&CaptureCHx, cap_value, isCapBeforeUpdate);
    TM_ChPolarityConfig(HT_GPTM1, TM_CH_3, CaptureCHx.ChannelPolarity);//�����´δ�������  �����ػ����½���
  }
}

