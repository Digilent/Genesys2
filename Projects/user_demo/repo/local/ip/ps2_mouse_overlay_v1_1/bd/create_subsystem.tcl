startgroup
set hier_name mouse_vga_pattern_test
create_bd_cell -type hier $hier_name
create_bd_cell -type ip -vlnv xilinx.com:ip:v_tc:6.1 $hier_name/v_tc_0
create_bd_cell -type ip -vlnv xilinx.com:ip:v_tpg:6.0 $hier_name/v_tpg_0
create_bd_cell -type ip -vlnv xilinx.com:ip:v_axi4s_vid_out:3.0 $hier_name/v_axi4s_vid_out_0
create_bd_cell -type ip -vlnv digilentinc.com:ip:ps2_mouse_overlay:1.1 $hier_name/ps2_mouse_overlay_0
create_bd_cell -type ip -vlnv digilentinc.com:ip:rgb2vga:1.0 $hier_name/rgb2vga_0

set_property -dict [list CONFIG.HAS_AXI4_LITE {false} CONFIG.HAS_INTC_IF {true} CONFIG.VIDEO_MODE {1080p} CONFIG.enable_detection {false} CONFIG.GEN_F0_VSYNC_VSTART {1083} CONFIG.GEN_F1_VSYNC_VSTART {1083} CONFIG.GEN_HACTIVE_SIZE {1920} CONFIG.GEN_HSYNC_END {2052} CONFIG.GEN_HFRAME_SIZE {2200} CONFIG.GEN_F0_VSYNC_HSTART {1920} CONFIG.GEN_F1_VSYNC_HSTART {1920} CONFIG.GEN_F0_VSYNC_HEND {1920} CONFIG.GEN_F1_VSYNC_HEND {1920} CONFIG.GEN_F0_VFRAME_SIZE {1125} CONFIG.GEN_F1_VFRAME_SIZE {1125} CONFIG.GEN_F0_VSYNC_VEND {1088} CONFIG.GEN_F1_VSYNC_VEND {1088} CONFIG.GEN_F0_VBLANK_HEND {1920} CONFIG.GEN_F1_VBLANK_HEND {1920} CONFIG.GEN_HSYNC_START {2008} CONFIG.GEN_VACTIVE_SIZE {1080} CONFIG.GEN_F0_VBLANK_HSTART {1920} CONFIG.GEN_F1_VBLANK_HSTART {1920}] [get_bd_cells $hier_name/v_tc_0]
set_property -dict [list CONFIG.has_axi4s_slave {false} CONFIG.has_intc_if {true} CONFIG.has_vtiming {false} CONFIG.pattern_control {9}] [get_bd_cells $hier_name/v_tpg_0]
set_property -dict [list CONFIG.m_video_format.VALUE_SRC USER] [get_bd_cells $hier_name/v_tpg_0]
set_property -dict [list CONFIG.m_video_format {2}] [get_bd_cells $hier_name/v_tpg_0]
set_property -dict [list CONFIG.RAM_ADDR_BITS {5} CONFIG.VTG_MASTER_SLAVE {1}] [get_bd_cells $hier_name/v_axi4s_vid_out_0]

connect_bd_intf_net [get_bd_intf_pins $hier_name/v_axi4s_vid_out_0/vid_io_out] [get_bd_intf_pins $hier_name/ps2_mouse_overlay_0/vid_i]
connect_bd_intf_net [get_bd_intf_pins $hier_name/ps2_mouse_overlay_0/vid_o] [get_bd_intf_pins $hier_name/rgb2vga_0/vid_in]
connect_bd_intf_net [get_bd_intf_pins $hier_name/v_tpg_0/video_out] [get_bd_intf_pins $hier_name/v_axi4s_vid_out_0/video_in]
connect_bd_intf_net [get_bd_intf_pins $hier_name/v_tc_0/vtiming_out] [get_bd_intf_pins $hier_name/v_axi4s_vid_out_0/vtiming_in]

create_bd_pin -dir I $hier_name/PixelClk
connect_bd_net [get_bd_pins $hier_name/PixelClk] [get_bd_pins $hier_name/ps2_mouse_overlay_0/PixelClk]
connect_bd_net [get_bd_pins $hier_name/PixelClk] [get_bd_pins $hier_name/v_axi4s_vid_out_0/aclk]
connect_bd_net [get_bd_pins $hier_name/PixelClk] [get_bd_pins $hier_name/v_axi4s_vid_out_0/vid_io_out_clk]
connect_bd_net [get_bd_pins $hier_name/PixelClk] [get_bd_pins $hier_name/v_tc_0/clk]
connect_bd_net [get_bd_pins $hier_name/PixelClk] [get_bd_pins $hier_name/rgb2vga_0/PixelClk]
connect_bd_net [get_bd_pins $hier_name/PixelClk] [get_bd_pins $hier_name/v_tpg_0/aclk]

create_bd_pin -dir IO $hier_name/PS2_Clk
create_bd_pin -dir IO $hier_name/PS2_Data
connect_bd_net [get_bd_pins $hier_name/PS2_Clk] [get_bd_pins $hier_name/ps2_mouse_overlay_0/PS2_Clk_IO]
connect_bd_net [get_bd_pins $hier_name/PS2_Data] [get_bd_pins $hier_name/ps2_mouse_overlay_0/PS2_Data_IO]
create_bd_pin -dir O -from 4 -to 0 $hier_name/vga_red
create_bd_pin -dir O -from 5 -to 0 $hier_name/vga_green
create_bd_pin -dir O -from 4 -to 0 $hier_name/vga_blue
create_bd_pin -dir O $hier_name/vga_hsync
create_bd_pin -dir O $hier_name/vga_vsync
connect_bd_net [get_bd_pins $hier_name/vga_hsync] [get_bd_pins $hier_name/rgb2vga_0/vga_pHSync]
connect_bd_net [get_bd_pins $hier_name/vga_vsync] [get_bd_pins $hier_name/rgb2vga_0/vga_pVSync]
connect_bd_net [get_bd_pins $hier_name/vga_red] [get_bd_pins $hier_name/rgb2vga_0/vga_pRed]
connect_bd_net [get_bd_pins $hier_name/vga_green] [get_bd_pins $hier_name/rgb2vga_0/vga_pGreen]
connect_bd_net [get_bd_pins $hier_name/vga_blue] [get_bd_pins $hier_name/rgb2vga_0/vga_pBlue]
endgroup

startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins $hier_name/v_tpg_0/ctrl]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins $hier_name/ps2_mouse_overlay_0/ctrl]
endgroup