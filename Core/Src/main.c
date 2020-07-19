#include <stdio.h>
#include "mw_clock.h"
#include "mw_redefines.h"
#include "os_loader.h"

/**
 * @brief  The application entry point.
 * @retval int
 */

int main(void) {
	debug_level = DBG_INFO;
	mw_init();
	mw_internal_clock_init();
	mw_external_clock_init(100);
	os_start();

	while (1) {
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
