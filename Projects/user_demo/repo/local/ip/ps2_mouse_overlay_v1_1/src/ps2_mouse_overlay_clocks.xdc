# We handshake mouse position data from the AXI clock domain to the PixelClk domain
# Make sure the path from the iData_int register to oData register is smaller than 2 OutClk (PixelClk) periods
set OutClk [get_clocks -of [get_ports PixelClk]]

set_max_delay -datapath_only -from [get_pins HandshakeData*/iData_int_reg[*]/C] -to [get_pins HandshakeData*/oData_reg[*]/D] [expr [get_property -min PERIOD $OutClk] * 2]