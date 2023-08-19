bins := use_trivium trivium_no_init_cycle gen_input_reg gen_assume use_trivium_noinit
for_cnf := trivium_noinit

.PHONY: all clean

all: $(bins) $(for_cnf).cnf

%.cnf: %.c
	cbmc $< --dimacs --outfile $@

%: %.c
	gcc $< -o $@

clean:
	rm $(bins) $(for_cnf).cnf
