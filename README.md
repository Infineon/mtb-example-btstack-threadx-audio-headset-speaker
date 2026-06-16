# Headset Speaker app

## Overview

This app demonstrates the use of a Bluetooth&reg; Headset via the AIROC&#8482; audio libraries. This app also supports the AAC codec for some Bluetooth&reg; devices.


## Requirements

- [ModusToolbox&trade; software](https://www.infineon.com/modustoolbox) v3.1 or later (tested with v3.1) with ModusToolbox&trade; CYW955513 Early Access Pack for CYW955513EVK-01
- Board support package (BSP) minimum required version for : v0.6.0.
- Programming language: C
- Associated parts: CYW955513EVK-01, [KIT-CYW55310-EVAL](https://www.infineon.com/evaluation-board/KIT-CYW55310-EVAL)

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm&reg; embedded compiler v10.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`

## Supported kits (make variable 'TARGET')
- CYW955513EVK-01 Wi-Fi Bluetooth&reg; prototyping kit (`CYW955513EVK-01`)
- [KIT-CYW55310-EVAL](https://www.infineon.com/evaluation-board/KIT-CYW55310-EVAL) Bluetooth&reg; Kit (`KIT-CYW55310-EVAL`)


## Hardware setup

This example uses the kit’s default configuration. See the respective kit guide to ensure that the kit is configured correctly.

**Note:** The CYW955513EVK-01 Wi-Fi Bluetooth&reg; prototyping kit ships with KitProg3 version 2.30 installed. The ModusToolbox&trade; software requires KitProg3 with latest version 2.50. Before using this code example, make sure that the board is upgraded to KitProg3 2.50. The tool and instructions are available in the [Firmware Loader](https://github.com/Infineon/Firmware-loader) GitHub repository. If you do not upgrade, you will see an error such as "unable to find CMSIS-DAP device" or "KitProg firmware is out of date".

### EVK Board Requirement

- This application is applicable for CYW955513EVK-01 with version >= REV02
- This application is applicable for [KIT-CYW55310-EVAL](https://www.infineon.com/evaluation-board/KIT-CYW55310-EVAL) with version >= REV03

### Codec Board Requirement

- This application use the CYW9BTAUDIO5 as the audio codec shield board

## Software setup

Install the BTSpy.

- [BTSpy](https://github.com/Infineon/btsdk-utils): Bluetooth&reg; Trace through HCI Uart

## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade; software</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox&trade; Application**). This launches the [Project Creator](https://www.infineon.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.infineon.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. (Optional) Change the suggested **New Application Name**.

5. The **Application(s) Root Path** defaults to the Eclipse workspace which is usually the desired location for the application. If you want to store the application in a different location, you can change the *Application(s) Root Path* value. Applications that share libraries should be in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.infineon.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mt_ide_user_guide.pdf*).

</details>

<details><summary><b>In command-line interface (CLI)</b></summary>

ModusToolbox&trade; software provides the Project Creator as both a GUI tool and the command line tool, "project-creator-cli". The CLI tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the "project-creator-cli" tool. On Windows, use the command line "modus-shell" program provided in the ModusToolbox&trade; software installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; software tools. You can access it by typing `modus-shell` in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

The "project-creator-cli" tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the `<id>` field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the `<id>` field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

<br />

The following example clones the "[Hello world](https://github.com/Infineon/mtb-example)" application with the desired name "MyHelloWorld" configured for the *CYW955513EVK-01* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CYW955513EVK-01 --app-id mtb-example-hal-hello-world --user-app-name MyHelloWorld --target-dir "C:/mtb_projects"
   ```

**Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

To work with a different supported kit later, use the [Library Manager](https://www.infineon.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can invoke the Library Manager GUI tool from the terminal using `make modlibs` command or use the Library Manager CLI tool "library-manager-cli" to change the BSP.

The "library-manager-cli" tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--add-bsp-name` | Name of the BSP that should be added to the application | Required
`--set-active-bsp` | Name of the BSP that should be as active BSP for the application | Required
`--add-bsp-version`| Specify the version of the BSP that should be added to the application if you do not wish to use the latest from manifest | Optional
`--add-bsp-location`| Specify the location of the BSP (local/shared) if you prefer to add the BSP in a shared path | Optional

<br />

Following example adds the CYW955513EVK-01 BSP to the already created application and makes it the active BSP for the app:

   ```
   library-manager-cli --project "C:/mtb_projects/MyHelloWorld" --add-bsp-name CYW955513EVK-01 --add-bsp-version "latest-v4.X" --add-bsp-location "local"

   library-manager-cli --project "C:/mtb_projects/MyHelloWorld" --set-active-bsp CYW955513EVK-01
   ```

</details>

<details><summary><b>In third-party IDEs</b></summary>

Use one of the following options:

- **Use the standalone [Project Creator](https://www.infineon.com/ModusToolboxProjectCreator) tool:**

   1. Launch Project Creator from the Windows Start menu or from *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/project-creator.exe*.

   2. In the initial **Choose Board Support Package** screen, select the BSP, and click **Next**.

   3. In the **Select Application** screen, select the appropriate IDE from the **Target IDE** drop-down menu.

   4. Click **Create** and follow the instructions printed in the bottom pane to import or open the exported project in the respective IDE.

<br />

- **Use command-line interface (CLI):**

   1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and then import the libraries using the `make getlibs` command.

   2. Export the application to a supported IDE using the `make <ide>` command.

   3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

### Instructions
To demonstrate the app, follow these steps:

1. Build and download the application to the AIROC&#8482; board.
2. Plug in a wired headset to the "Right-MIC" jack (J10) on the codec shield board
3. Press and hold the Custom button for 3 seconds to start pairing mode of the headset\_speaker, as shown in the following figure:

![alt text](<images/pairing button.png>)

4. Find and pair with the "HSPK" Bluetooth&reg; audio device from your phone.
(Note: If HSPK is presnt in connected device list Forget it first.)
5. Once the process is completed, stream music from your phone to the headset\_speaker app.

### Application Settings
Application specific settings are as below:

- SPEAKER
    - This option allows the device to enable output on an audio shield board to drive the external speaker on an EVK board if it is supported. By default the option is off. 

- AAC\_SUPPORT
    - This option allows the device to enable the AAC codec if the Bluetooth&reg; chip supports. 

### Button Functions
- On CYW955513EVK-01 and [KIT-CYW55310-EVAL](https://www.infineon.com/evaluation-board/KIT-CYW55310-EVAL)(3 buttons)<br/>

| Button event | click        | long press       | hold          |
|--------------|--------------|------------------|---------------|
| Custom       | Play, Pause  | Accept, Reject   | Discoverable  |
| Vol+         | Volume up    | Next Track       | -             |
| Vol-         | Volume down  | Last Track       | -             |

**Note:** For hold event press button for more than 5 seconds. 

### Testing with PTS
1. While testing with PTS for certification test cases, please define ENABLE\_PTS\_TESTING flag in makefile as shown below.
   CY\_APP\_DEFINES += -DENABLE\_PTS\_TESTING

2. For A2DP/SNK/REL/BV/02-I test case, to initiate disconnection from IUT, please long press and hold the Vol- button

3. For A2DP/SNK/CC/BV-03-I and A2DP/SNK/CC/BV-04-I test cases, 16Khz and 32Khz sampling rate will be supported by defining ENABLE\_PTS\_TESTING flag

    
## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For more details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-Eclipse_IDE_for_ModusToolbox_User_Guide_1-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcb86331e8).

**Note:** **(Only while debugging)** On the CM33 CPU, some code in `main()` may execute before the debugger halts at the beginning of `main()`. This means that some code executes twice - once before the debugger stops execution, and again after the debugger resets the program counter to the beginning of `main()`. See [KBA231071](https://community.infineon.com/t) to learn about this and for the workaround.

**Note:** Debugging is of limited value when there is an active Bluetooth&reg; LE connection because as soon as the Bluetooth&reg; LE device stops responding, the connection will get dropped.

## BTSpy

BTSpy is a trace utility that can be used in the AIROC&trade; Bluetooth&reg; platforms to view protocol and generic trace messages from the embedded device. BTSpy is available as part of the ModusToolbox&trade; installation. If not, download and install [BTSpy](https://github.com/Infineon/btsdk-utils).

1. Add the `HCI_TRACE_OVER_TRANSPORT` macro in the Makefile or command-line `CY_APP_DEFINES+=HCI_TRACE_OVER_TRANSPORT`.

2. Call `wiced_hci_trace_enable();`

3. Ensure that retarget-io is not enabled on the same UART port as BTSpy. There is no need to initialize the retarget-io library if the application wants to send both application messages and Bluetooth&reg; protocol traces to the same port through BTSpy.

4. Use compiler directives to either initialize the retarget-io library or BTSpy logs depending on the debug macro setting.

   For example:
   ``` cpp
   #ifdef HCI_TRACE_OVER_TRANSPORT
   #define TRANS_UART_BUFFER_SIZE 1024
   #define TRANSPORT_BUFFER_SIZE 1500
   #define TRANSPORT_BUFFER_COUNT 2
   #define TRANSPORT_UART_BAUD_RATE 3000000
   typedef wiced_bool_t (*classic_audio_rpc_cback_t)(uint16_t opcode, uint8_t *p_data, uint32_t data_len);
   #endif
   ```

### Enable BTSpy logs

1. In the Makefile, set `CY_APP_DEFINES+=HCI_TRACE_OVER_TRANSPORT`.
2. Build and program the board.
3. Open ClientControl.
4. Set the baud rate to 3000000.
5. Deselect flow control.
6. Select the port and click **Open port**.
7. Launch BTSpy.
8. Press and release the reset button on the board to get BTSpy logs.
9. You should see all the application traces and the Bluetooth&reg; HCI messages. These messages help debug the HCI commands issued to the Bluetooth&reg; controller. Application traces indicate the start/stop of advertisements, connection/disconnection, and PHY updates.


## Design and implementation

### Resources and settings

**Table 1. Application resources**

Resource         | Alias/object             | Purpose
:----------------|:-------------------------|:-----------------------------------------------------------
HCI-UART (HAL)   | cy\_retarget\_io\_uart\_obj  | UART HAL object used by retarget-io for the Debug UART port
LHL GPIO 6 (HAL) | PLATFORM\_BUTTON\_CUSTOM   | User custom button
BT GPIO 4 (HAL)  | PLATFORM\_BUTTON\_VOL_UP   | Volume+
BT GPIO 5 (HAL)  | PLATFORM\_BUTTON\_VOL\_DOWN | Volume-
BT GPIO 17 (HAL) | mSPI                     | SPI interface for cs47l35 dsp (SPI CLK)
LHL GPIO 8 (HAL) | mSPI                     | SPI interface for cs47l35 dsp (SPI MOSI)
LHL GPIO 9 (HAL) | mSPI                     | SPI interface for cs47l35 dsp (SPI MISO)
BT GPIO 16 (HAL) | mSPI                     | SPI interface for cs47l35 dsp (SPI CS)
TDM2 SCK         |                          | tdm and i2s interface for cs47l35 dsp 
TDM2 WS          |                          | tdm and i2s interface for cs47l35 dsp 
TDM2 DO          |                          | tdm and i2s interface for cs47l35 dsp 
TDM2 DI          |                          | tdm and i2s interface for cs47l35 dsp 

<br />

## Related resources

Resources            | Links
---------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Device documentation | Contact infineon sales for device documentation
Development kits     | Contact infineon sales for kit samples
Libraries on GitHub  | [mtb-pdl-cat5](https://github.com/Infineon/mtb-pdl-cat5) – CYW55513 peripheral driver library (PDL)  <br /> [mtb-hal-cat5](https://github.com/Infineon/mtb-hal-cat5) – Hardware abstraction layer (HAL) library <br /> [retarget-io](https://github.com/Infineon/retarget-io) – Utility library to retarget STDIO messages to a UART port
Tools                | [Eclipse IDE for ModusToolbox&trade; software](https://www.infineon.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use software and tools enabling rapid development with Infineon MCUs, covering applications from embedded sense and control to wireless and cloud-connected systems using AIROC&trade; Wi-Fi and Bluetooth&reg; connectivity devices.


## Other resources

Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com) to help you select the right device, and quickly and effectively integrate it into your design.


## Document history

Document title: *CE239135* - *Bluetooth&reg; headset-speaker*

 Version | Description of change
 ------- | ---------------------
 0.1.0   | New code example
 1.0.0   | First release version
 1.0.1   | Update library HAL to 1.2.0 and btsdk-audio to 4.9.1
 1.1.0   | Support Added for KIT-CYW55310-EVAL Kit
<br>


© 2026, Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
This software, associated documentation and materials ("Software") is owned by Infineon Technologies AG or one of its affiliates ("Infineon") and is protected by and subject to worldwide patent protection, worldwide copyright laws, and international treaty provisions. Therefore, you may use this Software only as provided in the license agreement accompanying the software package from which you obtained this Software. If no license agreement applies, then any use, reproduction, modification, translation, or compilation of this Software is prohibited without the express written permission of Infineon.
<br>
Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A SPECIFIC USE/PURPOSE OR MERCHANTABILITY. Infineon reserves the right to make changes to the Software without notice. You are responsible for properly designing, programming, and testing the functionality and safety of your intended application of the Software, as well as complying with any legal requirements related to its use. Infineon does not guarantee that the Software will be free from intrusion, data theft or loss, or other breaches (“Security Breaches”), and Infineon shall have no liability arising out of any Security Breaches. Unless otherwise explicitly approved by Infineon, the Software may not be used in any application where a failure of the Product or any consequences of the use thereof can reasonably be expected to result in personal injury.

