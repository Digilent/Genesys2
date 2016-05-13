# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
	set Component_Name [ipgui::add_param $IPINST -name Component_Name]
	set Page0 [ipgui::add_page $IPINST -name "Page 0" -layout vertical]
	set C_S00_AXI_ADDR_WIDTH [ipgui::add_param $IPINST -parent $Page0 -name C_S00_AXI_ADDR_WIDTH]
	set C_S00_AXI_DATA_WIDTH [ipgui::add_param $IPINST -parent $Page0 -name C_S00_AXI_DATA_WIDTH]
	set kMaxResolutionBits [ipgui::add_param $IPINST -parent $Page0 -name kMaxResolutionBits -widget textEdit]
	set kDataWidth [ipgui::add_param $IPINST -parent $Page0 -name kDataWidth -widget textEdit]
}

proc update_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to update C_S00_AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to validate C_S00_AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to update C_S00_AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to validate C_S00_AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.kMaxResolutionBits { PARAM_VALUE.kMaxResolutionBits } {
	# Procedure called to update kMaxResolutionBits when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.kMaxResolutionBits { PARAM_VALUE.kMaxResolutionBits } {
	# Procedure called to validate kMaxResolutionBits
	return true
}

proc update_PARAM_VALUE.kDataWidth { PARAM_VALUE.kDataWidth } {
	# Procedure called to update kDataWidth when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.kDataWidth { PARAM_VALUE.kDataWidth } {
	# Procedure called to validate kDataWidth
	return true
}


proc update_MODELPARAM_VALUE.kDataWidth { MODELPARAM_VALUE.kDataWidth PARAM_VALUE.kDataWidth } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.kDataWidth}] ${MODELPARAM_VALUE.kDataWidth}
}

proc update_MODELPARAM_VALUE.kMaxResolutionBits { MODELPARAM_VALUE.kMaxResolutionBits PARAM_VALUE.kMaxResolutionBits } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.kMaxResolutionBits}] ${MODELPARAM_VALUE.kMaxResolutionBits}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH { MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH}
}

