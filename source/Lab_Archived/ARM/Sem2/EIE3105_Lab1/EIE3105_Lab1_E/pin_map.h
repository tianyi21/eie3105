#define TIM4_CH1_IT1_RCC_GPIO		RCC_APB2Periph_GPIOB
#define TIM4_CH1_IT1_GPIO				GPIOB
#define TIM4_CH1_IT1_PIN				GPIO_Pin_6

#define TIM3_CH1_PWM_RCC_GPIO		RCC_APB2Periph_GPIOA
#define TIM3_CH1_PWM_GPIO				GPIOA
#define TIM3_CH1_PWM_PIN				GPIO_Pin_6

//Funcation prototypes
void TIM4_CH1_IC1_Init(void);
void TIM3_CH1_PWM_Init(void);
void USART2_Init(void);
void USARTSend(char *pucBuffer, unsigned long ulCount);
void PWM_Update(int clk);