library verilog;
use verilog.vl_types.all;
entity dual_ram_core is
    port(
        addra           : in     vl_logic_vector(9 downto 0);
        addrb           : in     vl_logic_vector(9 downto 0);
        clka            : in     vl_logic;
        clkb            : in     vl_logic;
        dina            : in     vl_logic_vector(15 downto 0);
        dinb            : in     vl_logic_vector(15 downto 0);
        douta           : out    vl_logic_vector(15 downto 0);
        doutb           : out    vl_logic_vector(15 downto 0);
        ena             : in     vl_logic;
        enb             : in     vl_logic;
        wea             : in     vl_logic;
        web             : in     vl_logic
    );
end dual_ram_core;
