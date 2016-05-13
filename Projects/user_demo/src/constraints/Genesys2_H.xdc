# Compress bitstream and configure for fast Flash programming
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]
set_property CFGBVS VCCO [current_design]
set_property BITSTREAM.CONFIG.SPI_32BIT_ADDR YES [current_design]
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 50 [current_design]
set_property BITSTREAM.CONFIG.SPI_FALL_EDGE YES [current_design]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# Audio

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#IO_L8N_T1_32 Sch=aud_adc_sdata
set_property PACKAGE_PIN AH19 [get_ports AUD_ADC_SDATA]
set_property IOSTANDARD LVCMOS18 [get_ports AUD_ADC_SDATA]

#IO_L10P_T1_32 Sch=aud_adr[0]
set_property PACKAGE_PIN AD19 [get_ports {AUD_ADDR_0[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {AUD_ADDR_0[0]}]

#IO_L8P_T1_32 Sch=aud_adr[1]
set_property PACKAGE_PIN AG19 [get_ports {AUD_ADDR_1[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {AUD_ADDR_1[0]}]

#IO_L11N_T1_SRCC_32 Sch=aud_bclk
set_property PACKAGE_PIN AG18 [get_ports AUD_BCLK]
set_property IOSTANDARD LVCMOS18 [get_ports AUD_BCLK]

#IO_L7P_T1_32 Sch=aud_dac_sdata
set_property PACKAGE_PIN AJ19 [get_ports AUD_DAC_SDATA]
set_property IOSTANDARD LVCMOS18 [get_ports AUD_DAC_SDATA]

#IO_L9P_T1_DQS_32 Sch=aud_lrclk
set_property PACKAGE_PIN AJ18 [get_ports AUD_LRCLK]
set_property IOSTANDARD LVCMOS18 [get_ports AUD_LRCLK]

#IO_L7N_T1_32 Sch=aud_mclk
set_property PACKAGE_PIN AK19 [get_ports AUD_MCLK]
set_property IOSTANDARD LVCMOS18 [get_ports AUD_MCLK]

#IO_L10N_T1_32 Sch=aud_scl
set_property PACKAGE_PIN AE19 [get_ports aud_scl_io]
set_property IOSTANDARD LVCMOS18 [get_ports aud_scl_io]

#IO_L11P_T1_SRCC_32 Sch=aud_sda
set_property PACKAGE_PIN AF18 [get_ports aud_sda_io]
set_property IOSTANDARD LVCMOS18 [get_ports aud_sda_io]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# Reset

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#IO_0_14 Sch=cpu_reset_n
set_property PACKAGE_PIN R19 [get_ports cpu_resetn]
set_property IOSTANDARD LVCMOS33 [get_ports cpu_resetn]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# DDR

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# For DDR constraints please refer to our website

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# Ethernet

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

set_property PACKAGE_PIN AK16 [get_ports eth_intb]
set_property IOSTANDARD LVCMOS18 [get_ports eth_intb]
set_property PULLUP true [get_ports eth_intb]
set_property PACKAGE_PIN AF12 [get_ports mdio_io_mdc]
set_property IOSTANDARD LVCMOS15 [get_ports mdio_io_mdc]
set_property PACKAGE_PIN AG12 [get_ports mdio_io_mdio_io]
set_property IOSTANDARD LVCMOS15 [get_ports mdio_io_mdio_io]
set_property PACKAGE_PIN AH24 [get_ports phy_rst_n]
set_property IOSTANDARD LVCMOS33 [get_ports phy_rst_n]
#set_property -dict { PACKAGE_PIN AK15  IOSTANDARD LVCMOS18 } [get_ports { ETH_PMEB }]; #IO_L1N_T0_32 Sch=eth_pmeb
set_property PACKAGE_PIN AG10 [get_ports rgmii_rxc]
set_property IOSTANDARD LVCMOS15 [get_ports rgmii_rxc]
set_property PACKAGE_PIN AH11 [get_ports rgmii_rx_ctl]
set_property IOSTANDARD LVCMOS15 [get_ports rgmii_rx_ctl]
set_property PACKAGE_PIN AJ14 [get_ports {rgmii_rd[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {rgmii_rd[0]}]
set_property PACKAGE_PIN AH14 [get_ports {rgmii_rd[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {rgmii_rd[1]}]
set_property PACKAGE_PIN AK13 [get_ports {rgmii_rd[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {rgmii_rd[2]}]
set_property PACKAGE_PIN AJ13 [get_ports {rgmii_rd[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {rgmii_rd[3]}]
set_property PACKAGE_PIN AE10 [get_ports rgmii_txc]
set_property IOSTANDARD LVCMOS15 [get_ports rgmii_txc]
set_property PACKAGE_PIN AJ12 [get_ports {rgmii_td[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {rgmii_td[0]}]
set_property PACKAGE_PIN AK11 [get_ports {rgmii_td[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {rgmii_td[1]}]
set_property PACKAGE_PIN AJ11 [get_ports {rgmii_td[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {rgmii_td[2]}]
set_property PACKAGE_PIN AK10 [get_ports {rgmii_td[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {rgmii_td[3]}]
set_property PACKAGE_PIN AK14 [get_ports rgmii_tx_ctl]
set_property IOSTANDARD LVCMOS15 [get_ports rgmii_tx_ctl]


#set_property -dict { PACKAGE_PIN W19   IOSTANDARD LVCMOS33 } [get_ports { FAN_PWM }]; #IO_25_14 Sch=fan_pwm
#set_property -dict { PACKAGE_PIN V21   IOSTANDARD LVCMOS33 } [get_ports { FAN_TACH }]; #IO_L22P_T3_A05_D21_14 Sch=fan_tach
#set_property -dict { PACKAGE_PIN AB30  IOSTANDARD LVCMOS33 } [get_ports { FMC_CLK_DIR }]; #IO_L10N_T1_13 Sch=fmc_clk_dir
#set_property -dict { PACKAGE_PIN AC24  IOSTANDARD LVCMOS33 } [get_ports { fmc_scl_io }]; #IO_L9P_T1_DQS_12 Sch=fmc_scl
#set_property -dict { PACKAGE_PIN AD24  IOSTANDARD LVCMOS33 } [get_ports { fmc_sda_io }]; #IO_L9N_T1_DQS_12 Sch=fmc_sda

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# HDMI Source

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
# TMDS signals
set_property PACKAGE_PIN AB20 [get_ports hdmi_tx_clk_n]
set_property IOSTANDARD TMDS_33 [get_ports hdmi_tx_clk_n]
set_property PACKAGE_PIN AA20 [get_ports hdmi_tx_clk_p]
set_property IOSTANDARD TMDS_33 [get_ports hdmi_tx_clk_p]
set_property PACKAGE_PIN AC21 [get_ports {hdmi_tx_data_n[0]}]
set_property IOSTANDARD TMDS_33 [get_ports {hdmi_tx_data_n[0]}]
set_property PACKAGE_PIN AC20 [get_ports {hdmi_tx_data_p[0]}]
set_property IOSTANDARD TMDS_33 [get_ports {hdmi_tx_data_p[0]}]
set_property PACKAGE_PIN AA23 [get_ports {hdmi_tx_data_n[1]}]
set_property IOSTANDARD TMDS_33 [get_ports {hdmi_tx_data_n[1]}]
set_property PACKAGE_PIN AA22 [get_ports {hdmi_tx_data_p[1]}]
set_property IOSTANDARD TMDS_33 [get_ports {hdmi_tx_data_p[1]}]
set_property PACKAGE_PIN AC25 [get_ports {hdmi_tx_data_n[2]}]
set_property IOSTANDARD TMDS_33 [get_ports {hdmi_tx_data_n[2]}]
set_property PACKAGE_PIN AB24 [get_ports {hdmi_tx_data_p[2]}]
set_property IOSTANDARD TMDS_33 [get_ports {hdmi_tx_data_p[2]}]

# Auxiliary signals
#set_property PACKAGE_PIN AG29 [get_ports hdmi_tx_hpd]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_tx_hpd]
#set_property PACKAGE_PIN AF27 [get_ports hdmi_tx_ddc_scl_io]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_tx_ddc_scl_io]
#set_property PACKAGE_PIN AF26 [get_ports hdmi_tx_ddc_sda_io]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_tx_ddc_sda_io]
#set_property PACKAGE_PIN Y24 [get_ports hdmi_tx_cec_tri_io]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_tx_cec_tri_io]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# HDMI Sink

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
# TMDS signals

#set_property PACKAGE_PIN AF28 [get_ports hdmi_rx_clk_n]
#set_property IOSTANDARD TMDS_33 [get_ports hdmi_rx_clk_n]
#set_property PACKAGE_PIN AE28 [get_ports hdmi_rx_clk_p]
#set_property IOSTANDARD TMDS_33 [get_ports hdmi_rx_clk_p]
#set_property PACKAGE_PIN AK26 [get_ports {hdmi_rx_data_n[0]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_rx_data_n[0]}]
#set_property PACKAGE_PIN AJ26 [get_ports {hdmi_rx_data_p[0]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_rx_data_p[0]}]
#set_property PACKAGE_PIN AG28 [get_ports {hdmi_rx_data_p[1]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_rx_data_n[1]}]
#set_property PACKAGE_PIN AG27 [get_ports {hdmi_rx_data_p[1]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_rx_data_p[1]}]
#set_property PACKAGE_PIN AH27 [get_ports {hdmi_rx_data_p[2]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_rx_data_n[2]}]
#set_property PACKAGE_PIN AH26 [get_ports {hdmi_rx_data_p[2]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_rx_data_p[2]}]

# Auxiliary signals
#set_property PACKAGE_PIN Y21 [get_ports hdmi_rx_cec]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_rx_cec]
set_property PACKAGE_PIN AH29 [get_ports {hdmi_rx_hpa[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_rx_hpa[0]}]
#set_property PACKAGE_PIN AJ28 [get_ports hdmi_rx_ddc_scl_io]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_rx_ddc_scl_io]
#set_property PACKAGE_PIN AJ29 [get_ports hdmi_rx_ddc_sda_io]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_rx_ddc_sda_io]


#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# Pmod

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
# Differentially routed Pmods. LVDS use case is limited by VCCO_14, which is 3.3V
# Can be used with LVCMOS33, if single-ended signalling is desired.
#set_property -dict { PACKAGE_PIN U28   IOSTANDARD LVDS     } [get_ports { JA_N[1] }]; #IO_L13N_T2_MRCC_14 Sch=ja_n[1]
#set_property -dict { PACKAGE_PIN U27   IOSTANDARD LVDS     } [get_ports { JA_P[1] }]; #IO_L13P_T2_MRCC_14 Sch=ja_p[1]
#set_property -dict { PACKAGE_PIN T27   IOSTANDARD LVDS     } [get_ports { JA_N[2] }]; #IO_L12N_T1_MRCC_14 Sch=ja_n[2]
#set_property -dict { PACKAGE_PIN T26   IOSTANDARD LVDS     } [get_ports { JA_P[2] }]; #IO_L12P_T1_MRCC_14 Sch=ja_p[2]
#set_property -dict { PACKAGE_PIN T23   IOSTANDARD LVDS     } [get_ports { JA_N[3] }]; #IO_L5N_T0_D07_14 Sch=ja_n[3]
#set_property -dict { PACKAGE_PIN T22   IOSTANDARD LVDS     } [get_ports { JA_P[3] }]; #IO_L5P_T0_D06_14 Sch=ja_p[3]
#set_property -dict { PACKAGE_PIN T21   IOSTANDARD LVDS     } [get_ports { JA_N[4] }]; #IO_L4N_T0_D05_14 Sch=ja_n[4]
#set_property -dict { PACKAGE_PIN T20   IOSTANDARD LVDS     } [get_ports { JA_P[4] }]; #IO_L4P_T0_D04_14 Sch=ja_p[4]
#set_property -dict { PACKAGE_PIN V30   IOSTANDARD LVDS     } [get_ports { JB_N[1] }]; #IO_L17N_T2_A13_D29_14 Sch=jb_n[1]
#set_property -dict { PACKAGE_PIN V29   IOSTANDARD LVDS     } [get_ports { JB_P[1] }]; #IO_L17P_T2_A14_D30_14 Sch=jb_p[1]
#set_property -dict { PACKAGE_PIN W26   IOSTANDARD LVDS     } [get_ports { JB_N[2] }]; #IO_L18N_T2_A11_D27_14 Sch=jb_n[2]
#set_property -dict { PACKAGE_PIN V25   IOSTANDARD LVDS     } [get_ports { JB_P[2] }]; #IO_L18P_T2_A12_D28_14 Sch=jb_p[2]
#set_property -dict { PACKAGE_PIN U25   IOSTANDARD LVDS     } [get_ports { JB_N[3] }]; #IO_L14N_T2_SRCC_14 Sch=jb_n[3]
#set_property -dict { PACKAGE_PIN T25   IOSTANDARD LVDS     } [get_ports { JB_P[3] }]; #IO_L14P_T2_SRCC_14 Sch=jb_p[3]
#set_property -dict { PACKAGE_PIN U23   IOSTANDARD LVDS     } [get_ports { JB_N[4] }]; #IO_L21N_T3_DQS_A06_D22_14 Sch=jb_n[4]
#set_property -dict { PACKAGE_PIN U22   IOSTANDARD LVDS     } [get_ports { JB_P[4] }]; #IO_L21P_T3_DQS_14 Sch=jb_p[4]

# Differentially routed dual analog/digital Pmod, XADC
# Analog usage is constrained by the XADC primitive
# Digital usage is constrained by the lines below. The set of supported I/O standards depends on VCCO_15, which is VADJ.
#set_property -dict { PACKAGE_PIN J24   IOSTANDARD LVDS     } [get_ports { XADC0R_N }]; #IO_L1N_T0_AD0N_15 Sch=xadc0r_n
#set_property -dict { PACKAGE_PIN J23   IOSTANDARD LVDS     } [get_ports { XADC0R_P }]; #IO_L1P_T0_AD0P_15 Sch=xadc0r_p
#set_property -dict { PACKAGE_PIN K24   IOSTANDARD LVDS     } [get_ports { XADC1R_N }]; #IO_L3N_T0_DQS_AD1N_15 Sch=xadc1r_n
#set_property -dict { PACKAGE_PIN K23   IOSTANDARD LVDS     } [get_ports { XADC1R_P }]; #IO_L3P_T0_DQS_AD1P_15 Sch=xadc1r_p
#set_property -dict { PACKAGE_PIN L23   IOSTANDARD LVDS     } [get_ports { XADC8R_N }]; #IO_L2N_T0_AD8N_15 Sch=xadc8r_n
#set_property -dict { PACKAGE_PIN L22   IOSTANDARD LVDS     } [get_ports { XADC8R_P }]; #IO_L2P_T0_AD8P_15 Sch=xadc8r_p
#set_property -dict { PACKAGE_PIN K21   IOSTANDARD LVDS     } [get_ports { XADC9R_N }]; #IO_L4N_T0_AD9N_15 Sch=xadc9r_n
#set_property -dict { PACKAGE_PIN L21   IOSTANDARD LVDS     } [get_ports { XADC9R_P }]; #IO_L4P_T0_AD9P_15 Sch=xadc9r_p

# General Pmods
#set_property -dict { PACKAGE_PIN AC26  IOSTANDARD LVCMOS33 } [get_ports { JC[1] }]; #IO_L19P_T3_13 Sch=jc[1]
#set_property -dict { PACKAGE_PIN AK28  IOSTANDARD LVCMOS33 } [get_ports { JC[10] }]; #IO_L20N_T3_13 Sch=jc[10]
#set_property -dict { PACKAGE_PIN AJ27  IOSTANDARD LVCMOS33 } [get_ports { JC[2] }]; #IO_L20P_T3_13 Sch=jc[2]
#set_property -dict { PACKAGE_PIN AH30  IOSTANDARD LVCMOS33 } [get_ports { JC[3] }]; #IO_L18N_T2_13 Sch=jc[3]
#set_property -dict { PACKAGE_PIN AK29  IOSTANDARD LVCMOS33 } [get_ports { JC[4] }]; #IO_L15P_T2_DQS_13 Sch=jc[4]
#set_property -dict { PACKAGE_PIN AD26  IOSTANDARD LVCMOS33 } [get_ports { JC[7] }]; #IO_L19N_T3_VREF_13 Sch=jc[7]
#set_property -dict { PACKAGE_PIN AG30  IOSTANDARD LVCMOS33 } [get_ports { JC[8] }]; #IO_L18P_T2_13 Sch=jc[8]
#set_property -dict { PACKAGE_PIN AK30  IOSTANDARD LVCMOS33 } [get_ports { JC[9] }]; #IO_L15N_T2_DQS_13 Sch=jc[9]
#set_property -dict { PACKAGE_PIN V27   IOSTANDARD LVCMOS33 } [get_ports { JD[1] }]; #IO_L16N_T2_A15_D31_14 Sch=jd[1]
#set_property -dict { PACKAGE_PIN W21   IOSTANDARD LVCMOS33 } [get_ports { JD[10] }]; #IO_L24P_T3_A01_D17_14 Sch=jd[10]
#set_property -dict { PACKAGE_PIN Y30   IOSTANDARD LVCMOS33 } [get_ports { JD[2] }]; #IO_L8P_T1_13 Sch=jd[2]
#set_property -dict { PACKAGE_PIN V24   IOSTANDARD LVCMOS33 } [get_ports { JD[3] }]; #IO_L23N_T3_A02_D18_14 Sch=jd[3]
#set_property -dict { PACKAGE_PIN W22   IOSTANDARD LVCMOS33 } [get_ports { JD[4] }]; #IO_L24N_T3_A00_D16_14 Sch=jd[4]
#set_property -dict { PACKAGE_PIN U24   IOSTANDARD LVCMOS33 } [get_ports { JD[7] }]; #IO_L23P_T3_A03_D19_14 Sch=jd[7]
#set_property -dict { PACKAGE_PIN Y26   IOSTANDARD LVCMOS33 } [get_ports { JD[8] }]; #IO_L1P_T0_13 Sch=jd[8]
#set_property -dict { PACKAGE_PIN V22   IOSTANDARD LVCMOS33 } [get_ports { JD[9] }]; #IO_L22N_T3_A04_D20_14 Sch=jd[9]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# LEDs

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#IO_L11N_T1_SRCC_14 Sch=led[0]
set_property PACKAGE_PIN T28 [get_ports {led[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[0]}]

#IO_L19P_T3_A10_D26_14 Sch=led[1]
set_property PACKAGE_PIN V19 [get_ports {led[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[1]}]

#IO_L15N_T2_DQS_DOUT_CSO_B_14 Sch=led[2]
set_property PACKAGE_PIN U30 [get_ports {led[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[2]}]

#IO_L15P_T2_DQS_RDWR_B_14 Sch=led[3]
set_property PACKAGE_PIN U29 [get_ports {led[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[3]}]

#IO_L19N_T3_A09_D25_VREF_14 Sch=led[4]
set_property PACKAGE_PIN V20 [get_ports {led[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[4]}]

#IO_L16P_T2_CSI_B_14 Sch=led[5]
set_property PACKAGE_PIN V26 [get_ports {led[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[5]}]

#IO_L20N_T3_A07_D23_14 Sch=led[6]
set_property PACKAGE_PIN W24 [get_ports {led[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[6]}]

#IO_L20P_T3_A08_D24_14 Sch=led[7]
set_property PACKAGE_PIN W23 [get_ports {led[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[7]}]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# OLED

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#IO_L18N_T2_32 Sch=oled_dc
set_property PACKAGE_PIN AC17 [get_ports {oled_gpio[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {oled_gpio[3]}]

#IO_L18P_T2_32 Sch=oled_res
set_property PACKAGE_PIN AB17 [get_ports {oled_gpio[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {oled_gpio[2]}]

#IO_L12P_T1_MRCC_32 Sch=oled_sclk
set_property PACKAGE_PIN AF17 [get_ports oled_sclk]
set_property IOSTANDARD LVCMOS18 [get_ports oled_sclk]

#IO_L24N_T3_32 Sch=oled_sdin
set_property PACKAGE_PIN Y15 [get_ports oled_sdin]
set_property IOSTANDARD LVCMOS18 [get_ports oled_sdin]

#IO_L3P_T0_DQS_12 Sch=oled_vbat
set_property PACKAGE_PIN AB22 [get_ports {oled_gpio[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {oled_gpio[0]}]

#IO_L12N_T1_MRCC_32 Sch=oled_vdd
set_property PACKAGE_PIN AG17 [get_ports {oled_gpio[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {oled_gpio[1]}]


#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# USB HID PIC - FPGA reserved  

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
##set_property -dict { PACKAGE_PIN U20   IOSTANDARD LVCMOS33 } [get_ports { PIC_SS_B }]; #IO_L6N_T0_D08_VREF_14 Sch=pic_ss_b

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# Digilent Adept DPTI/DSPI API support. PC-FPGA data transfer.  

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
#set_property PACKAGE_PIN AB27 [get_ports DPTI_FTCLK]
#set_property IOSTANDARD LVCMOS33 [get_ports DPTI_FTCLK]
#set_property PACKAGE_PIN AD27 [get_ports {DPTI_PDB[0]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {DPTI_PDB[0]}]
#set_property PACKAGE_PIN W27 [get_ports {DPTI_PDB[1]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {DPTI_PDB[1]}]
#set_property PACKAGE_PIN W28 [get_ports {DPTI_PDB[2]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {DPTI_PDB[2]}]
#set_property PACKAGE_PIN W29 [get_ports {DPTI_PDB[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {DPTI_PDB[3]}]
#set_property PACKAGE_PIN Y29 [get_ports {DPTI_PDB[4]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {DPTI_PDB[4]}]
#set_property PACKAGE_PIN Y28 [get_ports {DPTI_PDB[5]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {DPTI_PDB[5]}]
#set_property PACKAGE_PIN AA28 [get_ports {DPTI_PDB[6]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {DPTI_PDB[6]}]
#set_property PACKAGE_PIN AA26 [get_ports {DPTI_PDB[7]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {DPTI_PDB[7]}]
#set_property PACKAGE_PIN AC30 [get_ports DPTI_OE]
#set_property IOSTANDARD LVCMOS33 [get_ports DPTI_OE]
#set_property PACKAGE_PIN AB25 [get_ports DPTI_RD]
#set_property IOSTANDARD LVCMOS33 [get_ports DPTI_RD]
#set_property PACKAGE_PIN AB29 [get_ports DPTI_RXF]
#set_property IOSTANDARD LVCMOS33 [get_ports DPTI_RXF]
#set_property PACKAGE_PIN AB28 [get_ports DPTI_SIWU]
#set_property IOSTANDARD LVCMOS33 [get_ports DPTI_SIWU]
#set_property PACKAGE_PIN AD29 [get_ports DPTI_JTAGEN]
#set_property IOSTANDARD LVCMOS33 [get_ports DPTI_JTAGEN]
#set_property PACKAGE_PIN AA25 [get_ports DPTI_TXE]
#set_property IOSTANDARD LVCMOS33 [get_ports DPTI_TXE]
#set_property PACKAGE_PIN AC27 [get_ports DPTI_WR]
#set_property IOSTANDARD LVCMOS33 [get_ports DPTI_WR]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# USB HID to PS/2 converter

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

set_property PACKAGE_PIN AD23 [get_ports ps2_clk]
set_property IOSTANDARD LVCMOS33 [get_ports ps2_clk]
set_property PACKAGE_PIN AE20 [get_ports ps2_data]
set_property IOSTANDARD LVCMOS33 [get_ports ps2_data]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# Quad SPI Flash

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#IO_L6P_T0_FCS_B_14 Sch=qspi_csn
set_property PACKAGE_PIN U19 [get_ports {qspi_ss_io[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {qspi_ss_io[0]}]

#IO_L1P_T0_D00_MOSI_14 Sch=qspi_d[0]
set_property PACKAGE_PIN P24 [get_ports qspi_io0_io]
set_property IOSTANDARD LVCMOS33 [get_ports qspi_io0_io]

#IO_L1N_T0_D01_DIN_14 Sch=qspi_d[1]
set_property PACKAGE_PIN R25 [get_ports qspi_io1_io]
set_property IOSTANDARD LVCMOS33 [get_ports qspi_io1_io]

#IO_L2P_T0_D02_14 Sch=qspi_d[2]
set_property PACKAGE_PIN R20 [get_ports qspi_io2_io]
set_property IOSTANDARD LVCMOS33 [get_ports qspi_io2_io]

#IO_L2N_T0_D03_14 Sch=qspi_d[3]
set_property PACKAGE_PIN R21 [get_ports qspi_io3_io]
set_property IOSTANDARD LVCMOS33 [get_ports qspi_io3_io]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# SD

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#set_property -dict { PACKAGE_PIN P28   IOSTANDARD LVCMOS33 } [get_ports { sd_cd_tri_io[0] }]; #IO_L8N_T1_D12_14 Sch=sd_cd
#set_property -dict { PACKAGE_PIN R29   IOSTANDARD LVCMOS33 } [get_ports { sd_cmd_tri_io }]; #IO_L7N_T1_D10_14 Sch=sd_cmd
#set_property -dict { PACKAGE_PIN R26   IOSTANDARD LVCMOS33 } [get_ports { sd_dat0_i }]; #IO_L10N_T1_D15_14 Sch=sd_dat[0]
#set_property -dict { PACKAGE_PIN R30   IOSTANDARD LVCMOS33 } [get_ports { sd_dat1_i }]; #IO_L9P_T1_DQS_14 Sch=sd_dat[1]
#set_property -dict { PACKAGE_PIN P29   IOSTANDARD LVCMOS33 } [get_ports { sd_dat2_i }]; #IO_L7P_T1_D09_14 Sch=sd_dat[2]
#set_property -dict { PACKAGE_PIN T30   IOSTANDARD LVCMOS33 } [get_ports { sd_dat3_i }]; #IO_L9N_T1_DQS_D13_14 Sch=sd_dat[3]
#set_property -dict { PACKAGE_PIN AE24  IOSTANDARD LVCMOS33 } [get_ports { sd_reset[0] }]; #IO_L12N_T1_MRCC_12 Sch=sd_reset
#set_property -dict { PACKAGE_PIN R28   IOSTANDARD LVCMOS33 } [get_ports { sd_sck_o }]; #IO_L11P_T1_SRCC_14 Sch=sd_sclk

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# Sysclk, 200MHz

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
set_property IOSTANDARD LVDS [get_ports sysclk_clk_n]
set_property PACKAGE_PIN AD12 [get_ports sysclk_clk_p]
set_property IOSTANDARD LVDS [get_ports sysclk_clk_p]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# UART (PC-FPGA)

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# TXD, RXD naming uses terminal-centric naming convention
# RXD is reception at the terminal (PC), output from the FPGA 
set_property PACKAGE_PIN Y23 [get_ports uart_rxd_out]
set_property IOSTANDARD LVCMOS33 [get_ports uart_rxd_out]
# TXD is transmission at the terminal (PC), input to the FPGA
set_property PACKAGE_PIN Y20 [get_ports uart_txd_in]
set_property IOSTANDARD LVCMOS33 [get_ports uart_txd_in]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# USB 2.0 ULPI PHY

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
#set_property PACKAGE_PIN AD18 [get_ports usb_otg_clk]
#set_property IOSTANDARD LVCMOS18 [get_ports usb_otg_clk]
#set_property PACKAGE_PIN AE14 [get_ports {usb_otg_data_io[0]}]
#set_property IOSTANDARD LVCMOS18 [get_ports {usb_otg_data_io[0]}]
#set_property PACKAGE_PIN AE15 [get_ports {usb_otg_data_io[1]}]
#set_property IOSTANDARD LVCMOS18 [get_ports {usb_otg_data_io[1]}]
#set_property PACKAGE_PIN AC15 [get_ports {usb_otg_data_io[2]}]
#set_property IOSTANDARD LVCMOS18 [get_ports {usb_otg_data_io[2]}]
#set_property PACKAGE_PIN AC16 [get_ports {usb_otg_data_io[3]}]
#set_property IOSTANDARD LVCMOS18 [get_ports {usb_otg_data_io[3]}]
#set_property PACKAGE_PIN AB15 [get_ports {usb_otg_data_io[4]}]
#set_property IOSTANDARD LVCMOS18 [get_ports {usb_otg_data_io[4]}]
#set_property PACKAGE_PIN AA15 [get_ports {usb_otg_data_io[5]}]
#set_property IOSTANDARD LVCMOS18 [get_ports {usb_otg_data_io[5]}]
#set_property PACKAGE_PIN AD14 [get_ports {usb_otg_data_io[6]}]
#set_property IOSTANDARD LVCMOS18 [get_ports {usb_otg_data_io[6]}]
#set_property PACKAGE_PIN AC14 [get_ports {usb_otg_data_io[7]}]
#set_property IOSTANDARD LVCMOS18 [get_ports {usb_otg_data_io[7]}]
#set_property PACKAGE_PIN Y16 [get_ports usb_otg_dir]
#set_property IOSTANDARD LVCMOS18 [get_ports usb_otg_dir]
#set_property PACKAGE_PIN AA16 [get_ports usb_otg_next]
#set_property IOSTANDARD LVCMOS18 [get_ports usb_otg_next]
#set_property PACKAGE_PIN AB14 [get_ports usb_otg_rst]
#set_property IOSTANDARD LVCMOS18 [get_ports usb_otg_rst]
#set_property PACKAGE_PIN AA17 [get_ports usb_otg_stop]
#set_property IOSTANDARD LVCMOS18 [get_ports usb_otg_stop]
#set_property -dict { PACKAGE_PIN AF16  IOSTANDARD LVCMOS18 } [get_ports { usb_otg_vbusoc }]; #IO_L6N_T0_VREF_32 Sch=usb_otg_vbusoc

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# VGA 16bpp, 5-6-5

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
set_property PACKAGE_PIN AH20 [get_ports {vga_b[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_b[0]}]
set_property PACKAGE_PIN AG20 [get_ports {vga_b[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_b[1]}]
set_property PACKAGE_PIN AF21 [get_ports {vga_b[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_b[2]}]
set_property PACKAGE_PIN AK20 [get_ports {vga_b[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_b[3]}]
set_property PACKAGE_PIN AG22 [get_ports {vga_b[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_b[4]}]
set_property PACKAGE_PIN AJ23 [get_ports {vga_g[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_g[0]}]
set_property PACKAGE_PIN AJ22 [get_ports {vga_g[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_g[1]}]
set_property PACKAGE_PIN AH22 [get_ports {vga_g[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_g[2]}]
set_property PACKAGE_PIN AK21 [get_ports {vga_g[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_g[3]}]
set_property PACKAGE_PIN AJ21 [get_ports {vga_g[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_g[4]}]
set_property PACKAGE_PIN AK23 [get_ports {vga_g[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_g[5]}]
set_property PACKAGE_PIN AK25 [get_ports {vga_r[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_r[0]}]
set_property PACKAGE_PIN AG25 [get_ports {vga_r[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_r[1]}]
set_property PACKAGE_PIN AH25 [get_ports {vga_r[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_r[2]}]
set_property PACKAGE_PIN AK24 [get_ports {vga_r[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_r[3]}]
set_property PACKAGE_PIN AJ24 [get_ports {vga_r[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_r[4]}]
set_property PACKAGE_PIN AF20 [get_ports vga_hs]
set_property IOSTANDARD LVCMOS33 [get_ports vga_hs]
set_property PACKAGE_PIN AG23 [get_ports vga_vs]
set_property IOSTANDARD LVCMOS33 [get_ports vga_vs]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# DisplayPort

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#MGTREFCLK0N_118 Sch=disply_clp_n
set_property PACKAGE_PIN C7 [get_ports dp_lnk_clk_n]
create_clock -period 7.407 -name dp_lnk_clk_p [get_ports dp_lnk_clk_p]

#MGTXTXP0_118 Sch=tx_lane0_p
set_property PACKAGE_PIN D2 [get_ports {dp_tx_lane_p[0]}]

#MGTXTXP1_118 Sch=tx_lane1_p
set_property PACKAGE_PIN C4 [get_ports {dp_tx_lane_p[1]}]

#MGTXTXP2_118 Sch=tx_lane2_p
set_property PACKAGE_PIN B2 [get_ports {dp_tx_lane_p[2]}]

#MGTXTXP3_118 Sch=tx_lane3_p
set_property PACKAGE_PIN A4 [get_ports {dp_tx_lane_p[3]}]

#IO_L14N_T2_SRCC_32 Sch=tx_aux_ch_n
set_property PACKAGE_PIN AD16 [get_ports dp_aux_out_n]
set_property IOSTANDARD LVDS [get_ports dp_aux_out_n]

#IO_L14P_T2_SRCC_32 Sch=tx_aux_ch_p
set_property IOSTANDARD LVDS [get_ports dp_aux_out_p]

#IO_L16N_T2_32 Sch=tx_aux_ch_n
set_property PACKAGE_PIN AB18 [get_ports dp_aux_in_n]
set_property IOSTANDARD LVDS [get_ports dp_aux_in_n]

#IO_L16P_T2_32 Sch=tx_aux_ch_p
set_property IOSTANDARD LVDS [get_ports dp_aux_in_p]

#IO_L10P_T1_12 Sch=tx_hpd
set_property PACKAGE_PIN AD21 [get_ports dp_hpd]
set_property IOSTANDARD LVCMOS33 [get_ports dp_hpd]
set_property PULLDOWN true [get_ports dp_hpd]

#### FMC ports ###
## i2c
#set_property PACKAGE_PIN AC24 [get_ports fmc_iic_scl_io]
#set_property IOSTANDARD LVCMOS33 [get_ports fmc_iic_scl_io]
#set_property PACKAGE_PIN AD24 [get_ports fmc_iic_sda_io]
#set_property IOSTANDARD LVCMOS33 [get_ports fmc_iic_sda_io]
## clocks
#set_property PACKAGE_PIN E20 [get_ports {fmc_clk0_n[0]}]
#set_property PACKAGE_PIN D28 [get_ports {fmc_clk1_n[0]}]
#set_property PACKAGE_PIN K25 [get_ports {fmc_clk2_n[0]}]
#set_property IOSTANDARD BLVDS_25 [get_ports {fmc_clk?_?[0]}]
## FMC presence
#set_property PACKAGE_PIN AA21 [get_ports {fmc_prsnt_tri_io[0]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {fmc_prsnt_tri_io[0]}]
## FMC HA[11:0] ports
#set_property PACKAGE_PIN K29 [get_ports {fmc_ha_00_22_n_tri_io[0]}]
#set_property PACKAGE_PIN K28 [get_ports {fmc_ha_00_22_p_tri_io[0]}]
#set_property PACKAGE_PIN L28 [get_ports {fmc_ha_01_23_n_tri_io[0]}]
#set_property PACKAGE_PIN M28 [get_ports {fmc_ha_01_23_p_tri_io[0]}]
#set_property PACKAGE_PIN P22 [get_ports {fmc_ha_00_22_n_tri_io[1]}]
#set_property PACKAGE_PIN P21 [get_ports {fmc_ha_00_22_p_tri_io[1]}]
#set_property PACKAGE_PIN N26 [get_ports {fmc_ha_01_23_n_tri_io[1]}]
#set_property PACKAGE_PIN N25 [get_ports {fmc_ha_01_23_p_tri_io[1]}]
#set_property PACKAGE_PIN M25 [get_ports {fmc_ha_00_22_n_tri_io[2]}]
#set_property PACKAGE_PIN M24 [get_ports {fmc_ha_00_22_p_tri_io[2]}]
#set_property PACKAGE_PIN H29 [get_ports {fmc_ha_01_23_n_tri_io[2]}]
#set_property PACKAGE_PIN J29 [get_ports {fmc_ha_01_23_p_tri_io[2]}]
#set_property PACKAGE_PIN N30 [get_ports {fmc_ha_00_22_n_tri_io[3]}]
#set_property PACKAGE_PIN N29 [get_ports {fmc_ha_00_22_p_tri_io[3]}]
#set_property PACKAGE_PIN M30 [get_ports {fmc_ha_01_23_n_tri_io[3]}]
#set_property PACKAGE_PIN M29 [get_ports {fmc_ha_01_23_p_tri_io[3]}]
#set_property PACKAGE_PIN J28 [get_ports {fmc_ha_00_22_n_tri_io[4]}]
#set_property PACKAGE_PIN J27 [get_ports {fmc_ha_00_22_p_tri_io[4]}]
#set_property PACKAGE_PIN K30 [get_ports {fmc_ha_01_23_n_tri_io[4]}]
#set_property PACKAGE_PIN L30 [get_ports {fmc_ha_01_23_p_tri_io[4]}]
#set_property PACKAGE_PIN N22 [get_ports {fmc_ha_00_22_n_tri_io[5]}]
#set_property PACKAGE_PIN N21 [get_ports {fmc_ha_00_22_p_tri_io[5]}]
#set_property PACKAGE_PIN N24 [get_ports {fmc_ha_01_23_n_tri_io[5]}]
#set_property PACKAGE_PIN P23 [get_ports {fmc_ha_01_23_p_tri_io[5]}]
#set_property PACKAGE_PIN L27 [get_ports {fmc_ha_00_22_n_tri_io[6]}]
#set_property PACKAGE_PIN L26 [get_ports {fmc_ha_00_22_p_tri_io[6]}]
#set_property PACKAGE_PIN J26 [get_ports {fmc_ha_01_23_n_tri_io[6]}]
#set_property PACKAGE_PIN K26 [get_ports {fmc_ha_01_23_p_tri_io[6]}]
#set_property PACKAGE_PIN M27 [get_ports {fmc_ha_00_22_n_tri_io[7]}]
#set_property PACKAGE_PIN N27 [get_ports {fmc_ha_00_22_p_tri_io[7]}]
#set_property PACKAGE_PIN J22 [get_ports {fmc_ha_01_23_n_tri_io[7]}]
#set_property PACKAGE_PIN J21 [get_ports {fmc_ha_01_23_p_tri_io[7]}]
#set_property PACKAGE_PIN M23 [get_ports {fmc_ha_00_22_n_tri_io[8]}]
#set_property PACKAGE_PIN M22 [get_ports {fmc_ha_00_22_p_tri_io[8]}]
#set_property PACKAGE_PIN B25 [get_ports {fmc_ha_01_23_n_tri_io[8]}]
#set_property PACKAGE_PIN C25 [get_ports {fmc_ha_01_23_p_tri_io[8]}]
#set_property PACKAGE_PIN D19 [get_ports {fmc_ha_00_22_n_tri_io[9]}]
#set_property PACKAGE_PIN E19 [get_ports {fmc_ha_00_22_p_tri_io[9]}]
#set_property PACKAGE_PIN F30 [get_ports {fmc_ha_01_23_n_tri_io[9]}]
#set_property PACKAGE_PIN G29 [get_ports {fmc_ha_01_23_p_tri_io[9]}]
#set_property PACKAGE_PIN F27 [get_ports {fmc_ha_00_22_n_tri_io[10]}]
#set_property PACKAGE_PIN G27 [get_ports {fmc_ha_00_22_p_tri_io[10]}]
#set_property PACKAGE_PIN F28 [get_ports {fmc_ha_01_23_n_tri_io[10]}]
#set_property PACKAGE_PIN G28 [get_ports {fmc_ha_01_23_p_tri_io[10]}]
#set_property PACKAGE_PIN C21 [get_ports {fmc_ha_00_22_n_tri_io[11]}]
#set_property PACKAGE_PIN D21 [get_ports {fmc_ha_00_22_p_tri_io[11]}]
#set_property PACKAGE_PIN F18 [get_ports {fmc_ha_01_23_n_tri_io[11]}]
#set_property PACKAGE_PIN G18 [get_ports {fmc_ha_01_23_p_tri_io[11]}]
## FMC HB[10:0] ports
#set_property PACKAGE_PIN F13 [get_ports {fmc_hb_00_20_n_tri_io[0]}]
#set_property PACKAGE_PIN G13 [get_ports {fmc_hb_00_20_p_tri_io[0]}]
#set_property PACKAGE_PIN G15 [get_ports {fmc_hb_01_21_n_tri_io[0]}]
#set_property PACKAGE_PIN H15 [get_ports {fmc_hb_01_21_p_tri_io[0]}]
#set_property PACKAGE_PIN K15 [get_ports {fmc_hb_00_20_n_tri_io[1]}]
#set_property PACKAGE_PIN L15 [get_ports {fmc_hb_00_20_p_tri_io[1]}]
#set_property PACKAGE_PIN G14 [get_ports {fmc_hb_01_21_n_tri_io[1]}]
#set_property PACKAGE_PIN H14 [get_ports {fmc_hb_01_21_p_tri_io[1]}]
#set_property PACKAGE_PIN H16 [get_ports {fmc_hb_00_20_n_tri_io[2]}]
#set_property PACKAGE_PIN J16 [get_ports {fmc_hb_00_20_p_tri_io[2]}]
#set_property PACKAGE_PIN K16 [get_ports {fmc_hb_01_21_n_tri_io[2]}]
#set_property PACKAGE_PIN L16 [get_ports {fmc_hb_01_21_p_tri_io[2]}]
#set_property PACKAGE_PIN E13 [get_ports {fmc_hb_00_20_n_tri_io[3]}]
#set_property PACKAGE_PIN F12 [get_ports {fmc_hb_00_20_p_tri_io[3]}]
#set_property PACKAGE_PIN A13 [get_ports {fmc_hb_01_21_n_tri_io[3]}]
#set_property PACKAGE_PIN B13 [get_ports {fmc_hb_01_21_p_tri_io[3]}]
#set_property PACKAGE_PIN J14 [get_ports {fmc_hb_00_20_n_tri_io[4]}]
#set_property PACKAGE_PIN K14 [get_ports {fmc_hb_00_20_p_tri_io[4]}]
#set_property PACKAGE_PIN B15 [get_ports {fmc_hb_01_21_n_tri_io[4]}]
#set_property PACKAGE_PIN C15 [get_ports {fmc_hb_01_21_p_tri_io[4]}]
#set_property PACKAGE_PIN J12 [get_ports {fmc_hb_00_20_n_tri_io[5]}]
#set_property PACKAGE_PIN J11 [get_ports {fmc_hb_00_20_p_tri_io[5]}]
#set_property PACKAGE_PIN C11 [get_ports {fmc_hb_01_21_n_tri_io[5]}]
#set_property PACKAGE_PIN D11 [get_ports {fmc_hb_01_21_p_tri_io[5]}]
#set_property PACKAGE_PIN A12 [get_ports {fmc_hb_00_20_n_tri_io[6]}]
#set_property PACKAGE_PIN A11 [get_ports {fmc_hb_00_20_p_tri_io[6]}]
#set_property PACKAGE_PIN B12 [get_ports {fmc_hb_01_21_n_tri_io[6]}]
#set_property PACKAGE_PIN C12 [get_ports {fmc_hb_01_21_p_tri_io[6]}]
#set_property PACKAGE_PIN H12 [get_ports {fmc_hb_00_20_n_tri_io[7]}]
#set_property PACKAGE_PIN H11 [get_ports {fmc_hb_00_20_p_tri_io[7]}]
#set_property PACKAGE_PIN L13 [get_ports {fmc_hb_01_21_n_tri_io[7]}]
#set_property PACKAGE_PIN L12 [get_ports {fmc_hb_01_21_p_tri_io[7]}]
#set_property PACKAGE_PIN J13 [get_ports {fmc_hb_00_20_n_tri_io[8]}]
#set_property PACKAGE_PIN K13 [get_ports {fmc_hb_00_20_p_tri_io[8]}]
#set_property PACKAGE_PIN D13 [get_ports {fmc_hb_01_21_n_tri_io[8]}]
#set_property PACKAGE_PIN D12 [get_ports {fmc_hb_01_21_p_tri_io[8]}]
#set_property PACKAGE_PIN E15 [get_ports {fmc_hb_00_20_n_tri_io[9]}]
#set_property PACKAGE_PIN E14 [get_ports {fmc_hb_00_20_p_tri_io[9]}]
#set_property PACKAGE_PIN E11 [get_ports {fmc_hb_01_21_n_tri_io[9]}]
#set_property PACKAGE_PIN F11 [get_ports {fmc_hb_01_21_p_tri_io[9]}]
#set_property PACKAGE_PIN A15 [get_ports {fmc_hb_00_20_n_tri_io[10]}]
#set_property PACKAGE_PIN B14 [get_ports {fmc_hb_00_20_p_tri_io[10]}]
#set_property PACKAGE_PIN C14 [get_ports {fmc_hb_01_21_n_tri_io[10]}]
#set_property PACKAGE_PIN D14 [get_ports {fmc_hb_01_21_p_tri_io[10]}]
## FMC LA[16:0] ports
#set_property PACKAGE_PIN C27 [get_ports {fmc_la_00_32_n_tri_io[0]}]
#set_property PACKAGE_PIN D27 [get_ports {fmc_la_00_32_p_tri_io[0]}]
#set_property PACKAGE_PIN C26 [get_ports {fmc_la_01_33_n_tri_io[0]}]
#set_property PACKAGE_PIN D26 [get_ports {fmc_la_01_33_p_tri_io[0]}]
#set_property PACKAGE_PIN G30 [get_ports {fmc_la_00_32_n_tri_io[1]}]
#set_property PACKAGE_PIN H30 [get_ports {fmc_la_00_32_p_tri_io[1]}]
#set_property PACKAGE_PIN E30 [get_ports {fmc_la_01_33_n_tri_io[1]}]
#set_property PACKAGE_PIN E29 [get_ports {fmc_la_01_33_p_tri_io[1]}]
#set_property PACKAGE_PIN H27 [get_ports {fmc_la_00_32_n_tri_io[2]}]
#set_property PACKAGE_PIN H26 [get_ports {fmc_la_00_32_p_tri_io[2]}]
#set_property PACKAGE_PIN A30 [get_ports {fmc_la_01_33_n_tri_io[2]}]
#set_property PACKAGE_PIN B30 [get_ports {fmc_la_01_33_p_tri_io[2]}]
#set_property PACKAGE_PIN C30 [get_ports {fmc_la_00_32_n_tri_io[3]}]
#set_property PACKAGE_PIN D29 [get_ports {fmc_la_00_32_p_tri_io[3]}]
#set_property PACKAGE_PIN E25 [get_ports {fmc_la_01_33_n_tri_io[3]}]
#set_property PACKAGE_PIN F25 [get_ports {fmc_la_01_33_p_tri_io[3]}]
#set_property PACKAGE_PIN B29 [get_ports {fmc_la_00_32_n_tri_io[4]}]
#set_property PACKAGE_PIN C29 [get_ports {fmc_la_00_32_p_tri_io[4]}]
#set_property PACKAGE_PIN A28 [get_ports {fmc_la_01_33_n_tri_io[4]}]
#set_property PACKAGE_PIN B28 [get_ports {fmc_la_01_33_p_tri_io[4]}]
#set_property PACKAGE_PIN A27 [get_ports {fmc_la_00_32_n_tri_io[5]}]
#set_property PACKAGE_PIN B27 [get_ports {fmc_la_00_32_p_tri_io[5]}]
#set_property PACKAGE_PIN A26 [get_ports {fmc_la_01_33_n_tri_io[5]}]
#set_property PACKAGE_PIN A25 [get_ports {fmc_la_01_33_p_tri_io[5]}]
#set_property PACKAGE_PIN E26 [get_ports {fmc_la_00_32_n_tri_io[6]}]
#set_property PACKAGE_PIN F26 [get_ports {fmc_la_00_32_p_tri_io[6]}]
#set_property PACKAGE_PIN D24 [get_ports {fmc_la_01_33_n_tri_io[6]}]
#set_property PACKAGE_PIN E24 [get_ports {fmc_la_01_33_p_tri_io[6]}]
#set_property PACKAGE_PIN B24 [get_ports {fmc_la_00_32_n_tri_io[7]}]
#set_property PACKAGE_PIN C24 [get_ports {fmc_la_00_32_p_tri_io[7]}]
#set_property PACKAGE_PIN A23 [get_ports {fmc_la_01_33_n_tri_io[7]}]
#set_property PACKAGE_PIN B23 [get_ports {fmc_la_01_33_p_tri_io[7]}]
#set_property PACKAGE_PIN D23 [get_ports {fmc_la_00_32_n_tri_io[8]}]
#set_property PACKAGE_PIN E23 [get_ports {fmc_la_00_32_p_tri_io[8]}]
#set_property PACKAGE_PIN E21 [get_ports {fmc_la_01_33_n_tri_io[8]}]
#set_property PACKAGE_PIN F21 [get_ports {fmc_la_01_33_p_tri_io[8]}]
#set_property PACKAGE_PIN D18 [get_ports {fmc_la_00_32_n_tri_io[9]}]
#set_property PACKAGE_PIN D17 [get_ports {fmc_la_00_32_p_tri_io[9]}]
#set_property PACKAGE_PIN H22 [get_ports {fmc_la_01_33_n_tri_io[9]}]
#set_property PACKAGE_PIN H21 [get_ports {fmc_la_01_33_p_tri_io[9]}]
#set_property PACKAGE_PIN F22 [get_ports {fmc_la_00_32_n_tri_io[10]}]
#set_property PACKAGE_PIN G22 [get_ports {fmc_la_00_32_p_tri_io[10]}]
#set_property PACKAGE_PIN L18 [get_ports {fmc_la_01_33_n_tri_io[10]}]
#set_property PACKAGE_PIN L17 [get_ports {fmc_la_01_33_p_tri_io[10]}]
#set_property PACKAGE_PIN H17 [get_ports {fmc_la_00_32_n_tri_io[11]}]
#set_property PACKAGE_PIN J17 [get_ports {fmc_la_00_32_p_tri_io[11]}]
#set_property PACKAGE_PIN F17 [get_ports {fmc_la_01_33_n_tri_io[11]}]
#set_property PACKAGE_PIN G17 [get_ports {fmc_la_01_33_p_tri_io[11]}]
#set_property PACKAGE_PIN G20 [get_ports {fmc_la_00_32_n_tri_io[12]}]
#set_property PACKAGE_PIN H20 [get_ports {fmc_la_00_32_p_tri_io[12]}]
#set_property PACKAGE_PIN C22 [get_ports {fmc_la_01_33_n_tri_io[12]}]
#set_property PACKAGE_PIN D22 [get_ports {fmc_la_01_33_p_tri_io[12]}]
#set_property PACKAGE_PIN A22 [get_ports {fmc_la_00_32_n_tri_io[13]}]
#set_property PACKAGE_PIN B22 [get_ports {fmc_la_00_32_p_tri_io[13]}]
#set_property PACKAGE_PIN A21 [get_ports {fmc_la_01_33_n_tri_io[13]}]
#set_property PACKAGE_PIN A20 [get_ports {fmc_la_01_33_p_tri_io[13]}]
#set_property PACKAGE_PIN H19 [get_ports {fmc_la_00_32_n_tri_io[14]}]
#set_property PACKAGE_PIN J19 [get_ports {fmc_la_00_32_p_tri_io[14]}]
#set_property PACKAGE_PIN A18 [get_ports {fmc_la_01_33_n_tri_io[14]}]
#set_property PACKAGE_PIN B18 [get_ports {fmc_la_01_33_p_tri_io[14]}]
#set_property PACKAGE_PIN A17 [get_ports {fmc_la_00_32_n_tri_io[15]}]
#set_property PACKAGE_PIN A16 [get_ports {fmc_la_00_32_p_tri_io[15]}]
#set_property PACKAGE_PIN B17 [get_ports {fmc_la_01_33_n_tri_io[15]}]
#set_property PACKAGE_PIN C17 [get_ports {fmc_la_01_33_p_tri_io[15]}]
#set_property PACKAGE_PIN J18 [get_ports {fmc_la_00_32_n_tri_io[16]}]
#set_property PACKAGE_PIN K18 [get_ports {fmc_la_00_32_p_tri_io[16]}]
#set_property PACKAGE_PIN C16 [get_ports {fmc_la_01_33_n_tri_io[16]}]
#set_property PACKAGE_PIN D16 [get_ports {fmc_la_01_33_p_tri_io[16]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {fmc_??_0?_??_?_tri_io[*]}]
#set_property SLEW SLOW [get_ports {fmc_??_0?_??_?_tri_io[*]}]
#set_property DRIVE 4 [get_ports {fmc_??_0?_??_?_tri_io[*]}]
#set_property PULLUP true [get_ports {fmc_??_0?_??_?_tri_io[*]}]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# Fan

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#IO_25_14 Sch=FAN_PWM
set_property PACKAGE_PIN W19 [get_ports fan_en]
set_property IOSTANDARD LVCMOS33 [get_ports fan_en]

#IO_L22P_T3_A05_D21_14 Sch=FAN_TACH
#set_property PACKAGE_PIN V21 [get_ports FAN_TACHO]
#set_property IOSTANDARD LVCMOS33 [get_ports FAN_TACHO]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

# Buttons

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#IO_25_17 Sch=btnc
set_property PACKAGE_PIN E18 [get_ports btnc]
set_property IOSTANDARD LVCMOS12 [get_ports btnc]

#IO_0_15 Sch=btnd
set_property PACKAGE_PIN M19 [get_ports btnd]
set_property IOSTANDARD LVCMOS12 [get_ports btnd]

#IO_L6P_T0_15 Sch=btnl
set_property PACKAGE_PIN M20 [get_ports btnl]
set_property IOSTANDARD LVCMOS12 [get_ports btnl]

#IO_L24P_T3_17 Sch=btnr
set_property PACKAGE_PIN C19 [get_ports btnr]
set_property IOSTANDARD LVCMOS12 [get_ports btnr]

#IO_L24N_T3_17 Sch=btnu
set_property PACKAGE_PIN B19 [get_ports btnu]
set_property IOSTANDARD LVCMOS12 [get_ports btnu]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#Switches

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#IO_0_17 Sch=sw[0]
set_property PACKAGE_PIN G19 [get_ports {sw[0]}]
set_property IOSTANDARD LVCMOS12 [get_ports {sw[0]}]

#IO_25_16 Sch=sw[1]
set_property PACKAGE_PIN G25 [get_ports {sw[1]}]
set_property IOSTANDARD LVCMOS12 [get_ports {sw[1]}]

#IO_L19P_T3_16 Sch=sw[2]
set_property PACKAGE_PIN H24 [get_ports {sw[2]}]
set_property IOSTANDARD LVCMOS12 [get_ports {sw[2]}]

#IO_L6P_T0_17 Sch=sw[3]
set_property PACKAGE_PIN K19 [get_ports {sw[3]}]
set_property IOSTANDARD LVCMOS12 [get_ports {sw[3]}]

#IO_L19P_T3_A22_15 Sch=sw[4]
set_property PACKAGE_PIN N19 [get_ports {sw[4]}]
set_property IOSTANDARD LVCMOS12 [get_ports {sw[4]}]

#IO_25_15 Sch=sw[5]
set_property PACKAGE_PIN P19 [get_ports {sw[5]}]
set_property IOSTANDARD LVCMOS12 [get_ports {sw[5]}]

#IO_L10P_T1_D14_14 Sch=sw[6]
set_property PACKAGE_PIN P26 [get_ports {sw[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sw[6]}]

#IO_L8P_T1_D11_14 Sch=sw[7]
set_property PACKAGE_PIN P27 [get_ports {sw[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sw[7]}]

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#Power Monitor

#_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

#IO_L16P_T2_13 Sch=SYS_SCL
set_property PACKAGE_PIN AE30 [get_ports power_iic_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports power_iic_scl_io]

#IO_L16N_T2_13 Sch=SYS_SDA
set_property PACKAGE_PIN AF30 [get_ports power_iic_sda_io]
set_property IOSTANDARD LVCMOS33 [get_ports power_iic_sda_io]
