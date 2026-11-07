################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/levelx/common/src/lx_nor_flash_block_reclaim.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_close.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_defragment.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_driver_block_erase.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_driver_read.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_driver_write.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_extended_cache_enable.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_initialize.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_logical_sector_find.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_next_block_to_erase_find.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_open.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_partial_defragment.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_physical_sector_allocate.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_sector_read.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_sector_release.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_sector_write.c \
../Middlewares/ST/levelx/common/src/lx_nor_flash_system_error.c 

OBJS += \
./Middlewares/ST/levelx/common/src/lx_nor_flash_block_reclaim.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_close.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_defragment.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_block_erase.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_read.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_write.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_extended_cache_enable.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_initialize.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_logical_sector_find.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_next_block_to_erase_find.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_open.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_partial_defragment.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_physical_sector_allocate.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_read.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_release.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_write.o \
./Middlewares/ST/levelx/common/src/lx_nor_flash_system_error.o 

C_DEPS += \
./Middlewares/ST/levelx/common/src/lx_nor_flash_block_reclaim.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_close.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_defragment.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_block_erase.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_read.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_write.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_extended_cache_enable.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_initialize.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_logical_sector_find.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_next_block_to_erase_find.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_open.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_partial_defragment.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_physical_sector_allocate.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_read.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_release.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_write.d \
./Middlewares/ST/levelx/common/src/lx_nor_flash_system_error.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/levelx/common/src/%.o Middlewares/ST/levelx/common/src/%.su Middlewares/ST/levelx/common/src/%.cyclo: ../Middlewares/ST/levelx/common/src/%.c Middlewares/ST/levelx/common/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DTX_INCLUDE_USER_DEFINE_FILE -DFX_INCLUDE_USER_DEFINE_FILE -DLX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../AZURE_RTOS/App -I../FileX/App -I../LevelX/App -I../FileX/Target -I../LevelX/Target -I../NetXDuo/App -I../NetXDuo/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/filex/common/inc/ -I../Middlewares/ST/filex/ports/generic/inc/ -I../Drivers/BSP/Components/lan8742/ -I../Middlewares/ST/netxduo/common/drivers/ethernet/ -I../Middlewares/ST/netxduo/addons/dhcp/ -I../Middlewares/ST/netxduo/common/inc/ -I../Middlewares/ST/netxduo/ports/cortex_m7/gnu/inc/ -I../Middlewares/ST/netxduo/addons/web/ -I../Middlewares/ST/levelx/common/inc/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-levelx-2f-common-2f-src

clean-Middlewares-2f-ST-2f-levelx-2f-common-2f-src:
	-$(RM) ./Middlewares/ST/levelx/common/src/lx_nor_flash_block_reclaim.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_block_reclaim.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_block_reclaim.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_block_reclaim.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_close.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_close.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_close.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_close.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_defragment.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_defragment.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_defragment.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_defragment.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_block_erase.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_block_erase.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_block_erase.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_block_erase.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_read.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_read.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_read.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_read.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_write.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_write.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_write.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_driver_write.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_extended_cache_enable.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_extended_cache_enable.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_extended_cache_enable.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_extended_cache_enable.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_initialize.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_initialize.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_initialize.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_initialize.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_logical_sector_find.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_logical_sector_find.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_logical_sector_find.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_logical_sector_find.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_next_block_to_erase_find.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_next_block_to_erase_find.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_next_block_to_erase_find.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_next_block_to_erase_find.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_open.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_open.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_open.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_open.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_partial_defragment.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_partial_defragment.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_partial_defragment.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_partial_defragment.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_physical_sector_allocate.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_physical_sector_allocate.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_physical_sector_allocate.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_physical_sector_allocate.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_read.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_read.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_read.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_read.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_release.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_release.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_release.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_release.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_write.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_write.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_write.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_sector_write.su ./Middlewares/ST/levelx/common/src/lx_nor_flash_system_error.cyclo ./Middlewares/ST/levelx/common/src/lx_nor_flash_system_error.d ./Middlewares/ST/levelx/common/src/lx_nor_flash_system_error.o ./Middlewares/ST/levelx/common/src/lx_nor_flash_system_error.su

.PHONY: clean-Middlewares-2f-ST-2f-levelx-2f-common-2f-src

