
# Loading additional proc with user specified bodies to compute parameter values.
source [file join [file dirname [file dirname [info script]]] gui/d_axi_i2s_audio_v2_0.gtcl]

# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  set Component_Name  [  ipgui::add_param $IPINST -name "Component_Name" -display_name {Component Name}]
  set_property tooltip {Component Name} ${Component_Name}
  #Adding Page
  set Page_0  [  ipgui::add_page $IPINST -name "Page 0" -display_name {Page 0}]
  set_property tooltip {Page 0} ${Page_0}
  set C_AXI_STREAM_DATA_WIDTH  [  ipgui::add_param $IPINST -name "C_AXI_STREAM_DATA_WIDTH" -parent ${Page_0} -display_name {C Axi Stream Data Width}]
  set_property tooltip {C Axi Stream Data Width} ${C_AXI_STREAM_DATA_WIDTH}
  set C_AXI_L_DATA_WIDTH  [  ipgui::add_param $IPINST -name "C_AXI_L_DATA_WIDTH" -parent ${Page_0} -display_name {C Axi L Data Width}]
  set_property tooltip {C Axi L Data Width} ${C_AXI_L_DATA_WIDTH}
  set C_AXI_L_ADDR_WIDTH  [  ipgui::add_param $IPINST -name "C_AXI_L_ADDR_WIDTH" -parent ${Page_0} -display_name {C Axi L Addr Width}]
  set_property tooltip {C Axi L Addr Width} ${C_AXI_L_ADDR_WIDTH}
  set ENABLE_STREAM  [  ipgui::add_param $IPINST -name "ENABLE_STREAM" -parent ${Page_0} -display_name {Enable Stream}]
  set_property tooltip {Enable Stream} ${ENABLE_STREAM}
  #Adding Group
  set CLK_Control  [  ipgui::add_group $IPINST -name "CLK Control" -parent ${Page_0} -display_name {CLK Control}]
  set_property tooltip {CLK Control} ${CLK_Control}
  set ENABLE_LRCLK_2  [  ipgui::add_param $IPINST -name "ENABLE_LRCLK_2" -parent ${CLK_Control} -display_name {Enable LRCLK2}]
  set_property tooltip {Enable LRCLK2} ${ENABLE_LRCLK_2}
  set OUTPUT_ONLY_CLK  [  ipgui::add_param $IPINST -name "OUTPUT_ONLY_CLK" -parent ${CLK_Control} -display_name {Output Only BCLK and LRCLK}]
  set_property tooltip {Output Only BCLK and LRCLK} ${OUTPUT_ONLY_CLK}
  set INPUT_ONLY_CLK  [  ipgui::add_param $IPINST -name "INPUT_ONLY_CLK" -parent ${CLK_Control} -display_name {Input Only BCLK and LRCLK}]
  set_property tooltip {Input Only BCLK and LRCLK} ${INPUT_ONLY_CLK}



}

proc update_PARAM_VALUE.INPUT_ONLY_CLK { PARAM_VALUE.INPUT_ONLY_CLK PARAM_VALUE.OUTPUT_ONLY_CLK PARAM_VALUE.OUTPUT_ONLY_CLK } {
	# Procedure called to update INPUT_ONLY_CLK when any of the dependent parameters in the arguments change
	
	set INPUT_ONLY_CLK ${PARAM_VALUE.INPUT_ONLY_CLK}
	set OUTPUT_ONLY_CLK ${PARAM_VALUE.OUTPUT_ONLY_CLK}
	set values(OUTPUT_ONLY_CLK) [get_property value $OUTPUT_ONLY_CLK]
	if { [gen_USERPARAMETER_INPUT_ONLY_CLK_ENABLEMENT $values(OUTPUT_ONLY_CLK)] } {
		set_property enabled true $INPUT_ONLY_CLK
	} else {
		set_property enabled false $INPUT_ONLY_CLK
		set_property value [gen_USERPARAMETER_INPUT_ONLY_CLK_VALUE $values(OUTPUT_ONLY_CLK)] $INPUT_ONLY_CLK
	}
}

proc validate_PARAM_VALUE.INPUT_ONLY_CLK { PARAM_VALUE.INPUT_ONLY_CLK } {
	# Procedure called to validate INPUT_ONLY_CLK
	return true
}

proc update_PARAM_VALUE.C_AXI_STREAM_DATA_WIDTH { PARAM_VALUE.C_AXI_STREAM_DATA_WIDTH } {
	# Procedure called to update C_AXI_STREAM_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXI_STREAM_DATA_WIDTH { PARAM_VALUE.C_AXI_STREAM_DATA_WIDTH } {
	# Procedure called to validate C_AXI_STREAM_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_AXI_L_DATA_WIDTH { PARAM_VALUE.C_AXI_L_DATA_WIDTH } {
	# Procedure called to update C_AXI_L_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXI_L_DATA_WIDTH { PARAM_VALUE.C_AXI_L_DATA_WIDTH } {
	# Procedure called to validate C_AXI_L_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_AXI_L_ADDR_WIDTH { PARAM_VALUE.C_AXI_L_ADDR_WIDTH } {
	# Procedure called to update C_AXI_L_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXI_L_ADDR_WIDTH { PARAM_VALUE.C_AXI_L_ADDR_WIDTH } {
	# Procedure called to validate C_AXI_L_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.ENABLE_STREAM { PARAM_VALUE.ENABLE_STREAM } {
	# Procedure called to update ENABLE_STREAM when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.ENABLE_STREAM { PARAM_VALUE.ENABLE_STREAM } {
	# Procedure called to validate ENABLE_STREAM
	return true
}

proc update_PARAM_VALUE.ENABLE_LRCLK_2 { PARAM_VALUE.ENABLE_LRCLK_2 } {
	# Procedure called to update ENABLE_LRCLK_2 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.ENABLE_LRCLK_2 { PARAM_VALUE.ENABLE_LRCLK_2 } {
	# Procedure called to validate ENABLE_LRCLK_2
	return true
}

proc update_PARAM_VALUE.OUTPUT_ONLY_CLK { PARAM_VALUE.OUTPUT_ONLY_CLK } {
	# Procedure called to update OUTPUT_ONLY_CLK when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.OUTPUT_ONLY_CLK { PARAM_VALUE.OUTPUT_ONLY_CLK } {
	# Procedure called to validate OUTPUT_ONLY_CLK
	return true
}

proc update_PARAM_VALUE.C_AXI_L_BASEADDR { PARAM_VALUE.C_AXI_L_BASEADDR } {
	# Procedure called to update C_AXI_L_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXI_L_BASEADDR { PARAM_VALUE.C_AXI_L_BASEADDR } {
	# Procedure called to validate C_AXI_L_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_AXI_L_HIGHADDR { PARAM_VALUE.C_AXI_L_HIGHADDR } {
	# Procedure called to update C_AXI_L_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_AXI_L_HIGHADDR { PARAM_VALUE.C_AXI_L_HIGHADDR } {
	# Procedure called to validate C_AXI_L_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.C_DATA_WIDTH { MODELPARAM_VALUE.C_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	# WARNING: There is no corresponding user parameter named "C_DATA_WIDTH". Setting updated value from the model parameter.
set_property value 24 ${MODELPARAM_VALUE.C_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_AXI_STREAM_DATA_WIDTH { MODELPARAM_VALUE.C_AXI_STREAM_DATA_WIDTH PARAM_VALUE.C_AXI_STREAM_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_AXI_STREAM_DATA_WIDTH}] ${MODELPARAM_VALUE.C_AXI_STREAM_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_AXI_L_DATA_WIDTH { MODELPARAM_VALUE.C_AXI_L_DATA_WIDTH PARAM_VALUE.C_AXI_L_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_AXI_L_DATA_WIDTH}] ${MODELPARAM_VALUE.C_AXI_L_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_AXI_L_ADDR_WIDTH { MODELPARAM_VALUE.C_AXI_L_ADDR_WIDTH PARAM_VALUE.C_AXI_L_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_AXI_L_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_AXI_L_ADDR_WIDTH}
}

