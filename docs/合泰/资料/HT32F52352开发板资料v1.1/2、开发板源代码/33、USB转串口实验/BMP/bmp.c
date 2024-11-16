
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
	u8 *databuf;    		//数据读取存放地址
 	u32 readlen=BMP_DBUF_SIZE;//一次从SD卡读取的字节数长度
	

	
	
	S_Height=height;
	S_Width=width;
	databuf=(u8*)malloc(readlen);
	bin=(FIL *)malloc(sizeof(FIL));	//开辟FIL字节的内存区域 
	if(bin==NULL)								//内存申请失败.
	{		 
		free(databuf);
		return 0xff;				
	} 
	res=f_open(bin,(const TCHAR*)filename,FA_READ);//打开文件	 						  
	if(res==0)//打开成功.
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
		f_close(bin);//关闭文件    
		
	}
	
	free(databuf);	 
	free(bin);
	return res;
}


u8 dispbmp(u8 *filename,u16 x,u16 y,u16 width,u16 height)//尺寸小于240*320的bmp图片解码.
{
	FIL* f_bmp;
    u16 br;
	u16 ImgHeight,ImgWidth;
	u16 S_Height,S_Width;
	u8  color_byte;
	u16 tx,ty,color;	 
	//tx,ty的实际坐标	
	u8 res;
	u16 i,j;
	u8 *databuf;    		//数据读取存放地址
 	u16 readlen=BMP_DBUF_SIZE;//一次从SD卡读取的字节数长度

	u8 *bmpbuf;			  	//数据解码地址
	u8 biCompression=0;		//记录压缩方式
	
	u16 rowcnt;				//一次读取的行数
	u16 rowlen;	  		 	//水平方向字节数  
	u16 rowpix=0;			//水平方向像素数	  
	u8 rowadd;				//每行填充字节数


	BITMAPINFO *pbmp;   	//临时指针		 
	S_Height=height;
	S_Width=width;

	databuf=(u8*)malloc(readlen);		//开辟readlen字节的内存区域
	if(databuf==NULL)return 0xff;		//内存申请失败.
	f_bmp=(FIL *)malloc(sizeof(FIL));	//开辟FIL字节的内存区域 
	if(f_bmp==NULL)								//内存申请失败.
	{		 
		free(databuf);
		return 0xff;				
	} 	 

	res=f_open(f_bmp,(const TCHAR*)filename,FA_READ);//打开文件	 						  
	if(res==0)//打开成功.
	{ 
		f_read(f_bmp,databuf,sizeof(BITMAPINFO),(UINT*)&br);//读出BITMAPINFO信息 
		pbmp=(BITMAPINFO*)databuf;					//得到BMP的头部信息   
		color_byte=pbmp->bmiHeader.biBitCount/8;	//彩色位 16/24/32  
		biCompression=pbmp->bmiHeader.biCompression;//压缩方式
		ImgHeight=pbmp->bmiHeader.biHeight;	//得到图片高度
		ImgWidth=pbmp->bmiHeader.biWidth;  	//得到图片宽度 
		//水平像素必须是4的倍数!!
		if((ImgWidth*color_byte)%4)rowlen=((ImgWidth*color_byte)/4+1)*4;
		else rowlen=ImgWidth*color_byte;
		rowadd=rowlen-ImgWidth*color_byte;	//每行填充字节数
 		//开始解码BMP   
		color=0;//颜色清空	 													 
		tx=0 ;
		ty=ImgHeight-1;
		if(ImgWidth<=S_Width&&ImgHeight<=S_Height)
		{  				
			x+=(S_Width-ImgWidth)/2;	//偏移到正中央
			y+=(S_Height-ImgHeight)/2;	//偏移到正中央
			rowcnt=readlen/rowlen;						//一次读取的行数
			readlen=rowcnt*rowlen;						//一次读取的字节数
			rowpix=ImgWidth;					//水平像素数就是宽度 
			f_lseek(f_bmp,pbmp->bmfHeader.bfOffBits);	//偏移到数据起始位置 	  
			while(1)
			{	     
				res=f_read(f_bmp,databuf,readlen,(UINT *)&br);	//读出readlen个字节
				bmpbuf=databuf;									//数据首地址  
				if(br!=readlen)rowcnt=br/rowlen;				//最后剩下的行数
				if(color_byte==3)  			//24位BMP图片
				{
					for(j=0;j<rowcnt;j++)	//每次读到的行数
					{
						for(i=0;i<rowpix;i++)//写一行像素
						{
							color=(*bmpbuf++)>>3;		   		 	//B
							color+=((u16)(*bmpbuf++)<<3)&0X07E0;	//G
							color+=(((u16)*bmpbuf++)<<8)&0XF800;	//R
 						 	LCD_Fast_DrawPoint(x+tx,y+ty,color);//显示图片	
							tx++;
						}
						bmpbuf+=rowadd;//跳过填充区
						tx=0;
						ty--;
					}
				}else if(color_byte==2)//16位BMP图片
				{
					for(j=0;j<rowcnt;j++)//每次读到的行数
					{
						if(biCompression==0)//RGB:5,5,5
						{
							for(i=0;i<rowpix;i++)
							{
								color=((u16)*bmpbuf&0X1F);			//R
								color+=(((u16)*bmpbuf++)&0XE0)<<1; 	//G
		 						color+=((u16)*bmpbuf++)<<9;  	    //R,G	 
							    LCD_Fast_DrawPoint(x+tx,y+ty,color);//显示图片	
								tx++;
							}
						}else  //RGB 565
						{
							for(i=0;i<rowpix;i++)
							{											 
								color=*bmpbuf++;  			//G,B
		 						color+=((u16)*bmpbuf++)<<8;	//R,G	 
							  	LCD_Fast_DrawPoint(x+tx,y+ty,color);//显示图片	
								tx++;
							}
						}
						bmpbuf+=rowadd;//跳过填充区
						tx=0;
						ty--;
					}	
				}else if(color_byte==4)		//32位BMP图片
				{
					for(j=0;j<rowcnt;j++)	//每次读到的行数
					{
						for(i=0;i<rowpix;i++)
						{
							color=(*bmpbuf++)>>3;		   		 	//B
							color+=((u16)(*bmpbuf++)<<3)&0X07E0;	//G
							color+=(((u16)*bmpbuf++)<<8)&0XF800;	//R
 							LCD_Fast_DrawPoint(x+tx,y+ty,color);//显示图片				   
							tx++;//x轴增加一个像素 	  
						}
						bmpbuf+=rowadd;//跳过填充区
						tx=0;
						ty--;
					}

				}
				if(br!=readlen||res)break;	 
			}	 
		}	
		f_close(f_bmp);//关闭文件      
	}else res=0xff;//图片尺寸错误	
	free(databuf);	 
	free(f_bmp);
	return res;
}            


















