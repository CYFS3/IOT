
#include "bmp.h"
#include "string.h"
#include "ff.h" 
#include "lcd.h"
#include "stdlib.h"	     




u8 dispbinpic(u8 *filename,u16 x,u16 y,u16 width,u16 height)
{

	FIL* bin;
	u16 br;
	u16 ImgHeight,ImgWidth;
	u16 S_Height,S_Width;
	u32 cnt,num,num_mod;
	u8 res;
	u32 i,j;
	u8 *databuf;    		//���ݶ�ȡ��ŵ�ַ
 	u32 readlen=BMP_DBUF_SIZE;//һ�δ�SD����ȡ���ֽ�������
	

	
	
	S_Height=height;
	S_Width=width;
	databuf=(u8*)malloc(readlen);
	bin=(FIL *)malloc(sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	if(bin==NULL)								//�ڴ�����ʧ��.
	{		 
		free(databuf);
		return 0xff;				
	} 
	res=f_open(bin,(const TCHAR*)filename,FA_READ);//���ļ�	 						  
	if(res==0)//�򿪳ɹ�.
	{ 
		f_read(bin,databuf,4,(UINT*)&br);
		ImgWidth=((u16)databuf[0]<<8)+databuf[1];
		ImgHeight=((u16)databuf[2]<<8)+databuf[3];
		cnt=ImgWidth*ImgHeight*2;
		if(ImgWidth<=S_Width&&ImgHeight<=S_Height)
		{  	
			num=cnt/readlen;
			num_mod=cnt%readlen;
			LCD_Set_Window(x,y,width,height);
			LCD->LCD_REG=lcddev.wramcmd;
			for(i=0;i<num;i++)
			{
				res=f_read(bin,databuf,readlen,(UINT *)&br);

				for(j=0;j<readlen;j+=2)
				{
					LCD->LCD_RAM=*(u16*)&databuf[j];
				}
			}
			res=f_read(bin,databuf,num_mod,(UINT *)&br);
			for(j=0;j<num_mod;j+=2)
			{
					LCD->LCD_RAM=*(u16*)&databuf[j];
			}
			LCD_Set_Window(0,0,240,320);
			
		}
		f_close(bin);//�ر��ļ�    
		
	}
	
	free(databuf);	 
	free(bin);
	return res;
}


u8 dispbmp(u8 *filename,u16 x,u16 y,u16 width,u16 height)//�ߴ�С��240*320��bmpͼƬ����.
{
	FIL* f_bmp;
    u16 br;
	u16 ImgHeight,ImgWidth;
	u16 S_Height,S_Width;
	u8  color_byte;
	u16 tx,ty,color;	 
	//tx,ty��ʵ������	
	u8 res;
	u16 i,j;
	u8 *databuf;    		//���ݶ�ȡ��ŵ�ַ
 	u16 readlen=BMP_DBUF_SIZE;//һ�δ�SD����ȡ���ֽ�������

	u8 *bmpbuf;			  	//���ݽ����ַ
	u8 biCompression=0;		//��¼ѹ����ʽ
	
	u16 rowcnt;				//һ�ζ�ȡ������
	u16 rowlen;	  		 	//ˮƽ�����ֽ���  
	u16 rowpix=0;			//ˮƽ����������	  
	u8 rowadd;				//ÿ������ֽ���


	BITMAPINFO *pbmp;   	//��ʱָ��		 
	S_Height=height;
	S_Width=width;

	databuf=(u8*)malloc(readlen);		//����readlen�ֽڵ��ڴ�����
	if(databuf==NULL)return 0xff;		//�ڴ�����ʧ��.
	f_bmp=(FIL *)malloc(sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	if(f_bmp==NULL)								//�ڴ�����ʧ��.
	{		 
		free(databuf);
		return 0xff;				
	} 	 

	res=f_open(f_bmp,(const TCHAR*)filename,FA_READ);//���ļ�	 						  
	if(res==0)//�򿪳ɹ�.
	{ 
		f_read(f_bmp,databuf,sizeof(BITMAPINFO),(UINT*)&br);//����BITMAPINFO��Ϣ 
		pbmp=(BITMAPINFO*)databuf;					//�õ�BMP��ͷ����Ϣ   
		color_byte=pbmp->bmiHeader.biBitCount/8;	//��ɫλ 16/24/32  
		biCompression=pbmp->bmiHeader.biCompression;//ѹ����ʽ
		ImgHeight=pbmp->bmiHeader.biHeight;	//�õ�ͼƬ�߶�
		ImgWidth=pbmp->bmiHeader.biWidth;  	//�õ�ͼƬ��� 
		//ˮƽ���ر�����4�ı���!!
		if((ImgWidth*color_byte)%4)rowlen=((ImgWidth*color_byte)/4+1)*4;
		else rowlen=ImgWidth*color_byte;
		rowadd=rowlen-ImgWidth*color_byte;	//ÿ������ֽ���
 		//��ʼ����BMP   
		color=0;//��ɫ���	 													 
		tx=0 ;
		ty=ImgHeight-1;
		if(ImgWidth<=S_Width&&ImgHeight<=S_Height)
		{  				
			x+=(S_Width-ImgWidth)/2;	//ƫ�Ƶ�������
			y+=(S_Height-ImgHeight)/2;	//ƫ�Ƶ�������
			rowcnt=readlen/rowlen;						//һ�ζ�ȡ������
			readlen=rowcnt*rowlen;						//һ�ζ�ȡ���ֽ���
			rowpix=ImgWidth;					//ˮƽ���������ǿ�� 
			f_lseek(f_bmp,pbmp->bmfHeader.bfOffBits);	//ƫ�Ƶ�������ʼλ�� 	  
			while(1)
			{	     
				res=f_read(f_bmp,databuf,readlen,(UINT *)&br);	//����readlen���ֽ�
				bmpbuf=databuf;									//�����׵�ַ  
				if(br!=readlen)rowcnt=br/rowlen;				//���ʣ�µ�����
				if(color_byte==3)  			//24λBMPͼƬ
				{
					for(j=0;j<rowcnt;j++)	//ÿ�ζ���������
					{
						for(i=0;i<rowpix;i++)//дһ������
						{
							color=(*bmpbuf++)>>3;		   		 	//B
							color+=((u16)(*bmpbuf++)<<3)&0X07E0;	//G
							color+=(((u16)*bmpbuf++)<<8)&0XF800;	//R
 						 	LCD_Fast_DrawPoint(x+tx,y+ty,color);//��ʾͼƬ	
							tx++;
						}
						bmpbuf+=rowadd;//���������
						tx=0;
						ty--;
					}
				}else if(color_byte==2)//16λBMPͼƬ
				{
					for(j=0;j<rowcnt;j++)//ÿ�ζ���������
					{
						if(biCompression==0)//RGB:5,5,5
						{
							for(i=0;i<rowpix;i++)
							{
								color=((u16)*bmpbuf&0X1F);			//R
								color+=(((u16)*bmpbuf++)&0XE0)<<1; 	//G
		 						color+=((u16)*bmpbuf++)<<9;  	    //R,G	 
							    LCD_Fast_DrawPoint(x+tx,y+ty,color);//��ʾͼƬ	
								tx++;
							}
						}else  //RGB 565
						{
							for(i=0;i<rowpix;i++)
							{											 
								color=*bmpbuf++;  			//G,B
		 						color+=((u16)*bmpbuf++)<<8;	//R,G	 
							  	LCD_Fast_DrawPoint(x+tx,y+ty,color);//��ʾͼƬ	
								tx++;
							}
						}
						bmpbuf+=rowadd;//���������
						tx=0;
						ty--;
					}	
				}else if(color_byte==4)		//32λBMPͼƬ
				{
					for(j=0;j<rowcnt;j++)	//ÿ�ζ���������
					{
						for(i=0;i<rowpix;i++)
						{
							color=(*bmpbuf++)>>3;		   		 	//B
							color+=((u16)(*bmpbuf++)<<3)&0X07E0;	//G
							color+=(((u16)*bmpbuf++)<<8)&0XF800;	//R
 							LCD_Fast_DrawPoint(x+tx,y+ty,color);//��ʾͼƬ				   
							tx++;//x������һ������ 	  
						}
						bmpbuf+=rowadd;//���������
						tx=0;
						ty--;
					}

				}
				if(br!=readlen||res)break;	 
			}	 
		}	
		f_close(f_bmp);//�ر��ļ�      
	}else res=0xff;//ͼƬ�ߴ����	
	free(databuf);	 
	free(f_bmp);
	return res;
}            


















