------------------------------------------------------------------------------
--
-- File: vid_io_mux.vhd
-- Author: Elod Gyorgy
-- Original Project: Atlys2 User Demo 
-- Date: 3 December 2014
--
-------------------------------------------------------------------------------
-- (c) 2014 Copyright Digilent Incorporated
-- All Rights Reserved
-- 
-- This program is free software; distributed under the terms of BSD 3-clause 
-- license ("Revised BSD License", "New BSD License", or "Modified BSD License")
--
-- Redistribution and use in source and binary forms, with or without modification,
-- are permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice, this
--    list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright notice,
--    this list of conditions and the following disclaimer in the documentation
--    and/or other materials provided with the distribution.
-- 3. Neither the name(s) of the above-listed copyright holder(s) nor the names
--    of its contributors may be used to endorse or promote products derived
--    from this software without specific prior written permission.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
-- AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
-- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
-- ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE 
-- FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
-- DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
-- SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
-- CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
-- OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
-- OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--
-------------------------------------------------------------------------------
--
-- Purpose:
--    This module adds a mouse pointer overlaid on video data input over a
--    vid_io interface. The output is also a vid_io interface. The mouse
--    pointer is controlled by a PS/2 mouse connected to the PS2_Clk, PS2_Data
--    top-level ports.
--  
-------------------------------------------------------------------------------
   
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ps2_mouse_overlay_v1_0 is
	generic (
		-- Users to add parameters here
		kDataWidth : natural := 24;
      kMaxResolutionBits : natural := 12;
		-- User parameters ends
		-- Do not modify the parameters beyond this line


		-- Parameters of Axi Slave Bus Interface S00_AXI
		C_S00_AXI_DATA_WIDTH	: integer	:= 32;
		C_S00_AXI_ADDR_WIDTH	: integer	:= 4
	);
	port (
		-- Users to add ports here
		-- Video clock
		PixelClk : in std_logic;
		pClkEn : in std_logic;
		
		-- Video In interface
      pVde_in : in std_logic;
      pHSync_in : in std_logic;
      pVSync_in : in std_logic;
      pData_in : in std_logic_vector(kDataWidth-1 downto 0);
      
      -- Video Out interface
      pVde_out : out std_logic;
      pHSync_out : out std_logic;
      pVSync_out : out std_logic;
      pData_out : out std_logic_vector(kDataWidth-1 downto 0);
      
      --PS/2 Mouse interface
      PS2_Clk_IO  : inout std_logic;
      PS2_Data_IO : inout std_logic;
      
      
		-- User ports ends
		-- Do not modify the ports beyond this line

		-- Ports of Axi Slave Bus Interface S00_AXI
		s00_axi_aclk	: in std_logic;
		s00_axi_aresetn	: in std_logic;
		s00_axi_awaddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_awprot	: in std_logic_vector(2 downto 0);
		s00_axi_awvalid	: in std_logic;
		s00_axi_awready	: out std_logic;
		s00_axi_wdata	: in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_wstrb	: in std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0);
		s00_axi_wvalid	: in std_logic;
		s00_axi_wready	: out std_logic;
		s00_axi_bresp	: out std_logic_vector(1 downto 0);
		s00_axi_bvalid	: out std_logic;
		s00_axi_bready	: in std_logic;
		s00_axi_araddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_arprot	: in std_logic_vector(2 downto 0);
		s00_axi_arvalid	: in std_logic;
		s00_axi_arready	: out std_logic;
		s00_axi_rdata	: out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_rresp	: out std_logic_vector(1 downto 0);
		s00_axi_rvalid	: out std_logic;
		s00_axi_rready	: in std_logic
	);
end ps2_mouse_overlay_v1_0;

architecture arch_imp of ps2_mouse_overlay_v1_0 is

	-- component declaration
	component ps2_mouse_overlay_v1_0_S00_AXI is
		generic (
		C_S_AXI_DATA_WIDTH	: integer	:= 32;
		C_S_AXI_ADDR_WIDTH	: integer	:= 4
		);
		port (
      xEnPointer : out std_logic;
		S_AXI_ACLK	: in std_logic;
		S_AXI_ARESETN	: in std_logic;
		S_AXI_AWADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_AWPROT	: in std_logic_vector(2 downto 0);
		S_AXI_AWVALID	: in std_logic;
		S_AXI_AWREADY	: out std_logic;
		S_AXI_WDATA	: in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_WSTRB	: in std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
		S_AXI_WVALID	: in std_logic;
		S_AXI_WREADY	: out std_logic;
		S_AXI_BRESP	: out std_logic_vector(1 downto 0);
		S_AXI_BVALID	: out std_logic;
		S_AXI_BREADY	: in std_logic;
		S_AXI_ARADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_ARPROT	: in std_logic_vector(2 downto 0);
		S_AXI_ARVALID	: in std_logic;
		S_AXI_ARREADY	: out std_logic;
		S_AXI_RDATA	: out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_RRESP	: out std_logic_vector(1 downto 0);
		S_AXI_RVALID	: out std_logic;
		S_AXI_RREADY	: in std_logic
		);
	end component ps2_mouse_overlay_v1_0_S00_AXI;

   type mouserom_t is array(0 to 255) of std_logic_vector(1 downto 0);
   -- the memory that holds the cursor.
   -- 00 - black
   -- 01 - white
   -- 1x - transparent
   constant mouserom: mouserom_t := (
   "00","00","11","11","11","11","11","11","11","11","11","11","11","11","11","11",
   "00","01","00","11","11","11","11","11","11","11","11","11","11","11","11","11",
   "00","01","01","00","11","11","11","11","11","11","11","11","11","11","11","11",
   "00","01","01","01","00","11","11","11","11","11","11","11","11","11","11","11",
   "00","01","01","01","01","00","11","11","11","11","11","11","11","11","11","11",
   "00","01","01","01","01","01","00","11","11","11","11","11","11","11","11","11",
   "00","01","01","01","01","01","01","00","11","11","11","11","11","11","11","11",
   "00","01","01","01","01","01","01","01","00","11","11","11","11","11","11","11",
   "00","01","01","01","01","01","00","00","00","00","11","11","11","11","11","11",
   "00","01","01","01","01","01","00","11","11","11","11","11","11","11","11","11",
   "00","01","00","00","01","01","00","11","11","11","11","11","11","11","11","11",
   "00","00","11","11","00","01","01","00","11","11","11","11","11","11","11","11",
   "00","11","11","11","00","01","01","00","11","11","11","11","11","11","11","11",
   "11","11","11","11","11","00","01","01","00","11","11","11","11","11","11","11",
   "11","11","11","11","11","00","01","01","00","11","11","11","11","11","11","11",
   "11","11","11","11","11","11","00","00","11","11","11","11","11","11","11","11"
   );                                                                                                                   
   signal pMousePx : std_logic_vector(1 downto 0);
   
   -- width and height of cursor.
   constant kPointerOffset: natural := 16;
   type rgb_color_t is
      record
         r : std_logic_vector(7 downto 0);
         b : std_logic_vector(7 downto 0);
         g : std_logic_vector(7 downto 0);
      end record;
   
	constant kPointerColor1 : rgb_color_t := (r => (others => '1'), b => (others => '1'), g => (others => '1'));
	constant kPointerColor2 : rgb_color_t := (r => (others => '0'), b => (others => '0'), g => (others => '0'));
	
   signal xMsRst, xEnPointer, pEnPointer, pBufEnPointer : std_logic;
   signal pHCnt, pVCnt : unsigned(kMaxResolutionBits-1 downto 0) := (others => '0');
   signal pRomAddr : natural range 0 to kPointerOffset * kPointerOffset - 1 := 0;
   signal pRomHAddr : unsigned(3 downto 0);
   signal pRomVAddr : unsigned(3 downto 0);
   signal pData_q, pData_qq : std_logic_vector(kDataWidth-1 downto 0);
   signal pHSync_q, pVSync_q, pVDE_q, pHSync_qq, pVSync_qq, pVDE_qq : std_logic; 
   signal xPosX, xPosY, pPosX, pPosY : std_logic_vector(kMaxResolutionBits-1 downto 0);
   signal pBufPosX, pBufPosY : unsigned(kMaxResolutionBits-1 downto 0);
   signal pMouseEn, xRdyX, xRdyY, pFrameSync, pVldX, pVldY, pMouseEn_q : std_logic;
begin

-- Instantiation of Axi Bus Interface S00_AXI
ps2_mouse_overlay_v1_0_S00_AXI_inst : ps2_mouse_overlay_v1_0_S00_AXI
	generic map (
		C_S_AXI_DATA_WIDTH	=> C_S00_AXI_DATA_WIDTH,
		C_S_AXI_ADDR_WIDTH	=> C_S00_AXI_ADDR_WIDTH
	)
	port map (
      xEnPointer => xEnPointer,
		S_AXI_ACLK	=> s00_axi_aclk,
		S_AXI_ARESETN	=> s00_axi_aresetn,
		S_AXI_AWADDR	=> s00_axi_awaddr,
		S_AXI_AWPROT	=> s00_axi_awprot,
		S_AXI_AWVALID	=> s00_axi_awvalid,
		S_AXI_AWREADY	=> s00_axi_awready,
		S_AXI_WDATA	=> s00_axi_wdata,
		S_AXI_WSTRB	=> s00_axi_wstrb,
		S_AXI_WVALID	=> s00_axi_wvalid,
		S_AXI_WREADY	=> s00_axi_wready,
		S_AXI_BRESP	=> s00_axi_bresp,
		S_AXI_BVALID	=> s00_axi_bvalid,
		S_AXI_BREADY	=> s00_axi_bready,
		S_AXI_ARADDR	=> s00_axi_araddr,
		S_AXI_ARPROT	=> s00_axi_arprot,
		S_AXI_ARVALID	=> s00_axi_arvalid,
		S_AXI_ARREADY	=> s00_axi_arready,
		S_AXI_RDATA	=> s00_axi_rdata,
		S_AXI_RRESP	=> s00_axi_rresp,
		S_AXI_RVALID	=> s00_axi_rvalid,
		S_AXI_RREADY	=> s00_axi_rready
	);

	-- Add user logic here
   MouseCtlX : entity work.MouseCtl
      generic map (
         SYSCLK_FREQUENCY_HZ => 100_000_000)
      port map (
         clk => s00_axi_aclk,
         rst => xMsRst,
         xpos => xPosX,
         ypos => xPosY,
         zpos => open,
         left => open,
         middle => open,
         right => open,
         new_event => open,
         value => (others => '0'),
         setx => '0',
         sety => '0',
         setmax_x => '0',
         setmax_y => '0',
         
         PS2_Clk_IO => PS2_Clk_IO,
         PS2_Data_IO => PS2_Data_IO      
      );
   xMsRst <= not s00_axi_aresetn;
   
SyncBaseOverlayEn: entity work.SyncBase
   generic map (
      kResetTo => '0',
      kStages => 2) --use double FF synchronizer
   port map (
      aReset => '0',
      InClk => s00_axi_aclk,
      iIn => xEnPointer,
      OutClk => PixelClk,
      oOut => pEnPointer);

HandshakeDataPosX: entity work.HandshakeData
   generic map (
      kDataWidth  => 12)
   port map (
      InClk => s00_axi_aclk,
      OutClk => PixelClk,
      iData => xPosX,
      oData => pPosX,
      iPush => xRdyX, --we can push through a position whenever the synchronizer is ready; alternatively we could use MouseCtl(new_event)
      iRdy => xRdyX,
      oAck => pFrameSync,
      oValid => pVldX,
      aReset => xMsRst);

HandshakeDataPosY: entity work.HandshakeData
   generic map (
      kDataWidth  => 12)
   port map (
      InClk => s00_axi_aclk,
      OutClk => PixelClk,
      iData => xPosY,
      oData => pPosY,
      iPush => xRdyY, --we can push through a position whenever the synchronizer is ready; alternatively we could use MouseCtl(new_event)
      iRdy => xRdyY,
      oAck => pFrameSync,
      oValid => pVldY,
      aReset => xMsRst);      

-- Horizontal and vertical counters
PixelCounters: process(PixelClk)
begin
   if Rising_Edge(PixelClk) then
      if (pClkEn = '1') then
         if (pFrameSync = '1') then
            pVCnt <= (others => '0');
         elsif pVDE_q = '1' and pVDE_in = '0' then
            pHCnt <= (others => '0');
            pVCnt <= pVCnt + 1;
         elsif pVde_in = '1' then
            pHCnt <= pHCnt + 1;
         end if;
      end if;
   end if;
end process PixelCounters;

pFrameSync <= not pVSync_q and pVSync_in;

-- New mouse position is only taken at the end of frame to prevent pointer tearing
BufferOnFrame: process(PixelClk)
begin
   if Rising_Edge(PixelClk) then
      if (pClkEn = '1') then
         if (pFrameSync = '1') then
            if (pVldX = '1') then
               pBufPosX <= unsigned(pPosX);
            end if;
            if (pVldY = '1') then
               pBufPosY <= unsigned(pPosY);
            end if;
            pBufEnPointer <= pEnPointer;
         end if;
      end if;
   end if;
end process BufferOnFrame;
 
-- Comparators to determine if inside mouse pointer area (pMouseEn)
-- Address arithmetic to determine ROM address
-- Introduces one period delay relative to input video
process(PixelClk)
variable pDiffH, pDiffV : unsigned(kMaxResolutionBits-1 downto 0);
begin
   if Rising_Edge(PixelClk) then
      if (pClkEn = '1') then     
         pDiffH := pHCnt - pBufPosX;
         pDiffV := pVCnt - PBufPosY;
         pRomHAddr <= pDiffH(3 downto 0);
         pRomVAddr <= pDiffV(3 downto 0);
         
         if (pVCnt >= pBufPosY and pVCnt < (pBufPosY + kPointerOffset) and
            pHCnt >= pBufPosX and pHCnt < (pBufPosX + kPointerOffset)) then
            pMouseEn <= '1';
         else
            pMouseEn <= '0';
         end if;
                       
         pData_q <= pData_in;
         pHSync_q <= pHSync_in;
         pVSync_q <= pVSync_in;
         pVDE_q <= pVDE_in;
      end if;
   end if;
end process;
   
pRomAddr <= to_integer(pRomVAddr & pRomHAddr);
  
-- Get mouse pixel from pointer ROM
-- Since it introduces one more period delay, delay video data too
process(PixelClk)
begin
   if Rising_Edge(PixelClk) then
      if (pClkEn = '1') then
         pMousePx <= mouserom(pRomAddr);
         pMouseEn_q <= pMouseEn;
         
         pData_qq <= pData_q;
         pHSync_qq <= pHSync_q;
         pVSync_qq <= pVSync_q;
         pVDE_qq <= pVDE_q;
      end if;     
   end if;
end process;
   
   -- Pass either original pixel data or mouse overlay depending on
   -- the mouse position.
   process(PixelClk)
   begin
      if Rising_Edge(PixelClk) then
         if (pClkEn = '1') then
            if (pVDE_qq = '1' and pBufEnPointer = '1' and pMouseEn_q = '1') then
               case (pMousePx) is
                  when "01" =>
                     pData_out <= kPointerColor1.r & kPointerColor1.b & kPointerColor1.g; --vid_io interface uses RBG ordering
                  when "00" =>
                     pData_out <= kPointerColor2.r & kPointerColor2.b & kPointerColor2.g; --vid_io interface uses RBG ordering
                  when others =>
                     pData_out <= pData_qq; --transparent pixel in pointer rectangle
               end case;
            else
               pData_out <= pData_qq;
            end if;
            -- sync signals are passed independent of mouse pointer
            pHSync_out <= pHSync_qq;
            pVSync_out <= pVSync_qq;
            pVDE_out <= pVDE_qq;
         end if;        
      end if;
   end process;
	-- User logic ends

end arch_imp;
