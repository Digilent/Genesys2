create_clock -period 10.000 -name PixelClk -waveform {0.000 5.000} [get_ports PixelClk]
create_clock -period 9.259 -name s00_axi_aclk -waveform {0.000 5.000} [get_ports s00_axi_aclk]
