// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEInput.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	declaration for input stuff
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_INPUT_H
#define AE_INPUT_H
// ---------------------------------------------------------------------------
// Defines/Enums

#define AE_INPUT_KEY_NUM   256
#define AE_INPUT_MOUSE_NUM 3

#define AE_MOUSE_LEFT   0
#define AE_MOUSE_RIGHT  1
#define AE_MOUSE_MIDDLE 2
#define AE_KEY_CTRL     17
#define AE_KEY_SHIFT    16
#define AE_KEY_TAB      9
#define AE_KEY_ALT      10

// Gamepad buttons
#define AE_GAMEPAD_DPAD_UP        0x0001
#define AE_GAMEPAD_DPAD_DOWN      0x0002
#define AE_GAMEPAD_DPAD_LEFT      0x0004
#define AE_GAMEPAD_DPAD_RIGHT     0x0008
#define AE_GAMEPAD_START          0x0010
#define AE_GAMEPAD_BACK           0x0020
#define AE_GAMEPAD_LEFT_THUMB     0x0040
#define AE_GAMEPAD_RIGHT_THUMB    0x0080
#define AE_GAMEPAD_LEFT_SHOULDER  0x0100
#define AE_GAMEPAD_RIGHT_SHOULDER 0x0200
#define AE_GAMEPAD_A              0x1000
#define AE_GAMEPAD_B              0x2000
#define AE_GAMEPAD_X              0x4000
#define AE_GAMEPAD_Y              0x8000

// ---------------------------------------------------------------------------
// Structures

struct MousePositionData
{
    AEVec2 position;     // Mouse Position in Centered Coordinates
    AEVec2 prevPosition; // Previous Mouse Position in Centered Coordinates
    AEVec2 movement;     // Mouse Movement during one frame
    bool      inWindow;     // Specifies whether the mouse is contained insisde the window
};

// ---------------------------------------------------------------------------
// class definitions

// ---------------------------------------------------------------------------
// Function prototypes

bool AEInputHandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
bool AEInputInit();
void AEInputReset();
void AEInputUpdate();
void AEInputTerminate();

unsigned char AEInputKeyPressed(unsigned char key);        // Use the character index and the macros (#define) above
unsigned char AEInputKeyTriggered(unsigned char key);      // Use the character index and the macros (#define) above
unsigned char AEInputMousePressed(unsigned char button);   // Use the maccros (#define) above
unsigned char AEInputMouseTriggered(unsigned char button); // Use the maccros (#define) above
float         AEInputGetMouseWheel();

MousePositionData AEGetMouseData(); // Retrieves all mouse related info

// ---------------------------------------------------------------------------
// Gamepad Function prototypes

/*!
\brief	Returns whether the specified gamepad is connected.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
bool AEInputGamepadConnected(unsigned idx);

/*!
\brief	Returns whether the specified button is pressed.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	button Button to check (use one of the maccros defined above).
*/
bool AEInputGamepadButtonPressed(unsigned idx, unsigned button);

/*!
\brief	Returns whether the specified gamepad is connected
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	button Button to check (use one of the maccros defined above).
*/
bool AEInputGamepadButtonTriggered(unsigned idx, unsigned button);

/*!
\brief	Returns the LEFT stick X axis value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadStickLeftRawX(unsigned idx);

/*!
\brief	Returns the LEFT stick Y axis value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadStickLeftRawY(unsigned idx);

/*!
\brief	Returns the RIGHT stick X axis value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadStickRightRawX(unsigned idx);

/*!
\brief	Returns the RIGHT stick Y axis value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadStickRightRawY(unsigned idx);

/*!
\brief	Returns the LEFT stick axis value in vector format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark	The function implements a circular deadzone. You can set the deadzone
		values using the provided functions below.
\remark	The returned vector's magnitude is in the range [0, 1].
*/
AEVec2 AEInputGamepadStickLeft(unsigned idx);

/*!
\brief	Returns the RIGHT stick axis value in vector format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark	The function implements a circular deadzone. You can set the deadzone
		values using the provided functions below.
\remark	The returned vector's magnitude is in the range [0, 1].
*/
AEVec2 AEInputGamepadStickRight(unsigned idx);

/*!
\brief	Returns the LEFT trigger value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark The return value is in the range [0, 255].
\remark You shouldn't need to use this function in general. Use
		AEInputGamepadTriggerLeft instead.
*/
unsigned char AEInputGamepadTriggerLeftRaw(unsigned idx);

/*!
\brief	Returns the RIGHT trigger value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark The return value is in the range [0, 255].
\remark You shouldn't need to use this function in general. Use
		AEInputGamepadTriggerRight instead.
*/
unsigned char AEInputGamepadTriggerRightRaw(unsigned idx);

/*!
\brief	Returns the LEFT trigger value normalized.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark The return value is in the range [0, 1].
\remark This function ignores any value of the trigger below a threshold.
		You can get the threshold value using AEInputGamepadGetTriggerDeadzone.
		You can set the threshold value using AEInputGamepadSetTriggerDeadzone.
*/
float AEInputGamepadTriggerLeft(unsigned idx);

/*!
\brief	Returns the RIGHT trigger value normalized.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark The return value is in the range [0, 1].
\remark This function ignores any value of the trigger below a threshold.
		You can get the threshold value using AEInputGamepadGetTriggerDeadzone.
		You can set the threshold value using AEInputGamepadSetTriggerDeadzone.
*/
float AEInputGamepadTriggerRight(unsigned idx);

/*!
\brief	Sets the vibration of the specified gamepad (both motors).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	valNorm Speed of vibration in the range [0, 1].
*/
void AEInputGamepadSetVibration(unsigned idx, float valNorm);

/*!
\brief	Stops all vibration on specified gamepad.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
void AEInputGamepadStopVibration(unsigned idx);

/*!
\brief	Returns the current vibration on specified gamepad.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark	Because there are 2 motors in the controller, this function returns
		the value of the motor with the greatest vibration speed.
*/
float AEInputGamepadGetVibration(unsigned idx);

/*!
\brief	Sets the vibration of the specified gamepad (LEFT  motor).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	valNorm Speed of vibration in the range [0, 1].
*/
void AEInputGamepadSetVibrationLeft(unsigned idx, float valNorm);

/*!
\brief	Sets the vibration of the specified gamepad (RIGHT motor).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	valNorm Speed of vibration in the range [0, 1].
*/
void AEInputGamepadSetVibrationRight(unsigned idx, float valNorm);

/*!
\brief	Gets the vibration of the specified gamepad (LEFT  motor).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
float AEInputGamepadGetVibrationLeft(unsigned idx);

/*!
\brief	Gets the vibration of the specified gamepad (RIGHT motor).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
float AEInputGamepadGetVibrationRight(unsigned idx);

/*!
\brief	Sets the deadzone value for the LEFT thumbstick.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
void AEInputGamepadSetLeftThumbDeadzone(int deadzone);

/*!
\brief	Sets the deadzone value for the RIGHT thumbstick.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
void AEInputGamepadSetRightThumbDeadzone(int deadzone);

/*!
\brief	Sets the deadzone value for both triggers.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
void AEInpuGamepadtSetTriggerDeadzone(int deadzone);

/*!
\brief	Returns the deadzone value for the LEFT thumbstick.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadGetLeftThumbDeadzone();

/*!
\brief	Returns the deadzone value for the RIGHT thumbstick.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadGetRightThumbDeadzone();

/*!
\brief	Returns the deadzone value for the triggers.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadGetTriggerDeadzone();

/*!
\brief	Resets the deadzone value for the LEFT thumbstick to the default value.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadResettLeftThumbDeadzone();

/*!
\brief	Resets the deadzone value for the RIGHT thumbstick to the default value.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadResetRightThumbDeadzone();

/*!
\brief	Resets the deadzone value for the triggers to the default value.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
int AEInputGamepadResetTriggerDeadzone();

// ---------------------------------------------------------------------------

#endif // AE_INPUT_H
