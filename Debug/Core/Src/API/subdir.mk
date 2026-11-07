################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/API/api_handler.c 

OBJS += \
./Core/Src/API/api_handler.o 

C_DEPS += \
./Core/Src/API/api_handler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/API/%.o Core/Src/API/%.su Core/Src/API/%.cyclo: ../Core/Src/API/%.c Core/Src/API/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DFX_INCLUDE_USER_DEFINE_FILE -DLX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../AZURE_RTOS/App -I../FileX/App -I../LevelX/App -I../FileX/Target -I../LevelX/Target -I../NetXDuo/App -I../NetXDuo/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/filex/common/inc/ -I../Middlewares/ST/filex/ports/generic/inc/ -I../Drivers/BSP/Components/lan8742/ -I../Middlewares/ST/netxduo/common/drivers/ethernet/ -I../Middlewares/ST/netxduo/addons/dhcp/ -I../Middlewares/ST/netxduo/common/inc/ -I../Middlewares/ST/netxduo/ports/cortex_m7/gnu/inc/ -I../Middlewares/ST/netxduo/addons/web/ -I../Middlewares/ST/levelx/common/inc/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-API

clean-Core-2f-Src-2f-API:
	-$(RM) ./Core/Src/API/api_handler.cyclo ./Core/Src/API/api_handler.d ./Core/Src/API/api_handler.o ./Core/Src/API/api_handler.su

.PHONY: clean-Core-2f-Src-2f-API

