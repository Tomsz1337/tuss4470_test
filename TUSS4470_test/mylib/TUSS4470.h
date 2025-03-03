#ifndef TUSS4470_H
#define TUSS4470_H
#include "stdint.h"
typedef struct SPI_FrameParams
{
    uint8_t ucCpha;
	uint8_t ucCpol;
	uint8_t ucClsbf;
	uint32_t BAUD_RATE;
}SPI_FrameParams;

struct TUSS4470_settings
{
	uint8_t BPF_CONFIG_1;/*
	7	BPF_FC_TRIM_FRC R/W 0x0 Override factor settings for Bandpass filter trim and control via BPF_FC_TRIM register. Valid only when BPF_BYPASS = 0
			0x0 = Factory trim
			0x1 = Override Factory trim
	6 	BPF_BYPASS R/W 0x0 Select between Bandpass filter or high pass filter
			0x0 = BPF Enabled
			0x1 = HPF Enabled (BPF Bypass)
	5:0 BPF_HPF_FREQ R/W 0x0 
			If BPF_BYPASS = 0: Band pass filter center frequency. See "Bandpass filter center frequency configuration" table
			If BPF_BYPASS = 1: High pass filter corner frequency
				0x00 - 0x0F - 200kHz
				0x10 - 0x1F - 400kHz
				0x20 - 0x2F - 50kHz
				0x30 - 0x3F - 100kHz
	*/
	uint8_t BPF_CONFIG_2;/*
	7:6 RESERVED R 0x0 Reserved
	5:4 BPF_Q_SEL R/W 0x0 Bandpass filter Q factor. Valid only when BPF_BYPASS = 0
			0x0 = 4
			0x1 = 5
			0x2 = 2
			0x3 = 3
	3:0 BPF_FC_TRIM R/W 0x0 Offset BPF_HPF_FREQ when BPF_FC_TRIM_FRC = 1: BPF_HPF_FREQ = BPF_HPF_FREQ + BPF_FC_TRIM See "Bandpass filter center frequency range extension" table.
	*/
	uint8_t DEV_CTRL_1;/*
	7	LOGAMP_FRC R/W 0x0 Override for factory settings for LOGAMP_SLOPE_ADJ and LOGAMP_INT_ADJ
	6:4 LOGAMP_SLOPE_ADJ R/W 0x0 Slope or gain adjustment at the final output on VOUT pin. Slope adjustment depends on the setting of VOUT_SCALE_SEL.
			0x0 = 3.0× VOUT_SCALE_SEL+4.56×VOUT_SCALE_SEL V/V
			0x1 = 3.1× VOUT_SCALE_SEL+4.71×VOUT_SCALE_SEL V/V
			0x2 = 3.2× VOUT_SCALE_SEL+4.86×VOUT_SCALE_SEL V/V
			0x3 = 3.3× VOUT_SCALE_SEL+5.01×VOUT_SCALE_SEL V/V
			0x4 = 2.6× VOUT_SCALE_SEL+3.94×VOUT_SCALE_SEL V/V
			0x5 = 2.7× VOUT_SCALE_SEL+ 4.10×VOUT_SCALE_SEL V/V
			0x6 = 2.8× VOUT_SCALE_SEL+4.25×VOUT_SCALE_SEL V/V
			0x7 = 2.9× VOUT_SCALE_SEL+4.4×VOUT_SCALE_SEL V/V
	3:0 LOGAMP_INT_ADJ R/W 0x0 Logamp Intercept adjustment. See "Logamp intercept adjustment" table in specification for values.
	*/
	uint8_t DEV_CTRL_2;/*
	7	LOGAMP_DIS_FIRST R/W 0x0 Disable first logamp stage to reduce quiescent current
	6 	LOGAMP_DIS_LAST R/W 0x0 Disable last logamp stage quiescent current
	3 	RESERVED R 0x0 Reserved
	2 	VOUT_SCALE_SEL R/W 0x0 Select VOUT scaling
			0x0 = Select Vout gain to map output to 3.3 V
			0x1 = Select Vout gain to map output to 5.0 V
	1:0 LNA_GAIN R/W 0x0 Adjust LNA Gain in V/V
			0x0 = 15 V/V
			0x1 = 10 V/V
			0x2 = 20 V/V
			0x3 = 12.5 V/V
	*/
	uint8_t DEV_CTRL_3;/*
	4:2 DRV_PLS_FLT_DT R/W 0x0 Driver Pulse Fault Deglitch Time.
			In IO_MODE = 0 or IO_MODE = 1, DRV_PULSE_FLT will be set if
			start of burst is triggered and IO2 pin has not toggled for greater than
			deglitch Time.
			In IO_MODE = 2, DRV_PULSE_FLT will be set if start of burst is
			triggered and if IO1 or IO2 do not toggle a period longer than the
			deglitch time except when both pins are high.
			0x0 = 64 µs
			0x1 = 48 µs
			0x2 = 32 µs
			0x3 = 24 µs
			0x4 = 16 µs
			0x5 = 8 µs
			0x6 = 4 µs
			0x7 = Check Disabled
	1:0 IO_MODE R/W 0x0 Configuration for low voltage IO pins.
			0x0 = IOMODE 0
			0x1 = IOMODE 1
			0x2 = IOMODE 2
			0x3 = IOMODE 3
	*/
	uint8_t VDRV_CTRL;/*
	7	RESERVED R 0x0 Reserved
	6	DIS_VDRV_REG_LSTN R/W 0x0 Automatically disable VDRV charging in listen mode every time after burst mode is exited given VDRV_TRIGGER =0x0.
			0x0 = Do not automatically disable VDRV charging
			0x1 = Automatically disable VDRV charging
	5 	VDRV_HI_Z R/W 0x1 Turn off current source between VPWR and VRDV and disable VDRV regulation.
			0x0 = VDRV not Hi-Z
			0x1 = VDRV in Hi-Z mode
	4 	VDRV_CURRENT_LEVEL R/W 0x0 Pull up current at VDRV pin
			0x0 = 10 mA
			0x1 = 20 mA
	3:0 VDRV_VOLTAGE_LEVEL R/W 0x0 Regulated Voltage at VDRV pin Value is calculated as : VDRV = VDRV_VOLTAGE_LEVEL + 5 [V]
	*/
	uint8_t ECHO_INT_CONFIG;/*
	7:5 RESERVED R 0x0 Reserved
	4 	ECHO_INT_CMP_EN R/W 0x0 Enable echo interrupt comparator output
	3:0 ECHO_INT_THR_SEL R/W 0x7 Threshold level to issue interrupt on OUT4 pin. Applied to Low pass filter output.
			If VOUT_SCALE_SEL=0x0 : Threshold = 0.04 x ECHO_INT_THR_SEL + 0.4 [V]
			If VOUT_SCALE_SEL=0x1 : Threshold = 0.06 x ECHO_INT_THR_SEL + 0.6 [V]
	*/
	uint8_t ZC_CONFIG;/*
	7 	ZC_CMP_EN R/W 0x0 Enable Zero Cross Comparator for Frequency detection
	6 	ZC_EN_ECHO_INT R/W 0x0 When set, provides ZC information only when object is detected
	5 	ZC_CMP_IN_SEL R/W 0x0 Zero Comparator Input Select
			0x0 = INP - VCM
			0x1 = INP - INN
	4:3 ZC_CMP_STG_SEL R/W 0x2 Zero Cross Comparator Stage Select
	2:0 ZC_CMP_HYST R/W 0x4 Zero Cross Comparator Hysteresis Selection
			0x0 = 30  mV
			0x1 = 80  mV
			0x2 = 130 mV
			0x3 = 180 mV
			0x4 = 230 mV
			0x5 = 280 mV
			0x6 = 330 mV
			0x7 = 380 mV
	*/
	uint8_t BURST_PULSE;/*
	7 	HALF_BRG_MODE R/W 0x0 Use output driver in half-bridge mode. When enabled, drive both high-side FET together and low-side FETs together.
			0x0 = Disable half-bridge mode
			0x1 = Enable half bridge mode
	6 	PRE_DRIVER_MODE R/W 0x0 Pre-driver mode to drive external FETs
			0x0 = Disable pre-driver mode
			0x1 = Enable pre-driver mode
	5:0 BURST_PULSE R/W 0x0 Number of burst pulses. REG_VALUE=0x00 enables continuous burst mode
	*/
	uint8_t TOF_CONFIG;/*
	7 	SLEEP_MODE_EN R/W 0x0 For entering or exiting sleep mode
			0x0 = Wake up or exit Sleep Mode
			0x1 = Enter sleep mode
	6 	STDBY_MODE_EN R/W 0x0 For entering or exiting standby mode
			0x0 = Exit Standby Mode
			0x1 = Enter Standby mode
	5:2 RESERVED R 0x0 Reserved
	1 	VDRV_TRIGGER R/W 0x0 Control charging of VDRV pin when DIS_VDRV_REG_LSTN = 1. This has no effect when VDRV_HI_Z=0x1.
			0x0 = Disable IVDRV
			0x1 = Enable IVDRV
	0 	CMD_TRIGGER R/W 0x0 For IO_MODE=0x0, control enabling of burst mode. Ignored for other IO_MODE values.
			0x0 = Disable burst mode
			0x1 = Enable burst mode
	*/
};

void SPI_config(struct SPI_FrameParams *sParams);
void TUSS4470_write(uint8_t addr, uint8_t data, uint8_t *pucTXbuff);
void TUSS4470_read(uint8_t addr, uint8_t *pucTXbuff, uint8_t *pucRXbuff);
void TUSS4470_settings(struct TUSS4470_settings *sSettings, uint8_t *pucTXbuff);
void TUSS4470_status(uint8_t *pucTXbuff, uint8_t *pucRXbuff);

#endif