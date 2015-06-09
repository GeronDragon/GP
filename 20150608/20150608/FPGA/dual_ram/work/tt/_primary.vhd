library verilog;
use verilog.vl_types.all;
entity tt is
    generic(
        PERIOD          : integer := 200;
        DUTY_CYCLE      : real    := 5.000000e-001;
        OFFSET          : integer := 0
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of PERIOD : constant is 1;
    attribute mti_svvh_generic_type of DUTY_CYCLE : constant is 2;
    attribute mti_svvh_generic_type of OFFSET : constant is 1;
end tt;
