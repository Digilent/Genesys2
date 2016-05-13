set board_name nexys_video
set proj_name demo
set script_path [file dirname [info script]]
set srec_file $script_path/../../sdk/$proj_name/Debug/$proj_name.elf.srec


foreach arg $::argv {
   switch -glob $arg {
      -b* {set bit [string range $arg 2 end]}
      -s* {set srec [string range $arg 2 end]}
	  -i* {set img [string range $arg 2 end]}
      -o* {set out [string range $arg 2 end]}
   }
}
if {!([info exists bit] && [info exists srec] && [info exists out] && [info exists img])} {
   puts "Missing arguments. Correct syntax is: -b<bitfile_path> -s<srecfile_path> -i<imgfile_path> -o<outfile_path>"
   exit 1
}

if { [catch {write_cfgmem -force -format MCS -size 32 -interface SPIx4 -loadbit "up 0x0 $bit" -loaddata "up 0x00A00000 $srec" -loaddata "up 0x1000000" $out} fid] } {
   puts "$fid"
   exit 1
}

exit 0