################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/netxduo/addons/web/nx_tcpserver.c \
../Middlewares/ST/netxduo/addons/web/nx_web_http_server.c 

OBJS += \
./Middlewares/ST/netxduo/addons/web/nx_tcpserver.o \
./Middlewares/ST/netxduo/addons/web/nx_web_http_server.o 

C_DEPS += \
./Middlewares/ST/netxduo/addons/web/nx_tcpserver.d \
./Middlewares/ST/netxduo/addons/web/nx_web_http_server.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/netxduo/addons/web/%.o Middlewares/ST/netxduo/addons/web/%.su Middlewares/ST/netxduo/addons/web/%.cyclo: ../Middlewares/ST/netxduo/addons/web/%.c Middlewares/ST/netxduo/addons/web/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DFX_INCLUDE_USER_DEFINE_FILE -DLX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../AZURE_RTOS/App -I../FileX/App -I../LevelX/App -I../FileX/Target -I../LevelX/Target -I../NetXDuo/App -I../NetXDuo/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/filex/common/inc/ -I../Middlewares/ST/filex/ports/generic/inc/ -I../Drivers/BSP/Components/lan8742/ -I../Middlewares/ST/netxduo/common/drivers/ethernet/ -I../Middlewares/ST/netxduo/addons/dhcp/ -I../Middlewares/ST/netxduo/common/inc/ -I../Middlewares/ST/netxduo/ports/cortex_m7/gnu/inc/ -I../Middlewares/ST/netxduo/addons/web/ -I../Middlewares/ST/levelx/common/inc/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-web

clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-web:
	-$(RM) ./Middlewares/ST/netxduo/addons/web/nx_tcpserver.cyclo ./Middlewares/ST/netxduo/addons/web/nx_tcpserver.d ./Middlewares/ST/netxduo/addons/web/nx_tcpserver.o ./Middlewares/ST/netxduo/addons/web/nx_tcpserver.su ./Middlewares/ST/netxduo/addons/web/nx_web_http_server.cyclo ./Middlewares/ST/netxduo/addons/web/nx_web_http_server.d ./Middlewares/ST/netxduo/addons/web/nx_web_http_server.o ./Middlewares/ST/netxduo/addons/web/nx_web_http_server.su

.PHONY: clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-web

