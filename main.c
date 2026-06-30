#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "scicomm.h"

#pragma DATA_SECTION(vo,"CpuToCla1MsgRAM")
float vo;
#pragma DATA_SECTION(cla_output, "Cla1ToCpuMsgRAM")
float cla_output;
#pragma DATA_SECTION(x0, "Cla1ToCpuMsgRAM")
float x0;
#pragma DATA_SECTION(x1, "Cla1ToCpuMsgRAM")
float x1;
#pragma DATA_SECTION(x2, "Cla1ToCpuMsgRAM")
float x2;
#pragma DATA_SECTION(y0, "Cla1ToCpuMsgRAM")
float y0;
#pragma DATA_SECTION(y1, "Cla1ToCpuMsgRAM")
float y1;
#pragma DATA_SECTION(y2, "Cla1ToCpuMsgRAM")
float y2;


void main(void)
{
    Device_init();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    Board_init();

    EINT;
    ERTM;

    while (1)
    {
        if (SCI_getRxFIFOStatus(SCI0_BASE) > 3)
        {
            protocolReceiveData(SCI0_BASE, &vo, sizeof(float));
            CLA_forceTasks(myCLA0_BASE, CLA_TASKFLAG_1);
        }
    }
}

__interrupt void cla1Isr1(void)
{
    protocolSendData(SCI0_BASE, &cla_output, sizeof(float));
    Interrupt_clearACKGroup(INT_myCLA01_INTERRUPT_ACK_GROUP);
}