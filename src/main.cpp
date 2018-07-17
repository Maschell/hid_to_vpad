#include <wups.h>
#include <wups/config.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <string.h>
#include <controller_patcher/ControllerPatcher.hpp>
#include <utils/logger.h>
#include <vpad/input.h>
#include <nsysnet/socket.h>

WUPS_PLUGIN_NAME("HID to VPAD lite");
WUPS_PLUGIN_DESCRIPTION("Enables HID devices as controllers on your Wii U");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

// We want access to the SDCard!
WUPS_FS_ACCESS()

#define SD_PATH                     "sd:"
#define WIIU_PATH 					"/wiiu"
#define DEFAULT_HID_TO_VPAD_PATH 	SD_PATH WIIU_PATH "/apps/hidtovpad"

void rumbleChanged(bool newValue) {
    DEBUG_FUNCTION_LINE("rumbleChanged %d \n",newValue);
    ControllerPatcher::setRumbleActivated(newValue);
}

void networkClient(bool newValue) {
    DEBUG_FUNCTION_LINE("Trigger network %d\n",newValue);
    ControllerPatcher::setNetworkControllerActivated(newValue);
    if(newValue) {
        ControllerPatcher::startNetworkServer();
    } else {
        ControllerPatcher::stopNetworkServer();
    }
}

WUPS_GET_CONFIG() {
    WUPSConfig* config = new WUPSConfig("HID to VPAD");
    WUPSConfigCategory* catOther = config->addCategory("Other");

    //                    item Type             config id           displayed name              default value  onChangeCallback.
    catOther->addItem(new WUPSConfigItemBoolean("rumble",           "Enable rumble",            ControllerPatcher::isRumbleActivated(), rumbleChanged));
    catOther->addItem(new WUPSConfigItemBoolean("networkclient",    "Enable network client",    true,          networkClient));

    return config;
}


DEINITIALIZE_PLUGIN() {
    //CursorDrawer::destroyInstance();
    ControllerPatcher::DeInit();
	ControllerPatcher::stopNetworkServer();
}

ON_APPLICATION_START(args){
    socket_lib_init();
    log_init();

	DEBUG_FUNCTION_LINE("Initializing the controller data\n");	
	ControllerPatcher::Init(CONTROLLER_PATCHER_PATH);
    ControllerPatcher::disableControllerMapping();
    DEBUG_FUNCTION_LINE("Starting HID to VPAD network server\n");
    ControllerPatcher::startNetworkServer();
    ControllerPatcher::disableWiiUEnergySetting();
}

ON_APP_STATUS_CHANGED(status){
    if(status == WUPS_APP_STATUS_CLOSED){
         //CursorDrawer::destroyInstance();
        DEBUG_FUNCTION_LINE("ON_APPLICATION_ENDING\n");
        ControllerPatcher::destroyConfigHelper();
        DEBUG_FUNCTION_LINE("Calling stopNetworkServer\n");
        ControllerPatcher::stopNetworkServer();
        DEBUG_FUNCTION_LINE("Calling resetCallbackData\n");
        ControllerPatcher::resetCallbackData();
        ControllerPatcher::restoreWiiUEnergySetting();
    }
}

DECL_FUNCTION(int32_t, VPADRead, VPADChan chan, VPADStatus *buffer, uint32_t buffer_size, int32_t *error) {
    int32_t result = real_VPADRead(chan, buffer, buffer_size, error);
    //A keyboard only sends data when the state changes. We force it to call the sampling callback on each frame!
    ControllerPatcher::sampleKeyboardData();
	
    bool do_callback = (result > 0 && (buffer[0].hold & VPAD_BUTTON_TV));
    ControllerPatcher::handleCallbackData(do_callback);

    if(ControllerPatcher::areControllersConnected() && buffer_size > 0){
        ControllerPatcher::setRumble(UController_Type_Gamepad,!!VPADBASEGetMotorOnRemainingCount(VPAD_CHAN_0));

        if(ControllerPatcher::setControllerDataFromHID(buffer) == CONTROLLER_PATCHER_ERROR_NONE){

            if(buffer[0].hold & VPAD_BUTTON_HOME){
                //You can open the home menu this way, but not close it. Need a proper way to close it using the same button...
                //OSSendAppSwitchRequest(5,0,0); //Open the home menu!
            }

            if(error != NULL){
                *error = 0;
            }
            result = 1; // We want the WiiU to ignore everything else.
        }
    }

    if(ControllerPatcher::isButtonRemappingDone()){
        ControllerPatcher::buttonRemapping(buffer,result);
        //ControllerPatcher::printVPADButtons(buffer); //Leads to random crashes on app transitions.
    }

    return result;
}

WUPS_MUST_REPLACE(VPADRead,		WUPS_LOADER_LIBRARY_VPAD,      VPADRead);
