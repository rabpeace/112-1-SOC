----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 2023/09/21 15:26:04
-- Design Name: 
-- Module Name: v - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity hw1 is
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           led_o:out STD_LOGIC_VECTOR(7 downto 0));
end hw1;

architecture Behavioral of hw1 is
signal clk_cnt:std_logic_vector(26 downto 0);
signal led:STD_LOGIC_VECTOR(7 downto 0);
signal clk_1:std_logic;
begin
    led_o <=led;
    clk_1 <=clk_cnt(25);
    
    led_cnt:process(clk_1,rst)begin
        if rst = '0' then
            led <= (others=>'0');
        elsif clk_1 = '1' and clk_1'event then
            led <= led + '1' ;
        end if;
    end process;
    
    div_clk:process(clk,rst)begin
        if rst = '0' then
            clk_cnt <= (others=>'0');
        elsif clk = '1' then
            clk_cnt <= clk_cnt + '1';
        end if;
   end process;
end Behavioral;