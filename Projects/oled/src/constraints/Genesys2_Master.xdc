
# Clock Signal
set_property -dict { PACKAGE_PIN AD11  IOSTANDARD LVDS     } [get_ports { clk_n }]; #IO_L12N_T1_MRCC_33 Sch=sysclk_n
set_property -dict { PACKAGE_PIN AD12  IOSTANDARD LVDS     } [get_ports { clk_p }]; #IO_L12P_T1_MRCC_33 Sch=sysclk_p

# Buttons
set_property -dict { PACKAGE_PIN E18   IOSTANDARD LVCMOS12 } [get_ports { btnC }]; #IO_25_17 Sch=btnc
set_property -dict { PACKAGE_PIN M19   IOSTANDARD LVCMOS12 } [get_ports { btnD }]; #IO_0_15 Sch=btnd
set_property -dict { PACKAGE_PIN B19   IOSTANDARD LVCMOS12 } [get_ports { btnU }]; #IO_L24N_T3_17 Sch=btnu
set_property -dict { PACKAGE_PIN R19   IOSTANDARD LVCMOS33 } [get_ports { rstn }]; #IO_0_14 Sch=cpu_resetn

## LEDs
set_property -dict { PACKAGE_PIN T28   IOSTANDARD LVCMOS33 } [get_ports { led[0] }]; #IO_L11N_T1_SRCC_14 Sch=led[0]
set_property -dict { PACKAGE_PIN V19   IOSTANDARD LVCMOS33 } [get_ports { led[1] }]; #IO_L19P_T3_A10_D26_14 Sch=led[1]
set_property -dict { PACKAGE_PIN U30   IOSTANDARD LVCMOS33 } [get_ports { led[2] }]; #IO_L15N_T2_DQS_DOUT_CSO_B_14 Sch=led[2]
set_property -dict { PACKAGE_PIN U29   IOSTANDARD LVCMOS33 } [get_ports { led[3] }]; #IO_L15P_T2_DQS_RDWR_B_14 Sch=led[3]
set_property -dict { PACKAGE_PIN V20   IOSTANDARD LVCMOS33 } [get_ports { led[4] }]; #IO_L19N_T3_A09_D25_VREF_14 Sch=led[4]
set_property -dict { PACKAGE_PIN V26   IOSTANDARD LVCMOS33 } [get_ports { led[5] }]; #IO_L16P_T2_CSI_B_14 Sch=led[5]
set_property -dict { PACKAGE_PIN W24   IOSTANDARD LVCMOS33 } [get_ports { led[6] }]; #IO_L20N_T3_A07_D23_14 Sch=led[6]
set_property -dict { PACKAGE_PIN W23   IOSTANDARD LVCMOS33 } [get_ports { led[7] }]; #IO_L20P_T3_A08_D24_14 Sch=led[7]

# OLED Display
set_property -dict { PACKAGE_PIN AC17  IOSTANDARD LVCMOS18 } [get_ports { oled_dc }]; #IO_L18N_T2_32 Sch=oled_dc
set_property -dict { PACKAGE_PIN AB17  IOSTANDARD LVCMOS18 } [get_ports { oled_res }]; #IO_L18P_T2_32 Sch=oled_res
set_property -dict { PACKAGE_PIN AF17  IOSTANDARD LVCMOS18 } [get_ports { oled_sclk }]; #IO_L12P_T1_MRCC_32 Sch=oled_sclk
set_property -dict { PACKAGE_PIN Y15   IOSTANDARD LVCMOS18 } [get_ports { oled_sdin }]; #IO_L24N_T3_32 Sch=oled_sdin
set_property -dict { PACKAGE_PIN AB22  IOSTANDARD LVCMOS33 } [get_ports { oled_vbat }]; #IO_L3P_T0_DQS_12 Sch=oled_vbat
set_property -dict { PACKAGE_PIN AG17  IOSTANDARD LVCMOS18 } [get_ports { oled_vdd }]; #IO_L12N_T1_MRCC_32 Sch=oled_vdd
