
/*******************************************************************
*
* CAUTION: This file is automatically generated by libgen.
* Version: Xilinx EDK 9.2 EDK_Jm.16
* DO NOT EDIT.
*
* Copyright (c) 2005 Xilinx, Inc.  All rights reserved.
*
* Description: Driver parameters
*
*******************************************************************/


/* Definitions for peripheral PLB_BRAM_IF_CNTLR_0 */
#define XPAR_PLB_BRAM_IF_CNTLR_0_BASEADDR 0xFFFF0000
#define XPAR_PLB_BRAM_IF_CNTLR_0_HIGHADDR 0xFFFFFFFF


/******************************************************************/


/* Definitions for peripheral OPB_V20_0 */


/* Definitions for peripheral OPB_EMC_0 */
#define XPAR_OPB_EMC_0_MEM0_BASEADDR 0x20000000
#define XPAR_OPB_EMC_0_MEM0_HIGHADDR 0x200FFFFF
#define XPAR_OPB_EMC_0_MEM1_BASEADDR 0x28000000
#define XPAR_OPB_EMC_0_MEM1_HIGHADDR 0x287FFFFF


/* Definitions for peripheral OPB_AC97_CONTROLLER_REF_0 */
#define XPAR_OPB_AC97_CONTROLLER_REF_0_BASEADDR 0xA6000000
#define XPAR_OPB_AC97_CONTROLLER_REF_0_HIGHADDR 0xA60000FF


/* Definitions for peripheral OPB_EMC_USB_0 */
#define XPAR_OPB_EMC_USB_0_MEM0_BASEADDR 0xA5000000
#define XPAR_OPB_EMC_USB_0_MEM0_HIGHADDR 0xA50000FF


/* Definitions for peripheral PLB_DDR_0 */
#define XPAR_PLB_DDR_0_MEM0_BASEADDR 0x00000000
#define XPAR_PLB_DDR_0_MEM0_HIGHADDR 0x0FFFFFFF


/******************************************************************/

/* Definitions for driver EMAC */
#define XPAR_XEMAC_NUM_INSTANCES 1

/* Definitions for peripheral OPB_ETHERNET_0 */
#define XPAR_OPB_ETHERNET_0_DEVICE_ID 0
#define XPAR_OPB_ETHERNET_0_BASEADDR 0x60000000
#define XPAR_OPB_ETHERNET_0_HIGHADDR 0x60003FFF
#define XPAR_OPB_ETHERNET_0_ERR_COUNT_EXIST 1
#define XPAR_OPB_ETHERNET_0_DMA_PRESENT 1
#define XPAR_OPB_ETHERNET_0_MII_EXIST 1
#define XPAR_OPB_ETHERNET_0_CAM_EXIST 0
#define XPAR_OPB_ETHERNET_0_JUMBO_EXIST 0
#define XPAR_OPB_ETHERNET_0_TX_DRE_TYPE 0
#define XPAR_OPB_ETHERNET_0_RX_DRE_TYPE 0
#define XPAR_OPB_ETHERNET_0_TX_INCLUDE_CSUM 0
#define XPAR_OPB_ETHERNET_0_RX_INCLUDE_CSUM 0


/******************************************************************/


/* Canonical definitions for peripheral OPB_ETHERNET_0 */
#define XPAR_EMAC_0_DEVICE_ID XPAR_OPB_ETHERNET_0_DEVICE_ID
#define XPAR_EMAC_0_BASEADDR 0x60000000
#define XPAR_EMAC_0_HIGHADDR 0x60003FFF
#define XPAR_EMAC_0_ERR_COUNT_EXIST 1
#define XPAR_EMAC_0_DMA_PRESENT 1
#define XPAR_EMAC_0_MII_EXIST 1
#define XPAR_EMAC_0_CAM_EXIST 0
#define XPAR_EMAC_0_JUMBO_EXIST 0
#define XPAR_EMAC_0_TX_DRE_TYPE 0
#define XPAR_EMAC_0_RX_DRE_TYPE 0
#define XPAR_EMAC_0_TX_INCLUDE_CSUM 0
#define XPAR_EMAC_0_RX_INCLUDE_CSUM 0


/******************************************************************/

/* Definitions for driver UARTNS550 */
#define XPAR_XUARTNS550_NUM_INSTANCES 1
#define XPAR_XUARTNS550_CLOCK_HZ 100000000

/* Definitions for peripheral OPB_UART16550_0 */
#define XPAR_OPB_UART16550_0_DEVICE_ID 0
#define XPAR_OPB_UART16550_0_BASEADDR 0xA0000000
#define XPAR_OPB_UART16550_0_HIGHADDR 0xA0001FFF


/******************************************************************/


/* Canonical definitions for peripheral OPB_UART16550_0 */
#define XPAR_UARTNS550_0_CLOCK_FREQ_HZ 100000000
#define XPAR_UARTNS550_0_DEVICE_ID XPAR_OPB_UART16550_0_DEVICE_ID
#define XPAR_UARTNS550_0_BASEADDR 0xA0000000
#define XPAR_UARTNS550_0_HIGHADDR 0xA0001FFF


/******************************************************************/

/* Definitions for driver GPIO */
#define XPAR_XGPIO_NUM_INSTANCES 3

/* Definitions for peripheral OPB_GPIO_0 */
#define XPAR_OPB_GPIO_0_BASEADDR 0x90000000
#define XPAR_OPB_GPIO_0_HIGHADDR 0x900001FF
#define XPAR_OPB_GPIO_0_DEVICE_ID 0
#define XPAR_OPB_GPIO_0_INTERRUPT_PRESENT 0
#define XPAR_OPB_GPIO_0_IS_DUAL 1


/* Definitions for peripheral OPB_GPIO_EXP_HDR_0 */
#define XPAR_OPB_GPIO_EXP_HDR_0_BASEADDR 0x90001000
#define XPAR_OPB_GPIO_EXP_HDR_0_HIGHADDR 0x900011FF
#define XPAR_OPB_GPIO_EXP_HDR_0_DEVICE_ID 1
#define XPAR_OPB_GPIO_EXP_HDR_0_INTERRUPT_PRESENT 0
#define XPAR_OPB_GPIO_EXP_HDR_0_IS_DUAL 1


/* Definitions for peripheral OPB_GPIO_CHAR_LCD_0 */
#define XPAR_OPB_GPIO_CHAR_LCD_0_BASEADDR 0x90002000
#define XPAR_OPB_GPIO_CHAR_LCD_0_HIGHADDR 0x900021FF
#define XPAR_OPB_GPIO_CHAR_LCD_0_DEVICE_ID 2
#define XPAR_OPB_GPIO_CHAR_LCD_0_INTERRUPT_PRESENT 0
#define XPAR_OPB_GPIO_CHAR_LCD_0_IS_DUAL 0


/******************************************************************/

#define XPAR_XPS2_NUM_INSTANCES 2
#define XPAR_OPB_PS2_DUAL_REF_0_DEVICE_ID_0 0
#define XPAR_OPB_PS2_DUAL_REF_0_BASEADDR_0 0xA9000000
#define XPAR_OPB_PS2_DUAL_REF_0_HIGHADDR_0 (0xA9000000+0x3F)
#define XPAR_OPB_PS2_DUAL_REF_0_DEVICE_ID_1 1
#define XPAR_OPB_PS2_DUAL_REF_0_BASEADDR_1 (0xA9000000+0x1000)
#define XPAR_OPB_PS2_DUAL_REF_0_HIGHADDR_1 (0xA9000000+0x103F)

/******************************************************************/

/* Definitions for driver IIC */
#define XPAR_XIIC_NUM_INSTANCES 1

/* Definitions for peripheral OPB_IIC_0 */
#define XPAR_OPB_IIC_0_DEVICE_ID 0
#define XPAR_OPB_IIC_0_BASEADDR 0xA8000000
#define XPAR_OPB_IIC_0_HIGHADDR 0xA80001FF
#define XPAR_OPB_IIC_0_TEN_BIT_ADR 0
#define XPAR_OPB_IIC_0_GPO_WIDTH 1


/******************************************************************/


/* Canonical definitions for peripheral OPB_IIC_0 */
#define XPAR_IIC_0_DEVICE_ID XPAR_OPB_IIC_0_DEVICE_ID
#define XPAR_IIC_0_BASEADDR 0xA8000000
#define XPAR_IIC_0_HIGHADDR 0xA80001FF
#define XPAR_IIC_0_TEN_BIT_ADR 0
#define XPAR_IIC_0_GPO_WIDTH 1


/******************************************************************/

#define XPAR_INTC_MAX_NUM_INTR_INPUTS 10
#define XPAR_XINTC_HAS_IPR 1
#define XPAR_XINTC_USE_DCR 0
/* Definitions for driver INTC */
#define XPAR_XINTC_NUM_INSTANCES 1

/* Definitions for peripheral OPB_INTC_0 */
#define XPAR_OPB_INTC_0_DEVICE_ID 0
#define XPAR_OPB_INTC_0_BASEADDR 0xD1000FC0
#define XPAR_OPB_INTC_0_HIGHADDR 0xD1000FDF
#define XPAR_OPB_INTC_0_KIND_OF_INTR 0x00000000


/******************************************************************/

#define XPAR_INTC_SINGLE_BASEADDR 0xD1000FC0
#define XPAR_INTC_SINGLE_HIGHADDR 0xD1000FDF
#define XPAR_INTC_SINGLE_DEVICE_ID XPAR_OPB_INTC_0_DEVICE_ID
#define XPAR_OPB_ETHERNET_0_IP2INTC_IRPT_MASK 0X000001
#define XPAR_OPB_INTC_0_OPB_ETHERNET_0_IP2INTC_IRPT_INTR 0
#define XPAR_SYSTEM_USB_HPI_INT_MASK 0X000002
#define XPAR_OPB_INTC_0_SYSTEM_USB_HPI_INT_INTR 1
#define XPAR_MISC_LOGIC_0_PHY_MII_INT_MASK 0X000004
#define XPAR_OPB_INTC_0_MISC_LOGIC_0_PHY_MII_INT_INTR 2
#define XPAR_OPB_SYSACE_0_SYSACE_IRQ_MASK 0X000008
#define XPAR_OPB_INTC_0_OPB_SYSACE_0_SYSACE_IRQ_INTR 3
#define XPAR_OPB_AC97_CONTROLLER_REF_0_RECORD_INTERRUPT_MASK 0X000010
#define XPAR_OPB_INTC_0_OPB_AC97_CONTROLLER_REF_0_RECORD_INTERRUPT_INTR 4
#define XPAR_OPB_AC97_CONTROLLER_REF_0_PLAYBACK_INTERRUPT_MASK 0X000020
#define XPAR_OPB_INTC_0_OPB_AC97_CONTROLLER_REF_0_PLAYBACK_INTERRUPT_INTR 5
#define XPAR_OPB_IIC_0_IP2INTC_IRPT_MASK 0X000040
#define XPAR_OPB_INTC_0_OPB_IIC_0_IP2INTC_IRPT_INTR 6
#define XPAR_OPB_PS2_DUAL_REF_0_SYS_INTR2_MASK 0X000080
#define XPAR_OPB_INTC_0_OPB_PS2_DUAL_REF_0_SYS_INTR2_INTR 7
#define XPAR_OPB_PS2_DUAL_REF_0_SYS_INTR1_MASK 0X000100
#define XPAR_OPB_INTC_0_OPB_PS2_DUAL_REF_0_SYS_INTR1_INTR 8
#define XPAR_OPB_UART16550_0_IP2INTC_IRPT_MASK 0X000200
#define XPAR_OPB_INTC_0_OPB_UART16550_0_IP2INTC_IRPT_INTR 9

/******************************************************************/


/* Canonical definitions for peripheral OPB_INTC_0 */
#define XPAR_INTC_0_DEVICE_ID XPAR_OPB_INTC_0_DEVICE_ID
#define XPAR_INTC_0_BASEADDR 0xD1000FC0
#define XPAR_INTC_0_HIGHADDR 0xD1000FDF
#define XPAR_INTC_0_KIND_OF_INTR 0

#define XPAR_INTC_0_EMAC_0_VEC_ID XPAR_OPB_INTC_0_OPB_ETHERNET_0_IP2INTC_IRPT_INTR
#define XPAR_INTC_0_SYSACE_0_VEC_ID XPAR_OPB_INTC_0_OPB_SYSACE_0_SYSACE_IRQ_INTR
#define XPAR_INTC_0_IIC_0_VEC_ID XPAR_OPB_INTC_0_OPB_IIC_0_IP2INTC_IRPT_INTR
#define XPAR_INTC_0_PS2_1_SYS_INTR2_VEC_ID XPAR_OPB_INTC_0_OPB_PS2_DUAL_REF_0_SYS_INTR2_INTR
#define XPAR_INTC_0_PS2_0_SYS_INTR1_VEC_ID XPAR_OPB_INTC_0_OPB_PS2_DUAL_REF_0_SYS_INTR1_INTR
#define XPAR_INTC_0_UARTNS550_0_VEC_ID XPAR_OPB_INTC_0_OPB_UART16550_0_IP2INTC_IRPT_INTR

#define XPAR_INTC_0_PS2_1_VEC_ID XPAR_OPB_INTC_0_OPB_PS2_DUAL_REF_0_SYS_INTR2_INTR
#define XPAR_INTC_0_PS2_0_VEC_ID XPAR_OPB_INTC_0_OPB_PS2_DUAL_REF_0_SYS_INTR1_INTR

/******************************************************************/

/* Definitions for driver TFT_REF */
#define XPAR_XTFT_NUM_INSTANCES 1

/* Definitions for peripheral PLB_TFT_CNTLR_REF_0 */
#define XPAR_PLB_TFT_CNTLR_REF_0_DCR_BASEADDR 0xD0000200
#define XPAR_PLB_TFT_CNTLR_REF_0_DCR_HIGHADDR 0xD0000207
#define XPAR_PLB_TFT_CNTLR_REF_0_DEVICE_ID 0


/******************************************************************/

#define XPAR_XSYSACE_MEM_WIDTH 16
/* Definitions for driver SYSACE */
#define XPAR_XSYSACE_NUM_INSTANCES 1

/* Definitions for peripheral OPB_SYSACE_0 */
#define XPAR_OPB_SYSACE_0_DEVICE_ID 0
#define XPAR_OPB_SYSACE_0_BASEADDR 0xCF000000
#define XPAR_OPB_SYSACE_0_HIGHADDR 0xCF0001FF
#define XPAR_OPB_SYSACE_0_MEM_WIDTH 16


/******************************************************************/


/* Canonical definitions for peripheral OPB_SYSACE_0 */
#define XPAR_SYSACE_0_DEVICE_ID XPAR_OPB_SYSACE_0_DEVICE_ID
#define XPAR_SYSACE_0_BASEADDR 0xCF000000
#define XPAR_SYSACE_0_HIGHADDR 0xCF0001FF
#define XPAR_SYSACE_0_MEM_WIDTH 16


/******************************************************************/

#define XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ 300000000

/******************************************************************/

#define XPAR_CPU_ID 0
#define XPAR_PPC405_VIRTEX4_ID 0
#define XPAR_PPC405_VIRTEX4_CORE_CLOCK_FREQ_HZ 300000000
#define XPAR_PPC405_VIRTEX4_IDCR_BASEADDR 0x00000100
#define XPAR_PPC405_VIRTEX4_IDCR_HIGHADDR 0x0000010F
#define XPAR_PPC405_VIRTEX4_DISABLE_OPERAND_FORWARDING 1
#define XPAR_PPC405_VIRTEX4_MMU_ENABLE 1
#define XPAR_PPC405_VIRTEX4_DETERMINISTIC_MULT 0
#define XPAR_PPC405_VIRTEX4_PLBSYNCBYPASS 1
#define XPAR_PPC405_VIRTEX4_APU_CONTROL 0b1101111000000000
#define XPAR_PPC405_VIRTEX4_APU_UDI_1 0b101000011000100110000011
#define XPAR_PPC405_VIRTEX4_APU_UDI_2 0b101000111000100110000011
#define XPAR_PPC405_VIRTEX4_APU_UDI_3 0b101001011000100111000011
#define XPAR_PPC405_VIRTEX4_APU_UDI_4 0b101001111000100111000011
#define XPAR_PPC405_VIRTEX4_APU_UDI_5 0b101010011000110000000011
#define XPAR_PPC405_VIRTEX4_APU_UDI_6 0b101010111000110000000011
#define XPAR_PPC405_VIRTEX4_APU_UDI_7 0b101011011000110001000011
#define XPAR_PPC405_VIRTEX4_APU_UDI_8 0b101011111000110001000011
#define XPAR_PPC405_VIRTEX4_PVR_HIGH 0b0000
#define XPAR_PPC405_VIRTEX4_PVR_LOW 0b0000
#define XPAR_PPC405_VIRTEX4_HW_VER "1.01.a"

/******************************************************************/


/******************************************************************/

/* Cannonical Constant Names */

/******************************************************************/

#undef XPAR_UARTNS550_0_BASEADDR
#define XPAR_UARTNS550_0_BASEADDR (XPAR_OPB_UART16550_0_BASEADDR+0x1000)

/******************************************************************/

#define XPAR_GPIO_0_BASEADDR XPAR_OPB_GPIO_0_BASEADDR
#define XPAR_GPIO_0_HIGHADDR XPAR_OPB_GPIO_0_HIGHADDR
#define XPAR_GPIO_0_IS_DUAL XPAR_OPB_GPIO_0_IS_DUAL
#define XPAR_GPIO_0_DEVICE_ID XPAR_OPB_GPIO_0_DEVICE_ID
#define XPAR_GPIO_1_BASEADDR XPAR_OPB_GPIO_0_BASEADDR
#define XPAR_GPIO_1_HIGHADDR XPAR_OPB_GPIO_0_HIGHADDR
#define XPAR_GPIO_1_IS_DUAL XPAR_OPB_GPIO_0_IS_DUAL
#define XPAR_GPIO_1_DEVICE_ID XPAR_OPB_GPIO_0_DEVICE_ID
#define XPAR_GPIO_2_BASEADDR XPAR_OPB_GPIO_EXP_HDR_0_BASEADDR
#define XPAR_GPIO_2_HIGHADDR XPAR_OPB_GPIO_EXP_HDR_0_HIGHADDR
#define XPAR_GPIO_2_IS_DUAL XPAR_OPB_GPIO_EXP_HDR_0_IS_DUAL
#define XPAR_GPIO_2_DEVICE_ID XPAR_OPB_GPIO_EXP_HDR_0_DEVICE_ID
#define XPAR_GPIO_3_BASEADDR XPAR_OPB_GPIO_EXP_HDR_0_BASEADDR
#define XPAR_GPIO_3_HIGHADDR XPAR_OPB_GPIO_EXP_HDR_0_HIGHADDR
#define XPAR_GPIO_3_IS_DUAL XPAR_OPB_GPIO_EXP_HDR_0_IS_DUAL
#define XPAR_GPIO_3_DEVICE_ID XPAR_OPB_GPIO_EXP_HDR_0_DEVICE_ID
#define XPAR_GPIO_4_BASEADDR XPAR_OPB_GPIO_CHAR_LCD_0_BASEADDR
#define XPAR_GPIO_4_HIGHADDR XPAR_OPB_GPIO_CHAR_LCD_0_HIGHADDR
#define XPAR_GPIO_4_IS_DUAL XPAR_OPB_GPIO_CHAR_LCD_0_IS_DUAL
#define XPAR_GPIO_4_DEVICE_ID XPAR_OPB_GPIO_CHAR_LCD_0_DEVICE_ID

/******************************************************************/

#define XPAR_PS2_0_BASEADDR XPAR_OPB_PS2_DUAL_REF_0_BASEADDR_0
#define XPAR_PS2_0_HIGHADDR XPAR_OPB_PS2_DUAL_REF_0_HIGHADDR_0
#define XPAR_PS2_0_DEVICE_ID XPAR_OPB_PS2_DUAL_REF_0_DEVICE_ID_0
#define XPAR_PS2_1_BASEADDR XPAR_OPB_PS2_DUAL_REF_0_BASEADDR_1
#define XPAR_PS2_1_HIGHADDR XPAR_OPB_PS2_DUAL_REF_0_HIGHADDR_1
#define XPAR_PS2_1_DEVICE_ID XPAR_OPB_PS2_DUAL_REF_0_DEVICE_ID_1

/******************************************************************/

#define XPAR_TFT_0_BASEADDR XPAR_PLB_TFT_CNTLR_REF_0_DCR_BASEADDR

/******************************************************************/

#define XPAR_PLB_CLOCK_FREQ_HZ 100000000
#define XPAR_CORE_CLOCK_FREQ_HZ XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ
#define XPAR_DDR_0_SIZE 0x4000000

/******************************************************************/

#define XPAR_PERSISTENT_0_IIC_0_BASEADDR 1024
#define XPAR_PERSISTENT_0_IIC_0_HIGHADDR 2047
#define XPAR_PERSISTENT_0_IIC_0_EEPROMADDR 0xA0

/******************************************************************/

#define XPAR_PCI_0_CLOCK_FREQ_HZ    0

/******************************************************************/
