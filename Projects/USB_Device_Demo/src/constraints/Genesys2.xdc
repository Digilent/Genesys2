set_property PACKAGE_PIN E18 [get_ports {gpio_1_tri_i[0]}]
set_property PACKAGE_PIN B19 [get_ports {gpio_1_tri_i[1]}]
set_property PACKAGE_PIN C19 [get_ports {gpio_1_tri_i[2]}]
set_property PACKAGE_PIN M19 [get_ports {gpio_1_tri_i[3]}]
set_property PACKAGE_PIN M20 [get_ports {gpio_1_tri_i[4]}]
set_property PACKAGE_PIN W24 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_1_tri_i[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_1_tri_i[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_1_tri_i[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_1_tri_i[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_1_tri_i[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_tri_io[0]}]
set_property PACKAGE_PIN Y20 [get_ports UART_rxd]
set_property PACKAGE_PIN Y23 [get_ports UART_txd]
set_property IOSTANDARD LVCMOS33 [get_ports UART_rxd]
set_property IOSTANDARD LVCMOS33 [get_ports UART_txd]
set_property PACKAGE_PIN R19 [get_ports sys_rst]
set_property IOSTANDARD LVCMOS33 [get_ports sys_rst]

#set_property MARK_DEBUG true [get_nets design_1_i/clk_wiz_0/clk_out1]


set_property PACKAGE_PIN AD18 [get_ports ulpi_clk]
set_property IOSTANDARD LVCMOS18 [get_ports ulpi_clk]
set_property PACKAGE_PIN AB14 [get_ports ulpi_resetn]
set_property IOSTANDARD LVCMOS18 [get_ports ulpi_resetn]
set_property IOSTANDARD LVCMOS18 [get_ports {ulpi_data[7]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ulpi_data[6]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ulpi_data[5]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ulpi_data[4]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ulpi_data[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ulpi_data[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ulpi_data[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ulpi_data[0]}]
set_property PACKAGE_PIN AC14 [get_ports {ulpi_data[7]}]
set_property PACKAGE_PIN AD14 [get_ports {ulpi_data[6]}]
set_property PACKAGE_PIN AA15 [get_ports {ulpi_data[5]}]
set_property PACKAGE_PIN AB15 [get_ports {ulpi_data[4]}]
set_property PACKAGE_PIN AC16 [get_ports {ulpi_data[3]}]
set_property PACKAGE_PIN AC15 [get_ports {ulpi_data[2]}]
set_property PACKAGE_PIN AE15 [get_ports {ulpi_data[1]}]
set_property PACKAGE_PIN AE14 [get_ports {ulpi_data[0]}]
set_property PACKAGE_PIN W23 [get_ports led]
set_property IOSTANDARD LVCMOS33 [get_ports led]
set_property PACKAGE_PIN Y16 [get_ports ulpi_dir]
set_property PACKAGE_PIN AA16 [get_ports ulpi_nxt]
set_property PACKAGE_PIN AA17 [get_ports ulpi_stp]
set_property IOSTANDARD LVCMOS18 [get_ports ulpi_dir]
set_property IOSTANDARD LVCMOS18 [get_ports ulpi_nxt]
set_property IOSTANDARD LVCMOS18 [get_ports ulpi_stp]

create_generated_clock -name {DDR3_ck_p[0]} -source [get_pins {design_1_i/mig_7series_0/u_design_1_mig_7series_0_0_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/ddr_ck_gen_loop[0].ddr_ck_gen.ddr_ck/C}] -divide_by 1 -invert [get_ports {DDR3_ck_p[0]}]

#Input delay : Derived from the PHY maximum output delay : Max 9ns
# Propagation time (ltrace length = 15 cm)
#Output delay min : Must assure PHY input hold time: Output delay min > PHY input hold time (1.5ns) - 2 * propagation delay
#Output delay max : Must assure PHY setup time : Output delay max < Period (16.6ns) - PHY Setup Requirement (6ns) - 2 * propagation delay

create_clock -period 16.666 -name ulpi_clk -waveform {0.000 8.333} [get_ports ulpi_clk]
set_input_delay -clock [get_clocks ulpi_clk] -min -add_delay 1.200 [get_ports {ulpi_data[*]}]
set_input_delay -clock [get_clocks ulpi_clk] -max -add_delay 9.000 [get_ports {ulpi_data[*]}]
set_input_delay -clock [get_clocks ulpi_clk] -min -add_delay 1.200 [get_ports ulpi_dir]
set_input_delay -clock [get_clocks ulpi_clk] -max -add_delay 9.000 [get_ports ulpi_dir]
set_input_delay -clock [get_clocks ulpi_clk] -min -add_delay 1.200 [get_ports ulpi_nxt]
set_input_delay -clock [get_clocks ulpi_clk] -max -add_delay 9.000 [get_ports ulpi_nxt]
set_output_delay -clock [get_clocks ulpi_clk] -min -add_delay 1.000 [get_ports {ulpi_data[*]}]
set_output_delay -clock [get_clocks ulpi_clk] -max -add_delay 8.600 [get_ports {ulpi_data[*]}]
set_output_delay -clock [get_clocks ulpi_clk] -min -add_delay 1.000 [get_ports ulpi_stp]
set_output_delay -clock [get_clocks ulpi_clk] -max -add_delay 8.600 [get_ports ulpi_stp]





set_property MARK_DEBUG true [get_nets design_1_i/microblaze_0_interrupt_INTERRUPT]



