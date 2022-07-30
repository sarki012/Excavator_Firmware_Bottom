# Excavator_Firmware_Bottom

Firmware to control two track stepper motors and a stepper motor to spin the cab.

UART 1 is hooked to the radio. The radio sends 'l' for left track, 'r' for right track, and 'o' for orbit, or rotating the cab.

leftThread controls PWM2 for the left track stepper motor.

rightThread controls PWM1 for the right track stepper motor.

spinThread controls PWM3 for spinning the top of the excavator.
