################################################################################
# \file Makefile
# \version 1.0
#
# \brief
# Top-level application make file.
#
################################################################################
# \copyright
# Copyright 2018-2024, Cypress Semiconductor Corporation (an Infineon company)
# SPDX-License-Identifier: Apache-2.0
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################


################################################################################
# Basic Configuration
################################################################################

# Type of ModusToolbox Makefile Options include:
#
# COMBINED    -- Top Level Makefile usually for single standalone application
# APPLICATION -- Top Level Makefile usually for multi project application
# PROJECT     -- Project Makefile under Application
#
MTB_TYPE=COMBINED

# Target board/hardware (BSP).
# To change the target, it is recommended to use the Library manager
# ('make library-manager' from command line), which will also update Eclipse IDE launch
# configurations.
TARGET=CYW955513EVK-01

# Name of application (used to derive name of final linked file).
#
# If APPNAME is edited, ensure to update or regenerate launch
# configurations for your IDE.
APPNAME=mtb-example-btstack-threadx-audio-headset-speaker

# Name of toolchain to use. Options include:
#
# GCC_ARM -- GCC provided with ModusToolbox software
# ARM     -- ARM Compiler (must be installed separately)
# IAR     -- IAR Compiler (must be installed separately)
#
# See also: CY_COMPILER_PATH below
TOOLCHAIN=GCC_ARM

# Default build configuration. Options include:
#
# Debug -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
# Custom -- build with custom configuration, set the optimization flag in CFLAGS
#
# If CONFIG is manually edited, ensure to update or regenerate launch configurations
# for your IDE.
CONFIG=Debug

# If set to "true" or "1", display full command-lines when building.
VERBOSE=


################################################################################
# Advanced Configuration
################################################################################

# Enable optional code that is ordinarily disabled by default.
#
# Available components depend on the specific targeted hardware and firmware
# in use. In general, if you have
#
#    COMPONENTS=foo bar
#
# ... then code in directories named COMPONENT_foo and COMPONENT_bar will be
# added to the build
#
COMPONENTS=THREADX a2dp_sink_profile_btstack audiomanager audio_sink_route_config_lib \
           avrc_controller_btstack btsdk-include bt_hs_spk_lib button_manager \
           cyw9bt_audio5 handsfree_profile_btstack

# Like COMPONENTS, but disable optional code that was enabled by default.
DISABLE_COMPONENTS=

# By default the build system automatically looks in the Makefile's directory
# tree for source code and builds it. The SOURCES variable can be used to
# manually add source code to the build process from a location not searched
# by default, or otherwise not found by the build system.
SOURCES=

# Like SOURCES, but for include directories. Value should be paths to
# directories (without a leading -I).
INCLUDES=

# Add additional defines to the build process (without a leading -D).
DEFINES =  

# Select softfp or hardfp floating point. Default is softfp.
VFP_SELECT=

# Additional / custom C compiler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
CFLAGS=

# Additional / custom C++ compiler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
CXXFLAGS=

# Additional / custom assembler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
ASFLAGS=

# Additional / custom linker flags.
LDFLAGS=

# Additional / custom libraries to link in to the application.
LDLIBS=

# Path to the linker script to use (if empty, use the default linker script).
LINKER_SCRIPT=

# Custom pre-build commands to run.
PREBUILD=

# Custom post-build commands to run.
POSTBUILD=


################################################################################
# Paths
################################################################################

# Relative path to the project directory (default is the Makefile's directory).
#
# This controls where automatic source code discovery looks for code.
CY_APP_PATH=

# Relative path to the shared repo location.
#
# All .mtb files have the format, <URI>#<COMMIT>#<LOCATION>. If the <LOCATION> field
# begins with $$ASSET_REPO$$, then the repo is deposited in the path specified by
# the CY_GETLIBS_SHARED_PATH variable. The default location is one directory level
# above the current app directory.
# This is used with CY_GETLIBS_SHARED_NAME variable, which specifies the directory name.
CY_GETLIBS_SHARED_PATH=../

# Directory name of the shared repo location.
#
CY_GETLIBS_SHARED_NAME=mtb_shared

# Absolute path to the compiler's "bin" directory. The variable name depends on the 
# toolchain used for the build. Refer to the ModusToolbox user guide to get the correct
# variable name for the toolchain used in your build.
# 
# The default depends on the selected TOOLCHAIN (GCC_ARM uses the ModusToolbox
# software provided compiler by default).
CY_COMPILER_GCC_ARM_DIR=

# Additional application c compiler flags
AAC_SUPPORT ?= 0
SPEAKER ?= 0
OTA_FW_UPGRADE := 0
AUTO_ELNA_SWITCH ?= 0
AUTO_EPA_SWITCH ?= 0
SUPPORT_MXTDM ?= 1
CODEC_SPI_DIRECT_WRITE_MODE ?= 1
CODEC_SPI_WRITE_CHECK ?= 1

ifeq ($(AAC_SUPPORT), 1)
CY_APP_DEFINES += -DWICED_BT_A2DP_SINK_MAX_NUM_CODECS=2
CY_APP_DEFINES += -DA2DP_SINK_AAC_ENABLED
CY_APP_DEFINES += -DWICED_A2DP_EXT_CODEC=1
else
CY_APP_DEFINES += -DWICED_BT_A2DP_SINK_MAX_NUM_CODECS=1
endif

CY_APP_DEFINES += -DAPP_CFG_ENABLE_BR_AUDIO
CY_APP_DEFINES += -DBT_HS_SPK_CONTROL_BR_EDR_MAX_CONNECTIONS=1
CY_APP_DEFINES += -DWICED_BT_HFP_HF_MAX_CONN=BT_HS_SPK_CONTROL_BR_EDR_MAX_CONNECTIONS
CY_APP_DEFINES += -DWICED_BT_A2DP_SINK_MAX_NUM_CONN=BT_HS_SPK_CONTROL_BR_EDR_MAX_CONNECTIONS
CY_APP_DEFINES += -DMAX_CONNECTED_RCC_DEVICES=BT_HS_SPK_CONTROL_BR_EDR_MAX_CONNECTIONS
CY_APP_DEFINES += -DWICED_BT_HFP_HF_WBS_INCLUDED=TRUE

CY_APP_DEFINES += -DPLATFORM_LED_DISABLED
CY_APP_DEFINES += -DWICED_APP_LE_INCLUDED=TRUE
CY_APP_DEFINES += -DAPP_BUTTON_MAX=3

# Options for Power Consumption Measurement
ifeq ($(CYPRESS_LOWPOWER_MODE), 1)
FASTPAIR_ENABLE = 0
CY_APP_DEFINES += -DLOW_POWER_MEASURE_MODE
endif

ifeq ($(FASTPAIR_LOWPOWER_MODE), 1)
CY_APP_DEFINES += -DLOW_POWER_MEASURE_MODE
endif

ifeq ($(SPEAKER),1)
CY_APP_DEFINES+=-DSPEAKER
endif

ifeq ($(SUPPORT_MXTDM),1)
CY_APP_DEFINES+=-DSUPPORT_MXTDM
endif

ifeq ($(CODEC_SPI_DIRECT_WRITE_MODE),1)
CY_APP_DEFINES+=-DCODEC_SPI_DIRECT_WRITE_MODE
endif
ifeq ($(CODEC_SPI_WRITE_CHECK),1)
CY_APP_DEFINES+=-DCODEC_SPI_WRITE_CHECK_VOLUME
endif
CY_APP_DEFINES+=-DCODEC_SPI_DIRECT_ENABLE   # enable SPI when A2DP/HFP command is received

CY_APP_DEFINES+=-DHCI_TRACE_OVER_TRANSPORT

# Locate ModusToolbox helper tools folders in default installation
# locations for Windows, Linux, and macOS.
CY_WIN_HOME=$(subst \,/,$(USERPROFILE))
CY_TOOLS_PATHS ?= $(wildcard \
    $(CY_WIN_HOME)/ModusToolbox/tools_* \
    $(HOME)/ModusToolbox/tools_* \
    /Applications/ModusToolbox/tools_*)

# If you install ModusToolbox software in a custom location, add the path to its
# "tools_X.Y" folder (where X and Y are the version number of the tools
# folder). Make sure you use forward slashes.
CY_TOOLS_PATHS+=

# Default to the newest installed tools folder, or the users override (if it's
# found).
CY_TOOLS_DIR=$(lastword $(sort $(wildcard $(CY_TOOLS_PATHS))))

ifeq ($(CY_TOOLS_DIR),)
$(error Unable to find any of the available CY_TOOLS_PATHS -- $(CY_TOOLS_PATHS). On Windows, use forward slashes.)
endif

$(info Tools Directory: $(CY_TOOLS_DIR))

include $(CY_TOOLS_DIR)/make/start.mk
