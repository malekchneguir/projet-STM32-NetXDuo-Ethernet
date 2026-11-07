################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Sensors/temperature/max31865.c 

OBJS += \
./Core/Src/Sensors/temperature/max31865.o 

C_DEPS += \
./Core/Src/Sensors/temperature/max31865.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Sensors/temperature/%.o Core/Src/Sensors/temperature/%.su Core/Src/Sensors/temperature/%.cyclo: ../Core/Src/Sensors/temperature/%.c Core/Src/Sensors/temperature/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DFX_INCLUDE_USER_DEFINE_FILE -DLX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../AZURE_RTOS/App -I../FileX/App -I../LevelX/App -I../FileX/Target -I../LevelX/Target -I../NetXDuo/App -I../NetXDuo/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/filex/common/inc/ -I../Middlewares/ST/filex/ports/generic/inc/ -I../Drivers/BSP/Components/lan8742/ -I../Middlewares/ST/netxduo/common/drivers/ethernet/ -I../Middlewares/ST/netxduo/addons/dhcp/ -I../Middlewares/ST/netxduo/common/inc/ -I../Middlewares/ST/netxduo/ports/cortex_m7/gnu/inc/ -I../Middlewares/ST/netxduo/addons/web/ -I../Middlewares/ST/levelx/common/inc/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Sensors-2f-temperature

clean-Core-2f-Src-2f-Sensors-2f-temperature:
	-$(RM) ./Core/Src/Sensors/temperature/max31865.cyclo ./Core/Src/Sensors/temperature/max31865.d ./Core/Src/Sensors/temperature/max31865.o ./Core/Src/Sensors/temperature/max31865.su

.PHONY: clean-Core-2f-Src-2f-Sensors-2f-temperature

