#include "Usart.h"
#include "oled.h"
#include "speak.h"
#include "LED.h"
//是通过时钟频率计算得出的
void OlED_ShowAlert(char *alert);
void LED_On(void);
void LED_Off(void);
void Speak_Alert(void);
void Speak_Off(void);
void Speak_Humidity_Alert(void);
void deay_ms(int ms)
{
	int i;
	while(ms--)
	{
		for(i = 0; i < 8050; i++);
	}
}

//10US延时
void delay_10us()
{
	int i = 100;
	while(i--);
}

void DHT_OutPut()
{
	//结构体变量定义
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//使能GPIOA的时钟线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//配置GPIOA0的输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void DHT_InPut()
{
	//结构体变量定义
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//使能GPIOA的时钟线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//配置GPIOA0的输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入模式，直接将外部输入信号输入到MCU
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

//给DHT11发送开始检测温度
int DHT_Start()
{
label:
	//配置为输出模式，输出一个高电平（开始信号之前默认应该为高电平）
	DHT_OutPut();
	//输出高电平
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	deay_ms(10);
	//拉低输出
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
	//延时至少18ms
	deay_ms(20);
	//拉高输出 20 - 40us
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	delay_10us();
	delay_10us();
	delay_10us();
	//配置为输入模式去读取DHT11的回响信号
	DHT_InPut();
	//如果读取到低电平，那么说DHT11开始给我们发送回响信号
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
	{
		//等待80us低电平的回响信号结束
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0);
		//等待80us高电平的回响信号结束
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1);
		//成功返回1
		return 1;
	}
	else
	{
		//如果没有回响信号，那么重新发送开始信号 
		goto label;
	}
}

int get_value(int *temp, int *humi)
{
	int i,j, num;
	//存放数据的数组
	unsigned char value_buf[5] = {0};
	//开始信号发送成功，那么开始去读取40bit的数据
	if(DHT_Start() == 1)
	{
		for(i = 0; i < 5; i++)
		{
			for(j = 0; j < 8; j++)
			{
				//跳过每一个bit前的50us的低电平
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0);
				//读取到高电平，然后再去判断是0还是1
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1)
				{
					delay_10us();
					delay_10us();
					delay_10us();
					//高电平持续事件超出30us则一定是1
					if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1)
						num = 1;
					else 
						num = 0;
					
					//等待高电平持续时间结束
					while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1);
				}
				//数据的组合 因为是高位先出，所以需要使用左移运算符将数据移动到高位
				value_buf[i]  |=  (num << (7 - j));
			}
		}
		//校验和
		unsigned char check = value_buf[0] + value_buf[1] + value_buf[2] + value_buf[3]; 
		if(check == value_buf[4])
		{
			*temp = value_buf[0];
			*humi = value_buf[2];
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

int main()
{
    int flag = 0;
    int temp, humi;
    Usart1_Init(115200);
    OLED_Init();  //初始化Oled屏幕
    Speak_Init();//初始化蜂鸣器
	  LED_Init();
     while (1) {
        flag = get_value(&temp, &humi);
        if (flag == 0) {
            // ????
            printf("Error\r\n");09999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999987
        } else {
            // ?????OLED
					if(temp>90){
						 Speak_Alert();
						 OLED_ShowAlert("Temperature too high!");
						 LED_On();
						 Delay_50ms(20);
					}
						if(humi>40){
						 OLED_ShowAlert("Humidity too high!");
						 Delay_50ms(20);
					}
					else{
						Speak_Off();
						 LED_Off();
					}
            char temp_str[16];
            sprintf(temp_str, "Temp: %d C", temp);
            OLED_Clear();
            OLED_ShowString(0, 0, (u8 *)temp_str, 16);
           // OLED_Refresh_Gram();
            
            // ????,??500ms
            Delay_50ms(10);

            // ?????OLED
            char humi_str[16];
            sprintf(humi_str, "Humi: %d %%", humi);
            OLED_Clear();
            OLED_ShowString(0, 0, (u8 *)temp_str, 16);
            OLED_ShowString(0, 2, (u8 *)humi_str, 16);
           // OLED_Refresh_Gram();

            // ????,??500ms
            Delay_50ms(10);

            // ??????USART
            printf("Temp = %d C\r\n", temp);
            printf("Humi = %d %%\r\n", humi);
        }
    }
}
