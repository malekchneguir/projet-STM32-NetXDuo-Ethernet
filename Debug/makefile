################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include NetXDuo/App/subdir.mk
-include Middlewares/ST/threadx/ports/cortex_m7/gnu/src/subdir.mk
-include Middlewares/ST/threadx/common/src/subdir.mk
-include Middlewares/ST/netxduo/common/src/subdir.mk
-include Middlewares/ST/netxduo/common/drivers/ethernet/lan8742/subdir.mk
-include Middlewares/ST/netxduo/common/drivers/ethernet/subdir.mk
-include Middlewares/ST/netxduo/addons/web/subdir.mk
-include Middlewares/ST/netxduo/addons/dhcp/subdir.mk
-include Middlewares/ST/levelx/common/src/subdir.mk
-include Middlewares/ST/levelx/common/drivers/subdir.mk
-include Middlewares/ST/filex/common/src/subdir.mk
-include Middlewares/ST/filex/common/drivers/subdir.mk
-include FileX/App/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/Components/lan8742/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/Sensors/temperature/subdir.mk
-include Core/Src/Sensors/rpm/subdir.mk
-include Core/Src/Sensors/pressure/subdir.mk
-include Core/Src/Sensors/light/subdir.mk
-include Core/Src/Sensors/level/subdir.mk
-include Core/Src/Sensors/gas/subdir.mk
-include Core/Src/Sensors/current/subdir.mk
-include Core/Src/Sensors/subdir.mk
-include Core/Src/API/subdir.mk
-include Core/Src/subdir.mk
-include AZURE_RTOS/App/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := NetXDuoEthernet
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
NetXDuoEthernet.elf \

MAP_FILES += \
NetXDuoEthernet.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
NetXDuoEthernet.list \


# All Target
all: main-build

# Main-build Target
main-build: NetXDuoEthernet.elf secondary-outputs

# Tool invocations
NetXDuoEthernet.elf NetXDuoEthernet.map: $(OBJS) $(USER_OBJS) C:\Users\GIGABYTE\Desktop\nex\ ethernet\NetXDuoEthernet\STM32H753ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "NetXDuoEthernet.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\GIGABYTE\Desktop\nex ethernet\NetXDuoEthernet\STM32H753ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="NetXDuoEthernet.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

NetXDuoEthernet.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "NetXDuoEthernet.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) NetXDuoEthernet.elf NetXDuoEthernet.list NetXDuoEthernet.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
